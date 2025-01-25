#include "exeLoad.h"



exeLoad::exeLoad(std::string filename, std::string excutedirectory)
{
    // ��ʼ�� STARTUPINFO �ṹ��
    memset(&si, 0, sizeof(STARTUPINFO));
    si.cb = sizeof(STARTUPINFO);
    si.dwFlags = STARTF_USESHOWWINDOW;
    si.wShowWindow = SW_SHOW;

    // �����½���
    if (!CreateProcess(
        NULL, // Ӧ�ó�������
        const_cast<char*>(filename.c_str()), // �����в���
        NULL, // ���̰�ȫ����
        NULL, // �̰߳�ȫ����
        FALSE, // �Ƿ�̳о��
        0, // ������־
        NULL, // �½��̵Ļ�������
        const_cast<char*>(excutedirectory.c_str()), // ��ǰĿ¼
        &si, // ������Ϣ
        &pi // ������Ϣ
    ))
    {
        // �����������ʧ�ܣ��˳�����
        exit(1);
    }
}


exeLoad::~exeLoad()
{
	// �رս��̺��߳̾��
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
}
