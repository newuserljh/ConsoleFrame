#pragma once
#include <vector>
#include <memory>
#include "SMstruct.h"
/*
�����ڴ�ͻ��ˣ�����dll��
*/
class shareMemoryCli
{
public:
	shareMemoryCli(int num);
	~shareMemoryCli();

	bool openShareMemory();
	void destoryShareMemory();
	int getPidEmptyIndex();
	int getIndexByPID(DWORD pid);

public:
	_SM_ALL_SHARE_DATA *m_pSMAllData;
};

