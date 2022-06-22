#include "monster.h"


monster::monster()
{
}


monster::~monster()
{
}

bool monster::init()
{
	init_ground();
	if (pOb_list.empty())return false;
	for (auto i = 0; i < pOb_list.size(); i++)
	{
		try
		{
			MONSTER_PROPERTY temp((DWORD*)pOb_list[i]);
			m_monsterList.push_back(temp);
		}
		catch (...)
		{
			return false;
		}
	}
	return true;
}

bool monster::init_ground()
{
	if (pGr_list.empty())return false;
	m_groundList.clear();
	for (auto i = 0; i < pGr_list.size(); i++)
	{
		try 
		{
			GROUND_GOODS temp((DWORD*)pGr_list[i]);
			m_groundList.push_back(temp);
		}
		catch (...)
		{
			return false;
		}
	}
	return true;
}


std::string monster::message()
{
	std::stringstream ss;
	for (int i = 0; i != m_monsterList.size(); i++)
	{
		ss << "¹ÖÎïÃû×Ö£º" << m_monsterList[i].pName << std::endl;
	}
	return ss.str();
}


