#pragma once
#include "base.h"
/*
��Χ�����ࣺ
����NPC, ���������Ʒ��
*/

class monster :
	public base
{
public:
	monster();
	virtual ~monster();
	virtual bool init();
	bool init_ground();
	virtual std::string message();
	std::vector<MONSTER_PROPERTY> m_monsterList;
	std::vector<DWORD> pOb_list;
	std::vector<DWORD> pGr_list;
	std::vector<GROUND_GOODS> m_groundList;
private:


};

