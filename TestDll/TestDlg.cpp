// TestDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TestDll.h"
#include "TestDlg.h"
#include "afxdialogex.h"
#include <thread>
#include <mutex>
#include "monster.h"
#include "skill.h"
#include "bag.h"
#include "team.h"
#include "utils.h"
#include "gamecall.h"
#include "shareMemoryCli.h"
#include "config.h"
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
role Promenade; //Ԫ��
monster m_mon;
skill m_skill;
bag r_bag;
gamecall mfun;
team m_team;

std::mutex team_mutex;  // �����ļ��������� 


_declspec(naked) void CallTest()
{
	_asm pushad
	if (hook.EAX != 100)
	{
		tools::getInstance()->message("����!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
		shareCli.m_pSMAllData->m_sm_data[shareindex].server_alive = false;//��֤��Ҵ��
	}
	_asm  popad
	_asm ret
}

extern CTestDlg* pDlg;

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
	ON_BN_CLICKED(IDC_BUTTON4, &CTestDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_CHK_TEAM, &CTestDlg::OnBnClickedChkTeam)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_GJ, &CTestDlg::OnBnClickedBtnGj)
	ON_BN_CLICKED(IDC_BTN_TESTCALL, &CTestDlg::OnBnClickedBtnTestcall)
END_MESSAGE_MAP()


// CTestDlg ��Ϣ�������

void threadLogin()
{
	int i = 0;//���Ե�¼����
	do
	{
		shareCli.m_pSMAllData->m_sm_data[shareindex].cscript = "��¼��...";
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
		shareCli.m_pSMAllData->m_sm_data[shareindex].cscript = "��½ʧ��,���ڽ�������";
		shareCli.m_pSMAllData->m_sm_data[shareindex].server_alive = false;
	}
	else 
	{
		shareCli.m_pSMAllData->m_sm_data[shareindex].cscript = "��½���";
		shareCli.m_pSMAllData->m_sm_data[shareindex].roleName = std::string(r.m_roleproperty.Object.pName);
		pDlg->init_team();
	}
	return;
}

void threadAlive()
{
	while (true)
	{
		shareCli.m_pSMAllData->m_sm_data[shareindex].rcv_rand = shareCli.m_pSMAllData->m_sm_data[shareindex].send_rand;//��֤��Ҵ��
		shareCli.m_pSMAllData->m_sm_data[shareindex].ndPid=GetCurrentProcessId();//��֤��Ҵ��
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
	for (auto i=0;i<21;i++)
	{
		if (!(*r.m_euip[i].ID))continue; 
		s.Format("%d :%s �;�:%d/%d",i, r.m_euip[i].pName, *(r.m_euip[i].Use_Num),*(r.m_euip[i].Use_Num_Max));
		AppendText(m_edit2, s);
	}

	// Ԫ������
	if (*r.m_roleproperty.Is_has_Promenade==0)return;
	Promenade.init_promenade();
	if (*Promenade.m_roleproperty.Is_Promenade_Release==0) mfun.release_Promenade();
	if (!Promenade.init_promenade())return;
	s.Format("%s\n", Promenade.m_roleproperty.Object.pName);
	AppendText(m_edit2, s);
	if (*Promenade.m_roleproperty.Sex == 0)sex = "��";
	else sex = "Ů";
	if (*Promenade.m_roleproperty.Job == 0)job = "սʿ";
	else if (*Promenade.m_roleproperty.Job == 1)job = "��ʦ";
	else job = "��ʿ";
	s.Format("ְҵ:%s      �ȼ�:%d    �Ա�:%s", job, *Promenade.m_roleproperty.Level, sex);
	AppendText(m_edit2, s);;
	s.Format("HP:%d / %d", *Promenade.m_roleproperty.Object.HP, *Promenade.m_roleproperty.Object.HP_MAX);
	AppendText(m_edit2, s);
	s.Format("MP:%d / %d", *Promenade.m_roleproperty.Object.MP, *Promenade.m_roleproperty.Object.MP_MAX);
	AppendText(m_edit2, s);
	s.Format("��ǰ��ͼ:%s  ����%d,%d", Promenade.m_roleproperty.p_Current_Map, *Promenade.m_roleproperty.Object.X, *Promenade.m_roleproperty.Object.Y);
	AppendText(m_edit2, s);
	s.Format("������С:%d  ��������%d / %d", *Promenade.m_roleproperty.Bag_Size, *Promenade.m_roleproperty.BAG_W, *Promenade.m_roleproperty.BAG_W_MAX);
	AppendText(m_edit2, s);
	s.Format("ID:%x", *Promenade.m_roleproperty.Object.ID);
	AppendText(m_edit2, s);
	for (auto i = 0; i < 21; i++)
	{
		if (!(*Promenade.m_euip[i].ID))continue;
		s.Format("%d :%s �;�:%d/%d", i, Promenade.m_euip[i].pName, *(Promenade.m_euip[i].Use_Num), *(Promenade.m_euip[i].Use_Num_Max));
		AppendText(m_edit2, s);
	}
	Promenade.m_roleproperty.Object.Distance = mfun.caclDistance(*r.m_roleproperty.Object.X, *r.m_roleproperty.Object.Y, *Promenade.m_roleproperty.Object.X, *Promenade.m_roleproperty.Object.Y);
	s.Format("���������:%f", Promenade.m_roleproperty.Object.Distance);
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

/*��ʼ������������*/
bool CTestDlg:: initVariable()
{
	i_map = 0;
	s_ID = -1;
	tflag_attack = true;
	tflag_goto = true;
	tflag_pickup = true;
	/*����ʰȡ��Ʒ*/
	pick_goods_list.clear();
	pick_goods_list = tools::getInstance()->ReadTxt(tools::getInstance()->getParentPath(shareCli.m_pSMAllData->currDir) + "ʰȡ��Ʒ.txt");
	if (!pick_goods_list.size())return false;
	/*���ü���*/
	if (!Set_Skill())return false;
	/*�����ֵ�ͼ·��*/
	if (!Load_coordinate())return false;
	/*���빥�������б�*/
	attack_monlist.clear();
	attack_monlist = tools::getInstance()->ReadTxt(tools::getInstance()->getParentPath(shareCli.m_pSMAllData->currDir) + "��ħ���.txt");
	if (!attack_monlist.size())return false;

	return true;
}

/*
��������:�Զ�������Ʒ����������Ʒ����Ϊ���飬��ħ��Ʒ����ΪԪ��
*/
void CTestDlg::AutoRecvGoods(void)
{
	/*���վ���*/
	if (r_bag.caclGoodsNumber("��ڤ����") > 0) {
		mfun.RecovryGoods_To_Exp(*(r_bag.m_bag[r_bag.getGoodsIndex("��ڤ����")].ID),*r.m_roleproperty.VIP_Level);
		Sleep(200);
	}
	if (r_bag.caclGoodsNumber("��������") > 0) {
		mfun.RecovryGoods_To_Exp(*(r_bag.m_bag[r_bag.getGoodsIndex("��������")].ID), *r.m_roleproperty.VIP_Level);
		Sleep(200);
	}
	if (r_bag.caclGoodsNumber("��â����") > 0) {
		mfun.RecovryGoods_To_Exp(*(r_bag.m_bag[r_bag.getGoodsIndex("��â����")].ID), *r.m_roleproperty.VIP_Level);
		Sleep(200);
	}
	if (r_bag.caclGoodsNumber("��������") > 0) {
		mfun.RecovryGoods_To_Exp(*(r_bag.m_bag[r_bag.getGoodsIndex("��������")].ID), *r.m_roleproperty.VIP_Level);
		Sleep(200);
	}
	if (r_bag.caclGoodsNumber("���黤��") > 0) {
		mfun.RecovryGoods_To_Exp(*(r_bag.m_bag[r_bag.getGoodsIndex("���黤��")].ID), *r.m_roleproperty.VIP_Level);
		Sleep(200);
	}
	if (r_bag.caclGoodsNumber("�׽��ָ") > 0) {
		mfun.RecovryGoods_To_Exp(*(r_bag.m_bag[r_bag.getGoodsIndex("�׽��ָ")].ID), *r.m_roleproperty.VIP_Level);
		Sleep(200);
	}
	if (r_bag.caclGoodsNumber("��������") > 0) {
		mfun.RecovryGoods_To_Exp(*(r_bag.m_bag[r_bag.getGoodsIndex("��������")].ID), *r.m_roleproperty.VIP_Level);
		Sleep(200);
	}
	if (r_bag.caclGoodsNumber("ħ������") > 0) {
		mfun.RecovryGoods_To_Exp(*(r_bag.m_bag[r_bag.getGoodsIndex("ħ������")].ID), *r.m_roleproperty.VIP_Level);
		Sleep(200);
	}
	if (r_bag.caclGoodsNumber("�챦��ָ") > 0) {
		mfun.RecovryGoods_To_Exp(*(r_bag.m_bag[r_bag.getGoodsIndex("�챦��ָ")].ID), *r.m_roleproperty.VIP_Level);
		Sleep(200);
	}
	if (r_bag.caclGoodsNumber("����") > 0) {
		mfun.RecovryGoods_To_Exp(*(r_bag.m_bag[r_bag.getGoodsIndex("����")].ID), *r.m_roleproperty.VIP_Level);
		Sleep(200);
	}
	if (r_bag.caclGoodsNumber("��������") > 0) {
		mfun.RecovryGoods_To_Exp(*(r_bag.m_bag[r_bag.getGoodsIndex("��������")].ID), *r.m_roleproperty.VIP_Level);
		Sleep(200);
	}
	if (r_bag.caclGoodsNumber("����ͷ��") > 0) {
		mfun.RecovryGoods_To_Exp(*(r_bag.m_bag[r_bag.getGoodsIndex("����ͷ��")].ID), *r.m_roleproperty.VIP_Level);
		Sleep(200);
	}
	if (r_bag.caclGoodsNumber("ս��(��)") > 0) {
		mfun.RecovryGoods_To_Exp(*(r_bag.m_bag[r_bag.getGoodsIndex("ս��(��)")].ID), *r.m_roleproperty.VIP_Level);
		Sleep(200);
	}
	if (r_bag.caclGoodsNumber("ս��(Ů)") > 0) {
		mfun.RecovryGoods_To_Exp(*(r_bag.m_bag[r_bag.getGoodsIndex("ս��(Ů)")].ID), *r.m_roleproperty.VIP_Level);
		Sleep(200);
	}
	if (r_bag.caclGoodsNumber("����ս��(��)") > 0) {
		mfun.RecovryGoods_To_Exp(*(r_bag.m_bag[r_bag.getGoodsIndex("����ս��(��)")].ID), *r.m_roleproperty.VIP_Level);
		Sleep(200);
	}
	if (r_bag.caclGoodsNumber("����ս��(Ů)") > 0) {
		mfun.RecovryGoods_To_Exp(*(r_bag.m_bag[r_bag.getGoodsIndex("����ս��(Ů)")].ID), *r.m_roleproperty.VIP_Level);
		Sleep(200);
	}
	if (r_bag.caclGoodsNumber("��ħ����(��)") > 0) {
		mfun.RecovryGoods_To_Exp(*(r_bag.m_bag[r_bag.getGoodsIndex("��ħ����(��)")].ID), *r.m_roleproperty.VIP_Level);
		Sleep(200);
	}
	if (r_bag.caclGoodsNumber("��ħ����(Ů)") > 0) {
		mfun.RecovryGoods_To_Exp(*(r_bag.m_bag[r_bag.getGoodsIndex("��ħ����(Ů)")].ID), *r.m_roleproperty.VIP_Level);
		Sleep(200);
	}
	if (r_bag.caclGoodsNumber("���") > 0) {
		mfun.RecovryGoods_To_Exp(*(r_bag.m_bag[r_bag.getGoodsIndex("���")].ID), *r.m_roleproperty.VIP_Level);
		Sleep(200);
	}
	if (r_bag.caclGoodsNumber("���컭�") > 0) {
		mfun.RecovryGoods_To_Exp(*(r_bag.m_bag[r_bag.getGoodsIndex("���컭�")].ID), *r.m_roleproperty.VIP_Level);
		Sleep(200);
	}
	if (r_bag.caclGoodsNumber("������") > 0) {
		mfun.RecovryGoods_To_Exp(*(r_bag.m_bag[r_bag.getGoodsIndex("������")].ID), *r.m_roleproperty.VIP_Level);
		Sleep(200);
	}
	if (r_bag.caclGoodsNumber("ͭ��") > 0) {
		mfun.RecovryGoods_To_Exp(*(r_bag.m_bag[r_bag.getGoodsIndex("ͭ��")].ID), *r.m_roleproperty.VIP_Level);
		Sleep(200);
	}
	if (r_bag.caclGoodsNumber("�޼�") > 0) {
		mfun.RecovryGoods_To_Exp(*(r_bag.m_bag[r_bag.getGoodsIndex("�޼�")].ID), *r.m_roleproperty.VIP_Level);
		Sleep(200);
	}
	if (r_bag.caclGoodsNumber("ħ��Ȩ��") > 0) {
		mfun.RecovryGoods_To_Exp(*(r_bag.m_bag[r_bag.getGoodsIndex("ħ��Ȩ��")].ID), *r.m_roleproperty.VIP_Level);
		Sleep(500);
	}

	/*����Ԫ��*/
	if (r_bag.caclGoodsNumber("��ħ��") > 0) {
		mfun.RecovryGoods_To_Gold(*(r_bag.m_bag[r_bag.getGoodsIndex("��ħ��")].ID), *r.m_roleproperty.VIP_Level);
		Sleep(200);
	}
	if (r_bag.caclGoodsNumber("��������") > 0) {
		mfun.RecovryGoods_To_Gold(*(r_bag.m_bag[r_bag.getGoodsIndex("��������")].ID), *r.m_roleproperty.VIP_Level);
		Sleep(200);
	}
	if (r_bag.caclGoodsNumber("���ݽ�ָ") > 0) {
		mfun.RecovryGoods_To_Gold(*(r_bag.m_bag[r_bag.getGoodsIndex("���ݽ�ָ")].ID), *r.m_roleproperty.VIP_Level);
		Sleep(200);
	}
	if (r_bag.caclGoodsNumber("ħ������") > 0) {
		mfun.RecovryGoods_To_Gold(*(r_bag.m_bag[r_bag.getGoodsIndex("ħ������")].ID), *r.m_roleproperty.VIP_Level);
		Sleep(200);
	}
	if (r_bag.caclGoodsNumber("ħ����ѥ") > 0) {
		mfun.RecovryGoods_To_Gold(*(r_bag.m_bag[r_bag.getGoodsIndex("ħ����ѥ")].ID), *r.m_roleproperty.VIP_Level);
		Sleep(200);
	}
	if (r_bag.caclGoodsNumber("����ͷ��") > 0) {
		mfun.RecovryGoods_To_Gold(*(r_bag.m_bag[r_bag.getGoodsIndex("����ͷ��")].ID), *r.m_roleproperty.VIP_Level);
		Sleep(200);
	}
	if (r_bag.caclGoodsNumber("��ɫ����") > 0) {
		mfun.RecovryGoods_To_Gold(*(r_bag.m_bag[r_bag.getGoodsIndex("��ɫ����")].ID), *r.m_roleproperty.VIP_Level);
		Sleep(200);
	}
	if (r_bag.caclGoodsNumber("��ʿ����") > 0) {
		mfun.RecovryGoods_To_Gold(*(r_bag.m_bag[r_bag.getGoodsIndex("��ʿ����")].ID), *r.m_roleproperty.VIP_Level);
		Sleep(200);
	}
	if (r_bag.caclGoodsNumber("������ָ") > 0) {
		mfun.RecovryGoods_To_Gold(*(r_bag.m_bag[r_bag.getGoodsIndex("������ָ")].ID), *r.m_roleproperty.VIP_Level);
		Sleep(200);
	}
	if (r_bag.caclGoodsNumber("��������") > 0) {
		mfun.RecovryGoods_To_Gold(*(r_bag.m_bag[r_bag.getGoodsIndex("��������")].ID), *r.m_roleproperty.VIP_Level);
		Sleep(200);
	}
	if (r_bag.caclGoodsNumber("����սѥ") > 0) {
		mfun.RecovryGoods_To_Gold(*(r_bag.m_bag[r_bag.getGoodsIndex("����սѥ")].ID), *r.m_roleproperty.VIP_Level);
		Sleep(200);
	}
	if (r_bag.caclGoodsNumber("�������") > 0) {
		mfun.RecovryGoods_To_Gold(*(r_bag.m_bag[r_bag.getGoodsIndex("�������")].ID), *r.m_roleproperty.VIP_Level);
		Sleep(200);
	}
	if (r_bag.caclGoodsNumber("���ۻ���") > 0) {
		mfun.RecovryGoods_To_Gold(*(r_bag.m_bag[r_bag.getGoodsIndex("���ۻ���")].ID), *r.m_roleproperty.VIP_Level);
		Sleep(200);
	}
	if (r_bag.caclGoodsNumber("̩̹��ָ") > 0) {
		mfun.RecovryGoods_To_Gold(*(r_bag.m_bag[r_bag.getGoodsIndex("̩̹��ָ")].ID), *r.m_roleproperty.VIP_Level);
		Sleep(200);
	}
	if (r_bag.caclGoodsNumber("��������") > 0) {
		mfun.RecovryGoods_To_Gold(*(r_bag.m_bag[r_bag.getGoodsIndex("��������")].ID), *r.m_roleproperty.VIP_Level);
		Sleep(200);
	}
	if (r_bag.caclGoodsNumber("���Ƶ�ѥ") > 0) {
		mfun.RecovryGoods_To_Gold(*(r_bag.m_bag[r_bag.getGoodsIndex("���Ƶ�ѥ")].ID), *r.m_roleproperty.VIP_Level);
		Sleep(200);
	}
}

/*
��������:�سǴ���
*/
void CTestDlg::AutoReturnToCity()
{
	// ��鱳�����Ƿ��лسǾ���
	if (r_bag.caclGoodsNumber("���ûس���ʯ") > 0)
	{
		// ʹ�ûسǾ���
		mfun.useGoods(r_bag.getGoodsIndex("���ûس���ʯ"));
		Sleep(2000); // �ȴ��سǲ������

		// �سǺ������
		AutoRecvGoods();

		// ��鱳���ռ�
		if (r_bag.getBagSpace() < 10)
		{
			// �����ռ䲻�㣬���������Ʒ
			if (r_bag.caclGoodsNumber("ǿЧ̫����ˮ") > REMAIN_TAIYANG)
			{
				mfun.useGoods(r_bag.getGoodsIndex("ǿЧ̫����ˮ"));
				Sleep(100);
			}
			if (r_bag.caclGoodsNumber("����ҩˮ") > REMAIN_TAIYANG)
			{
				mfun.useGoods(r_bag.getGoodsIndex("����ҩˮ"));
				Sleep(200);
			}
		}
	}
	else
	{
		// ������û�лسǾ��ᣬ��ʾ�û�
		AfxMessageBox("������û�лسǾ��ᣬ�벹�䣡");
	}
}

void CTestDlg::OnBnClickedButton2()
{
	// TODO: ������Χ���� ���� ����NPC
	if (!r.init())return;
	m_mon.m_monsterList.clear();
	m_mon.m_groundList.clear();
	CString s;
	if (!r.Get_Envionment(m_mon.pOb_list))
	{
		s.Format("������Χ����\n");
		AppendText(m_edit2, s);
		return;
	}	
	if (!r.Get_Ground(m_mon.pGr_list))
	{
		s.Format("�����������\n");
		AppendText(m_edit2, s);
		return;
	}
	m_mon.init();
	s.Format("��Χ�����б����NPC��������ҡ������\n");
	AppendText(m_edit2, s);
	for (size_t i = 0; i < m_mon.m_monsterList.size(); i++)
	{
		s.Format("%s ID: %x, ����(x,y)��%d, %d   ���룺%.2f\n",
			m_mon.m_monsterList[i].pName, 
			*(m_mon.m_monsterList[i].ID),
			*(m_mon.m_monsterList[i].X),
			*(m_mon.m_monsterList[i].Y),
			mfun.caclDistance(*(r.m_roleproperty.Object.X), *(r.m_roleproperty.Object.Y), *(m_mon.m_monsterList[i].X), *(m_mon.m_monsterList[i].Y)));
		AppendText(m_edit2, s);
	}

	s.Format("���棺\n");
	AppendText(m_edit2, s);
	for (size_t i = 0; i < m_mon.m_groundList.size(); i++)
	{
		s.Format(" %s : %d/%d\n", m_mon.m_groundList[i].pName, *m_mon.m_groundList[i].X, *m_mon.m_groundList[i].Y);
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

	for (size_t i = 0; i < m_skill.m_skillList.size(); i++)
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
	s.Format("��С:%d   ʣ��: %d\n", r_bag.maxSize, r_bag.getBagSpace());
	AppendText(m_edit2, s);

	for (size_t i=0;i<r_bag.maxSize;i++)
	{
		if (*(r_bag.m_bag[i].ID))
		{
			s.Format("��%d��:%s   ID: %x\n", i, r_bag.m_bag[i].pName, *r_bag.m_bag[i].ID);
			AppendText(m_edit2, s);
		}
	}


}


void CTestDlg::OnBnClickedButton5()
{
	// TODO: ֹͣ�ű�
	tflag_attack = false;
	//tflag_goto= false;
	tflag_pickup= false;
	//�߳̿�����Ϊ��
	//WaitForSingleObject(m_threadGoto, 100);
	WaitForSingleObject(m_threadAttack, 60000);
	WaitForSingleObject(m_threadPickup, 60000);
}




/*
��������:ѡ����Ŀ��
����һ:��ɫ�ṹ��
������:���������б�
����ֵ��ѡ�й������ָ��
*/
MONSTER_PROPERTY Choose_Moster(CTestDlg* pDlg, std::vector<std::string>& vec)
{
	MONSTER_PROPERTY ret;
	if(*r.m_roleproperty.p_Target_ID == 0)
	{
		std::vector<MONSTER_PROPERTY> near_atak_mon_list = mfun.sort_aroud_monster(r, vec);
		if (near_atak_mon_list.size() > 0)
		{
			ret = near_atak_mon_list[0];
			*r.m_roleproperty.p_Target_ID = *ret.ID;
		}
		else
		{
			/*��Ѱ·���ҹ�*/
			if (pDlg->i_map == pDlg->map_xy.size())pDlg->i_map = 0;
			MapXY xy = pDlg->map_xy[pDlg->i_map];
			mfun.CurrentMapMove(xy.x, xy.y);//Ѱ·0.8s
			Sleep(800);
			mfun.CurrentMapMove(*r.m_roleproperty.Object.X, *r.m_roleproperty.Object.Y);//ֹͣ
			if (mfun.caclDistance(*r.m_roleproperty.Object.X, *r.m_roleproperty.Object.Y,xy.x,xy.y)<4)pDlg->i_map++;
		}
	}
	return ret;
}

/*
��������:�Զ����
����һ:��ɫ�ṹ��
������:���������б�
������:ʹ�ü���ID
����ֵ��trueΪ�Ѿ��������fasleΪλ�ô��󣨻���600��û�������
*/
bool Auto_Attack(CTestDlg* pDlg, std::vector<std::string>& vec, DWORD s_ID)
{
	*r.m_roleproperty.p_Target_ID = 0;
	MONSTER_PROPERTY att_mon = Choose_Moster(pDlg, vec);
	if (nullptr == att_mon.ID)return false;
	unsigned i = 0;/*��¼��������*/
	while( (*att_mon.HP> 0) && (0 != *r.m_roleproperty.p_Target_ID) && (i < 600))
	{
		if (mfun.caclDistance(*r.m_roleproperty.Object.X, *r.m_roleproperty.Object.Y, *att_mon.X, *att_mon.Y) > 7)
		{
			mfun.CurrentMapMove(*att_mon.X, *att_mon.Y);
			Sleep(500);
		}
		DWORD s_posion = m_skill.getSkillId("ʩ����");
		if( (s_posion!=-1))//��ʿ
		{
			if ((r_bag.ifHasPoison()>0)&& (*att_mon.IsPosion < 0x40))//�Ƿ��ж� 0û����0x40�춾��0x80�̶���0xc0���̶�,
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
��������:���ô�ּ���
����һ:��ɫ�ṹ��
����ֵ��ѡ�й������ָ��
*/
bool CTestDlg::Set_Skill()
{
	/*��������*/
	if (*r.m_roleproperty.Job == 0)/*սʿ*/
	{
		return false;
	}
    if (*r.m_roleproperty.Job == 1)/*��ʦ*/
	{
		DWORD  s_tmp = m_skill.getSkillId("�����ϵ�");
		if (s_tmp != -1)
		{
			s_ID = s_tmp;
			return true;
		}
		s_tmp = m_skill.getSkillId("�׵���");
		if (s_tmp != -1)
		{
			s_ID = s_tmp;
			return true;
		}
		s_tmp = m_skill.getSkillId("������");
		if (s_tmp != -1)
		{
			s_ID = s_tmp;
			return true;
		}
		s_tmp = m_skill.getSkillId("С����");
		if (s_tmp != -1)
		{
			s_ID = s_tmp;
			return true;
		}
		return false;
	}
	if (*r.m_roleproperty.Job == 2)/*��ʿ*/
	{
		return false;
	}
	 return false;

}


/*
��������:����Ѱ·�������
����һ:��ɫ�ṹ��
����ֵ��ѡ�й������ָ��
*/
bool CTestDlg::Load_coordinate()
{
	map_xy.clear();
	std::vector<std::string> map_coordinate= tools::getInstance()->ReadTxt(tools::getInstance()->getParentPath(shareCli.m_pSMAllData->currDir) + "\\��ħ���1.txt");
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
��������:��ʼ�����Timer
����һ:��ɫ�ṹ��
����ֵ��ѡ�й������ָ��
*/
bool CTestDlg::init_team()
{
	m_team.init();
	m_team_check_id = 0;
	pBtn = (CButton*)GetDlgItem(IDC_CHK_TEAM);  //�����Ӹ�ѡ��ؼ��ľ��
	team_list.clear();
	team_list = tools::getInstance()->ReadTxt(tools::getInstance()->getParentPath(shareCli.m_pSMAllData->currDir) + "\\������Ա.txt");
	if (pBtn->GetCheck())
	{
		if (*r.m_roleproperty.Team_is_allow != 1)mfun.team_open_close(1);//�������
		m_team_check_id = SetTimer(11111, 30000, NULL);
	}
	return true;
}

/*
��������:��ȡ����ļ������
����һ:��ɫ�ṹ��
����ֵ��ѡ�й������ָ��
*/
//void  TimerProc(HWND hWnd, UINT uMsg, UINT uID, DWORD dwTimew)
void  CTestDlg::MakeTeam(CTestDlg* pDlg)
{		
	m_team.init();
	if (pDlg->team_list.size()<2)return;
	if (pDlg->team_list.size() == m_team.m_team_list.size())return;//������
	if (strcmp(pDlg->team_list[0].c_str(),r.m_roleproperty.Object.pName)==0)//���Ƕӳ�
	{
		for (size_t i = 1; i < pDlg->team_list.size(); i++)
		{
			size_t temp = i;
			for (size_t j = 0; j < m_team.m_team_list.size(); j++)
			{
				if (strcmp(pDlg->team_list[i].c_str(), m_team.m_team_list[j].pName) == 0)
				{
					temp = i + 1;
					break;
				}			
			}
			if (temp==i) mfun.maketeam(pDlg->team_list[i]);
		}
	}
	else //���Ƕ�Ա
	{
		if (shareCli.m_pSMAllData->team_info == 0)
		{
			pBtn->SetCheck(0);
			pDlg->OnBnClickedChkTeam();
			return;
		}
		for (size_t j = 0; j < m_team.m_team_list.size(); j++)
		{
			if (strcmp(r.m_roleproperty.Object.pName, m_team.m_team_list[j].pName) == 0)
			{
				return;
			}
		}
	  mfun.allowteam(pDlg->team_list[0]);//ͬ�����
	}
}


/*Ѱ·�߳�*/
UINT __cdecl CTestDlg::threadGoto(LPVOID p)
{
	CTestDlg* pDlg = (CTestDlg*)p;
	size_t i = 0;
	while (pDlg->tflag_goto)
	{
		MapXY xy = pDlg->map_xy[i];
		shareCli.m_pSMAllData->m_sm_data[shareindex].cscript = std::string( "����Ѱ·��");
		//MapGoto_Point(pDlg->map_xy[i].x, pDlg->map_xy[i].y, pDlg->tflag_goto);
		i++;
		if (i = pDlg->map_xy.size())i = 0;
	}
	return 0;
}

/*����߳�*/
UINT __cdecl CTestDlg::threadAttack(LPVOID p)
{
	CTestDlg* pDlg = (CTestDlg*)p;
	CString s;
	s.Format("����߳̿�ʼ");
	AppendText(pDlg->m_edit2, s);
	while (pDlg->tflag_attack)
	{
		shareCli.m_pSMAllData->m_sm_data[shareindex].cscript = std::string("���");
		//Auto_Attack(pDlg, pDlg->attack_monlist, pDlg->s_ID);
		mfun.start_end_AutoAttack(pDlg->tflag_attack);
		//Sleep(ATTACK_SLEEP);
	}
	mfun.start_end_AutoAttack(pDlg->tflag_attack);
	shareCli.m_pSMAllData->m_sm_data[shareindex].cscript = std::string("����");
	s.Format("����߳�ֹͣ");
	AppendText(pDlg->m_edit2, s);
	return 0;
}

/*��������*/
UINT __cdecl CTestDlg::threadBagPocess(LPVOID p)
{
	CString s;
	CTestDlg* pDlg = (CTestDlg*)p;
	s.Format("��������߳̿�ʼ");
	AppendText(pDlg->m_edit2, s);
	while (pDlg->tflag_attack)
	{
		if (r_bag.getBagSpace() < 35)
		{
			pDlg->AutoRecvGoods();
			if (r_bag.caclGoodsNumber("1����Ԫ��") > 0) {
				mfun.useGoods(r_bag.getGoodsIndex("1����Ԫ��"));
				Sleep(200);
			}
			if (r_bag.caclGoodsNumber("2����Ԫ��") > 0) {
				mfun.useGoods(r_bag.getGoodsIndex("2����Ԫ��"));
				Sleep(200);
			}
			if (r_bag.caclGoodsNumber("5����Ԫ��") > 0) {
				mfun.useGoods(r_bag.getGoodsIndex("5����Ԫ��"));
				Sleep(200);
			}
			if (r_bag.caclGoodsNumber("10����Ԫ��") > 0) {
				mfun.useGoods(r_bag.getGoodsIndex("10����Ԫ��"));
				Sleep(200);
			}
			if (r_bag.caclGoodsNumber("20����Ԫ��") > 0)
			{
				mfun.useGoods(r_bag.getGoodsIndex("20����Ԫ��"));
				Sleep(200);
			}
	    }
		if (r_bag.getBagSpace() < 10)
		{
			if (r_bag.caclGoodsNumber("ǿЧ̫����ˮ") > REMAIN_TAIYANG)
			{
				mfun.useGoods(r_bag.getGoodsIndex("ǿЧ̫����ˮ"));
				Sleep(100);
			}
			if (r_bag.caclGoodsNumber("����ҩˮ") > REMAIN_TAIYANG) {
				mfun.useGoods(r_bag.getGoodsIndex("����ҩˮ"));
				Sleep(200);
			}
		}
		Sleep(2000);
	}
	s.Format("��������߳�ֹͣ");
	AppendText(pDlg->m_edit2, s);
	return 0;
}


/*�����߳�*/
UINT __cdecl CTestDlg::threadPickup(LPVOID p)
{
	CString s;
	CTestDlg* pDlg = (CTestDlg*)p;
	s.Format("�����߳̿�ʼ");
	AppendText(pDlg->m_edit2, s);
	while (pDlg->tflag_pickup)
	{
		std::vector<GROUND_GOODS> need2pick_list = mfun.sort_groud_goods(r, pDlg->pick_goods_list);
		if (need2pick_list.size())
		{
			GROUND_GOODS pick_temp = need2pick_list[0];
			std::vector<MONSTER_PROPERTY> near_mon = mfun.sort_aroud_monster(r, pDlg->attack_monlist, 6);
			if (near_mon.size() < 5)
			{
				pDlg->m_threadAttack->SuspendThread();
				s.Format("����ʰȡ��Ʒ:%s ���꣺%d,%d", pick_temp.pName, *pick_temp.X, *pick_temp.Y);
				AppendText(pDlg->m_edit2, s);
				int pick_try_accounts = 0;
				do
				{
					mfun.Run_or_Step_To(*pick_temp.X, *pick_temp.Y, 1);
					Sleep(800);
					mfun.Run_or_Step_To(*pick_temp.X, *pick_temp.Y, 1);
					Sleep(800);
					mfun.Run_or_Step_To(*pick_temp.X, *pick_temp.Y, 1);
					Sleep(800);
					pick_try_accounts++;
					if ((*r.m_roleproperty.Object.X == *pick_temp.X) && (*r.m_roleproperty.Object.X == *pick_temp.Y))
					{
						mfun.pickupGoods(*pick_temp.X, *pick_temp.Y);
					}
					need2pick_list.clear();
					need2pick_list = mfun.sort_groud_goods(r, pDlg->pick_goods_list);
					if (!need2pick_list.size())break;
				} while ((need2pick_list[0].X == pick_temp.X) && (need2pick_list[0].Y == pick_temp.Y) && (pick_try_accounts < 4));
				pDlg->m_threadAttack->ResumeThread();
			}
			else
			{
				Sleep(10000);//���
			}
		}
		Sleep(1000);
	}
	s.Format("�����߳�ֹͣ");
	AppendText(pDlg->m_edit2, s);
	return 0;
}

void CTestDlg::OnBnClickedButton9()
{
	// TODO: �ű�����

	/*�Զ���� ��Ҫ�������ٴ���߳����ȼ����� �ڱ�����Χ���󡢵��沢ʰȡ�߳� ���ȼ��и� ��Ѱ·�̡߳��������� ���ȼ����
	*
	*          ****ѡ��
	*�Զ����***   ��֣��жϴ���
	*          ****�������
	* */
	r.init();
	r.Get_Envionment(m_mon.pOb_list);
	r.Get_Ground(m_mon.pGr_list);
	m_mon.init();
	r_bag.maxSize = *r.m_roleproperty.Bag_Size;
	r_bag.bagBase = (DWORD)r.m_roleproperty.p_Bag_Base;
	m_skill.skillBase = (DWORD)r.m_roleproperty.p_Skill_Base;
	r_bag.init();
	m_skill.init();

	initVariable();
	//CString s;
	//std::vector<GROUND_GOODS> need2pick_list = mfun.sort_groud_goods(r, pick_goods_list);
	//for (auto i=0;i< need2pick_list.size();i++)
	//{
	//	s.Format("%s %d/%d  %f", need2pick_list[i].pName, *need2pick_list[i].X, *need2pick_list[i].Y, need2pick_list[i].Distance);
	//	AppendText(m_edit2, s);
	//}

	//if (need2pick_list.size())
	//{
	//	GROUND_GOODS pick_temp = need2pick_list[0];
	//	s.Format("%s %d/%d  %f", pick_temp.pName, *pick_temp.X, *pick_temp.Y, pick_temp.Distance);
	//	AppendText(m_edit2, s);
	//	mfun.CurrentMapMove(*pick_temp.X, *pick_temp.Y);
	//	Sleep(2000);
	//	//mfun.pickupGoods(*pick_temp.X, *pick_temp.Y);
	//}
	//m_threadGoto=AfxBeginThread(threadGoto, (LPVOID)this, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
	m_threadAttack= AfxBeginThread(threadAttack, (LPVOID)this);
	m_threadPickup= AfxBeginThread(threadPickup, (LPVOID)this, THREAD_PRIORITY_ABOVE_NORMAL);
		
}

void CTestDlg::OnBnClickedButton4()
{
	// �������+�������
	r.init();
	m_team.team_Base = r.m_roleproperty.Team_pointer;
	m_team.m_team_list.clear();
	m_team.init();
	CString s;
	s.Format("%p \n", m_team.team_Base);
	AppendText(m_edit2, s);
	if (m_team.m_team_list.size())
	{
		for (size_t i = 0; i < m_team.m_team_list.size(); i++)
		{
			s.Format("����:%s NEXT:%p Pre:%p \n", m_team.m_team_list[i].pName, m_team.m_team_list[i].Next, m_team.m_team_list[i].Previous);
			AppendText(m_edit2, s);
		}
	}
}


void CTestDlg::OnBnClickedChkTeam()
{
	// TODO: ���CHECK�����¼�
	pBtn = (CButton*)GetDlgItem(IDC_CHK_TEAM);  //�����Ӹ�ѡ��ؼ��ľ��
	CString s;

	if (pBtn->GetCheck()&&(m_team_check_id==0))
	{
		if (*r.m_roleproperty.Team_is_allow != 1)mfun.team_open_close(1);//�������
		m_team_check_id = SetTimer(11111, 30000, NULL);
	}
	else
	{
		if (m_team_check_id != 0)
		{			
			KillTimer(11111);
			m_team_check_id = 0;
			if (strcmp(team_list[0].c_str(),r.m_roleproperty.Object.pName)!=0)mfun.team_open_close(0);//��Աֱ�ӹرն��鿪��
			else//�ӳ� ���ɾ����Ա֮���ٹرն��鿪��
			{
				shareCli.m_pSMAllData->team_info = 0;
				s.Format("���Ƕӳ�,��������رն�Ա���鿪��,�Ե�10s");
				AppendText(m_edit2, s);
			}
		}
	}
}


void CTestDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	switch (nIDEvent)
	{
	case 11111:
		MakeTeam(this);
		break;
	default:
		break;
	}
	CDialogEx::OnTimer(nIDEvent);

}



void CTestDlg::OnBnClickedBtnGj()
{
	// TODO: ʹ�������Զ���ֹһ�
	if (!r.init())return;
	r_bag.maxSize = *r.m_roleproperty.Bag_Size;
	r_bag.bagBase = (DWORD)r.m_roleproperty.p_Bag_Base;
	r_bag.init();
    tflag_attack = !tflag_attack;
	if (tflag_attack)
	{
		m_threadAttack = AfxBeginThread(threadAttack, (LPVOID)this);
		m_threadBagProcess = AfxBeginThread(threadBagPocess, (LPVOID)this);
	}
	else
	{
		WaitForSingleObject(m_threadAttack, 60000);
		WaitForSingleObject(m_threadBagProcess, 60000);
	}	
}


void CTestDlg::OnBnClickedBtnTestcall()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������


}
