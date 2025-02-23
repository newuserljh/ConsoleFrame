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

// 注册表键名，用于存储ScriptManager实例的指针
static const char* const SCRIPT_MANAGER_KEY = "__script_manager_ptr";

class ScriptManager {
public:
	// 构造函数，初始化脚本路径和其他成员变量
	ScriptManager(const std::string& scriptPath)
		: scriptPath(scriptPath), L(nullptr), isRunning(false) {
	}

	// 析构函数，停止所有线程并关闭Lua状态
	~ScriptManager() {
		stop();
	}

	// 启动脚本管理器，解析脚本文件并启动相关线程
	void start() {
		if (isRunning) return;

		parseScriptFile();
		stateThread = std::thread(&ScriptManager::checkCharacterState, this);
		luaThread = std::thread(&ScriptManager::luaThreadFunc, this);
		isRunning = true;
	}

	// 停止脚本管理器，停止所有线程并清理资源
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
	// 触发器结构体，包含条件和标签
	struct Trigger {
		std::string condition;
		std::string label;
	};

	// 睡眠任务结构体，包含唤醒时间、Lua状态和引用
	struct SleepTask {
		std::chrono::steady_clock::time_point wakeTime;
		lua_State* L;
		int ref;

		// 显式构造函数
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

	// 战斗状态结构体，包含攻击次数、治疗次数和是否在战斗中
	struct CombatStatus {
		std::atomic<int> totalAttacks{ 0 };
		std::atomic<int> totalHeals{ 0 };
		std::atomic<bool> inCombat{ false };
	};

	std::string scriptPath;  // 脚本文件路径
	std::vector<Trigger> triggers;  // 触发器列表
	std::string scriptContent;  // 脚本内容
	lua_State* L;  // Lua状态指针
	std::atomic<bool> isRunning;  // 运行标志

	std::thread stateThread;  // 检查角色状态的线程
	std::thread luaThread;  // 执行Lua脚本的线程
	std::thread autoCombatThread;  // 自动战斗的线程

	std::mutex mtx;  // 互斥锁
	std::condition_variable cv;  // 条件变量

	std::atomic<bool> shouldJump{ false };  // 是否需要跳转
	std::string jumpLabel;  // 跳转目标标签

	std::priority_queue<SleepTask> sleepQueue;  // 睡眠任务队列
	std::mutex sleepMutex;  // 睡眠任务队列的互斥锁

	std::atomic<bool> combatRunning{ false };  // 自动战斗运行标志
	std::atomic<double> combatInterval{ 1.0 };  // 自动战斗间隔
	CombatStatus combatStatus;  // 战斗状态
	std::mutex combatMutex;  // 战斗状态的互斥锁
	std::condition_variable combatCV;  // 战斗状态的条件变量

	// 获取管理器实例（LuaJIT兼容）
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

	// 解析脚本文件，提取触发器并构建Lua脚本内容
	void parseScriptFile() {
		std::ifstream file(scriptPath);
		if (!file) throw std::runtime_error("无法打开脚本文件");

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

	// 评估给定条件，返回布尔结果
	bool evaluateCondition(const std::string& condition) {
		std::lock_guard<std::mutex> lock(mtx);
		if (!L) return false;

		std::string code = "return " + condition;
		if (luaL_dostring(L, code.c_str()) != 0) {
			std::cerr << "条件错误: " << lua_tostring(L, -1) << std::endl;
			lua_pop(L, 1);
			return false;
		}

		bool result = lua_toboolean(L, -1);
		lua_pop(L, 1);
		return result;
	}

	// 检查角色状态，根据触发器执行相应操作
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

	// Lua睡眠函数，将协程挂起指定秒数
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
			// 直接传递参数（类型严格匹配）
			mgr->sleepQueue.emplace(wakeTime, L, ref);
		}

		return lua_yield(L, 0);
	}

	// 检查睡眠任务队列，恢复已到期的任务
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

	// 开始自动战斗，设置战斗间隔并启动战斗线程
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

	// 停止自动战斗，通知战斗线程退出并等待其结束
	void stopAutoCombat() {
		if (combatRunning.exchange(false)) {
			combatCV.notify_all();
			if (autoCombatThread.joinable()) {
				autoCombatThread.join();
			}
		}
	}

	// 执行一次战斗动作，调用Lua函数 `on_combat_tick`
	void performCombatAction() {
		std::lock_guard<std::mutex> lock(mtx);
		if (L) {
			// 1. 将全局变量压入栈顶
			lua_getglobal(L, "on_combat_tick");

			// 2. 检查栈顶元素是否为函数
			if (lua_isfunction(L, -1)) {
				// 3. 调用函数
				if (lua_pcall(L, 0, 0, 0) != 0) {
					std::cerr << "战斗错误: " << lua_tostring(L, -1) << std::endl;
					lua_pop(L, 1); // 弹出错误信息
				}
			}
			else {
				lua_pop(L, 1); // 弹出非函数值
			}
		}
	}

	// Lua绑定函数，开始自动战斗
	static int luaStartAutoCombat(lua_State* L) {
		double interval = luaL_checknumber(L, 1);
		getManager(L)->startAutoCombat(interval);
		return 0;
	}

	// Lua绑定函数，停止自动战斗
	static int luaStopAutoCombat(lua_State* L) {
		getManager(L)->stopAutoCombat();
		return 0;
	}

	// Lua绑定函数，获取战斗状态
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

	// 主Lua线程函数，处理Lua脚本的执行
	void luaThreadFunc() {
		L = luaL_newstate();
		luaL_openlibs(L);

		// 存储管理器指针
		lua_pushstring(L, SCRIPT_MANAGER_KEY);
		lua_pushlightuserdata(L, this);
		lua_settable(L, LUA_REGISTRYINDEX);

		// 注册函数
		lua_pushcfunction(L, luaSleep);
		lua_setglobal(L, "sleep");
		lua_pushcfunction(L, luaStartAutoCombat);
		lua_setglobal(L, "start_auto_combat");
		lua_pushcfunction(L, luaStopAutoCombat);
		lua_setglobal(L, "stop_auto_combat");
		lua_pushcfunction(L, luaGetStatus);
		lua_setglobal(L, "get_combat_status");

		if (luaL_dostring(L, scriptContent.c_str())) {
			std::cerr << "Lua错误: " << lua_tostring(L, -1) << std::endl;
			return;
		}

		lua_getglobal(L, "main");
		if (lua_pcall(L, 0, 1, 0)) {
			std::cerr << "协程错误: " << lua_tostring(L, -1) << std::endl;
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
				std::cerr << "协程错误: " << lua_tostring(co, -1) << std::endl;
				break;
			}
		}
	}

	// 根据标签执行跳转操作
	void jumpToLabel(lua_State* co, const char* label) {
		std::string code = "goto " + std::string("::") + label + "::";
		if (luaL_loadstring(co, code.c_str())) {
			std::cerr << "跳转错误: " << lua_tostring(co, -1) << std::endl;
			return;
		}
		lua_sethook(co, [](lua_State* L, lua_Debug*) {
			luaL_error(L, "强制跳转");
			}, LUA_MASKCALL | LUA_MASKRET | LUA_MASKLINE, 0);
	}
};


// 测试代码
/*
int main() {
	try {
		ScriptManager manager("script.lua");
		manager.start();
		std::this_thread::sleep_for(std::chrono::seconds(60));
		manager.stop();
	}
	catch (const std::exception& e) {
		std::cerr << "错误: " << e.what() << std::endl;
	}
	return 0;
}

lua脚本部分

--游戏状态
health = 100
mana = 200

--触发器
-- Triggers:
health < 50 goto heal_phase
	mana < 30 goto restore_phase

	-- 战斗配置
	combat_config = {
		attack = "Fireball",
		heal = "Cure",
		restore = "Meditate"
}

--战斗回调
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
print("施放: "..skill)
end

-- 主逻辑
::main::
start_auto_combat(1.5) --每1.5秒战斗循环

while true do
print(string.format("状态: HP=%d MP=%d", health, mana))
sleep(1)
end

::heal_phase::
print("进入治疗阶段!")
health = 80
goto main

::restore_phase::
print("进入恢复阶段!")
mana = 100
goto main


*/