// TestDll.h : TestDll DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CTestDllApp
// �йش���ʵ�ֵ���Ϣ������� TestDll.cpp
//

class CTestDllApp : public CWinApp
{
public:
	CTestDllApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()

private:
	bool init();
};
void threadFunc();