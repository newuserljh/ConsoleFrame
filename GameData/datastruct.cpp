#include "datastruct.h"



ROLE_PROPERTY::ROLE_PROPERTY()
{
}

ROLE_PROPERTY::~ROLE_PROPERTY()
{

}

MONSTER_PROPERTY::MONSTER_PROPERTY()
{
	ID = nullptr;
	pName = nullptr;
	HP = nullptr;
}

MONSTER_PROPERTY::MONSTER_PROPERTY(DWORD* p)
{
	ID = (DWORD*)((DWORD)p + 0x8);
	pName = (char*)((DWORD)p + 0x10);
	HP = (DWORD*)((DWORD)p + 0x80);
	HP_MAX = (DWORD*)((DWORD)p+ 0x84);
	MP = (DWORD*)((DWORD)p + 0x88);
	MP_MAX = (DWORD*)((DWORD)p + 0x8C);
	X = (DWORD*)((DWORD)p + 0xac);
	Y = (DWORD*)((DWORD)p + 0xb0);
	IsPosion = (BYTE*)((DWORD)p + 0x34b);
}

MONSTER_PROPERTY::~MONSTER_PROPERTY()
{
	if (!pName)
	{
		delete pName;
	}
}

SKILL_PROPERTY::SKILL_PROPERTY()
{
	pName = nullptr;
}

SKILL_PROPERTY::~SKILL_PROPERTY()
{
	if (!pName)
	{
		delete pName;
	}
}

GOODS_PROPERTY::GOODS_PROPERTY()
{
	pName = nullptr;
}

GOODS_PROPERTY::~GOODS_PROPERTY()
{
	if (!pName)
	{
		delete pName;
	}
}

GROUND_GOODS::GROUND_GOODS()
{
	Distance = 10000;
	pName = nullptr;
}
GROUND_GOODS::GROUND_GOODS(DWORD* p)
{
	X = (DWORD*)((DWORD)p + 8);
	Y = (DWORD*)((DWORD)p + 0xc);
	pName = (char*)((DWORD)p + 0x18);
}
GROUND_GOODS::~GROUND_GOODS()
{
	if (!pName)
	{
		delete pName;
	}
}

TEAM_PROPERTY::TEAM_PROPERTY()
{
}
TEAM_PROPERTY::TEAM_PROPERTY(DWORD* p /*当前链条指针*/)
{
	Next = (TEAM_PROPERTY*)((*p));
	DWORD pt = (DWORD)p;
	Previous = (TEAM_PROPERTY*)((*(DWORD*)(pt + 4))); //指针加1，结果是对该指针增加1个储存单位，DWORD就+4
	pName=(char*)((pt + 0xc));
}
TEAM_PROPERTY::~TEAM_PROPERTY()
{

}