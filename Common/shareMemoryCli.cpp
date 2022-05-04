#include "shareMemoryCli.h"




shareMemoryCli::shareMemoryCli(int num)
{

}


shareMemoryCli::~shareMemoryCli()
{
}

/*
�������ܣ���һ�鹲���ڴ�
����ֵ��bool
*/
bool shareMemoryCli::openShareMemory()
{
	HANDLE hMapFile = OpenFileMapping(
		FILE_MAP_WRITE,
		FALSE,
		FILE_MAP_NAME);
	if (!hMapFile)
	{
		return false;
	}
	m_pSMAllData = (_SM_ALL_SHARE_DATA *)MapViewOfFile(
		hMapFile,
		FILE_MAP_READ | FILE_MAP_WRITE,
		0,
		0,
		0);
	if (!m_pSMAllData)
	{
		return false;
	}
	return true;
}
/*
�������� :�ͷŹ����ڴ�
����ֵ����
*/
void shareMemoryCli::destoryShareMemory()
{
	if (m_pSMAllData)
	{
		UnmapViewOfFile(m_pSMAllData);
		m_pSMAllData = nullptr;
	}
}

int shareMemoryCli::getPidEmptyIndex()
{
	for (int i = 0; i < MORE_OPEN_NUMBER; i++)
	{
		if (m_pSMAllData->m_sm_data[i].ndPid == 0)
		{
			return i;
		}
	}

	return -1;
}

