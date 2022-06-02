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
	if (!bagBase)return false;
	GOODS_PROPERTY temp;
	try
	{
		for (auto i=0;i<maxSize;i++)
		{
			if (!(*(DWORD*)(bagBase + i * 0x688 + 0x2c)))//ÅÐ¶ÏÎÞÎïÆ·
			{
				m_bag[i].ID=0;
				bSpace++;
				continue;
			}
			m_bag[i].pName = (char*)(bagBase + i * 0x688 + 1);
			m_bag[i].WD_low = *(BYTE*)(bagBase + i * 0x688 + 0x1a);
			m_bag[i].WD_high = *(BYTE*)(bagBase + i * 0x688 + 0x1b);
			m_bag[i].MD_low = *(BYTE*)(bagBase + i * 0x688 + 0x1c);
			m_bag[i].WD_high = *(BYTE*)(bagBase + i * 0x688 + 0x1d);
			m_bag[i].PA_low = *(BYTE*)(bagBase + i * 0x688 + 0x1e); 
			m_bag[i].PA_high = *(BYTE*)(bagBase + i * 0x688 + 0x1f);
			m_bag[i].Magic_low = *(BYTE*)(bagBase + i * 0x688 + 0x20);
			m_bag[i].Magic_high = *(BYTE*)(bagBase + i * 0x688 + 0x21);
			m_bag[i].Tao_low = *(BYTE*)(bagBase + i * 0x688 + 0x22);
			m_bag[i].Tao_high = *(BYTE*)(bagBase + i * 0x688 + 0x23);
			m_bag[i].Need_what = *(BYTE*)(bagBase + i * 0x688 + 0x24);
			m_bag[i].Need_Num = *(BYTE*)(bagBase + i * 0x688 + 0x25);
			m_bag[i].ID = *(DWORD*)(bagBase + i * 0x688 + 0x2c);
			m_bag[i].Use_Num = *(WORD*)(bagBase + i * 0x688 + 0x30);
			m_bag[i].Use_Num_Max = *(WORD*)(bagBase + i * 0x688 + 0x32);
		}
	}
	catch (...)
	{
		return false;
	}
	return true;
}