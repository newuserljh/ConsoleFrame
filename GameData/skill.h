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


private:
	std::vector<std::shared_ptr<SKILL_PROPERTY>> m_skillList;
};

