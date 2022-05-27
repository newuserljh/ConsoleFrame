#pragma once
#include <windows.h>
/*
基地址/偏移
*/

#define RoleBase 0x135fb30 //人物主体基址
#define YS_Base 0x135fb34 //元神基址
#define SKILLBASE 0x733640  //技能call地址
#define CALLBASE  0x135fb10 //所有call的ecx来源
#define ATTACK_SLEEP 1210 //攻击间隔毫秒
#define GOTOBASE 0x007AF860 //寻路call地址

#define Envi_Offset 0x412D68 //NPC,怪物遍历偏移
#define Ground_Offset 0x412354 //地面物品遍历偏移

const DWORD Base_RoleAddr = 0x0;


const DWORD Off_RoleOffset1 = 0x0;



const DWORD Call_RoleCall1 = 0x0;