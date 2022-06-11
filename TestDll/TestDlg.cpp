// TestDlg.cpp : ʵ���ļ�
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
// CTestDlg �Ի���
shareMemoryCli shareCli(MORE_OPEN_NUMBER);
int shareindex = -1;
//��ʼ��HOOk
HookReg hook;
role r;//��ɫ
monster m_mon;
skill m_skill;
bag r_bag;

_declspec(naked) void CallTest()
{
	_asm pushad
	if (hook.EAX != 0)
	{
		tools::getInstance()->message("����!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
		shareCli.m_pSMAllData->m_sm_data[shareindex].server_alive = false;//��֤��Ҵ��
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


// CTestDlg ��Ϣ�������

void threadLogin()
{
	int i = 0;//���Ե�¼����
	login li;
	do
	{
		shareCli.m_pSMAllData->m_sm_data[shareindex].cscript = "��¼��...";
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
		Sleep(1000);
	}
}
// ׷���ı���EditControl
void AppendText(CEdit &m_edit, CString strAdd)
{
	m_edit.SetSel(m_edit.GetWindowTextLength(), m_edit.GetWindowTextLength());
	m_edit.ReplaceSel(strAdd + L"\n");
}

void CTestDlg::OnBnClickedButton1()
{
	// ��������
	if (!r.init())return;
	CString s;
	s.Format("%s\n", r.m_roleproperty.Object.pName);
	AppendText(m_edit2,s);
	char* sex = {};
	char* job = {};
	if (*r.m_roleproperty.Sex == 0)sex = "��";
	else sex = "Ů";
	if (*r.m_roleproperty.Job == 0)job = "սʿ";
	else if (*r.m_roleproperty.Job == 1)job = "��ʦ";
	else job = "��ʿ";
	s.Format("ְҵ:%s      �ȼ�:%d    �Ա�:%s", job, *r.m_roleproperty.Level, sex);
	AppendText(m_edit2, s);;
	s.Format("HP:%d / %d", *r.m_roleproperty.Object.HP, *r.m_roleproperty.Object.HP_MAX);
	AppendText(m_edit2, s);
	s.Format("MP:%d / %d", *r.m_roleproperty.Object.MP, *r.m_roleproperty.Object.MP_MAX);
	AppendText(m_edit2, s);
	s.Format("��ǰ��ͼ:%s  ����%d,%d", r.m_roleproperty.p_Current_Map, *r.m_roleproperty.Object.X, *r.m_roleproperty.Object.Y);
	AppendText(m_edit2, s);
	s.Format("������С:%d  ��������%d / %d", *r.m_roleproperty.Bag_Size, *r.m_roleproperty.BAG_W, *r.m_roleproperty.BAG_W_MAX);
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

	//HOOK���ӷ�����ʧ�ܴ���
	hook.hookReg(0x5F8B69, 5, CallTest);

	CloseHandle(::CreateThread(NULL, NULL, LPTHREAD_START_ROUTINE(threadLogin), NULL, NULL, NULL));
	//��¼�ɹ�֮������ ����ͨѶ�߳�,��ʱ��֤�����Ϣ
	
	CloseHandle(::CreateThread(NULL, NULL, LPTHREAD_START_ROUTINE(threadAlive), NULL, NULL, NULL));


	//hookAPI(WSARecv, MyWSARecv);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}



void CTestDlg::OnBnClickedButton2()
{
	// TODO: ������Χ���� ����NPC
	if (!r.init())return;
	m_mon.m_monsterList.clear();
	m_mon.pOb_list.clear();
	CString s;
	if (!r.Get_Envionment(*r.m_roleproperty.Object.X, *r.m_roleproperty.Object.Y, m_mon.pOb_list))
	{
		s.Format("������Χ����\n");
		AppendText(m_edit2, s);
		return;
	}	
	m_mon.init();
	s.Format("��Χ�����б����NPC��������ҡ������\n");
	AppendText(m_edit2, s);

	for (auto i = 0; i < m_mon.m_monsterList.size(); i++)
	{
		s.Format("%s  ID: %d\n", m_mon.m_monsterList[i].pName, *(m_mon.m_monsterList[i].ID));
		AppendText(m_edit2, s);
	}

}


void CTestDlg::OnBnClickedButton3()
{
	// TODO: ���ܱ���
	if (!r.init())return;
	CString s;
	m_skill.m_skillList.clear();
	m_skill.skillBase = (DWORD)r.m_roleproperty.p_Skill_Base;
	if (!m_skill.init())
	{
		s.Format("�������ܴ���\n");
		AppendText(m_edit2, s);
		return;
	}
	s.Format("���ܣ�\n");
	AppendText(m_edit2, s);

	for (auto i = 0; i < m_skill.m_skillList.size(); i++)
	{
		s.Format("%s  �ȼ�:%d  ID: %d\n", m_skill.m_skillList[i].pName, *(m_skill.m_skillList[i].level), *(m_skill.m_skillList[i].ID));
		AppendText(m_edit2, s);
	}

}


void CTestDlg::OnBnClickedButton8()
{
	// TODO: ��������
	if (!r.init())return;
	CString s;
	r_bag.maxSize = *r.m_roleproperty.Bag_Size;
	r_bag.bagBase = (DWORD)r.m_roleproperty.p_Bag_Base;
	if(!r_bag.init())
	{
		s.Format("������������\n");
		AppendText(m_edit2, s);
		return;
	}
	s.Format("ʣ�౳����������: %d\n",r_bag.bSpace);
	AppendText(m_edit2, s);
}
