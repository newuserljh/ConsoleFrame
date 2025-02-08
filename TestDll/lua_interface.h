#pragma once
#include "bag.h"
#include "role.h"
#include "utils.h"
#include "gamecall.h"
#include <../luajit/src/lua.hpp>
#include <LuaBridge/LuaBridge.h>
#include <unordered_map>
#include <queue>

struct Position {
	int x, y;
};

using TransitionMap = std::unordered_map<std::string, std::vector<Position>>;
using MapNames = std::unordered_map<std::string, std::string>;
using Transitions = std::unordered_map<std::string, TransitionMap>;

class lua_interface
{
public:
	lua_interface();
	~lua_interface();

	void registerClasses();
	lua_State* getLuaState() const;

private:
	lua_State* L;
	role m_role;
	bag m_bag;
	gamecall m_gcall;
	tools &m_tools; // 单例类的引用

public:
	//导出本类函数
	bool presskey(int vkcode);
	bool find_path(const MapNames& map_names, const Transitions& transitions, const std::string& start_name, const std::string& end_name, std::vector<std::string>& path);
	void load_and_store_map_data(lua_State* L, const std::string& file_path, MapNames& map_names, Transitions& transitions);
	std::vector<std::pair<std::string, std::vector<Position>>> get_positions_for_path(const Transitions& transitions, const std::vector<std::string>& path);
private:

};

