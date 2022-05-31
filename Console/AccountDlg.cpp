// AccountDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Console.h"
#include "AccountDlg.h"
#include "afxdialogex.h"
#include <sstream>
#include <thread>
#include <windows.h>
#include "exeLoad.h"
#include "EipInject.h"
#include "utils.h"
// CAccountDlg �Ի���


IMPLEMENT_DYNAMIC(CAccountDlg, CDialogEx)

CAccountDlg::CAccountDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CAccountDlg::IDD, pParent)
	, m_userName(_T(""))
	, m_password(_T(""))
{
	m_shareMemSer.reset(new shareMemorySer(MORE_OPEN_NUMBER));
}

CAccountDlg::~CAccountDlg()
{
}

void CAccountDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LISTACCOUNT, m_listCtl);
	DDX_Text(pDX, IDC_EDIT1, m_userName);
	DDX_Text(pDX, IDC_EDIT2, m_password);
}


BEGIN_MESSAGE_MAP(CAccountDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CAccountDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CAccountDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CAccountDlg::OnBnClickedButton3)
END_MESSAGE_MAP()


// CAccountDlg ��Ϣ�������


// ��ʼ������Ϣ
bool CAccountDlg::initTables()
{
	//���÷��

	DWORD dwStyle = m_listCtl.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;// ѡ��ĳ��ʹ���и�����ֻ������report ����listctrl �� 
	dwStyle |= LVS_EX_GRIDLINES;// �����ߣ�ֻ������report ����listctrl �� 
	dwStyle |= LVS_EX_CHECKBOXES;//item ǰ����checkbox �ؼ� 
	m_listCtl.SetExtendedStyle(dwStyle); // ������չ��� 

	//��ʼ����ͷ
	int i = 0;
	m_listCtl.InsertColumn(i++, "Pid", LVCFMT_LEFT, 80);
	m_listCtl.InsertColumn(i++, "�û���", LVCFMT_LEFT, 80);
	m_listCtl.InsertColumn(i++, "����", LVCFMT_LEFT, 80);
	m_listCtl.InsertColumn(i++, "��ǰ�ű�", LVCFMT_LEFT, 80);

	//����MORE_OPEN_NUM��

	for (int i= 0; i < MORE_OPEN_NUMBER; i++)
	{
		m_listCtl.InsertItem(i, ""); 
	}

	return true;
}


BOOL CAccountDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	if (!initTables())
		return FALSE;
	if (!initMem())
		return FALSE;
	initAccount();
	auto tthread = std::thread(&CAccountDlg::threadCallBack, this);
	tthread.detach();
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}


bool CAccountDlg::updateDate()
{
	for (int i = 0; i < MORE_OPEN_NUMBER; i++)
	{
		//�������ݵ�����
		std::stringstream ss;
		ss << m_shareMemSer->m_pSMAllData->m_sm_data[i].ndPid;
		m_listCtl.SetItemText(i, 0, ss.str().c_str());
		m_listCtl.SetItemText(i, 1, m_shareMemSer->m_pSMAllData->m_sm_data[i].userName.c_str());
		m_listCtl.SetItemText(i, 2, m_shareMemSer->m_pSMAllData->m_sm_data[i].passWord.c_str());
		m_listCtl.SetItemText(i, 3, m_shareMemSer->m_pSMAllData->m_sm_data[i].cscript.c_str());

		if (m_listCtl.GetCheck(i)&&(m_shareMemSer->m_pSMAllData->m_sm_data[i].ndPid!=0))
		{

			if ((m_shareMemSer->m_pSMAllData->m_sm_data[i].send_rand != m_shareMemSer->m_pSMAllData->m_sm_data[i].rcv_rand)&&
				(m_shareMemSer->m_pSMAllData->m_sm_data[i].send_rand != 0))
			{
				::TerminateProcess(::OpenProcess(PROCESS_ALL_ACCESS, false, m_shareMemSer->m_pSMAllData->m_sm_data[i].ndPid),0);//ǿ�ƹرս���
				m_shareMemSer->m_pSMAllData->m_sm_data[i].ndPid = 0;
				log_inject(i);//���µ�¼
			}
			else
			{
				srand((unsigned int)time(0));//����������
				m_shareMemSer->m_pSMAllData->m_sm_data[i].send_rand = rand() + 1;
			}
			if (m_shareMemSer->m_pSMAllData->m_sm_data[i].server_alive ==false)//��֤��Ϸ������״̬
			{
				::TerminateProcess(::OpenProcess(PROCESS_ALL_ACCESS, false, m_shareMemSer->m_pSMAllData->m_sm_data[i].ndPid), 0);//ǿ�ƹرս���
				m_shareMemSer->m_pSMAllData->m_sm_data[i].ndPid = 0;
				log_inject(i);//���µ�¼
			}

		}
	}
	return true;
}





void CAccountDlg::OnBnClickedButton1()
{
	// TODO:  ����˺�
	UpdateData(TRUE);
	int index =  m_shareMemSer->findPidEmptyIndex();
	if (index == -1)
	{
		AfxMessageBox("�������Ķ࿪���������������˺ţ���ɾ������������˺ţ�\n");
		return;
	}
	std::stringstream ss;
	m_shareMemSer->m_pSMAllData->m_sm_data[index].userName = (std::string)m_userName.GetString();
	m_shareMemSer->m_pSMAllData->m_sm_data[index].passWord = (std::string)m_password.GetString();
	ss << m_shareMemSer->m_pSMAllData->m_sm_data[index].userName.c_str() << " " << m_shareMemSer->m_pSMAllData->m_sm_data[index].passWord.c_str();
	tools::getInstance()->write2file("..\\Account.txt", ss.str());
}

bool CAccountDlg::initMem()
{
	bool rt=  m_shareMemSer->createShareMemory();
	if (rt)
	{
		for (auto i = 0; i < MORE_OPEN_NUMBER; i++)
		{
			m_shareMemSer->m_pSMAllData->m_sm_data[i].send_rand = 0;
			m_shareMemSer->m_pSMAllData->m_sm_data[i].rcv_rand = 0;
			m_shareMemSer->m_pSMAllData->m_sm_data[i].server_alive = true;
		}
	}
	return rt;
}

bool CAccountDlg::initAccount()
{
	std::vector<std::string> vecA=tools::getInstance()->ReadTxt("..\\Account.txt");
	if (vecA.empty())return false;
	for (auto i=0;i<vecA.size();i++)
	{
		if (i >= MORE_OPEN_NUMBER)return false; //�˺ų������㿪����
		std::vector<std::string> temp = tools::getInstance()->splitString(vecA[i]);
		if (temp.size()<2)continue;
		m_shareMemSer->m_pSMAllData->m_sm_data[i].userName = (std::string)temp[0];
		m_shareMemSer->m_pSMAllData->m_sm_data[i].passWord = (std::string)temp[1];
		temp.clear();
	}
	return true;
}


void CAccountDlg::threadCallBack()
{
	//ˢ�µ��̻߳ص�����
	while (true)
	{
		updateDate();
		//TRACE("111\n");
		Sleep(3000);
	}
}

// TODO:  ɾ���˺ţ���ɾ����ѡ��ѡ�е��˺���Ϣ
void CAccountDlg::OnBnClickedButton2()
{
	std::vector<std::string> vecA = tools::getInstance()->ReadTxt("..\\Account.txt");
	if (vecA.empty())return;
	for (auto i = 0; i < MORE_OPEN_NUMBER; i++)
	{
		if (!m_listCtl.GetCheck(i))continue;
		std::stringstream ss;
		ss << m_shareMemSer->m_pSMAllData->m_sm_data[i].userName << " " << m_shareMemSer->m_pSMAllData->m_sm_data[i].passWord;
		if (vecA.empty())break;
		for (auto it=vecA.begin();it!=vecA.end(); it++)
		{			
			if (memcmp(ss.str().c_str(), (*it).c_str(),strlen(ss.str().c_str()))==0)
			{
				m_shareMemSer->m_pSMAllData->m_sm_data[i].userName = "";
				m_shareMemSer->m_pSMAllData->m_sm_data[i].passWord = "";
				it=vecA.erase(it);
				break;
			}			
		}
		ss.clear();
	}
	if (vecA.empty())
	{
		tools::getInstance()->write2file("..\\Account.txt", "", std::ios::out);
			return;
	}
	tools::getInstance()->write2file("..\\Account.txt", "", std::ios::out);
	for (auto i=0;i<vecA.size();i++)
	{	
		tools::getInstance()->write2file("..\\Account.txt", vecA[i]);
	}
}

// TODO:  ��ʼ�ű� �жϵ�ǰ��ѡ���Ƿ�ѡ�У�ֻ��½ѡ�е�
void CAccountDlg::OnBnClickedButton3()
{
		for (int i = 0; i < MORE_OPEN_NUMBER; i++)
	{		
			if (!m_listCtl.GetCheck(i))continue; //δѡ��
			if ((m_shareMemSer->m_pSMAllData->m_sm_data[i].userName=="")|| //�˺ſ�
				(m_shareMemSer->m_pSMAllData->m_sm_data[i].passWord == "")|| //�����
				(m_shareMemSer->m_pSMAllData->m_sm_data[i].ndPid!=0))continue;//��������
			log_inject(i);
	}
	
}

/*��¼��Ϸ��ע���ȡPID*/
void CAccountDlg::log_inject(int i)
{
	exeLoad e("G:\\��������2.9\\��������\\Data\\woool.dat.update", "G:\\��������2.9\\��������\\Data\\");
	m_shareMemSer->m_pSMAllData->m_sm_data[i].ndPid = e.pi.dwProcessId;
	EipInject in;
	in.eipinjectDll(L"G:\\VS_Projects\\ConsoleFrame-main\\Debug\\TestDll.dll", e.pi);
}
