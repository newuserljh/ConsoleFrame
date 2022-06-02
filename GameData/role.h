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
	bool Get_Envionment(DWORD x/*人物坐标x*/, DWORD y/*坐标Y*/, std::vector<DWORD>& vec/*存储对象容器*/,
		DWORD get_offset = Envi_Offset/*遍历偏移*/, DWORD g_range = 15/*遍历范围*/)/*遍历角色周围NPC 怪物，地面*/;
	ROLE_PROPERTY m_roleproperty;
private:

};

