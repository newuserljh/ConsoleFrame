#pragma once



#define RoleBase 0x135fb30 //人物主体基址
#define YS_Base 0x135fb34 //元神基址
#define SKILLBASE 0x733640  //技能call地址
#define CALLBASE  0x135fb10 //所有call的ecx来源
#define ATTACK_SLEEP 1210 //攻击间隔毫秒
#define GOTOBASE 0x007AF860 //寻路call地址

#define Envi_Offset 0x412D68 //NPC,怪物遍历偏移
#define Ground_Offset 0x412354 //地面物品遍历偏移

typedef struct
{
	UCHAR Goods_Name_Length;// + 0
	DWORD Goods_Name;//+ 1
	UCHAR  Goods_WD_low;//+ 1a / /物理防御下  /幸运武器
	UCHAR  Goods_WD_high;// + 1b 物理防御上  /命中武器
	UCHAR  Goods_MD_low;//+ 1c / 魔防
	UCHAR  Goods_MD_high;//+ 1D
	UCHAR  Goods_PA_low;//+ 1e        //攻击力
	UCHAR  Goods_PA_high;// + 1f
	UCHAR  Goods_Magic_low;// + 20   //魔法攻击
	UCHAR  Goods_Magic_high;// + 21
	UCHAR  Goods_Tao_low;//+ 22   //道术攻击
	UCHAR  Goods_Tao_high;//+ 23
	UCHAR  Goods_Need_what;// + 24  佩戴需求 0等级 1攻击 2魔法 3道术
	UCHAR  Goods_Need_Num;//+ 25  佩戴需求大小 等级 / 魔法。。
	DWORD Goods_ID;//+ 2c
	unsigned short Goods_Use_Num;//+ 30            耐久 / 使用次数
	unsigned short Goods_Use_Num_Max;// + 32    最大耐久 / 使用次数
}GOODS; //物品

typedef struct
{
	WORD  Skill_ID;// + 0xe
	DWORD  Skill_Name;//+ 14
}SKILL;//技能结构体
typedef struct
{
	DWORD  X;// 
	DWORD  Y;// 
} MAP_XY; //坐标结构体

typedef struct
{
	DWORD  ID;// +8
	DWORD  Name;// +0x10
	DWORD  HP;// +0x80
	DWORD  HP_MAX;// +0x84
	DWORD  MP;// +0x88
	DWORD  MP_MAX;// +0x8c
	MAP_XY XY;//坐标 X;// +0xac Y;// +0xb0
	UCHAR  IsPosion;// +0x34b  //是否中毒 0没毒，0x40红毒，0x80绿毒，0xc0红绿毒,
} OBJECT; //对象 怪物/NPC/他人/宝宝结构体

typedef struct
{
	OBJECT Object;
	DWORD Job;//  +0xe4 //职业 0/战士  1/法师 2/道士
	DWORD Level;// +0xec //等级
	DWORD Sex; //+0xdc 性别 0男 1女
	DWORD GJL_L;//攻击 0xA5C
	DWORD GJL_H;//+0xA60
	DWORD MFL_L;//魔法+0xA64
	DWORD MFL_H; //+0xA68
	DWORD DSL_L;//道术+0xA6c
	DWORD DSL_H;//+0xA70
	DWORD FY_L;//防御//+0xA7C
	DWORD FY_H;//+0xA80
	DWORD MF_L;//防御/+0xA84 
	DWORD MF_H;//0xA88
	WORD BAG_W; // 背包重量 低16位//+0xAA8
	WORD BAG_W_MAX; // 背包重量 低16位//+0xAA6
	DWORD SW; //声望//+0xAD0 
	DWORD p_Bag_Base; //背包基址，第一个格子指针0x135fb30]+0xC54]+ i * 0x688   (0-0x41)
	DWORD Bag_Size;//背包大小[0x135fb30  ]+0xc58 
	DWORD p_LR_Bag_Base;//灵兽背包基址，第一个格子指针[0x135fb30  ]+0xc60  
	DWORD LR_Bag_Size; //灵兽背包大小  +0xc64 
	DWORD p_Target_ID; //选中/攻击 目标ID +0x458 
	DWORD p_Skill_Base;//i为技能栏的顺序从0开始0x135fb30]+0x1358]+ i * 0x88
	DWORD p_ZB;/*== [[0x135fb30 ]+ 0xc48] + i * 0x688 //身上装备 0衣服，1武器，2勋章，3项链，4头盔
					，5右手镯，6左手镯，7右戒指，8左戒指，9靴子，10腰带，11宝石，12护符，13盾牌，14魂珠,
					15 极品御兽天衣 ，16 紋佩 ，18 神龙背饰 ，19 面具 ，20 仙翼*/ 
	DWORD LL; //[0x135fb30] + 0x139C//灵力
	DWORD p_Current_Map;// [135fb30] + 0x8299C8//所在地图 CHAR*
} HUMAN; 

typedef struct
{
	CHAR* s_from; /*发送单位:"exe"为主控端，"dll"为dll主程序，"角色名字"游戏客户端*/
	CHAR* s_to;/*发送单位:"exe"为主控端，"dll"为dll主程序，"角色名字"游戏客户端*/
	DWORD s_context;/*发送的内容*/
}GAME_COMMU;//通讯结构体



//=============全局变量===============================================
//extern HUMAN role1, role_YS; //主角色,元神; //人物结构体
//extern GOODS role_Bag[125], ls_Bag[59], ys_Bag[59], role_ZB[19]; //主体包裹,灵兽,元神，主体装备
