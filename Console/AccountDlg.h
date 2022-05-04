#pragma once
#include "afxcmn.h"
#include <string>
#include "SMstruct.h"
#include "shareMemorySer.h"
// CAccountDlg �Ի���

class CAccountDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CAccountDlg)

public:
	CAccountDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CAccountDlg();

// �Ի�������
	enum { IDD = IDD_DLGACCOUNT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

private:
	bool initMem();
	std::shared_ptr<shareMemorySer>m_shareMemSer;
	void threadCallBack();

public:
	CListCtrl m_listCtl;
	// ��ʼ������Ϣ
	bool initTables();

	virtual BOOL OnInitDialog();
	//���¹����ڴ�����ݵ�����
	bool updateDate();

	afx_msg void OnBnClickedButton1();
	CString m_userName;
	CString m_password;
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
};
