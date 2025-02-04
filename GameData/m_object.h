#pragma once
#include "base.h"
/*
��Χ�����ࣺ
����NPC, �����ң�������������Ʒ��
*/

class m_object :
	public base
{
public:
	m_object();
	virtual ~m_object();
	virtual bool init();
	bool init_ground();
	bool init_object();
	virtual std::string message();
	std::vector<DWORD> p_ground,p_monster,p_pets,p_npcs,p_players;
	std::vector<GROUND_GOODS> m_ground; //������Ʒ
	std::vector<MONSTER_PROPERTY> m_monster; //��Χ����
	std::vector<MONSTER_PROPERTY> m_npcs; //��ΧNPC
	std::vector<MONSTER_PROPERTY> m_players; //��Χ���
	std::vector<MONSTER_PROPERTY> m_pets; //��Χ����
private:

};

