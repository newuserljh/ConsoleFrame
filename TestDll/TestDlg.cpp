// TestDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "TestDll.h"
#include "TestDlg.h"
#include "afxdialogex.h"

#include "monster.h"
#include "skill.h"
#include "bag.h"
#include "utils.h"
#include "gamecall.h"
#include "shareMemoryCli.h"
#pragma comment(lib , "Common.lib")
#pragma comment(lib ,"GameData.lib")
//#include "HookAPI.h"
#include "HookReg.h"
// CTestDlg 对话框
shareMemoryCli shareCli(MORE_OPEN_NUMBER);
int shareindex = -1;
//初始化HOOk
HookReg hook;
role r;//角色
monster m_mon;
skill m_skill;
bag r_bag;
gamecall mfun;

/*打怪技能设置*/
DWORD s_ID = -1;
CWinThread* m_threadAttack;
bool tflag_attack = true;

/*寻路打怪坐标*/
std::vector<MapXY> map_xy;
CWinThread* m_threadGoto;
bool tflag_goto = true;


/*捡物设置*/
std::vector<std::string> pick_goods_list;
CWinThread* m_threadPickup;
bool tflag_pickup = true;

_declspec(naked) void CallTest()
{
	_asm pushad
	if (hook.EAX != 0)
	{
		tools::getInstance()->message("错误!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
		shareCli.m_pSMAllData->m_sm_data[shareindex].server_alive = false;//验证外挂存活
	}
	_asm  popad
	_asm ret
}



IMPLEMENT_DYNAMIC(CTestDlg, CDialogEx)

CTestDlg::CTestDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTestDlg::IDD, pParent)
{

}

CTestDlg::~CTestDlg()
{
}

void CTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT2, m_edit2);
}


BEGIN_MESSAGE_MAP(CTestDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CTestDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CTestDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CTestDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON8, &CTestDlg::OnBnClickedButton8)
	ON_BN_CLICKED(IDC_BUTTON5, &CTestDlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON9, &CTestDlg::OnBnClickedButton9)
END_MESSAGE_MAP()


// CTestDlg 消息处理程序

void threadLogin()
{
	int i = 0;//尝试登录次数
	do
	{
		shareCli.m_pSMAllData->m_sm_data[shareindex].cscript = "登录中...";
		Sleep(10000);
		mfun.presskey(shareCli.m_pSMAllData->m_sm_data[shareindex].ndPid);
		Sleep(100);
		mfun.loginGame(shareCli.m_pSMAllData->m_sm_data[shareindex].userName.c_str(), shareCli.m_pSMAllData->m_sm_data[shareindex].passWord.c_str());
		Sleep(1000);
		mfun.presskey(shareCli.m_pSMAllData->m_sm_data[shareindex].ndPid);
		Sleep(1000);
		mfun.presskey(shareCli.m_pSMAllData->m_sm_data[shareindex].ndPid);
		Sleep(1000);
		mfun.presskey(shareCli.m_pSMAllData->m_sm_data[shareindex].ndPid);
		Sleep(1000);
		mfun.presskey(shareCli.m_pSMAllData->m_sm_data[shareindex].ndPid);
		Sleep(1000);
		mfun.presskey(shareCli.m_pSMAllData->m_sm_data[shareindex].ndPid);
		Sleep(1000);
		mfun.presskey(shareCli.m_pSMAllData->m_sm_data[shareindex].ndPid);
		Sleep(1000);
		i++;
	} while ((!r.init()) && i < 10);
	if (!r.init())
	{
		shareCli.m_pSMAllData->m_sm_data[shareindex].cscript = "登陆失败,正在结束程序";
		shareCli.m_pSMAllData->m_sm_data[shareindex].server_alive = false;
	}
	shareCli.m_pSMAllData->m_sm_data[shareindex].cscript = "登陆完成";
	return;
}

void threadAlive()
{
	while (true)
	{
		shareCli.m_pSMAllData->m_sm_data[shareindex].rcv_rand = shareCli.m_pSMAllData->m_sm_data[shareindex].send_rand;//验证外挂存活
		Sleep(1000);
	}
}
// 追加文本到EditControl
void AppendText(CEdit &m_edit, CString strAdd)
{
	m_edit.SetSel(m_edit.GetWindowTextLength(), m_edit.GetWindowTextLength());
	m_edit.ReplaceSel(strAdd + L"\n");
}

void CTestDlg::OnBnClickedButton1()
{
	// 人物属性
	if (!r.init())return;
	CString s;
	s.Format("%s\n", r.m_roleproperty.Object.pName);
	AppendText(m_edit2,s);
	char* sex = {};
	char* job = {};
	if (*r.m_roleproperty.Sex == 0)sex = "男";
	else sex = "女";
	if (*r.m_roleproperty.Job == 0)job = "战士";
	else if (*r.m_roleproperty.Job == 1)job = "法师";
	else job = "道士";
	s.Format("职业:%s      等级:%d    性别:%s", job, *r.m_roleproperty.Level, sex);
	AppendText(m_edit2, s);;
	s.Format("HP:%d / %d", *r.m_roleproperty.Object.HP, *r.m_roleproperty.Object.HP_MAX);
	AppendText(m_edit2, s);
	s.Format("MP:%d / %d", *r.m_roleproperty.Object.MP, *r.m_roleproperty.Object.MP_MAX);
	AppendText(m_edit2, s);
	s.Format("当前地图:%s  坐标%d,%d", r.m_roleproperty.p_Current_Map, *r.m_roleproperty.Object.X, *r.m_roleproperty.Object.Y);
	AppendText(m_edit2, s);
	s.Format("背包大小:%d  背包负重%d / %d", *r.m_roleproperty.Bag_Size, *r.m_roleproperty.BAG_W, *r.m_roleproperty.BAG_W_MAX);
	AppendText(m_edit2, s);
	s.Format("ID:%x", *r.m_roleproperty.Object.ID);
	AppendText(m_edit2, s);
	for (auto i=0;i<21;i++)
	{
		if (!(*r.m_euip[i].ID))continue; 
		s.Format("%d :%s 耐久:%d/%d",i, r.m_euip[i].pName, *(r.m_euip[i].Use_Num),*(r.m_euip[i].Use_Num_Max));
		AppendText(m_edit2, s);
	}
}
//int
//WSAAPI
//MyWSARecv(
//	_In_ SOCKET s,
//	_In_reads_(dwBufferCount) __out_data_source(NETWORK) LPWSABUF lpBuffers,
//	_In_ DWORD dwBufferCount,
//	_Out_opt_ LPDWORD lpNumberOfBytesRecvd,
//	_Inout_ LPDWORD lpFlags,
//	_Inout_opt_ LPWSAOVERLAPPED lpOverlapped,
//	_In_opt_ LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine
//)
//{
//	
//	rcvryAPI(WSARecv, rcvryCode);
//   int ret= WSARecv(s, lpBuffers, dwBufferCount, lpNumberOfBytesRecvd, lpFlags, lpOverlapped, lpCompletionRoutine);
//	int er=WSAGetLastError();
//	if (er != 10035)
//	{
//		tools::getInstance()->message("错误:%d", er);
//	}
//
//	hookAPI(WSARecv, MyWSARecv);
//	return ret;
//}

BOOL CTestDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

  //初始化共享内存,取得共享内存索引
	if (!shareCli.openShareMemory())
	{
		tools::getInstance()->message("打开共享内存失败\n");
	}
	shareindex = shareCli.getIndexByPID(GetCurrentProcessId());

	//HOOK连接服务器失败代码
	hook.hookReg(0x5F8B69, 5, CallTest);

	CloseHandle(::CreateThread(NULL, NULL, LPTHREAD_START_ROUTINE(threadLogin), NULL, NULL, NULL));
	//登录成功之后设置 启动通讯线程,定时验证存活消息
	
	CloseHandle(::CreateThread(NULL, NULL, LPTHREAD_START_ROUTINE(threadAlive), NULL, NULL, NULL));


	//hookAPI(WSARecv, MyWSARecv);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}



void CTestDlg::OnBnClickedButton2()
{
	// TODO: 遍历周围对象 地面 怪物NPC
	if (!r.init())return;
	m_mon.m_monsterList.clear();
	m_mon.m_groundList.clear();
	CString s;
	if (!r.Get_Envionment(m_mon.pOb_list))
	{
		s.Format("遍历周围错误：\n");
		AppendText(m_edit2, s);
		return;
	}	
	if (!r.Get_Ground(m_mon.pGr_list))
	{
		s.Format("遍历地面错误：\n");
		AppendText(m_edit2, s);
		return;
	}
	m_mon.init();
	s.Format("周围对象列表（怪物、NPC、其他玩家、宠物）：\n");
	AppendText(m_edit2, s);
	for (auto i = 0; i < m_mon.m_monsterList.size(); i++)
	{
		s.Format("%s ID: %x\n", m_mon.m_monsterList[i].pName, *(m_mon.m_monsterList[i].ID));
		AppendText(m_edit2, s);
	}

	s.Format("地面：\n");
	AppendText(m_edit2, s);
	for (auto i = 0; i < m_mon.m_groundList.size(); i++)
	{
		s.Format(" %s : %d/%d\n", m_mon.m_groundList[i].pName, *m_mon.m_groundList[i].X, *m_mon.m_groundList[i].Y);
		AppendText(m_edit2, s);
	}

}


void CTestDlg::OnBnClickedButton3()
{
	// TODO: 技能遍历
	if (!r.init())return;
	CString s;
	m_skill.m_skillList.clear();
	m_skill.skillBase = (DWORD)r.m_roleproperty.p_Skill_Base;
	if (!m_skill.init())
	{
		s.Format("遍历技能错误：\n");
		AppendText(m_edit2, s);
		return;
	}
	s.Format("技能：\n");
	AppendText(m_edit2, s);

	for (auto i = 0; i < m_skill.m_skillList.size(); i++)
	{
		s.Format("%s  等级:%d  ID: %d\n", m_skill.m_skillList[i].pName, *(m_skill.m_skillList[i].level), *(m_skill.m_skillList[i].ID));
		AppendText(m_edit2, s);
	}

}


void CTestDlg::OnBnClickedButton8()
{
	// TODO: 背包遍历
	if (!r.init())return;
	CString s;
	r_bag.maxSize = *r.m_roleproperty.Bag_Size;
	r_bag.bagBase = (DWORD)r.m_roleproperty.p_Bag_Base;
	if(!r_bag.init())
	{
		s.Format("遍历背包错误：\n");
		AppendText(m_edit2, s);
		return;
	}
	for (auto i=0;i<r_bag.maxSize;i++)
	{
		if (*(r_bag.m_bag[i].ID))
		{
			s.Format("第%d格:%s   ID: %x\n", i, r_bag.m_bag[i].pName, *r_bag.m_bag[i].ID);
			AppendText(m_edit2, s);
		}
	}

}


void CTestDlg::OnBnClickedButton5()
{
	// TODO: 在此添加控件通知处理程序代码
}






/*
函数功能:选择打怪目标
参数一:角色结构体
参数二:攻击怪物列表
返回值：选中怪物对象指针
*/
DWORD Choose_Moster(role& r, std::vector<std::string>& vec)
{
	DWORD Object_ID = *r.m_roleproperty.p_Target_ID;
	while (Object_ID == 0)
	{
		Object_ID = mfun.Find_T_Monster(r, vec);
		if (Object_ID)
		{
			*r.m_roleproperty.p_Target_ID = *(DWORD*)(Object_ID + 0x8);
		}
		else
		{
			/*边寻路边找怪*/
			//m_threadGoto->ResumeThread();
			Sleep(2000);
			//m_threadGoto->SuspendThread();
		}
	}
	return Object_ID;
}

/*
函数功能:自动打怪
参数一:角色结构体
参数二:攻击怪物列表
参数三:使用技能ID
返回值：true为已经打死怪物，fasle为位置错误（或者600次没打死怪物）
*/
bool Auto_Attack(role r, std::vector<std::string>& vec, DWORD s_ID)
{
	*r.m_roleproperty.p_Target_ID = 0;
	DWORD p_Target = Choose_Moster(r, vec);
	
	MONSTER_PROPERTY att_mon((DWORD*)p_Target);
	unsigned i = 0;/*记录攻击次数*/
	while( (*att_mon.HP> 0) && (*att_mon.ID == *r.m_roleproperty.p_Target_ID) && (i < 600))
	{
		if (mfun.caclDistance(*r.m_roleproperty.Object.X, *r.m_roleproperty.Object.Y, *att_mon.X, *att_mon.Y) > 9)return FALSE;
		DWORD s_posion = m_skill.getSkillId("施毒术");
		if( (s_posion!=-1))//道士
		{
			if ((r_bag.ifHasPoison()>0)&& (*att_mon.IsPosion < 0x40))//是否中毒 0没毒，0x40红毒，0x80绿毒，0xc0红绿毒,
			{
				mfun.useSkillTo(s_posion, *att_mon.X, *att_mon.Y, *att_mon.ID);
				Sleep(ATTACK_SLEEP);
				mfun.useSkillTo(s_posion, *att_mon.X, *att_mon.Y, *att_mon.ID);
				Sleep(ATTACK_SLEEP);
				/*	mfun.presskey(VK_F1);
					Sleep(ATTACK_SLEEP);*/
			}
		}
		mfun.useSkillTo(s_ID, *att_mon.X, *att_mon.Y, *att_mon.ID);
		i++;
		Sleep(ATTACK_SLEEP);
	}
	return true;
}

/*
函数功能:载入设置
参数一:角色结构体
返回值：选中怪物对象指针
*/
bool Load_Settings()
{
	/*技能设置*/
	if (*r.m_roleproperty.Job == 0)/*战士*/
	{
		return false;
	}
    if (*r.m_roleproperty.Job == 1)/*法师*/
	{
		DWORD  s_tmp = m_skill.getSkillId("狂龙紫电");
		if (s_tmp != -1)
		{
			s_ID = s_tmp;
			return true;
		}
		s_tmp = m_skill.getSkillId("雷电术");
		if (s_tmp != -1)
		{
			s_ID = s_tmp;
			return true;
		}
		s_tmp = m_skill.getSkillId("冰箭术");
		if (s_tmp != -1)
		{
			s_ID = s_tmp;
			return true;
		}
		s_tmp = m_skill.getSkillId("小火球");
		if (s_tmp != -1)
		{
			s_ID = s_tmp;
			return true;
		}
		return false;
	}
	if (*r.m_roleproperty.Job == 2)/*道士*/
	{
		return false;
	}
	 return false;

}

int CTestDlg::threadAttack(void* p)
{
	CTestDlg* pDlg = (CTestDlg*)p;
	if(!Load_Settings())return 0;
	CString s;
	s.Format("打怪技能ID:%d", s_ID);
	AppendText(pDlg->m_edit2, s);
	std::vector<std::string> vec_mon = tools::getInstance()->ReadTxt("..\\逆魔大殿.txt");
	while (tflag_attack)
	{
		Auto_Attack(r, vec_mon, s_ID);
		Sleep(ATTACK_SLEEP);
	}
	return 0;
}

/*
函数功能:寻路到当前地图(x,y)附近
参数一:x
参数二:y
参数三:标识,只有为true的时候才走到目的地
返回值:bool,走完真,未走完false
*/
bool MapGoto_near(DWORD x,DWORD y, bool tflag)
{
	while (mfun.caclDistance(*r.m_roleproperty.Object.X, *r.m_roleproperty.Object.Y,x,y)>3)
	{
		mfun.CurrentMapMove(x, y);
		Sleep(500);
		if (!tflag)return false;
     }
	return true;  
}

/*
函数功能:寻路到当前地图(x,y)精确位置
参数一:x
参数二:y
参数三:标识,只有为true的时候才走到目的地
返回值:bool,走完真,未走完false
*/
bool MapGoto_Point(DWORD x, DWORD y,bool tflag)
{
	while (mfun.caclDistance(*r.m_roleproperty.Object.X, *r.m_roleproperty.Object.Y, x, y) > 0)
	{
		mfun.CurrentMapMove(x, y);
		Sleep(500);
		if (!tflag)return false;
	}
	return true;   
}


/*
函数功能:载入寻路打怪坐标
参数一:角色结构体
返回值：选中怪物对象指针
*/
bool Load_coordinate()
{
	map_xy.clear();
	std::vector<std::string> map_coordinate= tools::getInstance()->ReadTxt("..\\逆魔大殿1.txt");
	if (!map_coordinate.size())return false;
	for (size_t i = 0; i < map_coordinate.size(); i++)
	{
		MapXY xy;
		xy = mfun.splitXY(map_coordinate[i]);
		if ((xy.x!=-1)&&(xy.y!=-1))map_xy.push_back(xy);
	}
	if (!map_xy.size())return false;
	return true;
}

/*
函数功能:载入拾取物品
参数一:角色结构体
返回值：选中怪物对象指针
*/
bool Load_pick_goods()
{
	pick_goods_list.clear();
	std::vector<std::string> map_coordinate = tools::getInstance()->ReadTxt("..\\拾取物品.txt");
	if (!map_coordinate.size())return false;
	return true;
}

/*寻路线程*/
void threadGoto()
{
	if (!Load_coordinate())return;
	size_t i = 0;
	while (tflag_goto)
	{
		MapXY xy = map_xy[i];
		MapGoto_Point(map_xy[i].x, map_xy[i].y,tflag_goto);
		i++;
		if (i = map_xy.size())i = 0;
	}
}

/*捡物线程*/
void threadPickup()
{
	if (!Load_pick_goods())return;
	size_t i = 0;
	while (tflag_pickup)
	{
		r.Get_Ground(m_mon.pGr_list);
		m_mon.init_ground();
		for (size_t i=0;i<m_mon.m_groundList.size();i++)
		{
			for (size_t j = 0; j < pick_goods_list.size(); j++)
			{
				if (strcmp(m_mon.m_groundList[i].pName,pick_goods_list[j].c_str())==0)
				{
					MapGoto_Point(*m_mon.m_groundList[i].X, *m_mon.m_groundList[i].Y,true);
					mfun.pickupGoods(*m_mon.m_groundList[i].X, *m_mon.m_groundList[i].Y);
				}
			}
		}
		Sleep(2000);
	}
}


void CTestDlg::OnBnClickedButton9()
{
	// TODO: 脚本测试

	/*自动打怪 需要启动：①打怪线程优先级正常 ②遍历周围对象、地面并拾取线程 优先级中高 ③寻路线程、智能闪避 优先级最高
	*
	*          ****选怪
	*自动打怪***   打怪：判断打死
	*          ****继续打怪
	* */

	CString s;
	Load_Settings();
	s.Format("打怪技能ID:%d", s_ID);
	AppendText(m_edit2, s);
	std::vector<std::string> vec_mon = tools::getInstance()->ReadTxt("D:\\VS_PROJECT\\ConsoleFrame\\逆魔大殿.txt");
	for (size_t i = 0; i < vec_mon.size(); i++)
	{
		s.Format("%s\n", (char*)vec_mon[i].c_str());
		AppendText(m_edit2, s);
	}
	Auto_Attack(r, vec_mon, s_ID);


	//m_threadGoto=AfxBeginThread((AFX_THREADPROC)threadGoto,NULL, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
	//m_threadAttack= AfxBeginThread(thredAttack, NULL);
	//m_threadPickup= AfxBeginThread((AFX_THREADPROC)threadPickup, NULL, THREAD_PRIORITY_ABOVE_NORMAL);

	//tflag_attack = true;
	//HANDLE thrHandle;
	//thrHandle = (HANDLE)_beginthreadex(NULL, 0,
	//	(unsigned int(__stdcall*)(void*))threadAttack, (LPVOID)this, 0, NULL);  //将this指针传给子线程

	//CloseHandle(thrHandle);
}