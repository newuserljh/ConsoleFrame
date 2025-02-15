#pragma once
#include <windows.h>

//��Χ����
struct MONSTER_PROPERTY
{
	MONSTER_PROPERTY();
	MONSTER_PROPERTY(DWORD* p);
	~MONSTER_PROPERTY();

	DWORD*  ID;// +8
	char*  pName;// +0x10
	WORD* oType;// +0x68 //�������� 0Ϊ��� ��1 ΪNPC�� 2Ϊ����ͱ���(�󵶺͹���Ҳ��2)��0x400Ϊ��ħ(�����ͼ����)
 	DWORD*  HP;// +0x80
	DWORD*  HP_MAX;// +0x84
	DWORD*  MP;// +0x88
	DWORD*  MP_MAX;// +0x8c
	DWORD*  X;// X;// +0xac 
	DWORD*  Y;// Y;// +0xb0
	BYTE*  IsPosion;// +0x34b  //�Ƿ��ж� 0û����0x40�춾��0x80�̶���0xc0���̶�,
	float Distance; //����ͽ�ɫ�ľ���
};
//��������
struct ROLE_PROPERTY
{
	ROLE_PROPERTY();
	~ROLE_PROPERTY();	
	MONSTER_PROPERTY Object;
	DWORD* VIP_Level; //+0x5cc VIP�ȼ�
	DWORD* Job;//  +0xe4 //ְҵ 0/սʿ  1/��ʦ 2/��ʿ
	DWORD* Level;// +0xec //�ȼ�
	DWORD* Sex; //+0xdc �Ա� 0�� 1Ů
	DWORD* GJL_L;//���� 0xA5C
	DWORD* GJL_H;//+0xA60
	DWORD* MFL_L;//ħ��+0xA64
	DWORD* MFL_H; //+0xA68
	DWORD* DSL_L;//����+0xA6c
	DWORD* DSL_H;//+0xA70
	DWORD* FY_L;//����//+0xA7C
	DWORD* FY_H;//+0xA80
	DWORD* MF_L;//����/+0xA84 
	DWORD* MF_H;//0xA88
	WORD* BAG_W; // �������� +0xAA8
	WORD* BAG_W_MAX; // �������� //+0xAAA
	DWORD* SW; //����//+0xAD0 
	DWORD* p_Bag_Base; //������ַ����һ������ָ��0x135fb30]+0xC54]+ i * 0x688   (0-0x41)
	DWORD* Bag_Size;//������С[0x135fb30  ]+0xc58 
	DWORD* p_LR_Bag_Base;//���ޱ�����ַ����һ������ָ��[0x135fb30  ]+0xc60  
	DWORD* LR_Bag_Size; //���ޱ�����С  +0xc64 
	DWORD* p_Target_ID; //ѡ��/���� Ŀ��ID +0x458 
	DWORD* p_Skill_Base;//iΪ��������˳���0��ʼ0x135fb30]+0x1358]+ i * 0x88
	DWORD* p_ZB;/*== [[0x135fb30 ]+ 0xc48] + i * 0x688 //����װ�� 0�·���1������2ѫ�£�3������4ͷ��
					��5������6������7�ҽ�ָ��8���ָ��9ѥ�ӣ�10������11��ʯ��12������13���ƣ�14����,
					15 ��Ʒ�������� ��16 �y�� ��18 �������� ��19 ��� ��20 ����*/
	DWORD* LL; //[0x135fb30] + 0x139C//����
	char* p_Current_Map;// [135fb30] + 0x8299C8//���ڵ�ͼ CHAR*
    DWORD* PatCount; ///��������+0xA24
	DWORD* Team_is_allow;//[0x135fb30  ]+0x1054550 �Ƿ��������0������,1����,��ֱ���޸�
	DWORD* Team_pointer;//+0x1054550+8 �����������ָ�� ָ���ͷ��ָ�� 

	DWORD* Is_has_Promenade; //+0xAE4 / AE8  0��Ԫ�� 1��Ԫ��; +0x13A0 4�� 0 ��

	//Ԫ���������
	DWORD*  Is_Promenade_Release; // [0x135fb34]+0x3D8        Ԫ���Ƿ��ͷ� 1�� 0��  +0xAA4Ҳ��
	DWORD*  Promenade_Mode; // [0x135fb34]+0xA08        Ԫ��ģʽ 0ս�� 1���� 2ֹͣ ��ֱ���޸�ֵ�ı�ģʽ

	
	DWORD* p_Role_EXP;// = [0x135fb30] + 0xA84//��ǰ����ֵ unsigned����
	DWORD* p_Role_EXP_MAX;// = [0x135fb30] + 0xAA0//��ǰ�ȼ������ֵ unsigned
	double* p_Role_YB_Bind;// = [0x135fb30] + 0xC08//��Ԫ�� Double����
	DWORD* p_Role_Gold;// = [0x135fb30] + 0xC10//���
	DWORD* p_Role_GoldBind;// = [0x135fb30] + 0xC14//�󶨽��
	DWORD* p_Role_YB;// = [0x135fb30] + 0xBEC//Ԫ��

};



//���ܶ���
struct SKILL_PROPERTY 
{
	SKILL_PROPERTY();
	~SKILL_PROPERTY();
	BYTE* level;// +8
	DWORD* ID;// + 0xe
	char* pName;//+ 14
};

//��Ʒ����
struct GOODS_PROPERTY
{
	GOODS_PROPERTY();
	~GOODS_PROPERTY();
	BYTE Name_Length;// + 0
	char* pName;//+ 1
	BYTE*  WD_low;//+ 1a / /���������  /��������
	BYTE* WD_high;// + 1b ���������  /��������
	BYTE* MD_low;//+ 1c / ħ��
	BYTE* MD_high;//+ 1D
	BYTE* PA_low;//+ 1e        //������
	BYTE* PA_high;// + 1f
	BYTE* Magic_low;// + 20   //ħ������
	BYTE* Magic_high;// + 21
	BYTE* Tao_low;//+ 22   //��������
	BYTE* Tao_high;//+ 23
	BYTE* Need_what;// + 24  ������� 0�ȼ� 1���� 2ħ�� 3����
	BYTE* Need_Num;//+ 25  ��������С �ȼ� / ħ������
	DWORD* ID;//+ 2c
	WORD* Use_Num;//+ 30            �;� / ʹ�ô���
	WORD* Use_Num_Max;// + 32    ����;� / ʹ�ô���

	//�Լ�����
	DWORD goods_type; //1 ���� 2���� 3�·� 4ҩƷ 5����
	DWORD howProcess; //��Ʒ����ʽ 5������δ���壩 1��� 2���� 3����ʹ�� 4��ʣ
	DWORD remainNumbers; //��ʣ�¶��� ��howProcess=4ʱ�ټ������
};

//��Ʒ����
struct GROUND_GOODS
{
	GROUND_GOODS();
	GROUND_GOODS(DWORD* p);
	~GROUND_GOODS();
	char* pName;//+ 0x18
	DWORD* X;//+8
	DWORD* Y;//+0xc
	float Distance;//�����ɫ����
};

//�������
struct TASK_PROPERTY
{
	TASK_PROPERTY();
	~TASK_PROPERTY();
	char* pName;
};

//����
struct MapXY
{
	DWORD x;
	DWORD y;
};

//�����Ա����
struct TEAM_PROPERTY //���� ���� �� ��һ����������Ϊ�� ����������Ϊn+1 nΪ����������
{
	TEAM_PROPERTY();
	TEAM_PROPERTY(DWORD* p);
	~TEAM_PROPERTY();
	TEAM_PROPERTY* Next; //��һ������ָ��
	TEAM_PROPERTY* Previous;//��һ������ָ��  
	char* pName;//��Ա����
};
