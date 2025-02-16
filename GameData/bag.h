#pragma once
#include "base.h"
#include <map>
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
	int caclMpGoodsNumer();
	int caclHpGoodsNumer();
	DWORD maxSize; //背包总大小
	DWORD bSpace;//空格子数量
	DWORD bagBase;//背包基地址
	GOODS_PROPERTY m_bag[MAX_BAG_SIZE];//背包物品
	int getBagSpace();

	// 声明静态成员
	static bool initGoodsProcWayList(); //初始化物品处理列表，用与初始化下面几个容器
	static std::vector<std::string>  StoreVec, SellWeaponVec, SellClothesVec, SellJewelryVec;//分别存储 存仓物品 卖武器 衣服 首饰 名字
	static std::map<std::string, DWORD>SellMedicineVec;//存储 卖药品 的名字 和剩余数量
};

