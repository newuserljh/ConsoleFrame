#include "m_object.h"

m_object::m_object()
{
}


m_object::~m_object()
{
}

// ��ʼ������
bool m_object::init()
{
	return init_ground() && init_object();
}



// ��ʼ�����󣨹�����NPC����ң�
bool m_object::init_object()
{
	return init_entities(p_monster, m_monster) &&
		init_entities(p_pets, m_pets) &&
		init_entities(p_npcs, m_npcs) &&
		init_entities(p_players, m_players);
}

// ��ʼ��������Ʒ
bool m_object::init_ground()
{
	return init_entities(p_ground, m_ground);
}

// ���ɰ������й������ֵ���Ϣ�ַ���
std::string m_object::message()
{
	std::stringstream ss;
	for (const auto& monster : m_monster)
	{
		ss << "�������֣�" << monster.pName << std::endl;
	}
	return ss.str();
}