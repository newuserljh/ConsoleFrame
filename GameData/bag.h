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
	int caclGoodsNumber(std::string pName);
	int getGoodsIndex(std::string pName);
	int ifHasPoison(void);

	DWORD maxSize; //�����ܴ�С
	DWORD bSpace;//�ո�������
	DWORD bagBase;//��������ַ
    GOODS_PROPERTY m_bag[MAX_BAG_SIZE];
};

