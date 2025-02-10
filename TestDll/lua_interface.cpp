#include "stdafx.h"
#include "lua_interface.h"
#include "TestDlg.h"


lua_interface::lua_interface()
	: m_tools(*tools::getInstance())// ��ʼ�������������
{
	L = luaL_newstate();
	luaL_openlibs(L);
	registerClasses();

}

lua_interface::~lua_interface()
{
	lua_close(L);
}

void lua_interface::registerClasses()
{
	using namespace luabridge;

	// �ർ����lua
	getGlobalNamespace(L)
		.beginClass<role>("role") //role��
		.addConstructor<void(*)()>()
		.addFunction("init", &role::init)
		.addFunction("init_equip", &role::init_equip)
		.addFunction("init_promenade", &role::init_promenade)
		.addFunction("Get_Envionment", &role::Get_Envionment)
		.addFunction("Get_Ground", &role::Get_Ground)
		.endClass()

		.beginClass<bag>("bag") //bag��
		.addConstructor<void(*)()>()
		.addFunction("init", &bag::init)
		.addFunction("caclGoodsNumber", &bag::caclGoodsNumber)
		.addFunction("getGoodsIndex", &bag::getGoodsIndex)
		.addFunction("ifHasPoison", &bag::ifHasPoison)
		.addFunction("caclMpGoodsNumer", &bag::caclMpGoodsNumer)
		.addFunction("caclHpGoodsNumer", &bag::caclHpGoodsNumer)
		.addFunction("getBagSpace", &bag::getBagSpace)
		.endClass()

		.beginClass<tools>("tools")//tools��
		.addStaticFunction("getInstance", &tools::getInstance)
		//.addFunction("message", &tools::message)
		.addFunction("improveProcPriv", &tools::improveProcPriv)
		//.addFunction("findPattern", &tools::findPattern)
		.addFunction("getModuleInfo", &tools::getModuleInfo)
		.addFunction("log2file", &tools::log2file)
		.addFunction("write2file", &tools::write2file)
		.addFunction("write2file_c", &tools::write2file_c)
		//.addFunction("read", &tools::read)
		//.addFunction("write", &tools::write)
		//.addFunction("byte2hex", &tools::byte2hex)
		//.addFunction("lower2upper", &tools::lower2upper)
		//.addFunction("hexstr2upper", &tools::hexstr2upper)
		//.addFunction("hex2byte", &tools::hex2byte)
		//.addFunction("bytes2hexstr", &tools::bytes2hexstr)
		//.addFunction("hexstr2bytes", &tools::hexstr2bytes)
		.addFunction("fileIsexist", &tools::fileIsexist)
		.addFunction("ReadTxt", &tools::ReadTxt)
		.addFunction("splitString", &tools::splitString)
		.addFunction("GetCurrDir", &tools::GetCurrDir)
		.addFunction("getParentPath", &tools::getParentPath)
		.addFunction("char2wchar", &tools::char2wchar)
		.addFunction("wchar2char", &tools::wchar2char)
		.addFunction("ReleaseResource", &tools::ReleaseResource)
		.addFunction("eipinjectDll", &tools::eipinjectDll)
		.addStaticFunction("exeload", &tools::exeload)
		.endClass()

		.beginClass<gamecall>("gamecall") //gamecall��
		.addConstructor<void(*)()>()
		.addFunction("loginGame", &gamecall::loginGame)
		.addFunction("useGoods", &gamecall::useGoods)
		.addFunction("useSkillTo", &gamecall::useSkillTo)
		.addFunction("CurrentMapMove", &gamecall::CurrentMapMove)
		.addFunction("OpendNPC", &gamecall::OpendNPC)
		.addFunction("ChooseCmd", &gamecall::ChooseCmd)
		.addFunction("pickupGoods", &gamecall::pickupGoods)
		.addFunction("Run_or_Step_To", &gamecall::Run_or_Step_To)
		.addFunction("GetHwndByPid", &gamecall::GetHwndByPid)
		.addFunction("caclDistance", &gamecall::caclDistance)
		.addFunction("sort_aroud_monster", &gamecall::sort_aroud_monster)
		.addFunction("sort_groud_goods", &gamecall::sort_groud_goods)
		.addFunction("splitXY", &gamecall::splitXY)
		.addFunction("team_open_close", &gamecall::team_open_close)
		.addFunction("maketeam", &gamecall::maketeam)
		.addFunction("allowteam", &gamecall::allowteam)
		.addFunction("release_Promenade", &gamecall::release_Promenade)
		.addFunction("start_end_AutoAttack", &gamecall::start_end_AutoAttack)
		.addFunction("OpenRecovry", &gamecall::OpenRecovry)
		.addFunction("RecovryGoods", &gamecall::RecovryGoods)
		.addFunction("RecovryGoods_To_Exp", &gamecall::RecovryGoods_To_Exp)
		.addFunction("RecovryGoods_To_Gold", &gamecall::RecovryGoods_To_Gold)
		.addFunction("С��", &gamecall::small_exit)
		.addFunction("��������", &gamecall::immdia_rebirth)
		//.addFunction("SubmitInputbox", &gamecall::SubmitInputbox)
		.endClass()
				    
	    .beginClass<lua_interface>("lua_interface")// ���� lua_interface ��
		.addConstructor<void(*)()>()
		.addFunction("presskey", &lua_interface::presskey)
		//.addData("exampleMember", &lua_interface::exampleMember)
		.endClass();

	// �����󵼳��� Lua
	getGlobalNamespace(L)
		.beginNamespace("app")
		.addVariable("role", &m_role, false)
		.addVariable("bag", &m_bag, false)
		.addVariable("tools", &m_tools, false)
		.addVariable("gamecall", &m_gcall, false)
		.endNamespace();
}



lua_State* lua_interface::getLuaState() const
{
	return L;
}

//��װpresskey
bool lua_interface::presskey(int vkcode)
{
	return m_gcall.presskey(::GetCurrentProcessId(),vkcode);
}




// ���ز�����ѽ������ļ�
void lua_interface::load_and_store_map_data(lua_State* L, const std::string& file_path, MapNames& map_names, Transitions& transitions) 
{
	// ���� Lua �ļ�
int result = luaL_loadfile(L, file_path.c_str());
if (result != 0) {
	std::cerr << "Lua error: " << lua_tostring(L, -1) << std::endl;
	lua_pop(L, 1); // �Ƴ�������Ϣ
	return;
}

// ִ�� Lua �ļ�
result = lua_pcall(L, 0, LUA_MULTRET, 0);
if (result != 0) {
	std::cerr << "Lua error: " << lua_tostring(L, -1) << std::endl;
	lua_pop(L, 1); // �Ƴ�������Ϣ
	return;
}

// ��ȡ����ֵ��Ӧ����һ����
if (!lua_istable(L, -1)) {
	std::cerr << "Ԥ�ڷ���һ����" << std::endl;
	return;
}

// ����ȫ�ֱ���� 'map_names' �� 'transitions'
lua_pushnil(L); // ��һ����
while (lua_next(L, -2) != 0) {
	// 'key' ����ջ�� (-1)��'value' ����ջ������һ��λ�� (-2)
	if (lua_type(L, -2) == LUA_TSTRING) {
		const char* key = lua_tostring(L, -2);
		if (std::string(key) == "map_names") {
			lua_pushnil(L); // ��һ����
			while (lua_next(L, -2) != 0) {
				if (lua_type(L, -2) != LUA_TSTRING || lua_type(L, -1) != LUA_TSTRING) {
					std::cerr << "map_names ����ֵ�����ַ�������" << std::endl;
					lua_pop(L, 1); // �Ƴ� 'value'������ 'key' �Ա��������
					continue;
				}
				const char* map_id = lua_tostring(L, -2);
				const char* map_name = lua_tostring(L, -1);
				map_names[map_id] = map_name;

				// �������
				std::cout << "Map ID: " << map_id << ", Map Name: " << map_name << std::endl;

				lua_pop(L, 1); // �Ƴ� 'value'������ 'key' �Ա��������
			}
		}
		else if (std::string(key) == "transitions") {
			lua_pushnil(L); // ��һ����
			while (lua_next(L, -2) != 0) {
				if (lua_type(L, -2) != LUA_TSTRING || !lua_istable(L, -1)) {
					std::cerr << "transitions �������ַ������ͻ�ֵ���Ǳ�����" << std::endl;
					lua_pop(L, 1); // �Ƴ� 'value'������ 'key' �Ա��������
					continue;
				}
				const char* from_map = lua_tostring(L, -2);
				TransitionMap transition_map;

				lua_pushnil(L); // ��һ����
				while (lua_next(L, -2) != 0) {
					if (lua_type(L, -2) != LUA_TSTRING || !lua_istable(L, -1)) {
						std::cerr << "transitions �ڲ��������ַ������ͻ�ֵ���Ǳ�����" << std::endl;
						lua_pop(L, 1); // �Ƴ� 'value'������ 'key' �Ա��������
						continue;
					}
					const char* to_map = lua_tostring(L, -2);
					std::vector<Position> positions;

					lua_pushnil(L); // ��һ����
					while (lua_next(L, -2) != 0) {
						if (!lua_istable(L, -1)) {
							std::cerr << "positions ֵ���Ǳ�����" << std::endl;
							lua_pop(L, 1); // �Ƴ� 'value'������ 'key' �Ա��������
							continue;
						}

						Position pos;
						lua_pushnil(L); // ��һ����
						while (lua_next(L, -2) != 0) {
							if (lua_type(L, -2) != LUA_TNUMBER || lua_type(L, -1) != LUA_TNUMBER) {
								std::cerr << "positions ����ֵ������������" << std::endl;
								lua_pop(L, 1); // �Ƴ� 'value'������ 'key' �Ա��������
								continue;
							}
							pos.x = static_cast<int>(lua_tointeger(L, -2));
							pos.y = static_cast<int>(lua_tointeger(L, -1));
							lua_pop(L, 1); // �Ƴ� 'value'������ 'key' �Ա��������
						}

						positions.push_back(pos);

						lua_pop(L, 1); // �Ƴ� 'value'������ 'key' �Ա��������
					}

					transition_map[to_map] = positions;

					// �������
					std::cout << "From map: " << from_map << std::endl;
					std::cout << "  To map: " << to_map << std::endl;
					std::cout << "    Positions: ";
					for (const auto& pos : positions) {
						std::cout << "(" << pos.x << ", " << pos.y << ") ";
					}
					std::cout << std::endl;

					lua_pop(L, 1); // �Ƴ� 'value'������ 'key' �Ա��������
				}
				transitions[from_map] = transition_map;

				// ���û���ҵ��κ� 'to_map'�������ʾ��Ϣ
				if (transition_map.empty()) {
					std::cout << "Warning: No transitions found for From map: " << from_map << std::endl;
				}
				lua_pop(L, 1); // �Ƴ� 'value'������ 'key' �Ա��������
			}
		}
	}
	lua_pop(L, 1); // �Ƴ� 'value'������ 'key' �Ա��������
}
	}

//�ҵ���ͼ·��
bool lua_interface::find_path(const MapNames& map_names, const Transitions& transitions, const std::string& start_name, const std::string& end_name, std::vector<std::string>& path) {
	std::unordered_map<std::string, std::string> came_from;
	std::queue<std::string> queue;

	// ���ݵ�ͼ�����ҵ���Ӧ��ID
	std::string start_id, end_id;
	for (const auto& pair : map_names) {
		if (pair.second == start_name) {
			start_id = pair.first;
		}
		if (pair.second == end_name) {
			end_id = pair.first;
		}
	}

	if (start_id.empty() || end_id.empty()) {
		std::cerr << "δ�ҵ�ָ���ĵ�ͼ����" << std::endl;
		return false;
	}

	queue.push(start_id);
	came_from[start_id] = "";

	if (start_id.empty() || end_id.empty()) {
		std::cerr << "δ�ҵ�ָ���ĵ�ͼ����" << std::endl;
		return false;
	}

	queue.push(start_id);
	came_from[start_id] = "";

	while (!queue.empty()) {
		std::string current = queue.front();
		queue.pop();

		if (current == end_id) {
			// ����·��
			std::string node = end_id;
			while (!node.empty()) {
				path.insert(path.begin(), node);
				node = came_from[node];
			}
			return true;
		}

		auto it = transitions.find(current);
		if (it != transitions.end()) {
			for (const auto& transition : it->second) {
				const std::string& to_map = transition.first;
				if (came_from.find(to_map) == came_from.end()) {
					queue.push(to_map);
					came_from[to_map] = current;
				}
			}
		}
	}

	return false;
}


//����·����ȡ��ͼ����
std::vector<std::pair<std::string, std::vector<Position>>> lua_interface::get_positions_for_path(const Transitions& transitions, const std::vector<std::string>& path) {
	std::vector<std::pair<std::string, std::vector<Position>>> path_with_positions;

	for (size_t i = 0; i < path.size() - 1; ++i) {
		const std::string& from_map = path[i];
		const std::string& to_map = path[i + 1];

		auto it_from = transitions.find(from_map);
		if (it_from != transitions.end()) {
			auto it_to = it_from->second.find(to_map);
			if (it_to != it_from->second.end()) {
				path_with_positions.push_back({ to_map, it_to->second });
			}
			else {
				std::cerr << "δ�ҵ��� " << from_map << " �� " << to_map << " �Ĺ��ɵ�" << std::endl;
			}
		}
		else {
			std::cerr << "δ�ҵ���ͼ " << from_map << " �Ĺ�����Ϣ" << std::endl;
		}
	}

	return path_with_positions;
}

//ͨ�����ֻ�ȡ�����ΧNPCID
DWORD lua_interface::getEviroNPCIdByName(std::string npcName)
{
	if (!r.Get_Envionment(m_obj.p_pets, m_obj.p_npcs, m_obj.p_monster, m_obj.p_players))
	{
		std::cout << "������Χ����!!!" << std::endl;
		return -1;
	}

	for (size_t i = 0; i < m_obj.p_npcs.size(); i++)
	{
		if (npcName.compare((char*)(m_obj.p_npcs[i]+0x10)) == 0) return *(DWORD*)(m_obj.p_npcs[i] + 8);  // ����ƥ���NPC��ID
	}
		return -1;
}

//��װ��Ϸ����

//�ύ�ͽ�����
void lua_interface::applySJLP()
{
	int num = r_bag.caclGoodsNumber("�ͽ�����");
	std::cout <<"�ͽ����Ƶ�����Ϊ��" << num << std::endl;
	if (num > 0)
	{
		auto npcid = getEviroNPCIdByName("�ͽ�����");
		if (npcid == -1)return;
		mfun.OpendNPC(npcid);
		for (auto i = 0; i < num; ++i)
		{
			Sleep(50);
			mfun.ChooseCmd("@givepai");
			Sleep(50);
			mfun.ChooseCmd("@main");
		}
	}
}

//��ҩ  ����һ������  ������������ ����bool
bool lua_interface::buyMedicine(std::string med_name,BYTE num)
{
	auto npcid = getEviroNPCIdByName("ҩ���ƹ�");
	if (npcid == -1) return false;
	if (mfun.OpendNPC(npcid))
	{
		Sleep(50);
		if (*r.m_roleproperty.p_Role_GoldBind>100000) //ʹ�ð󶨽��
		{
			if (!mfun.ChooseCmd("@bindbuy"))return false;
			Sleep(100);
			if(!mfun.buyGoods(med_name, npcid, 1, num))return false;
			return true;
		}
		if (*r.m_roleproperty.p_Role_GoldBind > 100000) //ʹ�ý��
		{
			if (!mfun.ChooseCmd("@buy"))return false;
			Sleep(100);
			if (!mfun.buyGoods(med_name, npcid, 0,num))return false;
			return true;
		}
		else
		{
			std::cout << "��ң��󶨽�Ҳ��㣬����ʧ�ܣ�" << std::endl;
			return false;
		}
	}
	std::cout << "�Ի�NPC���󣬼���Ƿ���NPC��������" << std::endl;

	return true;
}
