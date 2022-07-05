#pragma once
#include "base.h"

/*
队伍类
*/

class team :
	public base
{
public:
	team();
	~team();
	bool init();
	
	DWORD* team_Base;
	DWORD*  member_num;
	std::vector<TEAM_PROPERTY> m_team_list;//最多组满11个人 第一个无数据 从team[1]队长 之后为队员
private: 
	DWORD* base_temp;
};


