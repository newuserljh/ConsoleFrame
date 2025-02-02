#pragma once
#include "base.h"
/*
周围对象类：
包含NPC, 怪物，玩家，宝宝，地面物品等
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
	std::vector<GROUND_GOODS> m_ground; //地面物品
	std::vector<MONSTER_PROPERTY> m_monster; //周围怪物
	std::vector<MONSTER_PROPERTY> m_npcs; //周围NPC
	std::vector<MONSTER_PROPERTY> m_players; //周围玩家
	std::vector<MONSTER_PROPERTY> m_pets; //周围宝宝
private:

};

