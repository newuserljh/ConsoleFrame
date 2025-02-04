#include "m_object.h"

m_object::m_object()
{
}


m_object::~m_object()
{
}

// 初始化函数
bool m_object::init()
{
	return init_ground() && init_object();
}



// 初始化对象（怪物、宠物、NPC、玩家）
bool m_object::init_object()
{
	return init_entities(p_monster, m_monster) &&
		init_entities(p_pets, m_pets) &&
		init_entities(p_npcs, m_npcs) &&
		init_entities(p_players, m_players);
}

// 初始化地面物品
bool m_object::init_ground()
{
	return init_entities(p_ground, m_ground);
}

// 生成包含所有怪物名字的消息字符串
std::string m_object::message()
{
	std::stringstream ss;
	for (const auto& monster : m_monster)
	{
		ss << "怪物名字：" << monster.pName << std::endl;
	}
	return ss.str();
}