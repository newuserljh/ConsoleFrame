// TestDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "TestDll.h"
#include "TestDlg.h"
#include "afxdialogex.h"
#include "role.h"
#include "monster.h"
#include "skill.h"
#include "bag.h"
#include "utils.h"
#include "login.h"
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
END_MESSAGE_MAP()


// CTestDlg 消息处理程序

void threadLogin()
{
	int i = 0;//尝试登录次数
	login li;
	do
	{
		shareCli.m_pSMAllData->m_sm_data[shareindex].cscript = "登录中...";
		Sleep(10000);
		li.pressEnter(shareCli.m_pSMAllData->m_sm_data[shareindex].ndPid);
		Sleep(100);
		li.game_login(shareCli.m_pSMAllData->m_sm_data[shareindex].userName.c_str(), shareCli.m_pSMAllData->m_sm_data[shareindex].passWord.c_str());
		Sleep(1000);
		li.pressEnter(shareCli.m_pSMAllData->m_sm_data[shareindex].ndPid);
		Sleep(1000);
		li.pressEnter(shareCli.m_pSMAllData->m_sm_data[shareindex].ndPid);
		Sleep(1000);
		li.pressEnter(shareCli.m_pSMAllData->m_sm_data[shareindex].ndPid);
		Sleep(1000);
		li.pressEnter(shareCli.m_pSMAllData->m_sm_data[shareindex].ndPid);
		Sleep(1000);
		li.pressEnter(shareCli.m_pSMAllData->m_sm_data[shareindex].ndPid);
		Sleep(1000);
		li.pressEnter(shareCli.m_pSMAllData->m_sm_data[shareindex].ndPid);
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
	// TODO: 遍历周围对象 怪物NPC
	if (!r.init())return;
	m_mon.m_monsterList.clear();
	m_mon.pOb_list.clear();
	CString s;
	if (!r.Get_Envionment(*r.m_roleproperty.Object.X, *r.m_roleproperty.Object.Y, m_mon.pOb_list))
	{
		s.Format("遍历周围错误：\n");
		AppendText(m_edit2, s);
		return;
	}	
	m_mon.init();
	s.Format("周围对象列表（怪物、NPC、其他玩家、宠物）：\n");
	AppendText(m_edit2, s);

	for (auto i = 0; i < m_mon.m_monsterList.size(); i++)
	{
		s.Format("%s  ID: %d\n", m_mon.m_monsterList[i].pName, *(m_mon.m_monsterList[i].ID));
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
	s.Format("剩余背包格子数量: %d\n",r_bag.bSpace);
	AppendText(m_edit2, s);
}
