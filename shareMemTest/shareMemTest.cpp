// shareMemTest.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "shareMemoryCli.h"
#include <iostream>
#include <string>
#pragma comment(lib, "Common.lib")
int main()
{
	//��ʼ�������ڴ�

	shareMemoryCli shareCli(MORE_OPEN_NUMBER);
	
	if (!shareCli.openShareMemory())
	{
		std::cout << "�򿪹����ڴ�ʧ�ܣ�" << std::endl;
	}
	
	std::cout << GetCurrentProcessId() << std::endl;

	int index = shareCli.getPidEmptyIndex();
	shareCli.m_pSMAllData->m_sm_data[index].ndPid = GetCurrentProcessId();
	//shareCli.m_pSMAllData->m_sm_data[index]
	if (!shareCli.m_pSMAllData->m_sm_data[index].userName.empty() && !shareCli.m_pSMAllData->m_sm_data[index].passWord.empty())
	{
		std::cout <<"�˺ţ�" <<shareCli.m_pSMAllData->m_sm_data[index].userName << " , ����:" << shareCli.m_pSMAllData->m_sm_data[index].passWord << std::endl;
	}

	getchar();
	return 0;
}

