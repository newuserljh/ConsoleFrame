#pragma once
#include "baseaddr.h"
#include "role.h"

#include<algorithm>
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
	bool Run_or_Step_To(DWORD x, DWORD y, DWORD run_step_flag);
	HWND GetHwndByPid(DWORD dwProcessID);
	float caclDistance(DWORD x1, DWORD y1, DWORD x2, DWORD y2);
	//DWORD Find_T_Monster(role& r, std::vector<std::string>& vec);
	std::vector<MONSTER_PROPERTY> sort_aroud_monster(role& r, std::vector<std::string>& vec, DWORD e_range=15);
	std::vector<GROUND_GOODS> sort_groud_goods(role& r, std::vector<std::string>& vec);
	MapXY splitXY(std::string str);
	bool team_open_close(DWORD tflag);
	bool maketeam(std::string pName);
	bool allowteam(std::string pName);
	bool release_Promenade(void);
	bool start_end_AutoAttack(int turn_flag);
	bool OpenRecovry(unsigned vip_level);
	bool RecovryGoods(DWORD goodsId);
	bool RecovryGoods_To_Exp(DWORD goodsId, unsigned	vip_level);
	bool RecovryGoods_To_Gold(DWORD goodsId, unsigned	vip_level);
private:
	static bool comp(const MONSTER_PROPERTY& a, const MONSTER_PROPERTY& b);
	static bool comp_groud(const GROUND_GOODS& a, const GROUND_GOODS& b);
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
函数功能:寻路到当前地图指定坐标4格以内
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
函数功能:走到指定坐标，每次走一格需要连续调用
参数一:坐标x
参数二:坐标y
参数三：走跑标记，1为走，2为跑
*/
bool gamecall::Run_or_Step_To(DWORD x, DWORD y,DWORD run_step_flag)
{
	DWORD call_addr;
	if (run_step_flag == 1)call_addr = CALL_STEP_TO_MAP;
	else if (run_step_flag == 2)call_addr = CALL_RUN_TO_MAP;
	else return false;
	try
	{
		_asm
		{
			pushad
			push y
			push x
			mov ecx, dword ptr ds : [CALL_ECX]
			mov edx, call_addr
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
			push edx
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
float gamecall::caclDistance(DWORD x1, DWORD y1, DWORD x2, DWORD y2)
{
	int x, y;
	x = x1 - x2;
	y = y1 - y2; 
	return (float)sqrt(abs(x * x) + abs(y * y));
}

///*
//函数功能:找到要攻击的怪物
//参数一:角色结构体
//参数二:攻击怪物列表
//返回值：返回找到的怪物对象指针
//*/
//DWORD gamecall::Find_T_Monster(role& r, std:: vector<std::string>& vec)
//{
//	std::vector<DWORD>  near_Mon_7;
//	r.Get_Envionment(near_Mon_7, 7); /*找到9格已内的怪物*/
//	if (near_Mon_7.size())
//	{
//		for (unsigned i = 0; i < near_Mon_7.size(); i++)
//		{
//			for (unsigned j = 0; j < vec.size(); j++)
//			{
//				if (strcmp((CHAR*)(near_Mon_7[i] + 0x10), vec[j].c_str()) == 0) /*strcmp返回0代表相等*/
//					return near_Mon_7[i];
//			}
//		}
//	}
//	return 0;
//}


/*
函数功能:怪物距离sort排序函数
参数一:怪物对象
参数二:怪物对象
返回值：bool
*/
bool gamecall:: comp(const MONSTER_PROPERTY& a, const MONSTER_PROPERTY& b)
{
	if (a.Distance < b.Distance) {
		return true;
	}
	else if (a.Distance == b.Distance && a.ID < b.ID) {
		return true;
	}
	else {
		return false;
	}
}
/*
函数功能:刷新周围怪物按距离排序
参数一:角色结构体
参数二:攻击怪物列表
参数三:遍历范围默认15
返回值：返回按距离排序的15格以内的攻击怪物怪物列表
*/
std::vector<MONSTER_PROPERTY> gamecall::sort_aroud_monster(role& r, std::vector<std::string>& vec,DWORD e_range)
{
	std::vector<MONSTER_PROPERTY> ret;
	std::vector<DWORD>  near_Mon;
	r.Get_Envionment(near_Mon, e_range); /*找到7格已内的怪物*/
	if (near_Mon.size())
	{
		for (unsigned i = 0; i < near_Mon.size(); i++)
		{
			for (unsigned j = 0; j < vec.size(); j++)
			{
				if (strcmp((CHAR*)(near_Mon[i] + 0x10), vec[j].c_str()) == 0) /*strcmp返回0代表相等*/
				{
					MONSTER_PROPERTY temp((DWORD*)near_Mon[i]);
					temp.Distance = caclDistance(*r.m_roleproperty.Object.X, *r.m_roleproperty.Object.Y, *temp.X, *temp.Y);
					ret.push_back(temp);
				}
			}
		}
	}
	std::sort(ret.begin(), ret.end(), &comp);
	return ret;
}


/*
函数功能:地面物品距离sort排序函数
参数一:地面对象
参数二:地面对象
返回值：bool
*/
bool gamecall::comp_groud(const GROUND_GOODS& a, const GROUND_GOODS& b)
{
	if (a.Distance <b.Distance) {
		return true;
	}
	else {
		return false;
	}
}
/*
函数功能:刷新地面物品按距离排序
参数一:角色结构体
参数二:拾取物品列表
返回值：返回按距离排序的15格以内的需要拾取的地面物品列表
*/
std::vector<GROUND_GOODS> gamecall::sort_groud_goods(role& r, std::vector<std::string>& vec)
{
	std::vector<GROUND_GOODS> ret;
	std::vector<DWORD>  near_groud;
	r.Get_Ground(near_groud, 15);
	if (near_groud.size())
	{
		for (unsigned i = 0; i < near_groud.size(); i++)
		{
			for (unsigned j = 0; j < vec.size(); j++)
			{
				if (strcmp((char*)(near_groud[i] + 0x18), vec[j].c_str()) == 0) /*strcmp返回0代表相等*/
				{
					GROUND_GOODS temp((DWORD*)near_groud[i]);
					temp.Distance = caclDistance(*r.m_roleproperty.Object.X, *r.m_roleproperty.Object.Y, *temp.X, *temp.Y);
					ret.push_back(temp);
					break;
				}
			}
		}
	}
	if (ret.size()>1)
	{
		std::sort(ret.begin(), ret.end(), &comp_groud);
	}

	return ret;
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
/*
函数功能:发起组队请求
参数一:开关标志 1开 0关
返回值：bool
*/
bool gamecall::team_open_close(DWORD tflag)
{
	try
	{
		_asm
		{
			pushad
			push tflag
			mov ecx, dword ptr ds : [CALL_ECX]
			mov edx, CALL_TEAM_OPEN_CLOSE
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
函数功能:发起组队请求
参数一:队员名字
返回值：bool
*/
bool gamecall::maketeam(std::string pName)
{
	const char* cname = pName.c_str();
	try
	{
		_asm
		{
			pushad
			push cname
			mov ecx, dword ptr ds : [CALL_ECX]
			mov edx, CALL_MAKE_TEAM
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
函数功能:同意组队请求
参数一:队长名字
返回值：bool
*/
bool gamecall::allowteam(std::string pName)
{
	const char* cname = pName.c_str();
	try
	{
		_asm
		{
			pushad
			push 1
			push cname
			mov ecx, dword ptr ds : [CALL_ECX]
			mov edx, CALL_ALLOW_TEAM
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
函数功能:释放元神
参数一:0
返回值：bool
*/
bool  gamecall::release_Promenade(void)
{
	try
	{
		_asm
		{
			pushad
			mov ecx, dword ptr ds : [CALL_ECX]
			mov edx, CALL_RELEASE_PROMENADE
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
函数功能:开关自动挂机
参数一:挂机标志，1为开，0为关
返回值：bool
*/
bool  gamecall::start_end_AutoAttack(int turn_flag)
{
	try
	{
		_asm
		{
			pushad
			push turn_flag
			mov ecx, 0x3D6B4F0
			mov edx, CALL_START_END_ATTACK
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
函数功能:打开自动回收对话框
参数一:VIP等级, vip等级>=1时，使用随身包裹回收，VIP等级为0时，使用NPC回收
返回值：bool
*/
bool  gamecall::OpenRecovry(unsigned vip_level)
{
	if (vip_level>0)
	{
		try
		{
			_asm
			{
				pushad
				push 0xFFFF
				push 0xFFFF
				push 0
				push 0
				push 0xF14DE0
				push 0xFFFFFFEC
				mov ecx, dword ptr ds : [CALL_ECX]
				mov edx, CALL_OPEN_BAG_REC
				call edx
				popad
			}
		}
		catch (...)
		{
			return false;
		}
	}
	else
	{
		if (OpendNPC(NPC_ZJ_RCV))
		{
			if ((*(DWORD*)P_CURRENT_NPC_DLG == NPC_ZJ_RCV))return true;
		}
		return false;
	}
	return true;
}

/*
函数功能:物品回收，根据对话框选择的回收经验或者元宝金币
参数一:物品ID
返回值：bool
*/
bool  gamecall::RecovryGoods(DWORD goodsId)
{
	try
	{
		_asm
		{
			pushad
			push goodsId
			mov ecx,P_CURRENT_NPC_DLG
			mov ecx, dword ptr ds : [ecx]
			push ecx
			mov ecx, dword ptr ds : [CALL_ECX]
			mov edx, CALL_RCV_GOODS
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
函数功能:物品回收经验
参数一:物品ID
参数二:VIP等级
返回值：bool
*/
bool  gamecall::RecovryGoods_To_Exp(DWORD goodsId,unsigned vip_level)
{
	OpenRecovry(vip_level);
	ChooseCmd("@Recovery_Exp_KaiQu");
	bool rtn= RecovryGoods(goodsId);
	ChooseCmd("@Recovery_Exp_KaiQu");
	return rtn;
}

/*
函数功能:物品回收元宝或金币
参数一:物品ID
参数二:VIP等级
返回值：bool
*/
bool  gamecall::RecovryGoods_To_Gold(DWORD goodsId, unsigned vip_level)
{
	OpenRecovry(vip_level);
	ChooseCmd("@Recovery_Gold_KaiQu");
	bool rtn= RecovryGoods(goodsId);
	ChooseCmd("@Recovery_Gold_KaiQu");
	return	rtn;
}
