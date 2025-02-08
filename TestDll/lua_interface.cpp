#include "stdafx.h"
#include "lua_interface.h"



lua_interface::lua_interface()
	: m_tools(*tools::getInstance())// 初始化单例类的引用
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

	// 类导出到lua
	getGlobalNamespace(L)
		.beginClass<role>("role") //role类
		.addConstructor<void(*)()>()
		.addFunction("init", &role::init)
		.addFunction("init_equip", &role::init_equip)
		.addFunction("init_promenade", &role::init_promenade)
		.addFunction("Get_Envionment", &role::Get_Envionment)
		.addFunction("Get_Ground", &role::Get_Ground)
		.endClass()

		.beginClass<bag>("bag") //bag类
		.addConstructor<void(*)()>()
		.addFunction("init", &bag::init)
		.addFunction("caclGoodsNumber", &bag::caclGoodsNumber)
		.addFunction("getGoodsIndex", &bag::getGoodsIndex)
		.addFunction("ifHasPoison", &bag::ifHasPoison)
		.addFunction("caclMpGoodsNumer", &bag::caclMpGoodsNumer)
		.addFunction("caclHpGoodsNumer", &bag::caclHpGoodsNumer)
		.addFunction("getBagSpace", &bag::getBagSpace)
		.endClass()

		.beginClass<tools>("tools")//tools类
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

		.beginClass<gamecall>("gamecall") //gamecall类
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
		.addFunction("小退", &gamecall::small_exit)
		.addFunction("立即复活", &gamecall::immdia_rebirth)
		//.addFunction("SubmitInputbox", &gamecall::SubmitInputbox)
		.endClass()
				    
	    .beginClass<lua_interface>("lua_interface")// 导出 lua_interface 类
		.addConstructor<void(*)()>()
		.addFunction("presskey", &lua_interface::presskey)
		//.addData("exampleMember", &lua_interface::exampleMember)
		.endClass();

	// 将对象导出到 Lua
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

//封装presskey
bool lua_interface::presskey(int vkcode)
{
	return m_gcall.presskey(::GetCurrentProcessId(),vkcode);
}




// 加载并检查已解析的文件
void lua_interface::load_and_store_map_data(lua_State* L, const std::string& file_path, MapNames& map_names, Transitions& transitions) 
{
	// 加载 Lua 文件
int result = luaL_loadfile(L, file_path.c_str());
if (result != 0) {
	std::cerr << "Lua error: " << lua_tostring(L, -1) << std::endl;
	lua_pop(L, 1); // 移除错误消息
	return;
}

// 执行 Lua 文件
result = lua_pcall(L, 0, LUA_MULTRET, 0);
if (result != 0) {
	std::cerr << "Lua error: " << lua_tostring(L, -1) << std::endl;
	lua_pop(L, 1); // 移除错误消息
	return;
}

// 获取返回值（应该是一个表）
if (!lua_istable(L, -1)) {
	std::cerr << "预期返回一个表" << std::endl;
	return;
}

// 遍历全局表查找 'map_names' 和 'transitions'
lua_pushnil(L); // 第一个键
while (lua_next(L, -2) != 0) {
	// 'key' 是在栈顶 (-1)，'value' 是在栈顶的下一个位置 (-2)
	if (lua_type(L, -2) == LUA_TSTRING) {
		const char* key = lua_tostring(L, -2);
		if (std::string(key) == "map_names") {
			lua_pushnil(L); // 第一个键
			while (lua_next(L, -2) != 0) {
				if (lua_type(L, -2) != LUA_TSTRING || lua_type(L, -1) != LUA_TSTRING) {
					std::cerr << "map_names 键或值不是字符串类型" << std::endl;
					lua_pop(L, 1); // 移除 'value'，保留 'key' 以便继续迭代
					continue;
				}
				const char* map_id = lua_tostring(L, -2);
				const char* map_name = lua_tostring(L, -1);
				map_names[map_id] = map_name;

				// 调试输出
				std::cout << "Map ID: " << map_id << ", Map Name: " << map_name << std::endl;

				lua_pop(L, 1); // 移除 'value'，保留 'key' 以便继续迭代
			}
		}
		else if (std::string(key) == "transitions") {
			lua_pushnil(L); // 第一个键
			while (lua_next(L, -2) != 0) {
				if (lua_type(L, -2) != LUA_TSTRING || !lua_istable(L, -1)) {
					std::cerr << "transitions 键不是字符串类型或值不是表类型" << std::endl;
					lua_pop(L, 1); // 移除 'value'，保留 'key' 以便继续迭代
					continue;
				}
				const char* from_map = lua_tostring(L, -2);
				TransitionMap transition_map;

				lua_pushnil(L); // 第一个键
				while (lua_next(L, -2) != 0) {
					if (lua_type(L, -2) != LUA_TSTRING || !lua_istable(L, -1)) {
						std::cerr << "transitions 内部键不是字符串类型或值不是表类型" << std::endl;
						lua_pop(L, 1); // 移除 'value'，保留 'key' 以便继续迭代
						continue;
					}
					const char* to_map = lua_tostring(L, -2);
					std::vector<Position> positions;

					lua_pushnil(L); // 第一个键
					while (lua_next(L, -2) != 0) {
						if (!lua_istable(L, -1)) {
							std::cerr << "positions 值不是表类型" << std::endl;
							lua_pop(L, 1); // 移除 'value'，保留 'key' 以便继续迭代
							continue;
						}

						Position pos;
						lua_pushnil(L); // 第一个键
						while (lua_next(L, -2) != 0) {
							if (lua_type(L, -2) != LUA_TNUMBER || lua_type(L, -1) != LUA_TNUMBER) {
								std::cerr << "positions 键或值不是数字类型" << std::endl;
								lua_pop(L, 1); // 移除 'value'，保留 'key' 以便继续迭代
								continue;
							}
							pos.x = static_cast<int>(lua_tointeger(L, -2));
							pos.y = static_cast<int>(lua_tointeger(L, -1));
							lua_pop(L, 1); // 移除 'value'，保留 'key' 以便继续迭代
						}

						positions.push_back(pos);

						lua_pop(L, 1); // 移除 'value'，保留 'key' 以便继续迭代
					}

					transition_map[to_map] = positions;

					// 调试输出
					std::cout << "From map: " << from_map << std::endl;
					std::cout << "  To map: " << to_map << std::endl;
					std::cout << "    Positions: ";
					for (const auto& pos : positions) {
						std::cout << "(" << pos.x << ", " << pos.y << ") ";
					}
					std::cout << std::endl;

					lua_pop(L, 1); // 移除 'value'，保留 'key' 以便继续迭代
				}
				transitions[from_map] = transition_map;

				// 如果没有找到任何 'to_map'，输出提示信息
				if (transition_map.empty()) {
					std::cout << "Warning: No transitions found for From map: " << from_map << std::endl;
				}
				lua_pop(L, 1); // 移除 'value'，保留 'key' 以便继续迭代
			}
		}
	}
	lua_pop(L, 1); // 移除 'value'，保留 'key' 以便继续迭代
}
	}

//找到跨图路径
bool lua_interface::find_path(const MapNames& map_names, const Transitions& transitions, const std::string& start_name, const std::string& end_name, std::vector<std::string>& path) {
	std::unordered_map<std::string, std::string> came_from;
	std::queue<std::string> queue;

	// 根据地图名称找到对应的ID
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
		std::cerr << "未找到指定的地图名称" << std::endl;
		return false;
	}

	queue.push(start_id);
	came_from[start_id] = "";

	if (start_id.empty() || end_id.empty()) {
		std::cerr << "未找到指定的地图名称" << std::endl;
		return false;
	}

	queue.push(start_id);
	came_from[start_id] = "";

	while (!queue.empty()) {
		std::string current = queue.front();
		queue.pop();

		if (current == end_id) {
			// 回溯路径
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


//根据路径提取过图坐标
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
				std::cerr << "未找到从 " << from_map << " 到 " << to_map << " 的过渡点" << std::endl;
			}
		}
		else {
			std::cerr << "未找到地图 " << from_map << " 的过渡信息" << std::endl;
		}
	}

	return path_with_positions;
}