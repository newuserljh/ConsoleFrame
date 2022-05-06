#pragma once
#include <windows.h>

BYTE rcvryCode[5] = { 0 };
bool hookAPI(LPVOID APIAddress, LPVOID JmpAddress)
{

	DWORD OldProtect = 0;/*之前的保护属性*/
	if (VirtualProtectEx(GetCurrentProcess(), APIAddress, 5, PAGE_EXECUTE_READWRITE, &OldProtect))
	{
		BYTE ShellCode[5];
		ZeroMemory(ShellCode, 5);
		ShellCode[0] = 0xE9;
		*(DWORD*)(ShellCode + 1) = (DWORD)JmpAddress - (DWORD)APIAddress - 5;/*跳转的长度(偏移)*/
		if (!::ReadProcessMemory(::GetCurrentProcess(), APIAddress, rcvryCode, 5, NULL)) return false;
		if (!WriteProcessMemory(GetCurrentProcess(), APIAddress, ShellCode, 5, NULL))	return false;
	}
	VirtualProtectEx(GetCurrentProcess(), APIAddress, 5, OldProtect, &OldProtect);/*恢复保护属性*/
	return true;
}

/*
函数:恢复API
参数一:API名字
参数二:恢复的code,5个字节
*/
bool rcvryAPI(LPVOID RecoveryAddress, BYTE* recoveryCode)
{
	DWORD OldProtect = 0;/*之前的保护属性*/
	if (VirtualProtectEx(GetCurrentProcess(), RecoveryAddress, 5, PAGE_EXECUTE_READWRITE, &OldProtect))
	{
		if (!WriteProcessMemory(GetCurrentProcess(), RecoveryAddress, recoveryCode, 5, NULL))
		{
			return false;
		}
		VirtualProtectEx(GetCurrentProcess(), RecoveryAddress, 5, OldProtect, &OldProtect);/*恢复保护属性*/
		return true;
	}
	return false;
}