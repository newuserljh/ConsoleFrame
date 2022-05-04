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
	virtual std::string message();
private:
	std::vector<std::shared_ptr<MONSTER_PROPERTY>> m_monsterList;
};

