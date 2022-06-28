// TestDll.cpp : ���� DLL �ĳ�ʼ�����̡�
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
//TODO:  ����� DLL ����� MFC DLL �Ƕ�̬���ӵģ�
//		��Ӵ� DLL �������κε���
//		MFC �ĺ������뽫 AFX_MANAGE_STATE ����ӵ�
//		�ú�������ǰ�档
//
//		����: 
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// �˴�Ϊ��ͨ������
//		}
//
//		�˺������κ� MFC ����
//		������ÿ��������ʮ����Ҫ��  ����ζ��
//		��������Ϊ�����еĵ�һ�����
//		���֣������������ж������������
//		������Ϊ���ǵĹ��캯���������� MFC
//		DLL ���á�
//
//		�й�������ϸ��Ϣ��
//		����� MFC ����˵�� 33 �� 58��
//

// CTestDllApp

BEGIN_MESSAGE_MAP(CTestDllApp, CWinApp)
END_MESSAGE_MAP()


// CTestDllApp ����

CTestDllApp::CTestDllApp()
{
	// TODO:  �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CTestDllApp ����

CTestDllApp theApp;


// CTestDllApp ��ʼ��

BOOL CTestDllApp::InitInstance()
{
	CWinApp::InitInstance();

	return init();
}

bool CTestDllApp::init()
{
	//�Զ����ʼ������
	//����ע�뷽ʽ���̻߳�����this dllж���˳��̣߳�
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

//�̺߳���������������������
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
