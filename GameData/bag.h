#pragma once
#include "base.h"
#define MAX_BAG_SIZE 126
/*
������
*/

class bag :
   public base
{
public:
	bag();
	~bag();
	bool init();


	DWORD maxSize; //�����ܴ�С
	DWORD bSpace;//�ո�������
	DWORD bagBase;//��������ַ
    GOODS_PROPERTY m_bag[MAX_BAG_SIZE];
};

