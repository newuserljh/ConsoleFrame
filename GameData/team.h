#pragma once
#include "base.h"

/*
������
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
	std::vector<TEAM_PROPERTY> m_team_list;//�������11���� ��һ�������� ��team[1]�ӳ� ֮��Ϊ��Ա
private: 
	DWORD* base_temp;
};


