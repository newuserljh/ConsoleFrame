#pragma once
#include <windows.h>
class login
{
public:
	login();
	~login();
	bool game_login(const char* username, const char* password);
	bool presskey(int vkcode);
	bool pressEnter( DWORD pid);
	HWND GetHwndByPid(DWORD dwProcessID);
private:
	bool postkeymsg(DWORD vkcode,HWND hwnd);
	void enumChildWin_PostEnter(HWND parent);
   
};

