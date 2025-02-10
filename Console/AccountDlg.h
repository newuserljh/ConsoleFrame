#pragma once
#include "afxcmn.h"
#include <string>
#include <atomic>
#include "../Common/SMstruct.h"
#include "../Common/shareMemorySer.h"
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
	bool initAccount();
	bool initGameDir();
	std::atomic<bool> stopThread{ false };  // ����֪ͨ�߳�ֹͣ
	void threadCallBack();//ˢ��ListCtrl
	CWinThread* m_pThread_login = nullptr; // �����߳�ָ��
	static UINT __cdecl ThreadLogin(LPVOID pParam);
	std::string GAME_DIR; //������ϷĿ¼


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
	void log_inject(int i);
	afx_msg void OnBnClickedBtnCsDir();
	afx_msg void OnDestroy();
};
