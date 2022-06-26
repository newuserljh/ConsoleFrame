#pragma once
#include "role.h"
// CTestDlg 对话框

class CTestDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CTestDlg)

public:
	CTestDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTestDlg();

// 对话框数据
	enum { IDD = IDD_DLGTEST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

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

	/*打怪技能设置*/
	DWORD s_ID ;
	CWinThread* m_threadAttack;
	bool tflag_attack;
	static UINT __cdecl threadAttack(LPVOID lparam);
	std::vector<std::string> attack_monlist;

	/*寻路打怪坐标*/
	std::vector<MapXY> map_xy;
	size_t i_map;//寻路点下标
	CWinThread* m_threadGoto;
	bool tflag_goto;
	static UINT __cdecl threadGoto(LPVOID lparam);
	/*捡物设置*/
	std::vector<std::string> pick_goods_list;
	CWinThread* m_threadPickup;
	bool tflag_pickup;
	static UINT __cdecl threadPickup(LPVOID lparam);


	CCriticalSection g_criticalsection; //边界锁，线程间同步用的。

	afx_msg void OnBnClickedButton4();
};
