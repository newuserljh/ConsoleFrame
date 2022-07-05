#pragma once
#include <windows.h>
#include <vector>
#include <string>
#pragma pack(4)

#define  FILE_MAP_NAME	 "xxooyy"
#define  MORE_OPEN_NUMBER 5
//共享内存
typedef struct _SM_SHARE_DATA
{
	DWORD ndPid; //进程pid
	std::string userName;
	std::string passWord;
	std::string cscript;  //脚本
	std::string roleName; //角色名字
	DWORD team_info;     //队伍信息，1队长发起组队信息，2为允许组队,3为已成功组队
	unsigned int send_rand =0;
	unsigned int rcv_rand = 0;
	bool server_alive = true;
}SM_SHARE_DATA;

struct _SM_ALL_SHARE_DATA
{
	SM_SHARE_DATA m_sm_data[MORE_OPEN_NUMBER];
	TCHAR currDir[MAX_PATH];
};

#pragma  pack()