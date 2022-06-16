#pragma once
#include "base.h"
#include <vector>
#include "baseaddr.h"
/*
人物类
一般包含具体的人物属性等
*/
class role :
	public base
{
public:
	role();
	virtual ~role();
	virtual bool init();
	bool init_equip();
	bool Get_Envionment(std::vector<DWORD>& vec/*存储对象容器*/, DWORD g_range = 15/*遍历范围*/)/*遍历角色周围NPC 怪物*/;
	bool Get_Ground(std::vector<DWORD>& vec, DWORD g_range = 15)/*地面*/;
	ROLE_PROPERTY m_roleproperty;
	GOODS_PROPERTY m_euip[21];
private:

};

