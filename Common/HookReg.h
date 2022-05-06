#pragma once
#include <windows.h>
#include "utils.h"

class HookReg
{
public:
	HookReg();
	~HookReg();
	bool hookReg(DWORD hookadd, UINT length, LPVOID MyCall);
	bool Unhook();
	DWORD hookAddr;
	HANDLE handle;   //hook进程句柄;
	UINT hookLength; //Hook长度;
	BYTE* hookCode; //保存Hook源代码;
	DWORD OldProtect;/*之前的保护属性*/
		//保存8个32位通用寄存器
	DWORD EAX = 0;
	DWORD ECX = 0;
	DWORD EDX = 0;
	DWORD EBX = 0;
	DWORD ESP = 0;
	DWORD EBP = 0;
	DWORD ESI = 0;
	DWORD EDI = 0;


protected:
private:
};

/*
构造函数 初始化
参数一:Hook地址;
参数二:hook的长度;
*/
HookReg::HookReg()
{
}

/*
析构函数;
*/
HookReg::~HookReg()
{
	Unhook();
	free(hookCode);
}

/*
函数:Hook获取寄存器值并实现自己的功能;
参数一:HOOK地址
参数二:HOOK长度
参数三:功能Call地址;
*/
bool HookReg::hookReg(DWORD hookadd, UINT length, LPVOID MyCall)
{
	handle = GetCurrentProcess();
	hookAddr = hookadd;
	hookLength = length;
	hookCode = (BYTE*)malloc(hookLength);/*源代码*/
	if (!VirtualProtectEx(handle, (LPVOID)hookAddr, hookLength, PAGE_EXECUTE_READWRITE, &OldProtect))
	{
		return false;
	}
	if (!ReadProcessMemory(handle, (LPVOID)hookAddr, hookCode, hookLength, NULL))
	{
		VirtualProtectEx(handle, (LPVOID)hookAddr, hookLength, OldProtect, &OldProtect);/*恢复保护属性*/
		return false;
	}
	BYTE* nopCode = (BYTE*)malloc(hookLength);/*源代码nop掉*/
	memset(nopCode, 0x90, hookLength);

	/*----------------------钩子空间--------------------------------------*/
	BYTE* myCodeAddr = (BYTE*)malloc(hookLength + 47 + 5 + 5);/*钩子空间:源代码+保存寄存器值+CALL功能+回跳Jmp*/
	BYTE Shellcode[47] = { 0 };/*保存mov8个寄存器code*/
	BYTE CallAsm[5] = { 0 };/*保存功能call代码*/
	BYTE JmpBack[5] = { 0 };/*保存回调代码*/
	Shellcode[0] = 0xA3;
	Shellcode[1] = (SIZE_T)&EAX & 0xFF;
	Shellcode[2] = (SIZE_T)&EAX >> 8 & 0xFF;
	Shellcode[3] = (SIZE_T)&EAX >> 16 & 0xFF;
	Shellcode[4] = (SIZE_T)&EAX >> 24 & 0xFF;

	Shellcode[5] = 0x89;
	Shellcode[6] = 0x0d;
	Shellcode[7] = (SIZE_T)&ECX & 0xFF;
	Shellcode[8] = (SIZE_T)&ECX >> 8 & 0xFF;
	Shellcode[9] = (SIZE_T)&ECX >> 16 & 0xFF;
	Shellcode[10] = (SIZE_T)&ECX >> 24 & 0xFF;

	Shellcode[11] = 0x89;
	Shellcode[12] = 0x15;
	Shellcode[13] = (SIZE_T)&EDX & 0xFF;
	Shellcode[14] = (SIZE_T)&EDX >> 8 & 0xFF;
	Shellcode[15] = (SIZE_T)&EDX >> 16 & 0xFF;
	Shellcode[16] = (SIZE_T)&EDX >> 24 & 0xFF;

	Shellcode[17] = 0x89;
	Shellcode[18] = 0x1d;
	Shellcode[19] = (SIZE_T)&EBX & 0xFF;
	Shellcode[20] = (SIZE_T)&EBX >> 8 & 0xFF;
	Shellcode[21] = (SIZE_T)&EBX >> 16 & 0xFF;
	Shellcode[22] = (SIZE_T)&EBX >> 24 & 0xFF;

	Shellcode[23] = 0x89;
	Shellcode[24] = 0x25;
	Shellcode[25] = (SIZE_T)&ESP & 0xFF;
	Shellcode[26] = (SIZE_T)&ESP >> 8 & 0xFF;
	Shellcode[27] = (SIZE_T)&ESP >> 16 & 0xFF;
	Shellcode[28] = (SIZE_T)&ESP >> 24 & 0xFF;

	Shellcode[29] = 0x89;
	Shellcode[30] = 0x2d;
	Shellcode[31] = (SIZE_T)&EBP & 0xFF;
	Shellcode[32] = (SIZE_T)&EBP >> 8 & 0xFF;
	Shellcode[33] = (SIZE_T)&EBP >> 16 & 0xFF;
	Shellcode[34] = (SIZE_T)&EBP >> 24 & 0xFF;


	Shellcode[35] = 0x89;
	Shellcode[36] = 0x35;
	Shellcode[37] = (SIZE_T)&ESI & 0xFF;
	Shellcode[38] = (SIZE_T)&ESI >> 8 & 0xFF;
	Shellcode[39] = (SIZE_T)&ESI >> 16 & 0xFF;
	Shellcode[40] = (SIZE_T)&ESI >> 24 & 0xFF;

	Shellcode[41] = 0x89;
	Shellcode[42] = 0x3D;
	Shellcode[43] = (SIZE_T)&EDI & 0xFF;
	Shellcode[44] = (SIZE_T)&EDI >> 8 & 0xFF;
	Shellcode[45] = (SIZE_T)&EDI >> 16 & 0xFF;
	Shellcode[46] = (SIZE_T)&EDI >> 24 & 0xFF;


	CallAsm[0] = 0xE8;
	*(DWORD*)(CallAsm + 1) = (DWORD)MyCall - ((DWORD)myCodeAddr + hookLength + 47 + 5);
	JmpBack[0] = 0xE9;
	*(DWORD*)(JmpBack + 1) = (DWORD)hookAddr + 5 - ((DWORD)myCodeAddr + hookLength + 47 + 5 + 5);
	DWORD bProtect = 0;

	if (!VirtualProtectEx(handle, myCodeAddr, hookLength + 47 + 5 + 5, PAGE_EXECUTE_READWRITE, &bProtect))
	{
		return false;
	}
	if (!WriteProcessMemory(handle, myCodeAddr, hookCode, hookLength, NULL))/*向钩子空间写入源代码*/
	{
		return false;
	}
	if (!WriteProcessMemory(handle, myCodeAddr + hookLength, Shellcode, 47, NULL))/*向钩子空间写入MOV代码*/
	{
		return false;
	}
	if (!WriteProcessMemory(handle, myCodeAddr + hookLength + 47, CallAsm, 5, NULL))/*向钩子空间写入CALl*/
	{
		return false;
	}
	if (!WriteProcessMemory(handle, myCodeAddr + hookLength + 47 + 5, JmpBack, 5, NULL))/*向钩子空间写入回调*/
	{
		return false;
	}
	//VirtualProtectEx(handle, myCodeAddr, hookLength + 47 + 5 + 5, bProtect, &bProtect);//回复狗子空间保护属性
	BYTE JmpTo[5] = { 0 };
	JmpTo[0] = 0XE9;
	*(DWORD*)(JmpTo + 1) = (DWORD)myCodeAddr - hookAddr - 5;
	if (!WriteProcessMemory(handle, (LPVOID)hookAddr, nopCode, hookLength, NULL))/*源代码nop*/
	{
		return false;
	}
	if (!WriteProcessMemory(handle, (LPVOID)hookAddr, JmpTo, 5, NULL))/*向源地址写入跳转指令*/
	{
		return false;
	}
	VirtualProtectEx(handle, (LPVOID)hookAddr, hookLength, OldProtect, &OldProtect);
	free(nopCode);

	return true;
}


bool HookReg::Unhook()
{
	if (!VirtualProtectEx(handle, (LPVOID)hookAddr, hookLength, PAGE_EXECUTE_READWRITE, &OldProtect))
	{
		return false;
	}
	if (!WriteProcessMemory(handle, (LPVOID)hookAddr, hookCode, 5, NULL))/*向源地址写入跳转指令*/
	{
		return false;
	}
	VirtualProtectEx(handle, (LPVOID)hookAddr, hookLength, OldProtect, &OldProtect);
	return true;
}