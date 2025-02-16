#pragma once
#include "base.h"
#include <map>
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
	int caclMpGoodsNumer();
	int caclHpGoodsNumer();
	DWORD maxSize; //�����ܴ�С
	DWORD bSpace;//�ո�������
	DWORD bagBase;//��������ַ
	GOODS_PROPERTY m_bag[MAX_BAG_SIZE];//������Ʒ
	int getBagSpace();

	// ������̬��Ա
	static bool initGoodsProcWayList(); //��ʼ����Ʒ�����б������ʼ�����漸������
	static std::vector<std::string>  StoreVec, SellWeaponVec, SellClothesVec, SellJewelryVec;//�ֱ�洢 �����Ʒ ������ �·� ���� ����
	static std::map<std::string, DWORD>SellMedicineVec;//�洢 ��ҩƷ ������ ��ʣ������
};

