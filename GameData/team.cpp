#include "team.h"

team::team()
{
	team_Base=nullptr;
	member_num=0;
	base_temp = nullptr;
}

team::~team()
{

}

bool team::init()
{
	if (team_Base == nullptr)return false;
	m_team_list.clear();
	member_num = (DWORD*)((DWORD)team_Base + 4);
	if (*member_num < 2)return true; 
	base_temp = (DWORD*)(*team_Base);
	try
	{
		for (size_t i = 0; i <=*member_num; i++)
		{
			TEAM_PROPERTY temp(base_temp);
			base_temp = (DWORD*)(temp.Next);
			if(i == 0)continue;
			m_team_list.push_back(temp);
		}
	}
	catch (...)
	{
		return false;
	}
	return true;
}



