// TestDlg.cpp : ʵ���ļ�
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
// CTestDlg �Ի���
shareMemoryCli shareCli(MORE_OPEN_NUMBER);
int shareindex = -1;
//��ʼ��HOOk
HookReg hook;
role r;//��ɫ

_declspec(naked) void CallTest()
{

	_asm pushad
	if (hook.EAX != 0)
		tools::getInstance()->message("����!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
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


// CTestDlg ��Ϣ�������

void threadLogin()
{
	int i = 0;//���Ե�¼����
	login li;
	do
	{
		shareCli.m_pSMAllData->m_sm_data[shareindex].cscript = "��¼��...";
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
		shareCli.m_pSMAllData->m_sm_data[shareindex].cscript = "��½ʧ��,���ڽ�������";
		shareCli.m_pSMAllData->m_sm_data[shareindex].server_alive = false;
	}
	shareCli.m_pSMAllData->m_sm_data[shareindex].cscript = "��½���";
	return;
}

void threadAlive()
{
	while (true)
	{
		shareCli.m_pSMAllData->m_sm_data[shareindex].rcv_rand = shareCli.m_pSMAllData->m_sm_data[shareindex].send_rand;//��֤��Ҵ��
		shareCli.m_pSMAllData->m_sm_data[shareindex].server_alive = false;//��֤��Ҵ��
		Sleep(1000);
	}
}

void CTestDlg::OnBnClickedButton1()
{
	// TODO:  ����


	tools::getInstance()->message("��ʼ��������Ϣʧ��!\n");

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
//		tools::getInstance()->message("����:%d", er);
//	}
//
//	hookAPI(WSARecv, MyWSARecv);
//	return ret;
//}

BOOL CTestDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
		//��ʼ�������ڴ�,ȡ�ù����ڴ�����
	if (!shareCli.openShareMemory())
	{
		tools::getInstance()->message("�򿪹����ڴ�ʧ��\n");
	}
	shareindex = shareCli.getIndexByPID(GetCurrentProcessId());
	hook.hookReg(0x5F8B69, 5, CallTest);

	CloseHandle(::CreateThread(NULL, NULL, LPTHREAD_START_ROUTINE(threadLogin), NULL, NULL, NULL));
	//��¼�ɹ�֮������ ����ͨѶ�߳�,��ʱ��֤�����Ϣ

	CloseHandle(::CreateThread(NULL, NULL, LPTHREAD_START_ROUTINE(threadAlive), NULL, NULL, NULL));


	//hookAPI(WSARecv, MyWSARecv);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}

