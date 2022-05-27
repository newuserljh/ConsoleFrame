#include "datastruct.h"



ROLE_PROPERTY::ROLE_PROPERTY()
{
}

ROLE_PROPERTY::~ROLE_PROPERTY()
{

}

MONSTER_PROPERTY::MONSTER_PROPERTY()
{
	pName = nullptr;
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