#pragma once
#include "base.h"
/*
������
һ�����������������Ե�
*/
class role :
	public base
{
public:
	role();
	virtual ~role();
	virtual bool init();

private:
	ROLE_PROPERTY m_roleproperty;
};

