#include "login.h"

login::login(){}

login::~login(){}

/*��¼��ϷCall*/
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

/*w���ܰ���call,�����������,�س�����Ч*/
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

/*���س��� ����:����PID
ԭ�����pid��������ھ��,
֮������Ӵ��ڼ��ؼ��ľ��
���ͻس���Ϣ*/
bool login::pressEnter(DWORD pid)
{
	enumChildWin_PostEnter(GetHwndByPid(pid));
	return true;

}

//ͨ��pid��ȡ���ھ��
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
				retHwnd = h;    //���ж�����ֵ
				//    /*
				//char buf[MAX_PATH] = { 0 };
				//sprintf_s(buf, "%0x", h);
				//MessageBox(NULL, buf, "��ʾ", MB_OK);
				//    */
			}
		}
		h = GetNextWindow(h, GW_HWNDNEXT);
	}
	return retHwnd;
}

/* �����Ӵ��ڼ��ؼ��ľ��
���ͻس���Ϣ */
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


























