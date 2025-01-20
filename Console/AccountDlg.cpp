// AccountDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Console.h"
#include "AccountDlg.h"
#include "afxdialogex.h"
#include <sstream>
#include <thread>
#include <windows.h>
#include "utils.h"
// CAccountDlg 对话框
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


// CAccountDlg 消息处理程序


// 初始化表单信息
bool CAccountDlg::initTables()
{
	//设置风格

	DWORD dwStyle = m_listCtl.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;// 选中某行使整行高亮（只适用与report 风格的listctrl ） 
	dwStyle |= LVS_EX_GRIDLINES;// 网格线（只适用与report 风格的listctrl ） 
	dwStyle |= LVS_EX_CHECKBOXES;//item 前生成checkbox 控件 
	m_listCtl.SetExtendedStyle(dwStyle); // 设置扩展风格 

	//初始化表头
	int i = 0;
	m_listCtl.InsertColumn(i++, "Pid", LVCFMT_LEFT, 80);
	m_listCtl.InsertColumn(i++, "用户名", LVCFMT_LEFT, 80);
	m_listCtl.InsertColumn(i++, "密码", LVCFMT_LEFT, 80);
	m_listCtl.InsertColumn(i++, "当前脚本", LVCFMT_LEFT, 80);

	//插入MORE_OPEN_NUM行

	for (int i= 0; i < MORE_OPEN_NUMBER; i++)
	{
		m_listCtl.InsertItem(i, ""); 
	}

	return true;
}


BOOL CAccountDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	if (!initTables())
		return FALSE;
	if (!initMem())
		return FALSE;
	initAccount();
	initGameDir();
	auto tthread = std::thread(&CAccountDlg::threadCallBack, this);
	tthread.detach();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}


bool CAccountDlg::updateDate()
{
	for (int i = 0; i < MORE_OPEN_NUMBER; i++)
	{
		//更新数据到表单中
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
				::TerminateProcess(::OpenProcess(PROCESS_ALL_ACCESS, false, m_shareMemSer->m_pSMAllData->m_sm_data[i].ndPid),0);//强制关闭进程
				m_shareMemSer->m_pSMAllData->m_sm_data[i].ndPid = 0;
				log_inject(i);//重新登录
			}
			else
			{
				srand((unsigned int)time(0));//先设置种子
				m_shareMemSer->m_pSMAllData->m_sm_data[i].send_rand = rand() + 1;
			}
			if (m_shareMemSer->m_pSMAllData->m_sm_data[i].server_alive ==false)//验证游戏服务器状态
			{
				::TerminateProcess(::OpenProcess(PROCESS_ALL_ACCESS, false, m_shareMemSer->m_pSMAllData->m_sm_data[i].ndPid), 0);//强制关闭进程
				m_shareMemSer->m_pSMAllData->m_sm_data[i].ndPid = 0;
				log_inject(i);//重新登录
			}

		}
	}
	return true;
}





void CAccountDlg::OnBnClickedButton1() //添加账号
{
	// TODO:  添加账号
	UpdateData(TRUE);
	int index =  m_shareMemSer->findPidEmptyIndex();
	if (index == -1)
	{
		AfxMessageBox("超过最大的多开数量，如果想添加账号，请删除上面的任意账号！\n");
		return;
	}
	std::stringstream ss;
	m_shareMemSer->m_pSMAllData->m_sm_data[index].userName = (std::string)m_userName.GetString();
	m_shareMemSer->m_pSMAllData->m_sm_data[index].passWord = (std::string)m_password.GetString();
	ss << m_shareMemSer->m_pSMAllData->m_sm_data[index].userName.c_str() << " " << m_shareMemSer->m_pSMAllData->m_sm_data[index].passWord.c_str();
	tools::getInstance()->write2file("..\\Account.txt", ss.str());
}

bool CAccountDlg::initMem() //初始化共享内存
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

bool CAccountDlg::initAccount() //读取账号
{
	std::vector<std::string> vecA=tools::getInstance()->ReadTxt("..\\Account.txt");
	if (vecA.empty())return false;
	for (size_t i=0;i<vecA.size();i++)
	{
		if (i >= MORE_OPEN_NUMBER)return false; //账号超过最大躲开数量
		std::vector<std::string> temp = tools::getInstance()->splitString(vecA[i]);
		if (temp.size()<2)continue;
		m_shareMemSer->m_pSMAllData->m_sm_data[i].userName = (std::string)temp[0];
		m_shareMemSer->m_pSMAllData->m_sm_data[i].passWord = (std::string)temp[1];
		temp.clear();
	}
	return true;
}

bool CAccountDlg::initGameDir() //读取游戏目录
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
	//刷新的线程回调函数
	while (true)
	{
		updateDate();
		//TRACE("111\n");
		Sleep(3000);
	}
}

// TODO:  删除账号，是删除复选框选中的账号信息
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

// TODO:  开始脚本 判断当前复选框是否被选中，只登陆选中的
void CAccountDlg::OnBnClickedButton3()
{
	if (GAME_DIR.empty())
	{
		AfxMessageBox("请先选择目录，到传奇世界data目录");
		return;
	}
		for (int i = 0; i < MORE_OPEN_NUMBER; i++)
	{		
			if (!m_listCtl.GetCheck(i))continue; //未选中
			if ((m_shareMemSer->m_pSMAllData->m_sm_data[i].userName=="")|| //账号空
				(m_shareMemSer->m_pSMAllData->m_sm_data[i].passWord == "")|| //密码空
				(m_shareMemSer->m_pSMAllData->m_sm_data[i].ndPid!=0))continue;//已启动过
			log_inject(i);
	}
	
}

/*登录游戏并注入获取PID*/
void CAccountDlg::log_inject(int i)
{
	std::string gamePath=GAME_DIR+"woool.dat.update";
	PROCESS_INFORMATION pi;
	tools::getInstance()->exeload(gamePath, GAME_DIR,pi);
	m_shareMemSer->m_pSMAllData->m_sm_data[i].ndPid = pi.dwProcessId;
	std::string dllPath  =std::string( m_shareMemSer->m_pSMAllData->currDir)+"TestDll.dll";
	tools::getInstance()->eipinjectDll(tools::getInstance()->char2wchar(dllPath.c_str()),pi);
}

// TODO: 选择传世所在的目录,并释放lua51.dll到游戏目录
void CAccountDlg::OnBnClickedBtnCsDir()
{
	char szPath[MAX_PATH]; //存放选择的目录路径 
	CString str;
	ZeroMemory(szPath, sizeof(szPath));
	BROWSEINFO bi;
	bi.hwndOwner = m_hWnd;
	bi.pidlRoot = NULL;
	bi.pszDisplayName = szPath;
	bi.lpszTitle = "请选择需要输出的目录：";
	bi.ulFlags = 0;
	bi.lpfn = NULL;
	bi.lParam = 0;
	bi.iImage = 0;
	//弹出选择目录对话框
	LPITEMIDLIST lp = SHBrowseForFolder(&bi);

	if (lp && SHGetPathFromIDList(lp, szPath))
	{
		GAME_DIR = szPath;
		GAME_DIR = GAME_DIR + "\\";
		if (!tools::getInstance()->fileIsexist(GAME_DIR + "woool.dat.update"))
		{
			AfxMessageBox("无效的目录，无法定位到woool.dat.update，请重新选择");
			return;
		}
		CreateDirectory(".\\cfg", NULL);// 创建 cfg 文件夹（如果不存在）
		tools::getInstance()->write2file(".\\cfg\\gamedir.cfg", GAME_DIR.c_str(),// 目录写入 cfg/gamedir.cfg 
			std::ios::out /*此模式打开丢弃文件所有内容*/);

		if (!tools::getInstance()->ReleaseResource(IDR_DLLRES1,GAME_DIR+"lua51.dll", "DLLRES", false))
		{
			AfxMessageBox("无法释放lua51.dll到游戏目录，请手动复制到指定目录");
		}
	}
	else
		AfxMessageBox("无效的目录，请重新选择");
}
