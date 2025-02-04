#pragma once
#include "base.h"


/*
周围对象类：
包含NPC, 怪物，玩家，宝宝，地面物品等
*/

class m_object : public base
{
public:
	m_object();
	virtual ~m_object();
	virtual bool init();
	bool init_ground();
	bool init_object();
	virtual std::string message();

	// 原始数据容器
	std::vector<DWORD> p_ground, p_monster, p_pets, p_npcs, p_players;

	// 已初始化的对象容器
	std::vector<GROUND_GOODS> m_ground; // 地面物品
	std::vector<MONSTER_PROPERTY> m_monster; // 周围怪物
	std::vector<MONSTER_PROPERTY> m_npcs; // 周围NPC
	std::vector<MONSTER_PROPERTY> m_players; // 周围玩家
	std::vector<MONSTER_PROPERTY> m_pets; // 周围宝宝

private:

};

// 通用的实体初始化函数模板
// T: 目标类型
// U: 源类型
template <typename T, typename U>
bool init_entities(const std::vector<U>& source, std::vector<T>& destination)
{
	destination.clear();
	for (const auto& item : source)
	{
		try
		{
			// 将源类型转换为目标类型并添加到目标容器中
			T temp(reinterpret_cast<DWORD*>(item));
			destination.push_back(temp);
		}
		catch (...)
		{
			// 捕获所有异常并返回 false
			return false;
		}
	}
	return true;
}
