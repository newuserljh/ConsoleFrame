#pragma once
#include "baseaddr.h"
#include "role.h"
/*
游戏功能CALL类
*/
class gamecall
{
public:
	gamecall(){}
	~gamecall(){}
	bool loginGame(const char* user, const char* pass);
	bool useGoods(int goodsIndex);
	bool useSkillTo(DWORD skillId, DWORD x, DWORD y, DWORD targetId = 0);
	bool CurrentMapMove(DWORD x, DWORD y);
	bool OpendNPC(DWORD npcID);
	bool ChooseCmd(char* command);
	bool pickupGoods(DWORD x,DWORD y);
	bool presskey(int vkcode);
	bool presskey(DWORD pid, int vkcode = VK_RETURN);
	HWND GetHwndByPid(DWORD dwProcessID);
	unsigned caclDistance(DWORD x1, DWORD y1, DWORD x2, DWORD y2);
	DWORD Find_T_Monster(role& r, std::vector<std::string>& vec);
	MapXY splitXY(std::string str);
private:

};


/*
函数功能:使用背包物品
参数一:物品在背包的位置(下标)
*/
bool gamecall::useGoods(int goodsIndex)
{
	try
	{
		_asm
		{
			pushad
			push 1
			push 0
			push 0
			push goodsIndex   //背包格子下标从0开始，快捷栏1-6为60-65  有VIP包裹 60-119 快捷栏120-125  
			mov ecx, dword ptr ds : [CALL_ECX]
			mov edx,CALL_USEGOODS
			call edx
			popad
		}
	}
	catch (...)
	{
		return false;
	}
	return true;
}

/*
函数功能:对目标(指定坐标)使用技能
参数一:技能ID
参数二:坐标x
参数三:坐标y
参数四:目标ID (可以缺省为0)
*/
bool gamecall::useSkillTo(DWORD skillId,DWORD x,DWORD y,DWORD targetId)
{
	try
	{
		_asm
		{
			pushad
			push skillId
			push y
			push x
			push targetId   
			mov ecx, dword ptr ds : [CALL_ECX]
			mov edx, CALL_USESKILL
			call edx
			popad
		}
	}
	catch (...)
	{
		return false;
	}
	return true;
}


/*
函数功能:寻路到当前地图指定坐标
参数一:坐标x
参数二:坐标y
*/
bool gamecall::CurrentMapMove(DWORD x, DWORD y)
{
	try
	{
		_asm
		{
			pushad
			push 0
			push y
			push x
			mov ecx, dword ptr ds : [CALL_ECX_1]
			mov edx, CALL_CURRENT_MAPMOVE
			call edx
			popad
		}
	}
	catch (...)
	{
		return false;
	}
	return true;
}

/*
函数功能:打开NPC
参数一:NPC的ID
*/
bool gamecall::OpendNPC(DWORD npcID)
{
	try
	{
		_asm
		{
			pushad
			push npcID
			mov edx, CALL_OPENNPC
			call edx
			popad
		}
	}
	catch (...)
	{
		return false;
	}
	return true;
}

/*
函数功能:选择对话
参数一:对话命令
*/
bool gamecall::ChooseCmd(char* command)
{
	try
	{
		_asm
		{
			pushad
			push 0xffff
			push 0xffff
			push 0
			push 0
			push command
			mov ecx, dword ptr ds : [CALL_ECX]
			mov edx, dword ptr ds : [CALL_EDX]
			mov edi, CALL_CHOOSE_CMD
			call edi
			popad
		}
	}
	catch (...)
	{
		return false;
	}
	return true;
}

/*
函数功能:拾取地面物品
参数一:坐标x
参数二:坐标y
*/
bool gamecall::pickupGoods(DWORD x,DWORD y)
{
	try
	{
		_asm
		{
			pushad
			push y
			push x
			mov ecx, dword ptr ds : [CALL_ECX]
			mov edx, CALL_PICKUP_GOODS
			call edx
			popad
		}
	}
	catch (...)
	{
		return false;
	}
	return true;
}

/*
函数功能:登录游戏
参数一:用户名	
参数二:密码
*/
bool gamecall::loginGame(const char* user,const char* pass)
{
	try
	{
		_asm
		{
			pushad
			push pass
			push user
			push ID_LONGIN
			mov ecx, dword ptr ds : [CALL_ECX]
			mov edx, CALL_LOGIN
			call edx
			popad
		}
	}
	catch (...)
	{
		return false;
	}
	return true;
}


/*
函数功能:模拟按键 不支持回车键
参数一:按键码
*/
bool gamecall::presskey(int vkcode)
{
	int hcode = ::MapVirtualKey(vkcode, MAPVK_VK_TO_VSC);//取硬件扫描码
	try
	{
		_asm
		{
			pushad
			push hcode
			push vkcode
			mov ecx, dword ptr ds : [CALL_ECX_2]
			mov edx, CALL_PRESSKEY
			call edx
			popad
		}
	}
	catch (...)
	{
		return false;
	}
	return true;
}

/*
函数功能:根据进程pid获取窗口句柄
参数一:进程PID
*/
HWND gamecall::GetHwndByPid(DWORD dwProcessID)
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
			}
		}
		h = GetNextWindow(h, GW_HWNDNEXT);
	}
	return retHwnd;
}

/*
函数功能:向HWND及其所有子控件发送指定按键消息
参数一:指定的父窗口句柄
参数二:消息 缺省为回车键
*/

/*
函数功能:postMessage模拟按键 支持回车键
参数一:进程PID
参数一:按键码 缺省为回车
*/
bool gamecall::presskey(DWORD pid,int vkcode)
{
	int hcode = ::MapVirtualKey(vkcode, MAPVK_VK_TO_VSC);//取硬件扫描码
	DWORD dwProcId;
	::GetWindowThreadProcessId(GetHwndByPid(pid), &dwProcId);
	HWND hWnd = GetForegroundWindow();
	//char buf[1000];
	while (hWnd)
	{
		DWORD ProcId;
		GetWindowThreadProcessId(hWnd, &ProcId);
		if (ProcId == dwProcId)
		{
			::PostMessage(hWnd, WM_KEYDOWN, vkcode, hcode);
			Sleep(10);
			::PostMessage(hWnd, WM_KEYUP, vkcode, hcode);
		}
		hWnd = GetNextWindow(hWnd, GW_HWNDNEXT);
	}	
	return true;
}

/*
函数功能:计算坐标(x1,y1)与坐标(x2,y2)的距离
参数一:x1
参数一:y1
参数三：x2
参数四：y2
返回值：距离
*/
unsigned gamecall::caclDistance(DWORD x1,DWORD y1, DWORD x2, DWORD y2)
{
	int x, y;
	x = x1 - x2;
	if (x < 0)x = 0 - x;
	y = y1 - y2;
	if (y < 0)y = 0 - y;
	return (x > y) ? x : y;
}


/*
函数功能:找到要攻击的怪物
参数一:角色结构体
参数二:攻击怪物列表
返回值：返回找到的怪物对象指针
*/
DWORD gamecall::Find_T_Monster(role& r, std:: vector<std::string>& vec)
{
	std::vector<DWORD> near_Mon_3, near_Mon_7;
	r.Get_Envionment(near_Mon_3, 3); /*找到3格已内的怪物*/
	if (near_Mon_3.size())
	{
		for (unsigned i = 0; i < near_Mon_3.size(); i++)
		{
			for (unsigned j = 0; j < vec.size(); j++){
			
				if (strcmp((CHAR*)(near_Mon_3[i] + 0x10), vec[j].c_str()) == 0) /*strcmp返回0代表相等*/
					return near_Mon_3[i];
			}
		}
	}
	r.Get_Envionment(near_Mon_7, 7); /*找到7格已内的怪物*/
	if (near_Mon_7.size())
	{
		for (unsigned i = 0; i < near_Mon_7.size(); i++)
		{
			for (unsigned j = 0; j < vec.size(); j++)
			{
				if (strcmp((CHAR*)(near_Mon_7[i] + 0x10), vec[j].c_str()) == 0) /*strcmp返回0代表相等*/
					return near_Mon_7[i];
			}
		}
	}
	return 0;
}

/*
函数功能:分割坐标字符串
参数一:坐标字符串
参数一:返回的数组指针
返回值：坐标数组指针,如果x,y都为-1 则分割出错
*/
MapXY gamecall::splitXY(std::string str)
{
	MapXY XYtemp;
	std::regex delimiters("\\s+");
	std::vector<std::string> temp(std::sregex_token_iterator(str.begin(), str.end(), delimiters, -1), std::sregex_token_iterator());
	if (temp.size()!=2) 
	{
		XYtemp.x = -1;
		XYtemp.y = -1;
	}
	else
	{
		XYtemp.x = stoi(temp[0]); //x
		XYtemp.y = stoi(temp[1]); //y
	}
	return XYtemp;
}