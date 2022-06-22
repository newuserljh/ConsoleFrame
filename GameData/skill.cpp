#include "skill.h"


skill::skill()
{
	skillBase = 0;
}


skill::~skill()
{
}

bool skill::init()
{
	skillBase = *(DWORD*)skillBase;
	if (!skillBase)return false;
	m_skillList.clear();
	try
	{
		int i = 0;
		SKILL_PROPERTY temp;
		do 
		{
			temp.ID = (DWORD*)(skillBase + i * 0x88 + 0xe);
			temp.level = (BYTE*)(skillBase + i * 0x88 + 8);
			temp.pName = (char*)(skillBase + i * 0x88 + 0x14);
			m_skillList.push_back(temp);
			i++;
		} while (*(DWORD*)(skillBase + i * 0x88 + 0x14));
	}
	catch (...)
	{
		return false;
	}
	return true;
}


/*
��������:ͨ�����ֻ�ȡ����ID
����һ:��������
����ֵ������ID�����Ϊ-1��ʾû�д˼���
*/
DWORD skill::getSkillId(char* sName)
{
	DWORD ret = -1;
	for (auto i=0;i<m_skillList.size();i++)
	{
		if (strcmp(sName,m_skillList[i].pName)==0)
		{
			return *m_skillList[i].ID;
		}
	}
	return ret;
}
