#include "bag.h"

bag::bag()
{
	bSpace = 0;
	maxSize = 0;
	bagBase = 0;
}

bag::~bag()
{

}

bool bag::init()
{
	bSpace = 0;
	bagBase = *(DWORD*)bagBase;
	if (!bagBase) return false;
	GOODS_PROPERTY temp;
	try
	{
		for (size_t i = 0; i < maxSize; i++)
		{
			m_bag[i].pName = (char*)(bagBase + i * 0x688 + 1);
			m_bag[i].WD_low = (BYTE*)(bagBase + i * 0x688 + 0x1a);
			m_bag[i].WD_high = (BYTE*)(bagBase + i * 0x688 + 0x1b);
			m_bag[i].MD_low = (BYTE*)(bagBase + i * 0x688 + 0x1c);
			m_bag[i].MD_high = (BYTE*)(bagBase + i * 0x688 + 0x1d);
			m_bag[i].PA_low = (BYTE*)(bagBase + i * 0x688 + 0x1e);
			m_bag[i].PA_high = (BYTE*)(bagBase + i * 0x688 + 0x1f);
			m_bag[i].Magic_low = (BYTE*)(bagBase + i * 0x688 + 0x20);
			m_bag[i].Magic_high = (BYTE*)(bagBase + i * 0x688 + 0x21);
			m_bag[i].Tao_low = (BYTE*)(bagBase + i * 0x688 + 0x22);
			m_bag[i].Tao_high = (BYTE*)(bagBase + i * 0x688 + 0x23);
			m_bag[i].Need_what = (BYTE*)(bagBase + i * 0x688 + 0x24);
			m_bag[i].Need_Num = (BYTE*)(bagBase + i * 0x688 + 0x25);
			m_bag[i].ID = (DWORD*)(bagBase + i * 0x688 + 0x2c);
			m_bag[i].Use_Num = (WORD*)(bagBase + i * 0x688 + 0x30);
			m_bag[i].Use_Num_Max = (WORD*)(bagBase + i * 0x688 + 0x32);
		}
	}
	catch (...)
	{
		return false;
	}
	return true;
}

/*
函数功能:获取物品的数量
参数一:物品名字
返回值:该物品在背包内的总数量
*/
int bag::caclGoodsNumber(std::string pName)
{
	int goodsnum = 0;
	for (size_t i = 0; i < maxSize; i++)
	{
		if (!(*m_bag[i].ID)) continue;
		if (strcmp(pName.c_str(), m_bag[i].pName) == 0)
		{
			goodsnum++;
		}
	}
	return goodsnum;
}

/*
函数功能:获取物品的背包下标
参数一:物品名字
返回值:找到该物品的第一个下标
*/
int bag::getGoodsIndex(std::string pName)
{
	int firstIndex = -1;
	for (size_t i = 0; i < maxSize; i++)
	{
		if (!(*m_bag[i].ID)) continue;
		if (strcmp(pName.c_str(), m_bag[i].pName) == 0)
		{
			firstIndex = i;
			break;
		}
	}
	return firstIndex;
}


/*
函数功能:背包是否有毒药
返回值:0没有毒药，1有红毒，2有绿毒，3有红绿毒
*/
int bag::ifHasPoison()
{
	int a = caclGoodsNumber("绿色毒药（大量）") +
		    caclGoodsNumber("绿色毒药（中量）") +
			caclGoodsNumber("绿色毒药（小量）") +
			caclGoodsNumber("道尊灵毒(绿)");
	int b = caclGoodsNumber("红色毒药（大量）") +
			caclGoodsNumber("红色毒药（中量）") +
			caclGoodsNumber("红色毒药（小量）") +
			caclGoodsNumber("道尊灵毒(红)");
	(a > 0) ? (a = 2) : (a = 0);
	(b > 0) ? (b = 1) : (b = 0);
	return a + b;
}

/*
函数功能:计算背包蓝药，只计算大量蓝药和特级蓝药
返回值:背包蓝药数量
*/
int bag::caclMpGoodsNumer()
{
	return caclGoodsNumber("魔法药（大量）") +
			caclGoodsNumber("特级魔法药") +
			caclGoodsNumber("特级魔法药包") *6+
			caclGoodsNumber("超级魔法药")*6;
}


/*
函数功能:计算背包红药，只计算大量红药和特级红药
返回值:背包蓝药数量
*/
int bag::caclHpGoodsNumer()
{
	return caclGoodsNumber("金创药（大量）") +
		caclGoodsNumber("特级金创药") +
		caclGoodsNumber("特级金创药包") *6+
		caclGoodsNumber("超级金创药")*6;
}


/*
函数功能:计算背包空格子数量
返回值:背包空格子数量
*/
int bag::getBagSpace()
{
	int space = 0;
	for (size_t i = 0; i < maxSize; i++)
	{
		if (!(*m_bag[i].ID)) space++;
	}
	return space;
}


