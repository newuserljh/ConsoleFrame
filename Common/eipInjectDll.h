#pragma once
#include <string>
#include <windows.h> 
#include "StringCovert.h"
/*
eipע��DLL
*/
class eipInjectDll
{
public:
	eipInjectDll();
	~eipInjectDll();

	bool InjectDll(WCHAR* dllpath, PROCESS_INFORMATION &pi);


};
