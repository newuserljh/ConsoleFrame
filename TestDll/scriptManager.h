#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <thread>
#include <atomic>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <chrono>
#include <lua.hpp>

// ע�����������ڴ洢ScriptManagerʵ����ָ��
static const char* const SCRIPT_MANAGER_KEY = "__script_manager_ptr";

class ScriptManager {
public:
	// ���캯������ʼ���ű�·����������Ա����
	ScriptManager(const std::string& scriptPath)
		: scriptPath(scriptPath), L(nullptr), isRunning(false) {
	}

	// ����������ֹͣ�����̲߳��ر�Lua״̬
	~ScriptManager() {
		stop();
	}

	// �����ű��������������ű��ļ�����������߳�
	void start() {
		if (isRunning) return;

		parseScriptFile();
		stateThread = std::thread(&ScriptManager::checkCharacterState, this);
		luaThread = std::thread(&ScriptManager::luaThreadFunc, this);
		isRunning = true;
	}

	// ֹͣ�ű���������ֹͣ�����̲߳�������Դ
	void stop() {
		if (!isRunning) return;

		{
			std::lock_guard<std::mutex> lock(mtx);
			isRunning = false;
			cv.notify_all();
		}

		stopAutoCombat();

		if (stateThread.joinable()) stateThread.join();
		if (luaThread.joinable()) luaThread.join();

		if (L) {
			lua_close(L);
			L = nullptr;
		}
	}

private:
	// �������ṹ�壬���������ͱ�ǩ
	struct Trigger {
		std::string condition;
		std::string label;
	};

	// ˯������ṹ�壬��������ʱ�䡢Lua״̬������
	struct SleepTask {
		std::chrono::steady_clock::time_point wakeTime;
		lua_State* L;
		int ref;

		// ��ʽ���캯��
		SleepTask(
			std::chrono::steady_clock::time_point t,
			lua_State* l,
			int r
		) : wakeTime(t), L(l), ref(r) {
		}

		bool operator<(const SleepTask& other) const {
			return wakeTime > other.wakeTime;
		}
	};

	// ս��״̬�ṹ�壬�����������������ƴ������Ƿ���ս����
	struct CombatStatus {
		std::atomic<int> totalAttacks{ 0 };
		std::atomic<int> totalHeals{ 0 };
		std::atomic<bool> inCombat{ false };
	};

	std::string scriptPath;  // �ű��ļ�·��
	std::vector<Trigger> triggers;  // �������б�
	std::string scriptContent;  // �ű�����
	lua_State* L;  // Lua״ָ̬��
	std::atomic<bool> isRunning;  // ���б�־

	std::thread stateThread;  // ����ɫ״̬���߳�
	std::thread luaThread;  // ִ��Lua�ű����߳�
	std::thread autoCombatThread;  // �Զ�ս�����߳�

	std::mutex mtx;  // ������
	std::condition_variable cv;  // ��������

	std::atomic<bool> shouldJump{ false };  // �Ƿ���Ҫ��ת
	std::string jumpLabel;  // ��תĿ���ǩ

	std::priority_queue<SleepTask> sleepQueue;  // ˯���������
	std::mutex sleepMutex;  // ˯��������еĻ�����

	std::atomic<bool> combatRunning{ false };  // �Զ�ս�����б�־
	std::atomic<double> combatInterval{ 1.0 };  // �Զ�ս�����
	CombatStatus combatStatus;  // ս��״̬
	std::mutex combatMutex;  // ս��״̬�Ļ�����
	std::condition_variable combatCV;  // ս��״̬����������

	// ��ȡ������ʵ����LuaJIT���ݣ�
	static ScriptManager* getManager(lua_State* L) {
		lua_pushstring(L, SCRIPT_MANAGER_KEY);
		lua_gettable(L, LUA_REGISTRYINDEX);
		if (lua_isnil(L, -1)) {
			luaL_error(L, "Script manager not initialized");
		}
		void* ptr = lua_touserdata(L, -1);
		lua_pop(L, 1);
		return static_cast<ScriptManager*>(ptr);
	}

	// �����ű��ļ�����ȡ������������Lua�ű�����
	void parseScriptFile() {
		std::ifstream file(scriptPath);
		if (!file) throw std::runtime_error("�޷��򿪽ű��ļ�");

		std::ostringstream contentStream;
		std::regex triggerRegex(R"((.+?)\s*goto\s+(\w+))");
		bool inTriggerSection = false;

		std::string line;
		while (std::getline(file, line)) {
			if (line.find("-- Triggers:") != std::string::npos) {
				inTriggerSection = true;
				continue;
			}

			if (inTriggerSection && !line.empty()) {
				std::smatch match;
				if (std::regex_match(line, match, triggerRegex)) {
					triggers.emplace_back(Trigger{
						match[1].str(),
						match[2].str()
						});
				}
			}
			else {
				inTriggerSection = false;
				contentStream << line << "\n";
			}
		}

		scriptContent = R"(
            function main()
                local co = coroutine.create(function()
                    )" + contentStream.str() + R"(
                end)
                return co
            end
        )";
	}

	// �����������������ز������
	bool evaluateCondition(const std::string& condition) {
		std::lock_guard<std::mutex> lock(mtx);
		if (!L) return false;

		std::string code = "return " + condition;
		if (luaL_dostring(L, code.c_str()) != 0) {
			std::cerr << "��������: " << lua_tostring(L, -1) << std::endl;
			lua_pop(L, 1);
			return false;
		}

		bool result = lua_toboolean(L, -1);
		lua_pop(L, 1);
		return result;
	}

	// ����ɫ״̬�����ݴ�����ִ����Ӧ����
	void checkCharacterState() {
		while (isRunning) {
			for (const auto& trigger : triggers) {
				if (evaluateCondition(trigger.condition)) {
					{
						std::lock_guard<std::mutex> lock(mtx);
						shouldJump = true;
						jumpLabel = trigger.label;
					}
					cv.notify_one();
					break;
				}
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}
	}

	// Lua˯�ߺ�������Э�̹���ָ������
	static int luaSleep(lua_State* L) {
		double seconds = luaL_checknumber(L, 1);
		lua_pushthread(L);
		int ref = luaL_ref(L, LUA_REGISTRYINDEX);

		ScriptManager* mgr = getManager(L);
		auto wakeTime = std::chrono::steady_clock::now() +
			std::chrono::duration_cast<std::chrono::steady_clock::duration>(
				std::chrono::duration<double>(seconds)
			);

		{
			std::lock_guard<std::mutex> lock(mgr->sleepMutex);
			// ֱ�Ӵ��ݲ����������ϸ�ƥ�䣩
			mgr->sleepQueue.emplace(wakeTime, L, ref);
		}

		return lua_yield(L, 0);
	}

	// ���˯��������У��ָ��ѵ��ڵ�����
	void checkSleepTasks() {
		std::lock_guard<std::mutex> lock(sleepMutex);
		auto now = std::chrono::steady_clock::now();

		while (!sleepQueue.empty() && sleepQueue.top().wakeTime <= now) {
			auto task = sleepQueue.top();
			sleepQueue.pop();

			lua_rawgeti(task.L, LUA_REGISTRYINDEX, task.ref);
			luaL_unref(task.L, LUA_REGISTRYINDEX, task.ref);

			if (lua_status(task.L) == LUA_YIELD) {
				lua_resume(task.L, 0);
			}
		}
	}

	// ��ʼ�Զ�ս��������ս�����������ս���߳�
	void startAutoCombat(double interval) {
		stopAutoCombat();
		combatInterval = interval;
		combatRunning = true;

		autoCombatThread = std::thread([this] {
			while (combatRunning) {
				performCombatAction();
				std::unique_lock<std::mutex> lock(combatMutex);
				combatCV.wait_for(lock,
					std::chrono::duration<double>(combatInterval),
					[this] { return !combatRunning; });
			}
			});
	}

	// ֹͣ�Զ�ս����֪ͨս���߳��˳����ȴ������
	void stopAutoCombat() {
		if (combatRunning.exchange(false)) {
			combatCV.notify_all();
			if (autoCombatThread.joinable()) {
				autoCombatThread.join();
			}
		}
	}

	// ִ��һ��ս������������Lua���� `on_combat_tick`
	void performCombatAction() {
		std::lock_guard<std::mutex> lock(mtx);
		if (L) {
			// 1. ��ȫ�ֱ���ѹ��ջ��
			lua_getglobal(L, "on_combat_tick");

			// 2. ���ջ��Ԫ���Ƿ�Ϊ����
			if (lua_isfunction(L, -1)) {
				// 3. ���ú���
				if (lua_pcall(L, 0, 0, 0) != 0) {
					std::cerr << "ս������: " << lua_tostring(L, -1) << std::endl;
					lua_pop(L, 1); // ����������Ϣ
				}
			}
			else {
				lua_pop(L, 1); // �����Ǻ���ֵ
			}
		}
	}

	// Lua�󶨺�������ʼ�Զ�ս��
	static int luaStartAutoCombat(lua_State* L) {
		double interval = luaL_checknumber(L, 1);
		getManager(L)->startAutoCombat(interval);
		return 0;
	}

	// Lua�󶨺�����ֹͣ�Զ�ս��
	static int luaStopAutoCombat(lua_State* L) {
		getManager(L)->stopAutoCombat();
		return 0;
	}

	// Lua�󶨺�������ȡս��״̬
	static int luaGetStatus(lua_State* L) {
		ScriptManager* mgr = getManager(L);
		lua_createtable(L, 0, 3);
		lua_pushinteger(L, mgr->combatStatus.totalAttacks);
		lua_setfield(L, -2, "attacks");
		lua_pushinteger(L, mgr->combatStatus.totalHeals);
		lua_setfield(L, -2, "heals");
		lua_pushboolean(L, mgr->combatRunning);
		lua_setfield(L, -2, "running");
		return 1;
	}

	// ��Lua�̺߳���������Lua�ű���ִ��
	void luaThreadFunc() {
		L = luaL_newstate();
		luaL_openlibs(L);

		// �洢������ָ��
		lua_pushstring(L, SCRIPT_MANAGER_KEY);
		lua_pushlightuserdata(L, this);
		lua_settable(L, LUA_REGISTRYINDEX);

		// ע�ắ��
		lua_pushcfunction(L, luaSleep);
		lua_setglobal(L, "sleep");
		lua_pushcfunction(L, luaStartAutoCombat);
		lua_setglobal(L, "start_auto_combat");
		lua_pushcfunction(L, luaStopAutoCombat);
		lua_setglobal(L, "stop_auto_combat");
		lua_pushcfunction(L, luaGetStatus);
		lua_setglobal(L, "get_combat_status");

		if (luaL_dostring(L, scriptContent.c_str())) {
			std::cerr << "Lua����: " << lua_tostring(L, -1) << std::endl;
			return;
		}

		lua_getglobal(L, "main");
		if (lua_pcall(L, 0, 1, 0)) {
			std::cerr << "Э�̴���: " << lua_tostring(L, -1) << std::endl;
			return;
		}

		lua_State* co = lua_tothread(L, -1);
		lua_pop(L, 1);

		while (isRunning) {
			checkSleepTasks();

			{
				std::unique_lock<std::mutex> lock(mtx);
				cv.wait_for(lock, std::chrono::milliseconds(100),
					[this] { return shouldJump || !isRunning; });

				if (shouldJump) {
					lua_pushstring(co, jumpLabel.c_str());
					lua_setglobal(co, "JUMP_TARGET");
					shouldJump = false;
				}
			}

			if (lua_status(co) == LUA_OK) {
				if (lua_resume(co, 0) == LUA_YIELD) {
					lua_getglobal(co, "JUMP_TARGET");
					if (lua_isstring(co, -1)) {
						const char* target = lua_tostring(co, -1);
						lua_pop(co, 1);
						jumpToLabel(co, target);
					}
				}
			}
			else {
				std::cerr << "Э�̴���: " << lua_tostring(co, -1) << std::endl;
				break;
			}
		}
	}

	// ���ݱ�ǩִ����ת����
	void jumpToLabel(lua_State* co, const char* label) {
		std::string code = "goto " + std::string("::") + label + "::";
		if (luaL_loadstring(co, code.c_str())) {
			std::cerr << "��ת����: " << lua_tostring(co, -1) << std::endl;
			return;
		}
		lua_sethook(co, [](lua_State* L, lua_Debug*) {
			luaL_error(L, "ǿ����ת");
			}, LUA_MASKCALL | LUA_MASKRET | LUA_MASKLINE, 0);
	}
};


// ���Դ���
/*
int main() {
	try {
		ScriptManager manager("script.lua");
		manager.start();
		std::this_thread::sleep_for(std::chrono::seconds(60));
		manager.stop();
	}
	catch (const std::exception& e) {
		std::cerr << "����: " << e.what() << std::endl;
	}
	return 0;
}

lua�ű�����

--��Ϸ״̬
health = 100
mana = 200

--������
-- Triggers:
health < 50 goto heal_phase
	mana < 30 goto restore_phase

	-- ս������
	combat_config = {
		attack = "Fireball",
		heal = "Cure",
		restore = "Meditate"
}

--ս���ص�
function on_combat_tick()
if health < 50 then
	cast(combat_config.heal)
	health = health + 30
else
cast(combat_config.attack)
health = health - math.random(10, 20)
end
mana = mana - math.random(5, 15)
end

function cast(skill)
print("ʩ��: "..skill)
end

-- ���߼�
::main::
start_auto_combat(1.5) --ÿ1.5��ս��ѭ��

while true do
print(string.format("״̬: HP=%d MP=%d", health, mana))
sleep(1)
end

::heal_phase::
print("�������ƽ׶�!")
health = 80
goto main

::restore_phase::
print("����ָ��׶�!")
mana = 100
goto main


*/