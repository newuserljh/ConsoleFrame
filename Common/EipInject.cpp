#include "EipInject.h"

EipInject::EipInject(){}
EipInject::~EipInject(){}


/*
��������:�ٳ�EIPע��DLl
����һ:dll·��������
������:Ҫע��Ľ�����Ϣ
*/
void EipInject::eipinjectDll(WCHAR* dllname, PROCESS_INFORMATION pi) {

	SuspendThread(pi.hThread);                         //�����߳�                     

	CONTEXT ct = { 0 };
	ct.ContextFlags = CONTEXT_CONTROL;
	GetThreadContext(pi.hThread, &ct);                 //��ȡ�������̼߳Ĵ������


	DWORD dwSize = sizeof(WCHAR) * 1024;             //0-0x100 д���� ֮��д����
	BYTE* pProcessMem = (BYTE*)::VirtualAllocEx(pi.hProcess, NULL, dwSize, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	DWORD dwWrited = 0;
	::WriteProcessMemory(pi.hProcess, (pProcessMem + 0x100), dllname,   //�Ȱ�·�������ݣ�д���ڴ����0x100��ʼ
		(wcslen(dllname) + 1) * sizeof(WCHAR), &dwWrited);



	FARPROC pLoadLibraryW = (FARPROC)::GetProcAddress(::GetModuleHandle("Kernel32"), "LoadLibraryW");
	BYTE ShellCode[32] = { 0 };
	DWORD* pdwAddr = NULL;


	ShellCode[0] = 0x60; // pushad
	ShellCode[1] = 0x9c; // pushfd
	ShellCode[2] = 0x68; // push
	pdwAddr = (DWORD*)&ShellCode[3]; // ShellCode[3/4/5/6]
	*pdwAddr = (DWORD)(pProcessMem + 0x100);
	ShellCode[7] = 0xe8;//call
	pdwAddr = (DWORD*)&ShellCode[8]; // ShellCode[8/9/10/11]
	*pdwAddr = (DWORD)pLoadLibraryW - ((DWORD)(pProcessMem + 7) + 5);  // ��Ϊֱ��call��ַ�ˣ����Զ�Ӧ��������Ҫת��������VA
	ShellCode[12] = 0x9d; // popfd
	ShellCode[13] = 0x61; // popad
	ShellCode[14] = 0xe9; // jmp


	pdwAddr = (DWORD*)&ShellCode[15]; // ShellCode[15/16/17/18]
	*pdwAddr = ct.Eip - ((DWORD)(pProcessMem + 14) + 5);  //��Ϊֱ��jmp��ַ�ˣ����Զ�Ӧ��������Ҫת��������VA
	::WriteProcessMemory(pi.hProcess, pProcessMem, ShellCode, sizeof(ShellCode), &dwWrited);
	ct.Eip = (DWORD)pProcessMem;
	::SetThreadContext(pi.hThread, &ct);


	::ResumeThread(pi.hThread);



}
