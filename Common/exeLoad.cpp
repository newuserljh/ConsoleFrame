#include "exeLoad.h"



exeLoad::exeLoad(std::string filename, std::string excutedirectory)
{
	memset(&si, 0, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_SHOW;
	//std::string file(filename.GetBuffer(0));  
	if (!CreateProcess(NULL, const_cast<char*>(filename.c_str()), NULL, NULL, FALSE, 0, NULL, const_cast<char*>(excutedirectory.c_str()), &si, &pi)) //8888Ϊ�����в���  
	{
		exit(1);
	}
}


exeLoad::~exeLoad()
{

}
