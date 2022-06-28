// TestDll.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "TestDll.h"
#include "TestDlg.h"
#include <thread>
#include <memory>
#include <tlhelp32.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CTestDlg* pDlg = NULL;
HHOOK mhook = 0;
//
//TODO:  如果此 DLL 相对于 MFC DLL 是动态链接的，
//		则从此 DLL 导出的任何调入
//		MFC 的函数必须将 AFX_MANAGE_STATE 宏添加到
//		该函数的最前面。
//
//		例如: 
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// 此处为普通函数体
//		}
//
//		此宏先于任何 MFC 调用
//		出现在每个函数中十分重要。  这意味着
//		它必须作为函数中的第一个语句
//		出现，甚至先于所有对象变量声明，
//		这是因为它们的构造函数可能生成 MFC
//		DLL 调用。
//
//		有关其他详细信息，
//		请参阅 MFC 技术说明 33 和 58。
//

// CTestDllApp

BEGIN_MESSAGE_MAP(CTestDllApp, CWinApp)
END_MESSAGE_MAP()


// CTestDllApp 构造

CTestDllApp::CTestDllApp()
{
	// TODO:  在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CTestDllApp 对象

CTestDllApp theApp;


// CTestDllApp 初始化

BOOL CTestDllApp::InitInstance()
{
	CWinApp::InitInstance();

	return init();
}

bool CTestDllApp::init()
{
	//自定义初始化函数
	//这种注入方式，线程会随着this dll卸载退出线程！
	::CreateThread(NULL, NULL, LPTHREAD_START_ROUTINE(threadFunc), NULL, NULL, NULL);
	return true;
}

LRESULT CALLBACK KeyBoardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if ((wParam == VK_HOME) && ((lParam & 0x40000000) != 0))
	{
		if(pDlg->IsWindowVisible())pDlg->ShowWindow(SW_HIDE);
		else pDlg->ShowWindow(SW_SHOW); //SW_SHOW

	}
	return CallNextHookEx(0, nCode, wParam, lParam);
}

DWORD GetMainThreadId(DWORD processId = 0)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if (processId == 0)
		processId = GetCurrentProcessId();

	DWORD threadId = 0;
	THREADENTRY32 te32 = { sizeof(te32) };
	HANDLE threadSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
	if (Thread32First(threadSnap, &te32))
	{
		do
		{
			if (processId == te32.th32OwnerProcessID)
			{
				threadId = te32.th32ThreadID;
				break;
			}
		} while (Thread32Next(threadSnap, &te32));
	}
	return threadId;
}

//线程函数，用来创建函数窗口
void threadFunc()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	mhook = SetWindowsHookEx(WH_KEYBOARD, KeyBoardProc, 0, GetMainThreadId());
	if (pDlg == NULL)
	{
		pDlg = new CTestDlg;
		pDlg->Create(IDD_DLGTEST);
	}
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))

	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return;
}
