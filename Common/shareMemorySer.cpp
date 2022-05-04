#include "shareMemorySer.h"


shareMemorySer::shareMemorySer(int num)
{

}


shareMemorySer::~shareMemorySer()
{
	destoryShareMemory();
}

/*
�������ܣ�����һ�鹲���ڴ�
����ֵ��bool
*/
bool shareMemorySer::createShareMemory()
{
	//����һ���СΪSM_ALL_SHARE_DATA�Ĺ����ڴ�
	HANDLE hMapFile = ::CreateFileMapping((HANDLE)-1, NULL, PAGE_READWRITE, 0, sizeof(_SM_ALL_SHARE_DATA), FILE_MAP_NAME);
	if (!hMapFile)
	{
		return false;
	}
	//�����ָ��ָ�򴴽��Ĺ����ڴ�
	m_pSMAllData = (_SM_ALL_SHARE_DATA *)MapViewOfFile(hMapFile, FILE_MAP_WRITE | FILE_MAP_READ, 0, 0, 0); 
	memset(m_pSMAllData, 0, sizeof(_SM_ALL_SHARE_DATA));
	if (!m_pSMAllData)
	{
		return false;
	}
	return true;
}
/*
�������ܣ��ͷŹ����ڴ�
����ֵ����
*/
void shareMemorySer::destoryShareMemory()
{
	if (m_pSMAllData)
	{
		UnmapViewOfFile(m_pSMAllData);
		m_pSMAllData = nullptr;
	}
}

int shareMemorySer::findPidEmptyIndex()
{
	for (int i = 0; i < MORE_OPEN_NUMBER; i++)
	{
		if (m_pSMAllData->m_sm_data[i].userName.empty())
		{
			return i;
		}
	}
	return -1;
}

void shareMemorySer::clearMemOfIndex(int index)
{
	for (int i = 0; i != MORE_OPEN_NUMBER; i++)
	{
		if (i == index)
		{
			memset(&m_pSMAllData->m_sm_data[i], 0, sizeof(SM_SHARE_DATA));
		}
	}
}
