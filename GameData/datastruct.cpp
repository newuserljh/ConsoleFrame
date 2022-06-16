#include "datastruct.h"



ROLE_PROPERTY::ROLE_PROPERTY()
{
}

ROLE_PROPERTY::~ROLE_PROPERTY()
{

}

MONSTER_PROPERTY::MONSTER_PROPERTY(){}

MONSTER_PROPERTY::MONSTER_PROPERTY(DWORD* p)
{
	this->ID = (DWORD*)(p + 0x8);
	this->pName = (char*)(p + 0x10);
	this->HP = (DWORD*)(p + 0x80);
	this->HP_MAX = (DWORD*)(p + 0x84);
	this->MP = (DWORD*)(p + 0x88);
	this->MP_MAX = (DWORD*)(p + 0x8C);
	this->X = (DWORD*)(p + 0xac);
	this->Y = (DWORD*)(p + 0xb0);
	this->IsPosion = (BYTE*)(p + 0x34c);
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
	pName = nullptr;
}

GROUND_GOODS::~GROUND_GOODS()
{
	if (!pName)
	{
		delete pName;
	}
}