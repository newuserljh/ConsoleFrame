#pragma once



#define RoleBase 0x135fb30 //���������ַ
#define YS_Base 0x135fb34 //Ԫ���ַ
#define SKILLBASE 0x733640  //����call��ַ
#define CALLBASE  0x135fb10 //����call��ecx��Դ
#define ATTACK_SLEEP 1210 //�����������
#define GOTOBASE 0x007AF860 //Ѱ·call��ַ

#define Envi_Offset 0x412D68 //NPC,�������ƫ��
#define Ground_Offset 0x412354 //������Ʒ����ƫ��

typedef struct
{
	UCHAR Goods_Name_Length;// + 0
	DWORD Goods_Name;//+ 1
	UCHAR  Goods_WD_low;//+ 1a / /���������  /��������
	UCHAR  Goods_WD_high;// + 1b ���������  /��������
	UCHAR  Goods_MD_low;//+ 1c / ħ��
	UCHAR  Goods_MD_high;//+ 1D
	UCHAR  Goods_PA_low;//+ 1e        //������
	UCHAR  Goods_PA_high;// + 1f
	UCHAR  Goods_Magic_low;// + 20   //ħ������
	UCHAR  Goods_Magic_high;// + 21
	UCHAR  Goods_Tao_low;//+ 22   //��������
	UCHAR  Goods_Tao_high;//+ 23
	UCHAR  Goods_Need_what;// + 24  ������� 0�ȼ� 1���� 2ħ�� 3����
	UCHAR  Goods_Need_Num;//+ 25  ��������С �ȼ� / ħ������
	DWORD Goods_ID;//+ 2c
	unsigned short Goods_Use_Num;//+ 30            �;� / ʹ�ô���
	unsigned short Goods_Use_Num_Max;// + 32    ����;� / ʹ�ô���
}GOODS; //��Ʒ

typedef struct
{
	WORD  Skill_ID;// + 0xe
	DWORD  Skill_Name;//+ 14
}SKILL;//���ܽṹ��
typedef struct
{
	DWORD  X;// 
	DWORD  Y;// 
} MAP_XY; //����ṹ��

typedef struct
{
	DWORD  ID;// +8
	DWORD  Name;// +0x10
	DWORD  HP;// +0x80
	DWORD  HP_MAX;// +0x84
	DWORD  MP;// +0x88
	DWORD  MP_MAX;// +0x8c
	MAP_XY XY;//���� X;// +0xac Y;// +0xb0
	UCHAR  IsPosion;// +0x34b  //�Ƿ��ж� 0û����0x40�춾��0x80�̶���0xc0���̶�,
} OBJECT; //���� ����/NPC/����/�����ṹ��

typedef struct
{
	OBJECT Object;
	DWORD Job;//  +0xe4 //ְҵ 0/սʿ  1/��ʦ 2/��ʿ
	DWORD Level;// +0xec //�ȼ�
	DWORD Sex; //+0xdc �Ա� 0�� 1Ů
	DWORD GJL_L;//���� 0xA5C
	DWORD GJL_H;//+0xA60
	DWORD MFL_L;//ħ��+0xA64
	DWORD MFL_H; //+0xA68
	DWORD DSL_L;//����+0xA6c
	DWORD DSL_H;//+0xA70
	DWORD FY_L;//����//+0xA7C
	DWORD FY_H;//+0xA80
	DWORD MF_L;//����/+0xA84 
	DWORD MF_H;//0xA88
	WORD BAG_W; // �������� ��16λ//+0xAA8
	WORD BAG_W_MAX; // �������� ��16λ//+0xAA6
	DWORD SW; //����//+0xAD0 
	DWORD p_Bag_Base; //������ַ����һ������ָ��0x135fb30]+0xC54]+ i * 0x688   (0-0x41)
	DWORD Bag_Size;//������С[0x135fb30  ]+0xc58 
	DWORD p_LR_Bag_Base;//���ޱ�����ַ����һ������ָ��[0x135fb30  ]+0xc60  
	DWORD LR_Bag_Size; //���ޱ�����С  +0xc64 
	DWORD p_Target_ID; //ѡ��/���� Ŀ��ID +0x458 
	DWORD p_Skill_Base;//iΪ��������˳���0��ʼ0x135fb30]+0x1358]+ i * 0x88
	DWORD p_ZB;/*== [[0x135fb30 ]+ 0xc48] + i * 0x688 //����װ�� 0�·���1������2ѫ�£�3������4ͷ��
					��5������6������7�ҽ�ָ��8���ָ��9ѥ�ӣ�10������11��ʯ��12������13���ƣ�14����,
					15 ��Ʒ�������� ��16 �y�� ��18 �������� ��19 ��� ��20 ����*/ 
	DWORD LL; //[0x135fb30] + 0x139C//����
	DWORD p_Current_Map;// [135fb30] + 0x8299C8//���ڵ�ͼ CHAR*
} HUMAN; 

typedef struct
{
	CHAR* s_from; /*���͵�λ:"exe"Ϊ���ضˣ�"dll"Ϊdll������"��ɫ����"��Ϸ�ͻ���*/
	CHAR* s_to;/*���͵�λ:"exe"Ϊ���ضˣ�"dll"Ϊdll������"��ɫ����"��Ϸ�ͻ���*/
	DWORD s_context;/*���͵�����*/
}GAME_COMMU;//ͨѶ�ṹ��



//=============ȫ�ֱ���===============================================
//extern HUMAN role1, role_YS; //����ɫ,Ԫ��; //����ṹ��
//extern GOODS role_Bag[125], ls_Bag[59], ys_Bag[59], role_ZB[19]; //�������,����,Ԫ������װ��
