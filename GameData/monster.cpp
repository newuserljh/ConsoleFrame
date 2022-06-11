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
		DWORD pObjext = pOb_list[i];
		try {
			MONSTER_PROPERTY temp;
			temp.ID = (DWORD*)(pObjext + 0x8);
			temp.pName = (char*)(pObjext + 0x10);
			temp.HP = (DWORD*)(pObjext + 0x80);
			temp.HP_MAX = (DWORD*)(pObjext + 0x84);
			temp.MP = (DWORD*)(pObjext + 0x88);
			temp.MP_MAX = (DWORD*)(pObjext + 0x8C);
			temp.X = (DWORD*)(pObjext + 0xac);
			temp.Y = (DWORD*)(pObjext + 0xb0);
			temp.IsPosion = (BYTE*)(pObjext + 0x34c);
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
	for (auto i = 0; i < pGr_list.size(); i++)
	{
		DWORD pObjext = pGr_list[i];
		try {
			GROUND_GOODS temp;
			temp.X = (DWORD*)(pObjext + 0xc);
			temp.Y= (DWORD*)(pObjext + 0x10);
			temp.pName = (char*)(pObjext + 0x18);
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


