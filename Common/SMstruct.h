#pragma once
#include <windows.h>
#include <vector>
#include <string>
#pragma pack(4)

#ifdef _DEBUG
#define FILE_MAP_NAME "xxooyy_debug"
#else
#define FILE_MAP_NAME "xxooyy"
#endif
#define  MORE_OPEN_NUMBER 10
//共享内存
typedef struct _SM_SHARE_DATA
{
	DWORD ndPid; //进程pid
	std::string userName;
	std::string passWord;
	std::string cscript;  //脚本
	std::string roleName; //角色名字
	unsigned int send_rand =0;
	unsigned int rcv_rand = 0;
	bool server_alive = true;
}SM_SHARE_DATA;

struct _SM_ALL_SHARE_DATA
{
	SM_SHARE_DATA m_sm_data[MORE_OPEN_NUMBER];
	TCHAR currDir[MAX_PATH];
	DWORD team_info; //队长发布队伍信息1未开启组队,2为停止组队
};

#pragma  pack()