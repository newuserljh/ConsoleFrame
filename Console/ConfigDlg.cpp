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
END_MESSAGE_MAP()


// CConfigDlg ��Ϣ�������
