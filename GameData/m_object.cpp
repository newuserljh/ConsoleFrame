#include "m_object.h"


m_object::m_object()
{
}


m_object::~m_object()
{
}

bool m_object::init()
{
	return init_ground() && init_object();
}

bool m_object::init_object()
{
	m_monster.clear();
	m_npcs.clear();
	m_pets.clear();
	m_players.clear();
	if (!p_monster.empty())
	{
		for (size_t i = 0; i < p_monster.size(); i++)
		{
			try
			{
				MONSTER_PROPERTY temp((DWORD*)p_monster[i]);
				m_monster.push_back(temp);
			}
			catch (...)
			{
				return false;
			}
		}
	}
	
	if (!p_pets.empty())
	{
		for (size_t i = 0; i < p_pets.size(); i++)
		{
			try
			{
				MONSTER_PROPERTY temp((DWORD*)p_pets[i]);
				m_pets.push_back(temp);
			}
			catch (...)
			{
				return false;
			}
		}
	}

	if (!p_npcs.empty())
	{
		for (size_t i = 0; i < p_npcs.size(); i++)
		{
			try
			{
				MONSTER_PROPERTY temp((DWORD*)p_npcs[i]);
				m_npcs.push_back(temp);
			}
			catch (...)
			{
				return false;
			}
		}
	}

	if (!p_players.empty())
	{
		for (size_t i = 0; i < p_players.size(); i++)
		{
			try
			{
				MONSTER_PROPERTY temp((DWORD*)p_players[i]);
				m_players.push_back(temp);
			}
			catch (...)
			{
				return false;
			}
		}
	}
	return true;
}

bool m_object::init_ground()
{
	m_ground.clear();
	if (!p_ground.empty())
	{
		for (size_t i = 0; i < p_ground.size(); i++)
		{
			try
			{
				GROUND_GOODS temp((DWORD*)p_ground[i]);
				m_ground.push_back(temp);
			}
			catch (...)
			{
				return false;
			}
		}
	}
	return true;
}


std::string m_object::message()
{
	std::stringstream ss;
	for (int i = 0; i != m_monster.size(); i++)
	{
		ss << "¹ÖÎïÃû×Ö£º" << m_monster[i].pName << std::endl;
	}
	return ss.str();
}


