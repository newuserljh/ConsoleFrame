#pragma once
#include "afxdialogex.h"
#include <afxwin.h>


// CGameDLG 对话框

class CGameDLG : public CDialogEx
{
	DECLARE_DYNAMIC(CGameDLG)

public:
	CGameDLG(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CGameDLG();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_MAIN };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
