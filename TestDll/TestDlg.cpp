// TestDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "TestDll.h"
#include "TestDlg.h"
#include "afxdialogex.h"
#include "role.h"
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

_declspec(naked) void CallTest()
{

	_asm pushad
	if (hook.EAX != 0)
		tools::getInstance()->message("错误!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
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
}


BEGIN_MESSAGE_MAP(CTestDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CTestDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CTestDlg 消息处理程序

void threadLogin()
{
	int i = 0;//尝试登录次数
	login li;
	do
	{
		shareCli.m_pSMAllData->m_sm_data[shareindex].cscript = "登录中...";
		Sleep(5000);
		li.pressEnter(shareCli.m_pSMAllData->m_sm_data[shareindex].ndPid);
		Sleep(1000);
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
		shareCli.m_pSMAllData->m_sm_data[shareindex].server_alive = false;//验证外挂存活
		Sleep(1000);
	}
}

void CTestDlg::OnBnClickedButton1()
{
	// TODO:  测试


	tools::getInstance()->message("初始化人物信息失败!\n");

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
	hook.hookReg(0x5F8B69, 5, CallTest);

	CloseHandle(::CreateThread(NULL, NULL, LPTHREAD_START_ROUTINE(threadLogin), NULL, NULL, NULL));
	//登录成功之后设置 启动通讯线程,定时验证存活消息

	CloseHandle(::CreateThread(NULL, NULL, LPTHREAD_START_ROUTINE(threadAlive), NULL, NULL, NULL));


	//hookAPI(WSARecv, MyWSARecv);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

