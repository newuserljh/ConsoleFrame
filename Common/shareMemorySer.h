#pragma once
#include "SMstruct.h"
#include <memory>
#include <vector>
/*
�����ڴ����ˣ����ڿ���̨��
*/
class shareMemorySer
{
public:
	shareMemorySer(int num);
	~shareMemorySer();

	bool createShareMemory();
	void destoryShareMemory();
	//����pid\�û���Ϊ��Ϊ�յ��ڴ��
	int findPidEmptyIndex();
	//���ĳһ���ڴ�
	void clearMemOfIndex(int index);
public:
	_SM_ALL_SHARE_DATA *m_pSMAllData;
};

