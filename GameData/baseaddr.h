#pragma once
#include <windows.h>
/*
基地址/偏移
*/

#define RoleBase 0x135fb30 //人物主体基址
#define YS_Base 0x135fb34 //元神基址
#define Envi_Offset 0x412D68 //NPC,怪物遍历偏移
#define Ground_Offset 0x412354 //地面物品遍历偏移



#define CALL_ECX  0x135fb10 //所有call的ecx来源
#define CALL_ECX_1 0x4E072C4 //寻路call的ecx
#define CALL_ECX_2 0x0135FB08//按键CALL的ecx

#define CALL_EDX 0x110D9D4//对话call的edx

#define ATTACK_SLEEP 1210 //攻击间隔毫秒

#define ID_LONGIN 0x8C00CFE5 //登录控件ID

#define CALL_LOGIN   0x689B90   //登录call
#define CALL_PRESSKEY 0x0062C750 //按键call
#define CALL_USESKILL 0x733640  //技能call地址
#define CALL_CURRENT_MAPMOVE 0x007AF860 //当前地图寻路call地址
#define CALL_USEGOODS 0x00718A10 //使用背包物品call
#define CALL_OPENNPC 0x7BC280   //打开NPC
#define CALL_CHOOSE_CMD 0x6AC2D0//选择对话命令
#define CALL_PICKUP_GOODS 0x6C9A30  //拾取地面物品


