#pragma once
#include "base.h"
#define MAX_BAG_SIZE 126
/*
背包类
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

	DWORD maxSize; //背包总大小
	DWORD bSpace;//空格子数量
	DWORD bagBase;//背包基地址
    GOODS_PROPERTY m_bag[MAX_BAG_SIZE];
};

