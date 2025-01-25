// AccountDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Console.h"
#include "AccountDlg.h"
#include "afxdialogex.h"
#include <sstream>
#include <thread>
#include <windows.h>
#include "utils.h"
// CAccountDlg �Ի���
std::string GAME_DIR;

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
	ON_BN_CLICKED(IDC_BTN_CS_DIR, &CAccountDlg::OnBnClickedBtnCsDir)
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
	initGameDir();
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





void CAccountDlg::OnBnClickedButton1() //����˺�
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

bool CAccountDlg::initMem() //��ʼ�������ڴ�
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
		strcat_s(m_shareMemSer->m_pSMAllData->currDir, tools::getInstance()->GetCurrDir().c_str());
		m_shareMemSer->m_pSMAllData->team_info = 1;
	}
	return rt;
}

bool CAccountDlg::initAccount() //��ȡ�˺�
{
	std::vector<std::string> vecA=tools::getInstance()->ReadTxt("..\\Account.txt");
	if (vecA.empty())return false;
	for (size_t i=0;i<vecA.size();i++)
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

bool CAccountDlg::initGameDir() //��ȡ��ϷĿ¼
{
	if (!tools::getInstance()->fileIsexist(".\\cfg\\gamedir.cfg"))return false;
	std::vector<std::string> vecA = tools::getInstance()->ReadTxt(".\\cfg\\gamedir.cfg");
	if (vecA.empty())return false;
	GAME_DIR = vecA[0];
	vecA.clear();
	if(!tools::getInstance()->fileIsexist(GAME_DIR +"lua51.dll"))GAME_DIR.clear();
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
	for (size_t i=0;i<vecA.size();i++)
	{	
		tools::getInstance()->write2file("..\\Account.txt", vecA[i]);
	}
}

// TODO:  ��ʼ�ű� �жϵ�ǰ��ѡ���Ƿ�ѡ�У�ֻ��½ѡ�е�
void CAccountDlg::OnBnClickedButton3()
{
	if (GAME_DIR.empty())
	{
		AfxMessageBox("����ѡ��Ŀ¼������������dataĿ¼");
		return;
	}
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
	std::string gamePath=GAME_DIR+"woool.dat.update";
	PROCESS_INFORMATION pi;
	tools::getInstance()->exeload(gamePath, GAME_DIR,pi);
	m_shareMemSer->m_pSMAllData->m_sm_data[i].ndPid = pi.dwProcessId;
	std::string dllPath  =std::string( m_shareMemSer->m_pSMAllData->currDir)+"TestDll.dll";
	tools::getInstance()->eipinjectDll(tools::getInstance()->char2wchar(dllPath.c_str()),pi);
}

// TODO: ѡ�������ڵ�Ŀ¼,���ͷ�lua51.dll����ϷĿ¼
void CAccountDlg::OnBnClickedBtnCsDir()
{
	char szPath[MAX_PATH]; //���ѡ���Ŀ¼·�� 
	CString str;
	ZeroMemory(szPath, sizeof(szPath));
	BROWSEINFO bi;
	bi.hwndOwner = m_hWnd;
	bi.pidlRoot = NULL;
	bi.pszDisplayName = szPath;
	bi.lpszTitle = "��ѡ����Ҫ�����Ŀ¼��";
	bi.ulFlags = 0;
	bi.lpfn = NULL;
	bi.lParam = 0;
	bi.iImage = 0;
	//����ѡ��Ŀ¼�Ի���
	LPITEMIDLIST lp = SHBrowseForFolder(&bi);

	if (lp && SHGetPathFromIDList(lp, szPath))
	{
		GAME_DIR = szPath;
		GAME_DIR = GAME_DIR + "\\";
		if (!tools::getInstance()->fileIsexist(GAME_DIR + "woool.dat.update"))
		{
			AfxMessageBox("��Ч��Ŀ¼���޷���λ��woool.dat.update��������ѡ��");
			return;
		}
		CreateDirectory(".\\cfg", NULL);// ���� cfg �ļ��У���������ڣ�
		tools::getInstance()->write2file(".\\cfg\\gamedir.cfg", GAME_DIR.c_str(),// Ŀ¼д�� cfg/gamedir.cfg 
			std::ios::out /*��ģʽ�򿪶����ļ���������*/);

		if (!tools::getInstance()->ReleaseResource(IDR_DLLRES1,GAME_DIR+"lua51.dll", "DLLRES", false))
		{
			AfxMessageBox("�޷��ͷ�lua51.dll����ϷĿ¼�����ֶ����Ƶ�ָ��Ŀ¼");
		}
	}
	else
		AfxMessageBox("��Ч��Ŀ¼��������ѡ��");
}
