#pragma once
#include "base.h"
/*
������
*/
class skill :
	public base
{
public:
	skill();
	virtual ~skill();
	virtual bool init();

	DWORD skillBase;
	std::vector<SKILL_PROPERTY> m_skillList;
private:

};

