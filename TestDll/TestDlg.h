#pragma once
#include "../GameData/role.h"
#include "resource.h" 
//#include "lua.hpp"
//#include "LuaBridge/LuaBridge.h"
#include "lua_interface.h"

// CTestDlg �Ի���

class CTestDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CTestDlg)

public:
	CTestDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CTestDlg();

// �Ի�������
	enum { IDD = IDD_DLGTEST };

public:
	lua_State* L; // Lua ״̬����

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton8();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton9();
	afx_msg void OnBnClickedButton4();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedChkTeam();
	CEdit m_edit2;
	bool Set_Skill();
	bool Load_coordinate();
	bool initVariable();
	bool init_team();
	/*��ּ�������*/
	DWORD s_ID ;
	CWinThread* m_threadAttack;
	bool tflag_attack;
	static UINT __cdecl threadAttack(LPVOID lparam);
	std::vector<std::string> attack_monlist;

	/*Ѱ·�������*/
	std::vector<MapXY> map_xy;
	size_t i_map;//Ѱ·���±�
	CWinThread* m_threadGoto;
	bool tflag_goto;
	static UINT __cdecl threadGoto(LPVOID lparam);
	/*��������*/
	std::vector<std::string> pick_goods_list;
	CWinThread* m_threadPickup;
	bool tflag_pickup;
	static UINT __cdecl threadPickup(LPVOID lparam);

	CWinThread* m_threadBagProcess;
	static UINT __cdecl CTestDlg::threadBagPocess(LPVOID lparam);

	/*�������*/
	CButton* pBtn;//���鸴ѡ��
	std::vector<std::string> team_list;
	void  MakeTeam(CTestDlg* p);
	int m_team_check_id;

	void  RoleIsDeath(void);//��������

	//�������ܱ���
	bool auto_avoid_mon;
	CWinThread* m_threadAutoAvoid;
	static UINT __cdecl threadAutoAvoidMon(LPVOID p);

	CCriticalSection g_criticalsection; //�߽������̼߳�ͬ���õġ�

	void AutoRecvGoods(void);//���ս������飬��ħװ��Ԫ��
	void AutoAvoidMonsters();//��������

	void AutoReturnToCity(); // �سǴ���������
	
	
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButton10();
	afx_msg void OnBnClickedBtnGj();
	afx_msg void OnBnClickedBtnRecnpc();


protected:
	lua_interface m_luaInterface; // ��� lua_interface ��Ա����
public:
	afx_msg void OnBnClickedBtnLuatst();

	//��Ա
public:
	DWORD mPid;
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};
