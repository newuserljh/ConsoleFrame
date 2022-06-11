// CGameDLG.cpp: 实现文件
//

#include "pch.h"
#include "GameDll.h"
#include "CGameDLG.h"

// CGameDLG 对话框

IMPLEMENT_DYNAMIC(CGameDLG, CDialogEx)

CGameDLG::CGameDLG(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DLG_MAIN, pParent)
{

}

CGameDLG::~CGameDLG()
{
}

void CGameDLG::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CGameDLG, CDialogEx)
END_MESSAGE_MAP()


// CGameDLG 消息处理程序
