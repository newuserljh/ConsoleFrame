#pragma once
#include <windows.h>

//��������
struct ROLE_PROPERTY
{
	ROLE_PROPERTY();
	~ROLE_PROPERTY();
	
	char* szpName;
	DWORD ndCurHp;
	DWORD ndMaxHp;

};

//��Χ����
struct MONSTER_PROPERTY 
{
	MONSTER_PROPERTY();
	~MONSTER_PROPERTY();

	char* szpName;
};

//���ܶ���
struct SKILL_PROPERTY 
{
	SKILL_PROPERTY();
	~SKILL_PROPERTY();

	char* szpName;
};

//�������
struct TASK_PROPERTY
{
	TASK_PROPERTY();
	~TASK_PROPERTY();

	char* szpName;
};