#pragma once
#include "role.h"
// CTestDlg �Ի���

class CTestDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CTestDlg)

public:
	CTestDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CTestDlg();

// �Ի�������
	enum { IDD = IDD_DLGTEST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	virtual BOOL OnInitDialog();
	CEdit m_edit2;
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton8();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton9();
	bool Set_Skill();
	bool Load_coordinate();
	bool initVariable();

	/*��ּ�������*/
	DWORD s_ID ;
	CWinThread* m_threadAttack;
	bool tflag_attack;
	static UINT __cdecl threadAttack(LPVOID lparam);
	std::vector<std::string> attack_monlist;

	/*Ѱ·�������*/
	std::vector<MapXY> map_xy;
	size_t i_map;//Ѱ·���±�
	CWinThread* m_threadGoto;
	bool tflag_goto;
	static UINT __cdecl threadGoto(LPVOID lparam);
	/*��������*/
	std::vector<std::string> pick_goods_list;
	CWinThread* m_threadPickup;
	bool tflag_pickup;
	static UINT __cdecl threadPickup(LPVOID lparam);


	CCriticalSection g_criticalsection; //�߽������̼߳�ͬ���õġ�

	afx_msg void OnBnClickedButton4();
};
