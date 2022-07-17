// ConfigDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Console.h"
#include "ConfigDlg.h"
#include "afxdialogex.h"


// CConfigDlg 对话框

IMPLEMENT_DYNAMIC(CConfigDlg, CDialogEx)

CConfigDlg::CConfigDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CConfigDlg::IDD, pParent)
{

}

CConfigDlg::~CConfigDlg()
{
}

void CConfigDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CConfigDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CConfigDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON__CHOOSE_DIR, &CConfigDlg::OnBnClickedButton)
END_MESSAGE_MAP()


// CConfigDlg 消息处理程序


void CConfigDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CConfigDlg::OnBnClickedButton()
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
		str.Format("选择的目录为 %s", szPath);
	   AfxMessageBox(str); 
		//编辑框中显示所选内容
		//CWnd* pWnd = GetDlgItem(IDC_EDIT_OUTNAME);
		//pWnd->SetWindowText(szPath);
		//pWnd->GetDC()->SetTextColor(m_TextBlackColor);
	}
	else
		AfxMessageBox("无效的目录，请重新选择");
	
}