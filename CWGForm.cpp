// CWGForm.cpp: 实现文件
//

#include "pch.h"
#include "CWGForm.h"
#include <vector>
#include <fstream>
#include<string>
#include <regex>

/*==========进程通信=========*/
//#define _MAP_ "gyarmy" 
//HANDLE g_hModule;
//HANDLE g_hMapFile;
//LPTSTR lpBuff;
//DWORD dwType;

using namespace std;
CWinThread* m_pThread_Environment; //遍历周围NPC 怪物
CWinThread* m_pThread_AutoAttack; //自动打怪
CWinThread* m_pThread_Goto; //打怪寻路
CWinThread* m_pThread_Mapping; //打怪寻路

CCriticalSection g_criticalsection;//*边界锁 线程同步用 
BOOL tflag/*遍历*/, tflag_auto/*打怪*/, tflag_goto/*寻路*/,tflag_map/*进程通信*/; //线程开关
HUMAN role1, role_YS; //主角色,元神; //人物结构体
GOODS role_Bag[125], ls_Bag[59], ys_Bag[59], role_ZB[19]; //主体包裹,灵兽,元神，主体装备



//int use_goods_index = -1;

//int Get_Goods_num(const char* name) //获得背包指定物品数量,最后一个物品下标赋值到use_goods_index;
//{
	//Get_Bag();
//	int goods_number = 0;
//	use_goods_index = -1;
//	for (int i = 0; i < 125; i++)
//	{
//		if (memcmp(name, baggoods[i].name, strlen(name)) == 0)
//		{
//			goods_number = goods_number + 1;
//			use_goods_index = i;
//		}
//	}
//	return goods_number;
//}

//int Find_Goods_index(const char* name) //获得背包指定物品数量,最后一个物品下标赋值到use_goods_index;
//{
//	Get_Bag();
//	for (int i = 0; i < 125; i++)
//	{
//		if (memcmp(name, baggoods[i].name, strlen(name)) == 0)return i;
//	}
//	return -1;
//}


void usebaggoods(int bag_index) //包裹格子下标
{
	_asm
	{
		push 0x1
		push 0x0
		push 0x0
		push bag_index  //背包格子下标从0开始，快捷栏1-6为60-65  有VIP包裹 60-119 快捷栏120-125                      
		mov ecx, dword ptr ds : [0x112F580]
		mov eax, 0x6D7F70
		call eax
	}
}

VOID CALLBACK 自动治疗(HWND hWnd, UINT nMsg, UINT_PTR nIDEvent, DWORD dwTime)
{
	//if (Get_Goods_num("治疗神水") > 1)
	//{
	//	usebaggoods(use_goods_index);
	//}
}
void GoTo_Current_Map(DWORD x,DWORD y)/*当前地图寻路*/
{
	_asm {
		pushad
		mov ecx, dword ptr ds : [0x4E072C4]
		push 0x0
		push y
		push x
		mov eax, GOTOBASE
		call eax
		popad
	}
}
void UseSkill_To(DWORD skill_ID/*技能ID*/,DWORD p_Target/*目标指针*/) /*对对象使用技能*/
{
	DWORD x = *(DWORD*)(p_Target + 0xac);
	DWORD y = *(DWORD*)(p_Target + 0xb0);
	DWORD t_ID = *(DWORD*)(p_Target + 8);
	_asm
	{
		pushad
		mov edx, edx
		mov eax, SKILLBASE
		mov ecx, dword ptr ds : [CALLBASE]
		push skill_ID
		push y
		push x
		push t_ID
		call eax
		popad
	}

}
BOOL If_Has_Skill(const CHAR* name/*技能名字*/)/*判断是否有某个技能*/
{
	for (unsigned i=0;i<role_Skill.size();i++)
	{
		if(!memcmp(name,(CHAR*)(role_Skill[i].Skill_Name), strlen(name)))return TRUE;
	}
    return FALSE;
}
BOOL If_Has_Skill(DWORD s_ID/*技能ID*/)/*判断是否有某个技能*/
{
	for (unsigned i = 0; i <role_Skill.size(); i++)
	{
		if (s_ID == (DWORD)(role_Skill[i].Skill_ID))return TRUE;
	}
	return FALSE;
}
unsigned Cacl_Distance(HUMAN role, DWORD t_x, DWORD t_y)/*计算角色与某坐标的距离*/
{
	int x, y;
	unsigned a, b;
	x = role.Object.XY.X - t_x;
	if (x<0)x = 0 - x;
	y = role.Object.XY.Y - t_y;
	if (y < 0)y = 0 - y;
	a = (unsigned)x;
	b = (unsigned)y;
	return (a > b) ? a : b;       

}
DWORD Find_T_Monster/*找到要攻击的怪物*/(HUMAN role/*角色结构体*/, vector<CHAR*>& vec/*攻击怪物列表*/)/*返回找到的怪物对象指针*/
{
	vector<DWORD> near_Mon_3, near_Mon_7;
	Get_Envionment(role.Object.XY.X, role.Object.XY.Y, near_Mon_3, Envi_Offset, 3); /*找到3格已内的怪物*/
	if (near_Mon_3.size())
	{
		for (unsigned i = 0; i < near_Mon_3.size(); i++)
		{
			for (unsigned j = 0; j < vec.size(); j++)
			{
				if (strcmp((CHAR*)(near_Mon_3[i]+0x10), vec[j])==0) /*strcmp返回0代表不相等*/
					return near_Mon_3[i];
			}
		}
	}
	Get_Envionment(role.Object.XY.X, role.Object.XY.Y, near_Mon_7, Envi_Offset, 7); /*找到7格已内的怪物*/
	if (near_Mon_7.size())
	{
		for (unsigned i = 0; i < near_Mon_7.size(); i++)
		{
			for (unsigned j = 0; j < vec.size(); j++)
			{
				if (strcmp((CHAR*)(near_Mon_7[i] + 0x10), vec[j]) == 0) /*strcmp返回0代表不相等*/
					return near_Mon_7[i];
			}
		}
	}
	return 0;
}
DWORD Choose_Moster(HUMAN role/*角色结构体*/, vector<CHAR*>& vec/*攻击怪物列表*/)/*返回选中怪物对象指针*/
{
	DWORD Object_ID = *(DWORD*)(role.p_Target_ID);
	while (Object_ID==0)
	{
		Object_ID = Find_T_Monster(role, vec);
		if (Object_ID)
		{
			*(DWORD*)(role.p_Target_ID) = *(DWORD*)(Object_ID + 0x8);
			m_pThread_Goto->SuspendThread();
		}
		else
		{
			/*边寻路边找怪*/
			m_pThread_Goto->ResumeThread();	
			Sleep(500);
		}
	}
	return Object_ID;
}

BOOL Auto_Attack/*自动打怪*/(HUMAN role/*角色结构体*/, vector<CHAR*>& vec/*攻击怪物列表*/, DWORD s_ID/*使用技能ID*/)
{
	*(DWORD*)role.p_Target_ID = 0;
	DWORD p_Target/*对象指针*/ =  Choose_Moster(role, vec);
	unsigned i = 0;/*记录攻击次数*/
	while((*(DWORD*)(p_Target + 0x80)>0)&&(*(DWORD*)(p_Target+0x8)==*(DWORD*)role.p_Target_ID)&&(i<600))
	{
	
		if (Cacl_Distance(role, *(DWORD*)(p_Target + 0xac), *(DWORD*)(p_Target + 0xb0)) > 9)return FALSE;
		UseSkill_To(s_ID, p_Target);
		i++;
		Sleep(ATTACK_SLEEP);
	}
	return true;
}
DWORD Load_Settings(HUMAN role/*角色结构体*/) /*返回使用技能ID*/
{
	/*技能设置*/
	DWORD s_ID = 0;
	if (If_Has_Skill("狂龙紫电"))s_ID = 0x47;
	else if (If_Has_Skill("雷电术"))s_ID = 11;
	else if (If_Has_Skill("冰箭术"))s_ID = 49;
	else if (If_Has_Skill("小火球"))s_ID = 1;
	else s_ID = 0;
	return s_ID;
}

vector<MAP_XY> split_XY(vector<string>& vec)/*分割坐标字符串转化位坐标结构体*/
{
	vector<MAP_XY> rvec;
	MAP_XY xy_temp;
	for (unsigned i=0;i<vec.size();i++)
	{
		std::regex delimiters("\\s+");
		vector<string> temp(sregex_token_iterator(vec[i].begin(),vec[i].end(),delimiters,-1), sregex_token_iterator());

		xy_temp.X = stoi(temp[0]);
		xy_temp.Y = stoi(temp[1]);
		rvec.push_back(xy_temp);
	}
	return rvec;
}

void 使用物品(const char* name) //物品名字
{
	//int index = Find_Goods_index(name);
	//usebaggoods(index);

}

void 点击对话框(const char* dlg_name) //对话内容
{
	_asm
	{
		mov edx, dword ptr ds : [0x00EED594]
		mov ecx, dword ptr ds : [0x0112F580]
		push 0xFFFF
		push 0xFFFF
		push 0x0
		push 0x0
		push dlg_name         //存储对话框命令地址 @获取神御    
		push edx
		mov eax, 0x670B70
		call  eax
	}
}

void 自动回收()
{
	const char* dlg_cmd_shenyu = "@获取神御"; //神於
	const char* dlg_cmd_sanbei = "@获得三倍经验"; //三倍
	const char* dlg_cmd_yijian = "@一件回收路"; //一键回收
	const char* dlg_cmd_jiangjun = "@旭日.一键&将军系列&1&灵力"; //一键回收将军
	const char* dlg_cmd_nimo = "@旭日.一键&逆魔系列&5&灵力"; //一键回收逆魔
	const char* dlg_cmd_tiexue = "@旭日.一键&铁血技能系列&4&灵力"; //一键回收铁血
	使用物品("赞助包天卡");
	Sleep(2000);
	点击对话框(dlg_cmd_shenyu);
	Sleep(500);
	点击对话框(dlg_cmd_sanbei);
	Sleep(500);
	点击对话框(dlg_cmd_yijian);
	Sleep(500);;
	点击对话框(dlg_cmd_jiangjun);
	Sleep(500);;
	点击对话框(dlg_cmd_nimo);
	Sleep(500);;
	点击对话框(dlg_cmd_tiexue);
}





BOOL CWGForm::OnInitDialog()
{
	/*========初始化人物，元神状态技能============*/
	CDialogEx::OnInitDialog();
	tflag = true;
	tflag_auto = true;
	tflag_goto = true;

	m_Monster_Listctrl.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES); // 整行选择、网格线
	m_Monster_Listctrl.InsertColumn(0, _T(""), LVCFMT_LEFT, 0);
	m_Monster_Listctrl.InsertColumn(1, _T("序号"), LVCFMT_LEFT, 100); // 插入第2列的列名
	m_Monster_Listctrl.InsertColumn(2, _T("名字"), LVCFMT_LEFT, 100); // 插入第3列的列名
	m_Monster_Listctrl.InsertColumn(3, _T("指针"), LVCFMT_LEFT, 100); // 插入第4列的列名

	role1 = Get_Human_State(RoleBase);
	role_YS = Get_Human_State(YS_Base);
	Get_Skill(role1.p_Skill_Base, role_Skill);
	Get_Skill(role_YS.p_Skill_Base, YS_Skill);
	/*=============启动线程3s遍历一次人物状态和环境地面==================*/
	m_pThread_Environment = AfxBeginThread(ThreadProc_Environmrnt, this);
	//m_pThread_Environment = AfxBeginThread(ThreadProc_Mapping, this);
	//m_pThread_Environment->ResumeThread();
	// TODO:  在此添加额外的初始化

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

UINT __cdecl CWGForm::ThreadProc_Mapping(LPVOID nParam) /*进程通信*/
{
	//dwType = 0;
	//g_hMapFile = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, _MAP_);	//打开共享内存
	//if (g_hMapFile == NULL)
	//{
	//	AfxMessageBox("OpenFileMapping Error: %d\n",MB_OK);
	//	return -1;
	//}	
	//lpBuff = (LPTSTR)MapViewOfFile(g_hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, BUFSIZ);//映射内存
	//if (lpBuff == NULL)
	//{
	//	AfxMessageBox("OpenFileMapping Error: %d\n", MB_OK);
	//	return -1;
	//}
	//while (tflag_map)
	//{
	//	CHAR* lpContent = "Test!!!";
	//	//dwType = 1234;	
	//	//strcpy((char*)lpBuff, lpContent);
	//	CopyMemory(lpBuff, &hwnd_count,sizeof(hwnd_count));
	//	Sleep(1000);
	//}
	//return 0;   // thread completed successfully
}
UINT __cdecl CWGForm::ThreadProc_Goto(LPVOID nParam)
{
	vector<MAP_XY> *xy = (vector<MAP_XY>*)nParam;
	auto it = xy->begin();
	while (tflag_goto&&(it!=xy->end()))
	{

		while (Cacl_Distance(role1, it->X, it->Y) > 3)
		{
			GoTo_Current_Map(it->X, it->Y);
			Sleep(500);
		}
		++it;
		if (it== xy->end())it= xy->begin();
	}
	return 0;   // thread completed successfully
}
UINT __cdecl CWGForm::ThreadProc_AutoAttack(LPVOID nParam) //自动打怪
{
	CWGForm* threadol = (CWGForm*)nParam;
	vector<CHAR*> attack_monster;
	/*========读取打怪列表=============================================================*/
	vector<string> vec_t = ReadTxt("G:\\VS_Projects\\GameAuto\\Release\\AttackMonster.txt");
	for (unsigned i = 0; i < vec_t.size(); i++)
	{
		attack_monster.push_back((CHAR*)vec_t[i].data());
	}
	/*========读取当前地图寻路点坐标=============================================================*/
	vector<string> vec_xy = ReadTxt("G:\\VS_Projects\\GameAuto\\Release\\逆魔大殿.txt");
	vector<MAP_XY>map_Auto_Goto = split_XY(vec_xy);
    /*=================设置打怪技能=========================================================*/
	DWORD Skill_ID=Load_Settings(role1);

	while (tflag_goto)
	{
		g_criticalsection.Lock();  //边界锁锁定 边界锁中间添加需要保护的代码，如读写文件的能

		g_criticalsection.Unlock(); //边界锁解除

		Auto_Attack(role1, attack_monster,Skill_ID);

	}
	return 0;   // thread completed successfully
}
UINT __cdecl CWGForm::ThreadProc_Environmrnt(LPVOID nParam) //遍历周围怪物NPC
{

	CWGForm* threadol = (CWGForm*)nParam;
	while (tflag)
	{

		role1 = Get_Human_State(RoleBase);
		role_YS = Get_Human_State(YS_Base);	
		Get_Envionment(role1.Object.XY.X, role1.Object.XY.Y, p_Moster, Envi_Offset);//遍历NPC怪物
		Get_Envionment(role1.Object.XY.X, role1.Object.XY.Y, p_Ground_Goods, Ground_Offset);//遍历地面
		Get_Bag_ZB(role_Bag, role1.p_Bag_Base, role1.Bag_Size); //包裹
		Get_Bag_ZB(ls_Bag, role1.p_LR_Bag_Base, role1.LR_Bag_Size);//灵兽包裹
		Get_Bag_ZB(role_ZB, role1.p_ZB, 19);//主体装备	
		CString strName, str指针, strNum,st;
		g_criticalsection.Lock();  //边界锁锁定 边界锁中间添加需要保护的代码，如读写文件的能
		g_criticalsection.Unlock(); //边界锁解除
		threadol->m_Monster_Listctrl.DeleteAllItems();
		for (size_t i = 0; i < p_Moster.size(); i++)
		{
			strNum.Format(_T("%d"), i + 1);
			str指针.Format(_T("%x"), p_Moster[i]);
			strName.Format(_T("%s"), (CHAR*)(p_Moster[i] + 0x10));
			threadol->m_Monster_Listctrl.InsertItem(i, _T("")); // 插入行
			threadol->m_Monster_Listctrl.SetItemText(i, 1, strNum); // 设置第2列(序号)
			threadol->m_Monster_Listctrl.SetItemText(i, 2, strName); // 设置第3列(名字)
			threadol->m_Monster_Listctrl.SetItemText(i, 3, str指针); // 设置第4列(坐标)
			strName = str指针 = strNum = "";
		}	
		Sleep(3000);
		p_Moster.clear();
		p_Ground_Goods.clear();
	}
	return 0;   // thread completed successfully
}

void CWGForm::OnDestroy()
{
	CDialogEx::OnDestroy();
	// TODO: 在此处添加消息处理程序代码
	tflag = FALSE;/*遍历*/
	tflag_goto = FALSE;/*寻路*/
	tflag_auto = FALSE;/*打怪*/
	WaitForSingleObject(m_pThread_Environment, 100);
	WaitForSingleObject(m_pThread_Goto, 100);
	WaitForSingleObject(m_pThread_AutoAttack, 100);

	if (m_pThread_Goto->m_hThread)
	{
		TerminateThread(m_pThread_Goto->m_hThread, EXIT_FAILURE);
		CloseHandle(m_pThread_Goto->m_hThread);
	}

	if (m_pThread_Environment->m_hThread)
	{
		TerminateThread(m_pThread_Environment->m_hThread, EXIT_FAILURE);
		CloseHandle(m_pThread_Environment->m_hThread);
	}
	if (m_pThread_AutoAttack->m_hThread)
	{
		TerminateThread(m_pThread_AutoAttack->m_hThread, EXIT_FAILURE);
		CloseHandle(m_pThread_AutoAttack->m_hThread);
	}

	//g_criticalsection.Lock();
	////file.Close();
	//g_criticalsection.Unlock();
}

void CWGForm::OnBnClickedBtnCalltest()
{
	// TODO: 在此添加控件通知处理程序代码
	//SetTimer(ID_TIMER_ZHILIAO, ZHILIAO_ELAPSE, &自动治疗);
	//AfxMessageBox(str坐标, MB_OK);
	//SetTimer(ID_TIMER_HUISHOU, HUISHOU_ELAPSE, &自动回收);
	CString s, st;
	/*=================启动打怪寻路线程=========================================================*/
	//m_pThread_AutoAttack = AfxBeginThread(ThreadProc_AutoAttack, this, THREAD_PRIORITY_NORMAL, 0, 0);

	///*========读取当前地图寻路点坐标=============================================================*/
	//vector<string> vec_t = ReadTxt("G:\\VS_Projects\\GameAuto\\Release\\逆魔大殿.txt");
	//vector<MAP_XY> map_Auto_Goto = split_XY(vec_t);

	///*=================启动寻路线程=========================================================*/
	//m_pThread_Goto = AfxBeginThread(ThreadProc_Goto, (LPVOID)(&map_Auto_Goto), THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);


	//for (unsigned i = 0; i < p_Moster.size(); i++)
	//{
	//	st.Format(_T("%s\n"), (CHAR*)(p_Moster[i] + 0x10));
	//	s = st + s;
	//}
	//st.Format(_T("%d,%d\n"), role1.Object.XY.X, role1.Object.XY.Y);


}	




CHAR t = 1;
void CWGForm::OnBnClickedBtnPause()
{
	// TODO: 在此添加控件通知处理程序代码
	if (t == 1)
	{
		m_pThread_Environment->SuspendThread();
		t = 0;
		GetDlgItem(IDC_BTN_PAUSE)->SetWindowText("继续");
	}
	else
	{
		m_pThread_Environment->ResumeThread();
		t = 1;
		GetDlgItem(IDC_BTN_PAUSE)->SetWindowText("暂停");
	}
	//KillTimer(ID_TIMER_ZHILIAO);

}
void CWGForm::OnBnClickedButtonStop()
{
	// TODO: 在此添加控件通知处理程序代码
//KillTimer(ID_TIMER_ZHILIAO);
//KillTimer(ID_TIMER_HUISHOU);
//Resum寄存器((LPVOID)H_Monster_NPC_Address, HOOK_Monster_NPC, 6, 钩子空间);
}


void CWGForm::MessageBox(DWORD dw)
{
	CString s;
	s.Format("%d", dw);
	AfxMessageBox(s, MB_OK);
}



