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
	X = (DWORD*)((DWORD)p + 0xc);
	Y = (DWORD*)((DWORD)p + 0x10);
	pName = (char*)((DWORD)p + 0x18);
}
GROUND_GOODS::~GROUND_GOODS()
{
	if (!pName)
	{
		delete pName;
	}
}