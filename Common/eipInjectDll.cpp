#include "eipInjectDll.h"
eipInjectDll::eipInjectDll()
{

}


eipInjectDll::~eipInjectDll()
{

}

/*
�������ܣ�eipע��DLL
����һ��dll������·������
��������Ҫע��Ľ��̽ṹ��
����ֵ����
*/
bool eipInjectDll::InjectDll(WCHAR* dllpath, PROCESS_INFORMATION &pi)
{
	
	SuspendThread(pi.hThread); //�����߳� 
	CONTEXT ct = { 0 };
	ct.ContextFlags = CONTEXT_CONTROL;
	GetThreadContext(pi.hThread, &ct); //��ȡ�������̼߳Ĵ������ 

	DWORD dwSize = sizeof(WCHAR) * 1024; //0-0x100 д���� ֮��д���� 
	BYTE* pProcessMem = (BYTE*)::VirtualAllocEx(pi.hProcess, NULL, dwSize, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	if (NULL == pProcessMem)return  false;
	DWORD dwWrited = 0;
	if(!::WriteProcessMemory(pi.hProcess, (pProcessMem + 0x100), (LPVOID)dllpath, //�Ȱ�·�������ݣ�д���ڴ����0x100��ʼ 
		(wcslen(dllpath) + 1) * sizeof(WCHAR), &dwWrited))return false;

	FARPROC pLoadLibraryW = (FARPROC)::GetProcAddress(::GetModuleHandle("Kernel32"), "LoadLibraryW");
	if(NULL== pLoadLibraryW)return false;
	BYTE ShellCode[32] = { 0 };
	DWORD* pdwAddr = NULL;

	ShellCode[0] = 0x60; // pushad 
	ShellCode[1] = 0x9c; // pushfd 
	ShellCode[2] = 0x68; // push 
	pdwAddr = (DWORD*)&ShellCode[3]; // ShellCode[3/4/5/6] 
	*pdwAddr = (DWORD)(pProcessMem + 0x100);
	ShellCode[7] = 0xe8;//call 
	pdwAddr = (DWORD*)&ShellCode[8]; // ShellCode[8/9/10/11] 
	*pdwAddr = (DWORD)pLoadLibraryW - ((DWORD)(pProcessMem + 7) + 5); // ��Ϊֱ��call��ַ�ˣ����Զ�Ӧ��������Ҫת��������VA 
	ShellCode[12] = 0x9d; // popfd 
	ShellCode[13] = 0x61; // popad 
	ShellCode[14] = 0xe9; // jmp 

	pdwAddr = (DWORD*)&ShellCode[15]; // ShellCode[15/16/17/18] 
	*pdwAddr = ct.Eip - ((DWORD)(pProcessMem + 14) + 5); //��Ϊֱ��jmp��ַ�ˣ����Զ�Ӧ��������Ҫת��������VA 
	if(!::WriteProcessMemory(pi.hProcess, pProcessMem, ShellCode, sizeof(ShellCode), &dwWrited))return false;
	ct.Eip = (DWORD)pProcessMem;
	::SetThreadContext(pi.hThread, &ct);
	::ResumeThread(pi.hThread);
	return true;
}

