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
	bool Get_Envionment(DWORD x/*��������x*/, DWORD y/*����Y*/, std::vector<DWORD>& vec/*�洢��������*/,
		DWORD get_offset = Envi_Offset/*����ƫ��*/, DWORD g_range = 15/*������Χ*/)/*������ɫ��ΧNPC �������*/;
	ROLE_PROPERTY m_roleproperty;
private:

};

