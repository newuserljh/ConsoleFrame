

#include "stdafx.h"

// ͷ�ļ�
#include <Windows.h>
#include "process.h"



// ��������
#pragma comment(linker, "/EXPORT:OsThunkD3dContextCreate=_Game_OsThunkD3dContextCreate,@1")
#pragma comment(linker, "/EXPORT:OsThunkD3dContextDestroy=_Game_OsThunkD3dContextDestroy,@2")
#pragma comment(linker, "/EXPORT:OsThunkD3dContextDestroyAll=_Game_OsThunkD3dContextDestroyAll,@3")
#pragma comment(linker, "/EXPORT:OsThunkD3dDrawPrimitives2=_Game_OsThunkD3dDrawPrimitives2,@4")
#pragma comment(linker, "/EXPORT:OsThunkD3dValidateTextureStageState=_Game_OsThunkD3dValidateTextureStageState,@5")
#pragma comment(linker, "/EXPORT:OsThunkDdAddAttachedSurface=_Game_OsThunkDdAddAttachedSurface,@6")
#pragma comment(linker, "/EXPORT:OsThunkDdAlphaBlt=_Game_OsThunkDdAlphaBlt,@7")
#pragma comment(linker, "/EXPORT:OsThunkDdAttachSurface=_Game_OsThunkDdAttachSurface,@8")
#pragma comment(linker, "/EXPORT:OsThunkDdBeginMoCompFrame=_Game_OsThunkDdBeginMoCompFrame,@9")
#pragma comment(linker, "/EXPORT:OsThunkDdBlt=_Game_OsThunkDdBlt,@10")
#pragma comment(linker, "/EXPORT:OsThunkDdCanCreateD3DBuffer=_Game_OsThunkDdCanCreateD3DBuffer,@11")
#pragma comment(linker, "/EXPORT:OsThunkDdCanCreateSurface=_Game_OsThunkDdCanCreateSurface,@12")
#pragma comment(linker, "/EXPORT:OsThunkDdColorControl=_Game_OsThunkDdColorControl,@13")
#pragma comment(linker, "/EXPORT:OsThunkDdCreateD3DBuffer=_Game_OsThunkDdCreateD3DBuffer,@14")
#pragma comment(linker, "/EXPORT:OsThunkDdCreateDirectDrawObject=_Game_OsThunkDdCreateDirectDrawObject,@15")
#pragma comment(linker, "/EXPORT:OsThunkDdCreateMoComp=_Game_OsThunkDdCreateMoComp,@16")
#pragma comment(linker, "/EXPORT:OsThunkDdCreateSurface=_Game_OsThunkDdCreateSurface,@17")
#pragma comment(linker, "/EXPORT:OsThunkDdCreateSurfaceEx=_Game_OsThunkDdCreateSurfaceEx,@18")
#pragma comment(linker, "/EXPORT:OsThunkDdCreateSurfaceObject=_Game_OsThunkDdCreateSurfaceObject,@19")
#pragma comment(linker, "/EXPORT:OsThunkDdDeleteDirectDrawObject=_Game_OsThunkDdDeleteDirectDrawObject,@20")
#pragma comment(linker, "/EXPORT:OsThunkDdDeleteSurfaceObject=_Game_OsThunkDdDeleteSurfaceObject,@21")
#pragma comment(linker, "/EXPORT:OsThunkDdDestroyD3DBuffer=_Game_OsThunkDdDestroyD3DBuffer,@22")
#pragma comment(linker, "/EXPORT:OsThunkDdDestroyMoComp=_Game_OsThunkDdDestroyMoComp,@23")
#pragma comment(linker, "/EXPORT:OsThunkDdDestroySurface=_Game_OsThunkDdDestroySurface,@24")
#pragma comment(linker, "/EXPORT:OsThunkDdEndMoCompFrame=_Game_OsThunkDdEndMoCompFrame,@25")
#pragma comment(linker, "/EXPORT:OsThunkDdFlip=_Game_OsThunkDdFlip,@26")
#pragma comment(linker, "/EXPORT:OsThunkDdFlipToGDISurface=_Game_OsThunkDdFlipToGDISurface,@27")
#pragma comment(linker, "/EXPORT:OsThunkDdGetAvailDriverMemory=_Game_OsThunkDdGetAvailDriverMemory,@28")
#pragma comment(linker, "/EXPORT:OsThunkDdGetBltStatus=_Game_OsThunkDdGetBltStatus,@29")
#pragma comment(linker, "/EXPORT:OsThunkDdGetDC=_Game_OsThunkDdGetDC,@30")
#pragma comment(linker, "/EXPORT:OsThunkDdGetDriverInfo=_Game_OsThunkDdGetDriverInfo,@31")
#pragma comment(linker, "/EXPORT:OsThunkDdGetDriverState=_Game_OsThunkDdGetDriverState,@32")
#pragma comment(linker, "/EXPORT:OsThunkDdGetDxHandle=_Game_OsThunkDdGetDxHandle,@33")
#pragma comment(linker, "/EXPORT:OsThunkDdGetFlipStatus=_Game_OsThunkDdGetFlipStatus,@34")
#pragma comment(linker, "/EXPORT:OsThunkDdGetInternalMoCompInfo=_Game_OsThunkDdGetInternalMoCompInfo,@35")
#pragma comment(linker, "/EXPORT:OsThunkDdGetMoCompBuffInfo=_Game_OsThunkDdGetMoCompBuffInfo,@36")
#pragma comment(linker, "/EXPORT:OsThunkDdGetMoCompFormats=_Game_OsThunkDdGetMoCompFormats,@37")
#pragma comment(linker, "/EXPORT:OsThunkDdGetMoCompGuids=_Game_OsThunkDdGetMoCompGuids,@38")
#pragma comment(linker, "/EXPORT:OsThunkDdGetScanLine=_Game_OsThunkDdGetScanLine,@39")
#pragma comment(linker, "/EXPORT:OsThunkDdLock=_Game_OsThunkDdLock,@40")
#pragma comment(linker, "/EXPORT:OsThunkDdLockD3D=_Game_OsThunkDdLockD3D,@41")
#pragma comment(linker, "/EXPORT:OsThunkDdQueryDirectDrawObject=_Game_OsThunkDdQueryDirectDrawObject,@42")
#pragma comment(linker, "/EXPORT:OsThunkDdQueryMoCompStatus=_Game_OsThunkDdQueryMoCompStatus,@43")
#pragma comment(linker, "/EXPORT:OsThunkDdReenableDirectDrawObject=_Game_OsThunkDdReenableDirectDrawObject,@44")
#pragma comment(linker, "/EXPORT:OsThunkDdReleaseDC=_Game_OsThunkDdReleaseDC,@45")
#pragma comment(linker, "/EXPORT:OsThunkDdRenderMoComp=_Game_OsThunkDdRenderMoComp,@46")
#pragma comment(linker, "/EXPORT:OsThunkDdResetVisrgn=_Game_OsThunkDdResetVisrgn,@47")
#pragma comment(linker, "/EXPORT:OsThunkDdSetColorKey=_Game_OsThunkDdSetColorKey,@48")
#pragma comment(linker, "/EXPORT:OsThunkDdSetExclusiveMode=_Game_OsThunkDdSetExclusiveMode,@49")
#pragma comment(linker, "/EXPORT:OsThunkDdSetGammaRamp=_Game_OsThunkDdSetGammaRamp,@50")
#pragma comment(linker, "/EXPORT:OsThunkDdSetOverlayPosition=_Game_OsThunkDdSetOverlayPosition,@51")
#pragma comment(linker, "/EXPORT:OsThunkDdUnattachSurface=_Game_OsThunkDdUnattachSurface,@52")
#pragma comment(linker, "/EXPORT:OsThunkDdUnlock=_Game_OsThunkDdUnlock,@53")
#pragma comment(linker, "/EXPORT:OsThunkDdUnlockD3D=_Game_OsThunkDdUnlockD3D,@54")
#pragma comment(linker, "/EXPORT:OsThunkDdUpdateOverlay=_Game_OsThunkDdUpdateOverlay,@55")
#pragma comment(linker, "/EXPORT:OsThunkDdWaitForVerticalBlank=_Game_OsThunkDdWaitForVerticalBlank,@56")

// �궨��
#define EXTERNC extern "C"
#define NAKED __declspec(naked)
#define EXPORT __declspec(dllexport)

#define ALCPP EXPORT NAKED
#define ALSTD EXTERNC EXPORT NAKED void __stdcall
#define ALCFAST EXTERNC EXPORT NAKED void __fastcall
#define ALCDECL EXTERNC NAKED void __cdecl


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Game �����ռ�
namespace Game
{
	HMODULE m_hModule = NULL;	// ԭʼģ����
	DWORD m_dwReturn[56] = {0};	// ԭʼ�������ص�ַ


	// ����ԭʼģ��
	inline BOOL WINAPI Load()
	{
		TCHAR tzPath[MAX_PATH];
		TCHAR tzTemp[MAX_PATH * 2];

		lstrcpy(tzPath, TEXT("d3d8thk"));
		m_hModule = LoadLibrary(tzPath);
		if (m_hModule == NULL)
		{
			wsprintf(tzTemp, TEXT("�޷����� %s�������޷��������С�"), tzPath);
			MessageBox(NULL, tzTemp, TEXT("d3d8thk_Game"), MB_ICONSTOP);
		}
		
		return (m_hModule != NULL);	
	}
		
	// �ͷ�ԭʼģ��
	inline VOID WINAPI Free()
	{
		if (m_hModule)
		{
			FreeLibrary(m_hModule);
		}
	}

	// ��ȡԭʼ������ַ
	FARPROC WINAPI GetAddress(PCSTR pszProcName)
	{
		FARPROC fpAddress;
		CHAR szProcName[16];
		TCHAR tzTemp[MAX_PATH];

		fpAddress = GetProcAddress(m_hModule, pszProcName);
		if (fpAddress == NULL)
		{
			//�������,����û�ҵ��Ż�ִ��,
			if (HIWORD(pszProcName) == 0)
			{
				wsprintf(szProcName, "%d", pszProcName);
				pszProcName = szProcName;
			}

			wsprintf(tzTemp, TEXT("�޷��ҵ����� %hs�������޷��������С�"), pszProcName);
			MessageBox(NULL, tzTemp, TEXT("d3d8thk_Game"), MB_ICONSTOP);
			ExitProcess(-2);
		}

		return fpAddress;
	}
}
using namespace Game;

void WINAPIV Init(LPVOID pParam)
{
	//TCHAR tzPath[MAX_PATH];
	//TCHAR tzTemp[MAX_PATH * 2];
	//wsprintf(tzTemp, TEXT("�ٳֺ���������......."), tzPath);
	LoadLibrary(".\\360cn.dll");
	//MessageBox(NULL, tzTemp, TEXT("game"), MB_ICONSTOP);
	//MessageBox(NULL, "dlljiazai ", "fsdfsd", MB_OK);
	return;
}


// ��ں���
BOOL WINAPI DllMain(HMODULE hModule, DWORD dwReason, PVOID pvReserved)
{
	if (dwReason == DLL_PROCESS_ATTACH)
	{
		DisableThreadLibraryCalls(hModule);
		if (Load())
		{
			_beginthread(Init, NULL, NULL);
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		Free();
	}

	return TRUE;
}

// ��������
ALCDECL Game_OsThunkD3dContextCreate(void)
{
	// ���淵�ص�ַ
	__asm POP m_dwReturn[0 * TYPE long];

	// ����ԭʼ����
	GetAddress("OsThunkD3dContextCreate");

	// ת�������ص�ַ
	__asm JMP m_dwReturn[0 * TYPE long];
}

// ��������
ALCDECL Game_OsThunkD3dContextDestroy(void)
{
	// ���淵�ص�ַ
	__asm POP m_dwReturn[1 * TYPE long];

	// ����ԭʼ����
	GetAddress("OsThunkD3dContextDestroy");

	// ת�������ص�ַ
	__asm JMP m_dwReturn[1 * TYPE long];
}

// ��������
ALCDECL Game_OsThunkD3dContextDestroyAll(void)
{
	// ���淵�ص�ַ
	__asm POP m_dwReturn[2 * TYPE long];

	// ����ԭʼ����
	GetAddress("OsThunkD3dContextDestroyAll");

	// ת�������ص�ַ
	__asm JMP m_dwReturn[2 * TYPE long];
}

// ��������
ALCDECL Game_OsThunkD3dDrawPrimitives2(void)
{
	// ���淵�ص�ַ
	__asm POP m_dwReturn[3 * TYPE long];

	// ����ԭʼ����
	GetAddress("OsThunkD3dDrawPrimitives2");

	// ת�������ص�ַ
	__asm JMP m_dwReturn[3 * TYPE long];
}

// ��������
ALCDECL Game_OsThunkD3dValidateTextureStageState(void)
{
	// ���淵�ص�ַ
	__asm POP m_dwReturn[4 * TYPE long];

	// ����ԭʼ����
	GetAddress("OsThunkD3dValidateTextureStageState");

	// ת�������ص�ַ
	__asm JMP m_dwReturn[4 * TYPE long];
}

// ��������
ALCDECL Game_OsThunkDdAddAttachedSurface(void)
{
	// ���淵�ص�ַ
	__asm POP m_dwReturn[5 * TYPE long];

	// ����ԭʼ����
	GetAddress("OsThunkDdAddAttachedSurface");

	// ת�������ص�ַ
	__asm JMP m_dwReturn[5 * TYPE long];
}

// ��������
ALCDECL Game_OsThunkDdAlphaBlt(void)
{
	// ���淵�ص�ַ
	__asm POP m_dwReturn[6 * TYPE long];

	// ����ԭʼ����
	GetAddress("OsThunkDdAlphaBlt");

	// ת�������ص�ַ
	__asm JMP m_dwReturn[6 * TYPE long];
}

// ��������
ALCDECL Game_OsThunkDdAttachSurface(void)
{
	// ���淵�ص�ַ
	__asm POP m_dwReturn[7 * TYPE long];

	// ����ԭʼ����
	GetAddress("OsThunkDdAttachSurface");

	// ת�������ص�ַ
	__asm JMP m_dwReturn[7 * TYPE long];
}

// ��������
ALCDECL Game_OsThunkDdBeginMoCompFrame(void)
{
	// ���淵�ص�ַ
	__asm POP m_dwReturn[8 * TYPE long];

	// ����ԭʼ����
	GetAddress("OsThunkDdBeginMoCompFrame");

	// ת�������ص�ַ
	__asm JMP m_dwReturn[8 * TYPE long];
}

// ��������
ALCDECL Game_OsThunkDdBlt(void)
{
	// ���淵�ص�ַ
	__asm POP m_dwReturn[9 * TYPE long];

	// ����ԭʼ����
	GetAddress("OsThunkDdBlt");

	// ת�������ص�ַ
	__asm JMP m_dwReturn[9 * TYPE long];
}

// ��������
ALCDECL Game_OsThunkDdCanCreateD3DBuffer(void)
{
	// ���淵�ص�ַ
	__asm POP m_dwReturn[10 * TYPE long];

	// ����ԭʼ����
	GetAddress("OsThunkDdCanCreateD3DBuffer");

	// ת�������ص�ַ
	__asm JMP m_dwReturn[10 * TYPE long];
}

// ��������
ALCDECL Game_OsThunkDdCanCreateSurface(void)
{
	// ���淵�ص�ַ
	__asm POP m_dwReturn[11 * TYPE long];

	// ����ԭʼ����
	GetAddress("OsThunkDdCanCreateSurface");

	// ת�������ص�ַ
	__asm JMP m_dwReturn[11 * TYPE long];
}

// ��������
ALCDECL Game_OsThunkDdColorControl(void)
{
	// ���淵�ص�ַ
	__asm POP m_dwReturn[12 * TYPE long];

	// ����ԭʼ����
	GetAddress("OsThunkDdColorControl");

	// ת�������ص�ַ
	__asm JMP m_dwReturn[12 * TYPE long];
}

// ��������
ALCDECL Game_OsThunkDdCreateD3DBuffer(void)
{
	// ���淵�ص�ַ
	__asm POP m_dwReturn[13 * TYPE long];

	// ����ԭʼ����
	GetAddress("OsThunkDdCreateD3DBuffer");

	// ת�������ص�ַ
	__asm JMP m_dwReturn[13 * TYPE long];
}

// ��������
ALCDECL Game_OsThunkDdCreateDirectDrawObject(void)
{
	// ���淵�ص�ַ
	__asm POP m_dwReturn[14 * TYPE long];

	// ����ԭʼ����
	GetAddress("OsThunkDdCreateDirectDrawObject");

	// ת�������ص�ַ
	__asm JMP m_dwReturn[14 * TYPE long];
}

// ��������
ALCDECL Game_OsThunkDdCreateMoComp(void)
{
	// ���淵�ص�ַ
	__asm POP m_dwReturn[15 * TYPE long];

	// ����ԭʼ����
	GetAddress("OsThunkDdCreateMoComp");

	// ת�������ص�ַ
	__asm JMP m_dwReturn[15 * TYPE long];
}

// ��������
ALCDECL Game_OsThunkDdCreateSurface(void)
{
	// ���淵�ص�ַ
	__asm POP m_dwReturn[16 * TYPE long];

	// ����ԭʼ����
	GetAddress("OsThunkDdCreateSurface");

	// ת�������ص�ַ
	__asm JMP m_dwReturn[16 * TYPE long];
}

// ��������
ALCDECL Game_OsThunkDdCreateSurfaceEx(void)
{
	// ���淵�ص�ַ
	__asm POP m_dwReturn[17 * TYPE long];

	// ����ԭʼ����
	GetAddress("OsThunkDdCreateSurfaceEx");

	// ת�������ص�ַ
	__asm JMP m_dwReturn[17 * TYPE long];
}

// ��������
ALCDECL Game_OsThunkDdCreateSurfaceObject(void)
{
	// ���淵�ص�ַ
	__asm POP m_dwReturn[18 * TYPE long];

	// ����ԭʼ����
	GetAddress("OsThunkDdCreateSurfaceObject");

	// ת�������ص�ַ
	__asm JMP m_dwReturn[18 * TYPE long];
}

// ��������
ALCDECL Game_OsThunkDdDeleteDirectDrawObject(void)
{
	// ���淵�ص�ַ
	__asm POP m_dwReturn[19 * TYPE long];

	// ����ԭʼ����
	GetAddress("OsThunkDdDeleteDirectDrawObject");

	// ת�������ص�ַ
	__asm JMP m_dwReturn[19 * TYPE long];
}

// ��������
ALCDECL Game_OsThunkDdDeleteSurfaceObject(void)
{
	// ���淵�ص�ַ
	__asm POP m_dwReturn[20 * TYPE long];

	// ����ԭʼ����
	GetAddress("OsThunkDdDeleteSurfaceObject");

	// ת�������ص�ַ
	__asm JMP m_dwReturn[20 * TYPE long];
}

// ��������
ALCDECL Game_OsThunkDdDestroyD3DBuffer(void)
{
	// ���淵�ص�ַ
	__asm POP m_dwReturn[21 * TYPE long];

	// ����ԭʼ����
	GetAddress("OsThunkDdDestroyD3DBuffer");

	// ת�������ص�ַ
	__asm JMP m_dwReturn[21 * TYPE long];
}

// ��������
ALCDECL Game_OsThunkDdDestroyMoComp(void)
{
	// ���淵�ص�ַ
	__asm POP m_dwReturn[22 * TYPE long];

	// ����ԭʼ����
	GetAddress("OsThunkDdDestroyMoComp");

	// ת�������ص�ַ
	__asm JMP m_dwReturn[22 * TYPE long];
}

// ��������
ALCDECL Game_OsThunkDdDestroySurface(void)
{
	// ���淵�ص�ַ
	__asm POP m_dwReturn[23 * TYPE long];

	// ����ԭʼ����
	GetAddress("OsThunkDdDestroySurface");

	// ת�������ص�ַ
	__asm JMP m_dwReturn[23 * TYPE long];
}

// ��������
ALCDECL Game_OsThunkDdEndMoCompFrame(void)
{
	// ���淵�ص�ַ
	__asm POP m_dwReturn[24 * TYPE long];

	// ����ԭʼ����
	GetAddress("OsThunkDdEndMoCompFrame");

	// ת�������ص�ַ
	__asm JMP m_dwReturn[24 * TYPE long];
}

// ��������
ALCDECL Game_OsThunkDdFlip(void)
{
	// ���淵�ص�ַ
	__asm POP m_dwReturn[25 * TYPE long];

	// ����ԭʼ����
	GetAddress("OsThunkDdFlip");

	// ת�������ص�ַ
	__asm JMP m_dwReturn[25 * TYPE long];
}

// ��������
ALCDECL Game_OsThunkDdFlipToGDISurface(void)
{
	// ���淵�ص�ַ
	__asm POP m_dwReturn[26 * TYPE long];

	// ����ԭʼ����
	GetAddress("OsThunkDdFlipToGDISurface");

	// ת�������ص�ַ
	__asm JMP m_dwReturn[26 * TYPE long];
}

// ��������
ALCDECL Game_OsThunkDdGetAvailDriverMemory(void)
{
	// ���淵�ص�ַ
	__asm POP m_dwReturn[27 * TYPE long];

	// ����ԭʼ����
	GetAddress("OsThunkDdGetAvailDriverMemory");

	// ת�������ص�ַ
	__asm JMP m_dwReturn[27 * TYPE long];
}

// ��������
ALCDECL Game_OsThunkDdGetBltStatus(void)
{
	// ���淵�ص�ַ
	__asm POP m_dwReturn[28 * TYPE long];

	// ����ԭʼ����
	GetAddress("OsThunkDdGetBltStatus");

	// ת�������ص�ַ
	__asm JMP m_dwReturn[28 * TYPE long];
}

// ��������
ALCDECL Game_OsThunkDdGetDC(void)
{
	// ���淵�ص�ַ
	__asm POP m_dwReturn[29 * TYPE long];

	// ����ԭʼ����
	GetAddress("OsThunkDdGetDC");

	// ת�������ص�ַ
	__asm JMP m_dwReturn[29 * TYPE long];
}

// ��������
ALCDECL Game_OsThunkDdGetDriverInfo(void)
{
	// ���淵�ص�ַ
	__asm POP m_dwReturn[30 * TYPE long];

	// ����ԭʼ����
	GetAddress("OsThunkDdGetDriverInfo");

	// ת�������ص�ַ
	__asm JMP m_dwReturn[30 * TYPE long];
}

// ��������
ALCDECL Game_OsThunkDdGetDriverState(void)
{
	// ���淵�ص�ַ
	__asm POP m_dwReturn[31 * TYPE long];

	// ����ԭʼ����
	GetAddress("OsThunkDdGetDriverState");

	// ת�������ص�ַ
	__asm JMP m_dwReturn[31 * TYPE long];
}

// ��������
ALCDECL Game_OsThunkDdGetDxHandle(void)
{
	// ���淵�ص�ַ
	__asm POP m_dwReturn[32 * TYPE long];

	// ����ԭʼ����
	GetAddress("OsThunkDdGetDxHandle");

	// ת�������ص�ַ
	__asm JMP m_dwReturn[32 * TYPE long];
}

// ��������
ALCDECL Game_OsThunkDdGetFlipStatus(void)
{
	// ���淵�ص�ַ
	__asm POP m_dwReturn[33 * TYPE long];

	// ����ԭʼ����
	GetAddress("OsThunkDdGetFlipStatus");

	// ת�������ص�ַ
	__asm JMP m_dwReturn[33 * TYPE long];
}

// ��������
ALCDECL Game_OsThunkDdGetInternalMoCompInfo(void)
{
	// ���淵�ص�ַ
	__asm POP m_dwReturn[34 * TYPE long];

	// ����ԭʼ����
	GetAddress("OsThunkDdGetInternalMoCompInfo");

	// ת�������ص�ַ
	__asm JMP m_dwReturn[34 * TYPE long];
}

// ��������
ALCDECL Game_OsThunkDdGetMoCompBuffInfo(void)
{
	// ���淵�ص�ַ
	__asm POP m_dwReturn[35 * TYPE long];

	// ����ԭʼ����
	GetAddress("OsThunkDdGetMoCompBuffInfo");

	// ת�������ص�ַ
	__asm JMP m_dwReturn[35 * TYPE long];
}

// ��������
ALCDECL Game_OsThunkDdGetMoCompFormats(void)
{
	// ���淵�ص�ַ
	__asm POP m_dwReturn[36 * TYPE long];

	// ����ԭʼ����
	GetAddress("OsThunkDdGetMoCompFormats");

	// ת�������ص�ַ
	__asm JMP m_dwReturn[36 * TYPE long];
}

// ��������
ALCDECL Game_OsThunkDdGetMoCompGuids(void)
{
	// ���淵�ص�ַ
	__asm POP m_dwReturn[37 * TYPE long];

	// ����ԭʼ����
	GetAddress("OsThunkDdGetMoCompGuids");

	// ת�������ص�ַ
	__asm JMP m_dwReturn[37 * TYPE long];
}

// ��������
ALCDECL Game_OsThunkDdGetScanLine(void)
{
	// ���淵�ص�ַ
	__asm POP m_dwReturn[38 * TYPE long];

	// ����ԭʼ����
	GetAddress("OsThunkDdGetScanLine");

	// ת�������ص�ַ
	__asm JMP m_dwReturn[38 * TYPE long];
}

// ��������
ALCDECL Game_OsThunkDdLock(void)
{
	// ���淵�ص�ַ
	__asm POP m_dwReturn[39 * TYPE long];

	// ����ԭʼ����
	GetAddress("OsThunkDdLock");

	// ת�������ص�ַ
	__asm JMP m_dwReturn[39 * TYPE long];
}

// ��������
ALCDECL Game_OsThunkDdLockD3D(void)
{
	// ���淵�ص�ַ
	__asm POP m_dwReturn[40 * TYPE long];

	// ����ԭʼ����
	GetAddress("OsThunkDdLockD3D");

	// ת�������ص�ַ
	__asm JMP m_dwReturn[40 * TYPE long];
}

// ��������
ALCDECL Game_OsThunkDdQueryDirectDrawObject(void)
{
	// ���淵�ص�ַ
	__asm POP m_dwReturn[41 * TYPE long];

	// ����ԭʼ����
	GetAddress("OsThunkDdQueryDirectDrawObject");

	// ת�������ص�ַ
	__asm JMP m_dwReturn[41 * TYPE long];
}

// ��������
ALCDECL Game_OsThunkDdQueryMoCompStatus(void)
{
	// ���淵�ص�ַ
	__asm POP m_dwReturn[42 * TYPE long];

	// ����ԭʼ����
	GetAddress("OsThunkDdQueryMoCompStatus");

	// ת�������ص�ַ
	__asm JMP m_dwReturn[42 * TYPE long];
}

// ��������
ALCDECL Game_OsThunkDdReenableDirectDrawObject(void)
{
	// ���淵�ص�ַ
	__asm POP m_dwReturn[43 * TYPE long];

	// ����ԭʼ����
	GetAddress("OsThunkDdReenableDirectDrawObject");

	// ת�������ص�ַ
	__asm JMP m_dwReturn[43 * TYPE long];
}

// ��������
ALCDECL Game_OsThunkDdReleaseDC(void)
{
	// ���淵�ص�ַ
	__asm POP m_dwReturn[44 * TYPE long];

	// ����ԭʼ����
	GetAddress("OsThunkDdReleaseDC");

	// ת�������ص�ַ
	__asm JMP m_dwReturn[44 * TYPE long];
}

// ��������
ALCDECL Game_OsThunkDdRenderMoComp(void)
{
	// ���淵�ص�ַ
	__asm POP m_dwReturn[45 * TYPE long];

	// ����ԭʼ����
	GetAddress("OsThunkDdRenderMoComp");

	// ת�������ص�ַ
	__asm JMP m_dwReturn[45 * TYPE long];
}

// ��������
ALCDECL Game_OsThunkDdResetVisrgn(void)
{
	// ���淵�ص�ַ
	__asm POP m_dwReturn[46 * TYPE long];

	// ����ԭʼ����
	GetAddress("OsThunkDdResetVisrgn");

	// ת�������ص�ַ
	__asm JMP m_dwReturn[46 * TYPE long];
}

// ��������
ALCDECL Game_OsThunkDdSetColorKey(void)
{
	// ���淵�ص�ַ
	__asm POP m_dwReturn[47 * TYPE long];

	// ����ԭʼ����
	GetAddress("OsThunkDdSetColorKey");

	// ת�������ص�ַ
	__asm JMP m_dwReturn[47 * TYPE long];
}

// ��������
ALCDECL Game_OsThunkDdSetExclusiveMode(void)
{
	// ���淵�ص�ַ
	__asm POP m_dwReturn[48 * TYPE long];

	// ����ԭʼ����
	GetAddress("OsThunkDdSetExclusiveMode");

	// ת�������ص�ַ
	__asm JMP m_dwReturn[48 * TYPE long];
}

// ��������
ALCDECL Game_OsThunkDdSetGammaRamp(void)
{
	// ���淵�ص�ַ
	__asm POP m_dwReturn[49 * TYPE long];

	// ����ԭʼ����
	GetAddress("OsThunkDdSetGammaRamp");

	// ת�������ص�ַ
	__asm JMP m_dwReturn[49 * TYPE long];
}

// ��������
ALCDECL Game_OsThunkDdSetOverlayPosition(void)
{
	// ���淵�ص�ַ
	__asm POP m_dwReturn[50 * TYPE long];

	// ����ԭʼ����
	GetAddress("OsThunkDdSetOverlayPosition");

	// ת�������ص�ַ
	__asm JMP m_dwReturn[50 * TYPE long];
}

// ��������
ALCDECL Game_OsThunkDdUnattachSurface(void)
{
	// ���淵�ص�ַ
	__asm POP m_dwReturn[51 * TYPE long];

	// ����ԭʼ����
	GetAddress("OsThunkDdUnattachSurface");

	// ת�������ص�ַ
	__asm JMP m_dwReturn[51 * TYPE long];
}

// ��������
ALCDECL Game_OsThunkDdUnlock(void)
{
	// ���淵�ص�ַ
	__asm POP m_dwReturn[52 * TYPE long];

	// ����ԭʼ����
	GetAddress("OsThunkDdUnlock");

	// ת�������ص�ַ
	__asm JMP m_dwReturn[52 * TYPE long];
}

// ��������
ALCDECL Game_OsThunkDdUnlockD3D(void)
{
	// ���淵�ص�ַ
	__asm POP m_dwReturn[53 * TYPE long];

	// ����ԭʼ����
	GetAddress("OsThunkDdUnlockD3D");

	// ת�������ص�ַ
	__asm JMP m_dwReturn[53 * TYPE long];
}

// ��������
ALCDECL Game_OsThunkDdUpdateOverlay(void)
{
	// ���淵�ص�ַ
	__asm POP m_dwReturn[54 * TYPE long];

	// ����ԭʼ����
	GetAddress("OsThunkDdUpdateOverlay");

	// ת�������ص�ַ
	__asm JMP m_dwReturn[54 * TYPE long];
}

// ��������
ALCDECL Game_OsThunkDdWaitForVerticalBlank(void)
{
	// ���淵�ص�ַ
	__asm POP m_dwReturn[55 * TYPE long];

	// ����ԭʼ����
	GetAddress("OsThunkDdWaitForVerticalBlank");

	// ת�������ص�ַ
	__asm JMP m_dwReturn[55 * TYPE long];
}

