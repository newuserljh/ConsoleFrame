#include "eipInjectDll.h"
eipInjectDll::eipInjectDll()
{

}


eipInjectDll::~eipInjectDll()
{

}

/*
函数功能：eip注入DLL
参数一：dll的完整路径名字
参数二：要注入的进程结构体
返回值：无
*/
bool eipInjectDll::InjectDll(WCHAR* dllpath, PROCESS_INFORMATION &pi)
{
	
	SuspendThread(pi.hThread); //挂起线程 
	CONTEXT ct = { 0 };
	ct.ContextFlags = CONTEXT_CONTROL;
	GetThreadContext(pi.hThread, &ct); //获取，保存线程寄存器相关 

	DWORD dwSize = sizeof(WCHAR) * 1024; //0-0x100 写代码 之后写数据 
	BYTE* pProcessMem = (BYTE*)::VirtualAllocEx(pi.hProcess, NULL, dwSize, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	if (NULL == pProcessMem)return  false;
	DWORD dwWrited = 0;
	if(!::WriteProcessMemory(pi.hProcess, (pProcessMem + 0x100), (LPVOID)dllpath, //先把路径（数据）写到内存里，从0x100开始 
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
	*pdwAddr = (DWORD)pLoadLibraryW - ((DWORD)(pProcessMem + 7) + 5); // 因为直接call地址了，所以对应机器码需要转换，计算VA 
	ShellCode[12] = 0x9d; // popfd 
	ShellCode[13] = 0x61; // popad 
	ShellCode[14] = 0xe9; // jmp 

	pdwAddr = (DWORD*)&ShellCode[15]; // ShellCode[15/16/17/18] 
	*pdwAddr = ct.Eip - ((DWORD)(pProcessMem + 14) + 5); //因为直接jmp地址了，所以对应机器码需要转换，计算VA 
	if(!::WriteProcessMemory(pi.hProcess, pProcessMem, ShellCode, sizeof(ShellCode), &dwWrited))return false;
	ct.Eip = (DWORD)pProcessMem;
	::SetThreadContext(pi.hThread, &ct);
	::ResumeThread(pi.hThread);
	return true;
}

