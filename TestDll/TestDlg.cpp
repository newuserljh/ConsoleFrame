// TestDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TestDll.h"
#include "TestDlg.h"
#include "afxdialogex.h"

#include "monster.h"
#include "skill.h"
#include "bag.h"
#include "utils.h"
#include "gamecall.h"
#include "shareMemoryCli.h"
#pragma comment(lib , "Common.lib")
#pragma comment(lib ,"GameData.lib")
//#include "HookAPI.h"
#include "HookReg.h"
// CTestDlg �Ի���
shareMemoryCli shareCli(MORE_OPEN_NUMBER);
int shareindex = -1;
//��ʼ��HOOk
HookReg hook;
role r;//��ɫ
monster m_mon;
skill m_skill;
bag r_bag;
gamecall mfun;

/*��ּ�������*/
DWORD s_ID = -1;
CWinThread* m_threadAttack;
bool tflag_attack = true;

/*Ѱ·�������*/
std::vector<MapXY> map_xy;
CWinThread* m_threadGoto;
bool tflag_goto = true;


/*��������*/
std::vector<std::string> pick_goods_list;
CWinThread* m_threadPickup;
bool tflag_pickup = true;

_declspec(naked) void CallTest()
{
	_asm pushad
	if (hook.EAX != 0)
	{
		tools::getInstance()->message("����!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
		shareCli.m_pSMAllData->m_sm_data[shareindex].server_alive = false;//��֤��Ҵ��
	}
	_asm  popad
	_asm ret
}



IMPLEMENT_DYNAMIC(CTestDlg, CDialogEx)

CTestDlg::CTestDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTestDlg::IDD, pParent)
{

}

CTestDlg::~CTestDlg()
{
}

void CTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT2, m_edit2);
}


BEGIN_MESSAGE_MAP(CTestDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CTestDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CTestDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CTestDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON8, &CTestDlg::OnBnClickedButton8)
	ON_BN_CLICKED(IDC_BUTTON5, &CTestDlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON9, &CTestDlg::OnBnClickedButton9)
END_MESSAGE_MAP()


// CTestDlg ��Ϣ�������

void threadLogin()
{
	int i = 0;//���Ե�¼����
	do
	{
		shareCli.m_pSMAllData->m_sm_data[shareindex].cscript = "��¼��...";
		Sleep(10000);
		mfun.presskey(shareCli.m_pSMAllData->m_sm_data[shareindex].ndPid);
		Sleep(100);
		mfun.loginGame(shareCli.m_pSMAllData->m_sm_data[shareindex].userName.c_str(), shareCli.m_pSMAllData->m_sm_data[shareindex].passWord.c_str());
		Sleep(1000);
		mfun.presskey(shareCli.m_pSMAllData->m_sm_data[shareindex].ndPid);
		Sleep(1000);
		mfun.presskey(shareCli.m_pSMAllData->m_sm_data[shareindex].ndPid);
		Sleep(1000);
		mfun.presskey(shareCli.m_pSMAllData->m_sm_data[shareindex].ndPid);
		Sleep(1000);
		mfun.presskey(shareCli.m_pSMAllData->m_sm_data[shareindex].ndPid);
		Sleep(1000);
		mfun.presskey(shareCli.m_pSMAllData->m_sm_data[shareindex].ndPid);
		Sleep(1000);
		mfun.presskey(shareCli.m_pSMAllData->m_sm_data[shareindex].ndPid);
		Sleep(1000);
		i++;
	} while ((!r.init()) && i < 10);
	if (!r.init())
	{
		shareCli.m_pSMAllData->m_sm_data[shareindex].cscript = "��½ʧ��,���ڽ�������";
		shareCli.m_pSMAllData->m_sm_data[shareindex].server_alive = false;
	}
	shareCli.m_pSMAllData->m_sm_data[shareindex].cscript = "��½���";
	return;
}

void threadAlive()
{
	while (true)
	{
		shareCli.m_pSMAllData->m_sm_data[shareindex].rcv_rand = shareCli.m_pSMAllData->m_sm_data[shareindex].send_rand;//��֤��Ҵ��
		Sleep(1000);
	}
}
// ׷���ı���EditControl
void AppendText(CEdit &m_edit, CString strAdd)
{
	m_edit.SetSel(m_edit.GetWindowTextLength(), m_edit.GetWindowTextLength());
	m_edit.ReplaceSel(strAdd + L"\n");
}

void CTestDlg::OnBnClickedButton1()
{
	// ��������
	if (!r.init())return;
	CString s;
	s.Format("%s\n", r.m_roleproperty.Object.pName);
	AppendText(m_edit2,s);
	char* sex = {};
	char* job = {};
	if (*r.m_roleproperty.Sex == 0)sex = "��";
	else sex = "Ů";
	if (*r.m_roleproperty.Job == 0)job = "սʿ";
	else if (*r.m_roleproperty.Job == 1)job = "��ʦ";
	else job = "��ʿ";
	s.Format("ְҵ:%s      �ȼ�:%d    �Ա�:%s", job, *r.m_roleproperty.Level, sex);
	AppendText(m_edit2, s);;
	s.Format("HP:%d / %d", *r.m_roleproperty.Object.HP, *r.m_roleproperty.Object.HP_MAX);
	AppendText(m_edit2, s);
	s.Format("MP:%d / %d", *r.m_roleproperty.Object.MP, *r.m_roleproperty.Object.MP_MAX);
	AppendText(m_edit2, s);
	s.Format("��ǰ��ͼ:%s  ����%d,%d", r.m_roleproperty.p_Current_Map, *r.m_roleproperty.Object.X, *r.m_roleproperty.Object.Y);
	AppendText(m_edit2, s);
	s.Format("������С:%d  ��������%d / %d", *r.m_roleproperty.Bag_Size, *r.m_roleproperty.BAG_W, *r.m_roleproperty.BAG_W_MAX);
	AppendText(m_edit2, s);
	s.Format("ID:%x", *r.m_roleproperty.Object.ID);
	AppendText(m_edit2, s);
	for (auto i=0;i<21;i++)
	{
		if (!(*r.m_euip[i].ID))continue; 
		s.Format("%d :%s �;�:%d/%d",i, r.m_euip[i].pName, *(r.m_euip[i].Use_Num),*(r.m_euip[i].Use_Num_Max));
		AppendText(m_edit2, s);
	}
}
//int
//WSAAPI
//MyWSARecv(
//	_In_ SOCKET s,
//	_In_reads_(dwBufferCount) __out_data_source(NETWORK) LPWSABUF lpBuffers,
//	_In_ DWORD dwBufferCount,
//	_Out_opt_ LPDWORD lpNumberOfBytesRecvd,
//	_Inout_ LPDWORD lpFlags,
//	_Inout_opt_ LPWSAOVERLAPPED lpOverlapped,
//	_In_opt_ LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine
//)
//{
//	
//	rcvryAPI(WSARecv, rcvryCode);
//   int ret= WSARecv(s, lpBuffers, dwBufferCount, lpNumberOfBytesRecvd, lpFlags, lpOverlapped, lpCompletionRoutine);
//	int er=WSAGetLastError();
//	if (er != 10035)
//	{
//		tools::getInstance()->message("����:%d", er);
//	}
//
//	hookAPI(WSARecv, MyWSARecv);
//	return ret;
//}

BOOL CTestDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

  //��ʼ�������ڴ�,ȡ�ù����ڴ�����
	if (!shareCli.openShareMemory())
	{
		tools::getInstance()->message("�򿪹����ڴ�ʧ��\n");
	}
	shareindex = shareCli.getIndexByPID(GetCurrentProcessId());

	//HOOK���ӷ�����ʧ�ܴ���
	hook.hookReg(0x5F8B69, 5, CallTest);

	CloseHandle(::CreateThread(NULL, NULL, LPTHREAD_START_ROUTINE(threadLogin), NULL, NULL, NULL));
	//��¼�ɹ�֮������ ����ͨѶ�߳�,��ʱ��֤�����Ϣ
	
	CloseHandle(::CreateThread(NULL, NULL, LPTHREAD_START_ROUTINE(threadAlive), NULL, NULL, NULL));


	//hookAPI(WSARecv, MyWSARecv);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}



void CTestDlg::OnBnClickedButton2()
{
	// TODO: ������Χ���� ���� ����NPC
	if (!r.init())return;
	m_mon.m_monsterList.clear();
	m_mon.m_groundList.clear();
	CString s;
	if (!r.Get_Envionment(m_mon.pOb_list))
	{
		s.Format("������Χ����\n");
		AppendText(m_edit2, s);
		return;
	}	
	if (!r.Get_Ground(m_mon.pGr_list))
	{
		s.Format("�����������\n");
		AppendText(m_edit2, s);
		return;
	}
	m_mon.init();
	s.Format("��Χ�����б����NPC��������ҡ������\n");
	AppendText(m_edit2, s);
	for (auto i = 0; i < m_mon.m_monsterList.size(); i++)
	{
		s.Format("%s ID: %x\n", m_mon.m_monsterList[i].pName, *(m_mon.m_monsterList[i].ID));
		AppendText(m_edit2, s);
	}

	s.Format("���棺\n");
	AppendText(m_edit2, s);
	for (auto i = 0; i < m_mon.m_groundList.size(); i++)
	{
		s.Format(" %s : %d/%d\n", m_mon.m_groundList[i].pName, *m_mon.m_groundList[i].X, *m_mon.m_groundList[i].Y);
		AppendText(m_edit2, s);
	}

}


void CTestDlg::OnBnClickedButton3()
{
	// TODO: ���ܱ���
	if (!r.init())return;
	CString s;
	m_skill.m_skillList.clear();
	m_skill.skillBase = (DWORD)r.m_roleproperty.p_Skill_Base;
	if (!m_skill.init())
	{
		s.Format("�������ܴ���\n");
		AppendText(m_edit2, s);
		return;
	}
	s.Format("���ܣ�\n");
	AppendText(m_edit2, s);

	for (auto i = 0; i < m_skill.m_skillList.size(); i++)
	{
		s.Format("%s  �ȼ�:%d  ID: %d\n", m_skill.m_skillList[i].pName, *(m_skill.m_skillList[i].level), *(m_skill.m_skillList[i].ID));
		AppendText(m_edit2, s);
	}

}


void CTestDlg::OnBnClickedButton8()
{
	// TODO: ��������
	if (!r.init())return;
	CString s;
	r_bag.maxSize = *r.m_roleproperty.Bag_Size;
	r_bag.bagBase = (DWORD)r.m_roleproperty.p_Bag_Base;
	if(!r_bag.init())
	{
		s.Format("������������\n");
		AppendText(m_edit2, s);
		return;
	}
	for (auto i=0;i<r_bag.maxSize;i++)
	{
		if (*(r_bag.m_bag[i].ID))
		{
			s.Format("��%d��:%s   ID: %x\n", i, r_bag.m_bag[i].pName, *r_bag.m_bag[i].ID);
			AppendText(m_edit2, s);
		}
	}

}


void CTestDlg::OnBnClickedButton5()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}






/*
��������:ѡ����Ŀ��
����һ:��ɫ�ṹ��
������:���������б�
����ֵ��ѡ�й������ָ��
*/
DWORD Choose_Moster(role& r, std::vector<std::string>& vec)
{
	DWORD Object_ID = *r.m_roleproperty.p_Target_ID;
	while (Object_ID == 0)
	{
		Object_ID = mfun.Find_T_Monster(r, vec);
		if (Object_ID)
		{
			*r.m_roleproperty.p_Target_ID = *(DWORD*)(Object_ID + 0x8);
		}
		else
		{
			/*��Ѱ·���ҹ�*/
			//m_threadGoto->ResumeThread();
			Sleep(2000);
			//m_threadGoto->SuspendThread();
		}
	}
	return Object_ID;
}

/*
��������:�Զ����
����һ:��ɫ�ṹ��
������:���������б�
������:ʹ�ü���ID
����ֵ��trueΪ�Ѿ��������fasleΪλ�ô��󣨻���600��û�������
*/
bool Auto_Attack(role r, std::vector<std::string>& vec, DWORD s_ID)
{
	*r.m_roleproperty.p_Target_ID = 0;
	DWORD p_Target = Choose_Moster(r, vec);
	
	MONSTER_PROPERTY att_mon((DWORD*)p_Target);
	unsigned i = 0;/*��¼��������*/
	while( (*att_mon.HP> 0) && (*att_mon.ID == *r.m_roleproperty.p_Target_ID) && (i < 600))
	{
		if (mfun.caclDistance(*r.m_roleproperty.Object.X, *r.m_roleproperty.Object.Y, *att_mon.X, *att_mon.Y) > 9)return FALSE;
		DWORD s_posion = m_skill.getSkillId("ʩ����");
		if( (s_posion!=-1))//��ʿ
		{
			if ((r_bag.ifHasPoison()>0)&& (*att_mon.IsPosion < 0x40))//�Ƿ��ж� 0û����0x40�춾��0x80�̶���0xc0���̶�,
			{
				mfun.useSkillTo(s_posion, *att_mon.X, *att_mon.Y, *att_mon.ID);
				Sleep(ATTACK_SLEEP);
				mfun.useSkillTo(s_posion, *att_mon.X, *att_mon.Y, *att_mon.ID);
				Sleep(ATTACK_SLEEP);
				/*	mfun.presskey(VK_F1);
					Sleep(ATTACK_SLEEP);*/
			}
		}
		mfun.useSkillTo(s_ID, *att_mon.X, *att_mon.Y, *att_mon.ID);
		i++;
		Sleep(ATTACK_SLEEP);
	}
	return true;
}

/*
��������:��������
����һ:��ɫ�ṹ��
����ֵ��ѡ�й������ָ��
*/
bool Load_Settings()
{
	/*��������*/
	if (*r.m_roleproperty.Job == 0)/*սʿ*/
	{
		return false;
	}
    if (*r.m_roleproperty.Job == 1)/*��ʦ*/
	{
		DWORD  s_tmp = m_skill.getSkillId("�����ϵ�");
		if (s_tmp != -1)
		{
			s_ID = s_tmp;
			return true;
		}
		s_tmp = m_skill.getSkillId("�׵���");
		if (s_tmp != -1)
		{
			s_ID = s_tmp;
			return true;
		}
		s_tmp = m_skill.getSkillId("������");
		if (s_tmp != -1)
		{
			s_ID = s_tmp;
			return true;
		}
		s_tmp = m_skill.getSkillId("С����");
		if (s_tmp != -1)
		{
			s_ID = s_tmp;
			return true;
		}
		return false;
	}
	if (*r.m_roleproperty.Job == 2)/*��ʿ*/
	{
		return false;
	}
	 return false;

}

int CTestDlg::threadAttack(void* p)
{
	CTestDlg* pDlg = (CTestDlg*)p;
	if(!Load_Settings())return 0;
	CString s;
	s.Format("��ּ���ID:%d", s_ID);
	AppendText(pDlg->m_edit2, s);
	std::vector<std::string> vec_mon = tools::getInstance()->ReadTxt("..\\��ħ���.txt");
	while (tflag_attack)
	{
		Auto_Attack(r, vec_mon, s_ID);
		Sleep(ATTACK_SLEEP);
	}
	return 0;
}

/*
��������:Ѱ·����ǰ��ͼ(x,y)����
����һ:x
������:y
������:��ʶ,ֻ��Ϊtrue��ʱ����ߵ�Ŀ�ĵ�
����ֵ:bool,������,δ����false
*/
bool MapGoto_near(DWORD x,DWORD y, bool tflag)
{
	while (mfun.caclDistance(*r.m_roleproperty.Object.X, *r.m_roleproperty.Object.Y,x,y)>3)
	{
		mfun.CurrentMapMove(x, y);
		Sleep(500);
		if (!tflag)return false;
     }
	return true;  
}

/*
��������:Ѱ·����ǰ��ͼ(x,y)��ȷλ��
����һ:x
������:y
������:��ʶ,ֻ��Ϊtrue��ʱ����ߵ�Ŀ�ĵ�
����ֵ:bool,������,δ����false
*/
bool MapGoto_Point(DWORD x, DWORD y,bool tflag)
{
	while (mfun.caclDistance(*r.m_roleproperty.Object.X, *r.m_roleproperty.Object.Y, x, y) > 0)
	{
		mfun.CurrentMapMove(x, y);
		Sleep(500);
		if (!tflag)return false;
	}
	return true;   
}


/*
��������:����Ѱ·�������
����һ:��ɫ�ṹ��
����ֵ��ѡ�й������ָ��
*/
bool Load_coordinate()
{
	map_xy.clear();
	std::vector<std::string> map_coordinate= tools::getInstance()->ReadTxt("..\\��ħ���1.txt");
	if (!map_coordinate.size())return false;
	for (size_t i = 0; i < map_coordinate.size(); i++)
	{
		MapXY xy;
		xy = mfun.splitXY(map_coordinate[i]);
		if ((xy.x!=-1)&&(xy.y!=-1))map_xy.push_back(xy);
	}
	if (!map_xy.size())return false;
	return true;
}

/*
��������:����ʰȡ��Ʒ
����һ:��ɫ�ṹ��
����ֵ��ѡ�й������ָ��
*/
bool Load_pick_goods()
{
	pick_goods_list.clear();
	std::vector<std::string> map_coordinate = tools::getInstance()->ReadTxt("..\\ʰȡ��Ʒ.txt");
	if (!map_coordinate.size())return false;
	return true;
}

/*Ѱ·�߳�*/
void threadGoto()
{
	if (!Load_coordinate())return;
	size_t i = 0;
	while (tflag_goto)
	{
		MapXY xy = map_xy[i];
		MapGoto_Point(map_xy[i].x, map_xy[i].y,tflag_goto);
		i++;
		if (i = map_xy.size())i = 0;
	}
}

/*�����߳�*/
void threadPickup()
{
	if (!Load_pick_goods())return;
	size_t i = 0;
	while (tflag_pickup)
	{
		r.Get_Ground(m_mon.pGr_list);
		m_mon.init_ground();
		for (size_t i=0;i<m_mon.m_groundList.size();i++)
		{
			for (size_t j = 0; j < pick_goods_list.size(); j++)
			{
				if (strcmp(m_mon.m_groundList[i].pName,pick_goods_list[j].c_str())==0)
				{
					MapGoto_Point(*m_mon.m_groundList[i].X, *m_mon.m_groundList[i].Y,true);
					mfun.pickupGoods(*m_mon.m_groundList[i].X, *m_mon.m_groundList[i].Y);
				}
			}
		}
		Sleep(2000);
	}
}


void CTestDlg::OnBnClickedButton9()
{
	// TODO: �ű�����

	/*�Զ���� ��Ҫ�������ٴ���߳����ȼ����� �ڱ�����Χ���󡢵��沢ʰȡ�߳� ���ȼ��и� ��Ѱ·�̡߳��������� ���ȼ����
	*
	*          ****ѡ��
	*�Զ����***   ��֣��жϴ���
	*          ****�������
	* */

	CString s;
	Load_Settings();
	s.Format("��ּ���ID:%d", s_ID);
	AppendText(m_edit2, s);
	std::vector<std::string> vec_mon = tools::getInstance()->ReadTxt("D:\\VS_PROJECT\\ConsoleFrame\\��ħ���.txt");
	for (size_t i = 0; i < vec_mon.size(); i++)
	{
		s.Format("%s\n", (char*)vec_mon[i].c_str());
		AppendText(m_edit2, s);
	}
	Auto_Attack(r, vec_mon, s_ID);


	//m_threadGoto=AfxBeginThread((AFX_THREADPROC)threadGoto,NULL, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
	//m_threadAttack= AfxBeginThread(thredAttack, NULL);
	//m_threadPickup= AfxBeginThread((AFX_THREADPROC)threadPickup, NULL, THREAD_PRIORITY_ABOVE_NORMAL);

	//tflag_attack = true;
	//HANDLE thrHandle;
	//thrHandle = (HANDLE)_beginthreadex(NULL, 0,
	//	(unsigned int(__stdcall*)(void*))threadAttack, (LPVOID)this, 0, NULL);  //��thisָ�봫�����߳�

	//CloseHandle(thrHandle);
}