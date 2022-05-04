
// ConsoleDlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"
#include "AccountDlg.h"
#include "ConfigDlg.h"
#include "shareMemorySer.h"
#include <memory>
#include <thread>
// CConsoleDlg �Ի���
class CConsoleDlg : public CDialogEx
{
// ����
public:
	CConsoleDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_CONSOLE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CTabCtrl m_tabCtl;
	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);

	CAccountDlg m_accountDlg;
	CConfigDlg m_configDlg;
	CDialogEx*  m_pDialog[2];


private:
	bool initDlg();

public:
	int m_curSelTab;
};
