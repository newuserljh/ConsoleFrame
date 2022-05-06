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
// CTestDlg 对话框
shareMemoryCli shareCli(MORE_OPEN_NUMBER);
int shareindex = -1;

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
	login li;
	shareCli.m_pSMAllData->m_sm_data[shareindex].cscript = "登录中...";
	Sleep(10000);
	li.pressEnter(shareCli.m_pSMAllData->m_sm_data[shareindex].ndPid);
	Sleep(1000);
	li.game_login(shareCli.m_pSMAllData->m_sm_data[shareindex].userName.c_str(), shareCli.m_pSMAllData->m_sm_data[shareindex].passWord.c_str());
	Sleep(3000);
	li.pressEnter(shareCli.m_pSMAllData->m_sm_data[shareindex].ndPid);
	Sleep(3000);
	li.pressEnter(shareCli.m_pSMAllData->m_sm_data[shareindex].ndPid);
	Sleep(3000);
	li.pressEnter(shareCli.m_pSMAllData->m_sm_data[shareindex].ndPid);
	Sleep(3000);
	li.pressEnter(shareCli.m_pSMAllData->m_sm_data[shareindex].ndPid);
	Sleep(3000);
	li.pressEnter(shareCli.m_pSMAllData->m_sm_data[shareindex].ndPid);
	Sleep(3000);
	li.pressEnter(shareCli.m_pSMAllData->m_sm_data[shareindex].ndPid);
	Sleep(3000);
	li.pressEnter(shareCli.m_pSMAllData->m_sm_data[shareindex].ndPid);
	Sleep(3000);
	shareCli.m_pSMAllData->m_sm_data[shareindex].cscript = "登陆完成";
	return;
}

void threadAlive()
{
	while (true)
	{
		shareCli.m_pSMAllData->m_sm_data[shareindex].rcv_rand = shareCli.m_pSMAllData->m_sm_data[shareindex].send_rand;//验证存活
		Sleep(1000);
	}
}

void CTestDlg::OnBnClickedButton1()
{
	// TODO:  人物属性测试
	

	role r;
	if (!r.init())
	{
		tools::getInstance()->message("初始化人物信息失败!\n");
	}
	else
	{
		
	}
}



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

	CloseHandle(::CreateThread(NULL, NULL, LPTHREAD_START_ROUTINE(threadLogin), NULL, NULL, NULL));
	//登录成功之后设置 启动通讯线程,定时验证存活消息
	CloseHandle(::CreateThread(NULL, NULL, LPTHREAD_START_ROUTINE(threadAlive), NULL, NULL, NULL));

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}
