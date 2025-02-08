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
	if (AllocConsole())
	{
		// �ض����׼����/���������̨
		freopen("CONIN$", "r", stdin);
		freopen("CONOUT$", "w", stdout);
		freopen("CONOUT$", "w", stderr);

		// ��ѡ�����ÿ���̨����
		SetConsoleTitle("Debug Console");

		// ���������Ϣ������̨
		std::cout << "Debug console initialized." << std::endl;
	}
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

//�����Ӵ���͸��
void SetTransparent(HWND hwnd, BYTE alpha)
{
	// ���ô���Ϊ�ֲ㴰��
	LONG_PTR exStyle = GetWindowLongPtr(hwnd, GWL_EXSTYLE);
	if (!(exStyle & WS_EX_LAYERED))
	{
		SetWindowLongPtr(hwnd, GWL_EXSTYLE, exStyle | WS_EX_LAYERED);
	}

	// ����͸����
	SetLayeredWindowAttributes(hwnd, 0, alpha, LWA_ALPHA);
}

// �ú������մ���������Ŀ��PID��������ƥ��Ĵ��ھ��
HWND FindWindowByClassAndPid(const TCHAR* className, DWORD targetPid) {
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	// ʹ��FindWindow���Ҿ���ָ�������ĵ�һ����������
	HWND hwnd = FindWindow(className, NULL);

	while (hwnd != NULL) {
		DWORD pid;
		// ��ȡ�ô��ڶ�Ӧ�Ľ���ID
		GetWindowThreadProcessId(hwnd, &pid);

		if (pid == targetPid) {
			// ����ҵ��Ĵ���PID��Ŀ��PIDƥ�䣬�򷵻ظô��ھ��
			return hwnd;
		}

		// ������һ����ͬ�����Ĵ��ڣ�������ڣ�
		hwnd = FindWindowEx(NULL, hwnd, className, NULL);
	}

	// ���û���ҵ�ƥ��Ĵ��ڣ�����NULL
	return NULL;
}


void EmbedIntoGameWindow(HWND dlgHwnd, HWND gameHwnd) {
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	// Set the dialog as a child of the game's main window
	if (!::SetParent(dlgHwnd, gameHwnd)) {
		OutputDebugString("SetParent failed\n");
		return;
	}

	// Modify the window style to remove popup and add child styles
	LONG_PTR style = ::GetWindowLongPtr(dlgHwnd, GWL_STYLE);
	style &= ~WS_POPUP; // Remove popup style
	style |= WS_CHILD;  // Add child style
	if (!::SetWindowLongPtr(dlgHwnd, GWL_STYLE, style)) {
		OutputDebugString("SetWindowLong failed\n");
		return;
	}

	 //Optionally modify extended window styles
	LONG_PTR exStyle = ::GetWindowLongPtr(dlgHwnd, GWL_EXSTYLE);
	exStyle &= ~(WS_EX_DLGMODALFRAME | WS_EX_CLIENTEDGE | WS_EX_STATICEDGE);
	if (!::SetWindowLongPtr(dlgHwnd, GWL_EXSTYLE, exStyle)) {
		OutputDebugString("SetWindowLongEx failed\n");
		return;
	}

	// Adjust the size and position of the dialog to fit within the game window
	RECT rc, rc_child;
	if (!::GetClientRect(gameHwnd, &rc)) {
		OutputDebugString("GetClientRect failed\n");
		return;
	}

	if (!::GetClientRect(dlgHwnd, &rc_child) ){
		OutputDebugString("Child GetClientRect failed\n");
		return;
	}

	if (!::MoveWindow(dlgHwnd, 0, 0, rc_child.right - rc_child.left, rc_child.bottom - rc_child.top, TRUE)) {
		OutputDebugString("MoveWindow failed\n");
		return;
	}

  // ����͸����Ϊ 128 (��͸��)
	SetTransparent(dlgHwnd, 128);

	OutputDebugString("Successfully embedded MFC dialog into game window\n");
}
//�̺߳���������������������
void threadFunc()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	//mhook = SetWindowsHookEx(WH_KEYBOARD, KeyBoardProc, 0, GetMainThreadId());
	if (pDlg == NULL)
	{
		pDlg = new CTestDlg;
		if(!pDlg->Create(IDD_DLGTEST)) {
			OutputDebugString("MFC dialog creation failed\n");
			return;
		}
		pDlg->ShowWindow(SW_SHOW);
		OutputDebugString("MFC dialog created successfully\n");

		DWORD processId = GetCurrentProcessId();
		HWND hGameWnd = FindWindowByClassAndPid("WOLIICLIENT",processId);
		if (hGameWnd) {
			EmbedIntoGameWindow(pDlg->m_hWnd, hGameWnd);
		}
		else {
			OutputDebugString("Main window handle is invalid\n");
		}
	
	}
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))

	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return;
}


int CTestDllApp::ExitInstance()
{
	// TODO: �ڴ����ר�ô����/����û���
	  // �ͷſ���̨
	FreeConsole();
	return CWinApp::ExitInstance();
}
