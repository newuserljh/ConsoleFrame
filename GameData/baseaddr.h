#pragma once
#include <windows.h>
/*
����ַ/ƫ��
*/

#define RoleBase 0x135fb30 //���������ַ
#define YS_Base 0x135fb34 //Ԫ���ַ
#define Envi_Offset 0x412D68 //NPC,�������ƫ��
#define Ground_Offset 0x412354 //������Ʒ����ƫ��



#define CALL_ECX  0x135fb10 //����call��ecx��Դ
#define CALL_ECX_1 0x4E072C4 //Ѱ·call��ecx
#define CALL_ECX_2 0x0135FB08//����CALL��ecx

#define CALL_EDX 0x110D9D4//�Ի�call��edx

#define ATTACK_SLEEP 1210 //�����������

#define ID_LONGIN 0x8C00CFE5 //��¼�ؼ�ID

#define CALL_LOGIN   0x689B90   //��¼call
#define CALL_PRESSKEY 0x0062C750 //����call
#define CALL_USESKILL 0x733640  //����call��ַ
#define CALL_CURRENT_MAPMOVE 0x007AF860 //��ǰ��ͼѰ·call��ַ
#define CALL_USEGOODS 0x00718A10 //ʹ�ñ�����Ʒcall
#define CALL_OPENNPC 0x7BC280   //��NPC
#define CALL_CHOOSE_CMD 0x6AC2D0//ѡ��Ի�����
#define CALL_PICKUP_GOODS 0x6C9A30  //ʰȡ������Ʒ
#define CALL_STEP_TO_MAP 0x7331D0  //��·��ָ�����꣬ÿ����һ����Ҫ��������
#define CALL_RUN_TO_MAP 0x733270    //�ܲ���ָ�����꣬ÿ����2����Ҫ�������ã�ֻ���ߵ�2������
#define CALL_TEAM_OPEN_CLOSE 0x76F2B0  //���鿪��
#define CALL_MAKE_TEAM 0x76F7B0    //�����������
#define CALL_ALLOW_TEAM 0x7358C0    //ͬ���������
