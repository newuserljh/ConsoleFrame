#pragma once
#include<windows.h>  
#include "atlstr.h"  
#include <string>
#pragma comment(lib, "Kernel32.lib")  

/*
����һ���ⲿ��exe
*/
class exeLoad
{
public:
	exeLoad(std::string filename,std::string excutedirectory);
	~exeLoad();

	STARTUPINFO si; //һЩ�ر���������  
	PROCESS_INFORMATION pi; //�ر��������ý���  
};

