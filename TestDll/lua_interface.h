#pragma once
#include "bag.h"
#include "role.h"
#include "utils.h"
#include "gamecall.h"
#include <../luajit/src/lua.hpp>
#include <LuaBridge/LuaBridge.h>

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

};

