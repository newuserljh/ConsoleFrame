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
// CTestDlg �Ի���
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


// CTestDlg ��Ϣ�������

void threadLogin()
{
	login li;
	shareCli.m_pSMAllData->m_sm_data[shareindex].cscript = "��¼��...";
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
	shareCli.m_pSMAllData->m_sm_data[shareindex].cscript = "��½���";
	return;
}

void threadAlive()
{
	while (true)
	{
		shareCli.m_pSMAllData->m_sm_data[shareindex].rcv_rand = shareCli.m_pSMAllData->m_sm_data[shareindex].send_rand;//��֤���
		Sleep(1000);
	}
}

void CTestDlg::OnBnClickedButton1()
{
	// TODO:  �������Բ���
	

	role r;
	if (!r.init())
	{
		tools::getInstance()->message("��ʼ��������Ϣʧ��!\n");
	}
	else
	{
		
	}
}



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

	CloseHandle(::CreateThread(NULL, NULL, LPTHREAD_START_ROUTINE(threadLogin), NULL, NULL, NULL));
	//��¼�ɹ�֮������ ����ͨѶ�߳�,��ʱ��֤�����Ϣ
	CloseHandle(::CreateThread(NULL, NULL, LPTHREAD_START_ROUTINE(threadAlive), NULL, NULL, NULL));

	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}
