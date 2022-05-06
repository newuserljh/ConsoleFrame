#include "login.h"

login::login(){}

login::~login(){}

/*登录游戏Call*/
bool login::game_login(const char* user, const char* pass)
{
	__try
	{
		_asm
		{
			pushad
			push pass
			push user
			push 0x8C00CFE5
			mov ecx, dword ptr ds : [0x135FB10]
			mov edx,0x689B90
			call edx
			popad
		}
	}
	__except(1)
	{
		return false;
	}
	return true;

}

/*w万能按键call,参数虚拟键码,回车键无效*/
bool login::presskey(int vkcode)
{
	int hcode = ::MapVirtualKey(vkcode, MAPVK_VK_TO_VSC);
	__try
	{
		_asm {
			pushad 
			push hcode
			push vkcode
			mov ecx, dword ptr ds : [0x0135FB08]
			mov edx,0x62C750
			call edx
			popad
		}
	}
	__except (1)
	{
		return false;
	}
	return true;

}



bool login::postkeymsg(DWORD vkcode,HWND hwnd)
{
	int hcode = ::MapVirtualKey(vkcode, MAPVK_VK_TO_VSC);
	::PostMessage(hwnd, WM_KEYDOWN, VK_RETURN, 0x1C0001);
	Sleep(10);
	::PostMessage(hwnd, WM_KEYUP, VK_RETURN, 0x1C0001);
	return true;
}

/*按回车键 参数:进程PID
原理根据pid获得主窗口句柄,
之后遍历子窗口及控件的句柄
发送回车消息*/
bool login::pressEnter(DWORD pid)
{
	enumChildWin_PostEnter(GetHwndByPid(pid));
	return true;

}

//通过pid获取窗口句柄
HWND login::GetHwndByPid(DWORD dwProcessID)
{
	HWND h = GetTopWindow(0);
	HWND retHwnd = NULL;
	while (h)
	{
		DWORD pid = 0;
		DWORD dwTheardId = GetWindowThreadProcessId(h, &pid);
		if (dwTheardId != 0)
		{
			if (pid == dwProcessID && GetParent(h) == NULL && ::IsWindowVisible(h))
			{
				retHwnd = h;    //会有多个相等值
				//    /*
				//char buf[MAX_PATH] = { 0 };
				//sprintf_s(buf, "%0x", h);
				//MessageBox(NULL, buf, "提示", MB_OK);
				//    */
			}
		}
		h = GetNextWindow(h, GW_HWNDNEXT);
	}
	return retHwnd;
}

/* 遍历子窗口及控件的句柄
发送回车消息 */
void login::enumChildWin_PostEnter(HWND parent)
{
	DWORD dwProcId;
	::GetWindowThreadProcessId(parent, &dwProcId);
	HWND hWnd = GetForegroundWindow();
	//char buf[1000];
	while (hWnd)
	{
		DWORD ProcId;
		GetWindowThreadProcessId(hWnd, &ProcId);
		if (ProcId == dwProcId)
		{
			// ::GetWindowText(hWnd,buf, 300);
			//std::cout << buf << std::endl;
			postkeymsg(VK_RETURN, hWnd);
		}
		hWnd = GetNextWindow(hWnd, GW_HWNDNEXT);
	}
}


























