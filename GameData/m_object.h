#pragma once
#include "base.h"


/*
��Χ�����ࣺ
����NPC, �����ң�������������Ʒ��
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

	// ԭʼ��������
	std::vector<DWORD> p_ground, p_monster, p_pets, p_npcs, p_players;

	// �ѳ�ʼ���Ķ�������
	std::vector<GROUND_GOODS> m_ground; // ������Ʒ
	std::vector<MONSTER_PROPERTY> m_monster; // ��Χ����
	std::vector<MONSTER_PROPERTY> m_npcs; // ��ΧNPC
	std::vector<MONSTER_PROPERTY> m_players; // ��Χ���
	std::vector<MONSTER_PROPERTY> m_pets; // ��Χ����

private:

};

// ͨ�õ�ʵ���ʼ������ģ��
// T: Ŀ������
// U: Դ����
template <typename T, typename U>
bool init_entities(const std::vector<U>& source, std::vector<T>& destination)
{
	destination.clear();
	for (const auto& item : source)
	{
		try
		{
			// ��Դ����ת��ΪĿ�����Ͳ���ӵ�Ŀ��������
			T temp(reinterpret_cast<DWORD*>(item));
			destination.push_back(temp);
		}
		catch (...)
		{
			// ���������쳣������ false
			return false;
		}
	}
	return true;
}
