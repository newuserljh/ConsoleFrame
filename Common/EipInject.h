#pragma once
#include<windows.h>  
#include "atlstr.h"  
#include <string>
#pragma comment(lib, "Kernel32.lib")  
class EipInject
{
public:
	EipInject();
	~EipInject();

	void eipinjectDll(WCHAR* dllname, PROCESS_INFORMATION pi);

};

