#pragma once
#include "base.h"
#include <vector>
#include "baseaddr.h"
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
	bool init_equip();
	bool init_promenade();
	bool Get_Envionment(std::vector<DWORD>& vec, DWORD g_range = 15);
	bool Get_Ground(std::vector<DWORD>& vec, DWORD g_range = 15);
	ROLE_PROPERTY m_roleproperty;
	GOODS_PROPERTY m_euip[21];
private:

};

