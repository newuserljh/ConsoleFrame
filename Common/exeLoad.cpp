#include "exeLoad.h"



exeLoad::exeLoad(std::string filename, std::string excutedirectory)
{
    // 初始化 STARTUPINFO 结构体
    memset(&si, 0, sizeof(STARTUPINFO));
    si.cb = sizeof(STARTUPINFO);
    si.dwFlags = STARTF_USESHOWWINDOW;
    si.wShowWindow = SW_SHOW;

    // 创建新进程
    if (!CreateProcess(
        NULL, // 应用程序名称
        const_cast<char*>(filename.c_str()), // 命令行参数
        NULL, // 进程安全属性
        NULL, // 线程安全属性
        FALSE, // 是否继承句柄
        0, // 创建标志
        NULL, // 新进程的环境变量
        const_cast<char*>(excutedirectory.c_str()), // 当前目录
        &si, // 启动信息
        &pi // 进程信息
    ))
    {
        // 如果创建进程失败，退出程序
        exit(1);
    }
}


exeLoad::~exeLoad()
{
	// 关闭进程和线程句柄
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
}
