// ConfigDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Console.h"
#include "ConfigDlg.h"
#include "afxdialogex.h"


// CConfigDlg �Ի���

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


// CConfigDlg ��Ϣ�������


void CConfigDlg::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}


void CConfigDlg::OnBnClickedButton()
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
		str.Format("ѡ���Ŀ¼Ϊ %s", szPath);
	   AfxMessageBox(str); 
		//�༭������ʾ��ѡ����
		//CWnd* pWnd = GetDlgItem(IDC_EDIT_OUTNAME);
		//pWnd->SetWindowText(szPath);
		//pWnd->GetDC()->SetTextColor(m_TextBlackColor);
	}
	else
		AfxMessageBox("��Ч��Ŀ¼��������ѡ��");
	
}