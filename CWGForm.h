#pragma once
#include "afxdialogex.h"
#include "GameTip.h"
#include "afxmt.h"
#include "common.h"

extern  HWND g_hwnd[20] ;
extern  size_t hwnd_count ;
extern HHOOK g_hook[20];

// CWGForm 对话框

class CWGForm : public CDialogEx
{
	//DECLARE_DYNAMIC(CWGForm)

public:
	CWGForm(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CWGForm();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_MAIN };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	DECLARE_MESSAGE_MAP()

private:

public:
	static UINT __cdecl ThreadProc_Environmrnt(LPVOID nParam);
	static UINT __cdecl ThreadProc_AutoAttack(LPVOID nParam);
	static UINT __cdecl ThreadProc_Goto(LPVOID nParam);
	static UINT __cdecl ThreadProc_Mapping(LPVOID nParam);
	afx_msg void OnBnClickedBtnCalltest();
	afx_msg void OnBnClickedButtonStop();
	afx_msg void MessageBox(DWORD dw);
	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBtnPause();


	CListCtrl m_Monster_Listctrl;
};

