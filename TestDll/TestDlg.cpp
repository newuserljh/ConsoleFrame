// TestDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "TestDll.h"
#include "TestDlg.h"
#include "afxdialogex.h"
#include <thread>
#include <mutex>
#include "monster.h"
#include "skill.h"
#include "bag.h"
#include "team.h"
#include "utils.h"
#include "gamecall.h"
#include "shareMemoryCli.h"
#include "config.h"
#pragma comment(lib , "Common.lib")
#pragma comment(lib ,"GameData.lib")
//#include "HookAPI.h"
#include "HookReg.h"
// CTestDlg 对话框
shareMemoryCli shareCli(MORE_OPEN_NUMBER);
int shareindex = -1;
//初始化HOOk
HookReg hook;
role r;//角色
role Promenade; //元神
monster m_mon;
skill m_skill;
bag r_bag;
gamecall mfun;
team m_team;

std::mutex team_mutex;  // 队伍文件变量互斥 


_declspec(naked) void CallTest()
{
	_asm pushad
	if (hook.EAX != 100)
	{
		tools::getInstance()->message("错误!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
		shareCli.m_pSMAllData->m_sm_data[shareindex].server_alive = false;//验证外挂存活
	}
	_asm  popad
	_asm ret
}

extern CTestDlg* pDlg;

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
	ON_BN_CLICKED(IDC_BUTTON4, &CTestDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_CHK_TEAM, &CTestDlg::OnBnClickedChkTeam)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_GJ, &CTestDlg::OnBnClickedBtnGj)
	ON_BN_CLICKED(IDC_BTN_TESTCALL, &CTestDlg::OnBnClickedBtnTestcall)
END_MESSAGE_MAP()


// CTestDlg 消息处理程序

void threadLogin()
{
	int i = 0;//尝试登录次数
	do
	{
		shareCli.m_pSMAllData->m_sm_data[shareindex].cscript = "登录中...";
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
		shareCli.m_pSMAllData->m_sm_data[shareindex].cscript = "登陆失败,正在结束程序";
		shareCli.m_pSMAllData->m_sm_data[shareindex].server_alive = false;
	}
	else 
	{
		shareCli.m_pSMAllData->m_sm_data[shareindex].cscript = "登陆完成";
		shareCli.m_pSMAllData->m_sm_data[shareindex].roleName = std::string(r.m_roleproperty.Object.pName);
		pDlg->init_team();
	}
	return;
}

void threadAlive()
{
	while (true)
	{
		shareCli.m_pSMAllData->m_sm_data[shareindex].rcv_rand = shareCli.m_pSMAllData->m_sm_data[shareindex].send_rand;//验证外挂存活
		shareCli.m_pSMAllData->m_sm_data[shareindex].ndPid=GetCurrentProcessId();//验证外挂存活
		Sleep(1000);
	}
}
// 追加文本到EditControl
void AppendText(CEdit &m_edit, CString strAdd)
{
	m_edit.SetSel(m_edit.GetWindowTextLength(), m_edit.GetWindowTextLength());
	m_edit.ReplaceSel(strAdd + L"\n");
}

void CTestDlg::OnBnClickedButton1()
{
	// 人物属性
	if (!r.init())return;
	CString s;
	s.Format("%s\n", r.m_roleproperty.Object.pName);
	AppendText(m_edit2,s);
	char* sex = {};
	char* job = {};
	if (*r.m_roleproperty.Sex == 0)sex = "男";
	else sex = "女";
	if (*r.m_roleproperty.Job == 0)job = "战士";
	else if (*r.m_roleproperty.Job == 1)job = "法师";
	else job = "道士";
	s.Format("职业:%s      等级:%d    性别:%s", job, *r.m_roleproperty.Level, sex);
	AppendText(m_edit2, s);;
	s.Format("HP:%d / %d", *r.m_roleproperty.Object.HP, *r.m_roleproperty.Object.HP_MAX);
	AppendText(m_edit2, s);
	s.Format("MP:%d / %d", *r.m_roleproperty.Object.MP, *r.m_roleproperty.Object.MP_MAX);
	AppendText(m_edit2, s);
	s.Format("当前地图:%s  坐标%d,%d", r.m_roleproperty.p_Current_Map, *r.m_roleproperty.Object.X, *r.m_roleproperty.Object.Y);
	AppendText(m_edit2, s);
	s.Format("背包大小:%d  背包负重%d / %d", *r.m_roleproperty.Bag_Size, *r.m_roleproperty.BAG_W, *r.m_roleproperty.BAG_W_MAX);
	AppendText(m_edit2, s);
	s.Format("ID:%x", *r.m_roleproperty.Object.ID);
	AppendText(m_edit2, s);
	for (auto i=0;i<21;i++)
	{
		if (!(*r.m_euip[i].ID))continue; 
		s.Format("%d :%s 耐久:%d/%d",i, r.m_euip[i].pName, *(r.m_euip[i].Use_Num),*(r.m_euip[i].Use_Num_Max));
		AppendText(m_edit2, s);
	}

	// 元神属性
	if (*r.m_roleproperty.Is_has_Promenade==0)return;
	Promenade.init_promenade();
	if (*Promenade.m_roleproperty.Is_Promenade_Release==0) mfun.release_Promenade();
	if (!Promenade.init_promenade())return;
	s.Format("%s\n", Promenade.m_roleproperty.Object.pName);
	AppendText(m_edit2, s);
	if (*Promenade.m_roleproperty.Sex == 0)sex = "男";
	else sex = "女";
	if (*Promenade.m_roleproperty.Job == 0)job = "战士";
	else if (*Promenade.m_roleproperty.Job == 1)job = "法师";
	else job = "道士";
	s.Format("职业:%s      等级:%d    性别:%s", job, *Promenade.m_roleproperty.Level, sex);
	AppendText(m_edit2, s);;
	s.Format("HP:%d / %d", *Promenade.m_roleproperty.Object.HP, *Promenade.m_roleproperty.Object.HP_MAX);
	AppendText(m_edit2, s);
	s.Format("MP:%d / %d", *Promenade.m_roleproperty.Object.MP, *Promenade.m_roleproperty.Object.MP_MAX);
	AppendText(m_edit2, s);
	s.Format("当前地图:%s  坐标%d,%d", Promenade.m_roleproperty.p_Current_Map, *Promenade.m_roleproperty.Object.X, *Promenade.m_roleproperty.Object.Y);
	AppendText(m_edit2, s);
	s.Format("背包大小:%d  背包负重%d / %d", *Promenade.m_roleproperty.Bag_Size, *Promenade.m_roleproperty.BAG_W, *Promenade.m_roleproperty.BAG_W_MAX);
	AppendText(m_edit2, s);
	s.Format("ID:%x", *Promenade.m_roleproperty.Object.ID);
	AppendText(m_edit2, s);
	for (auto i = 0; i < 21; i++)
	{
		if (!(*Promenade.m_euip[i].ID))continue;
		s.Format("%d :%s 耐久:%d/%d", i, Promenade.m_euip[i].pName, *(Promenade.m_euip[i].Use_Num), *(Promenade.m_euip[i].Use_Num_Max));
		AppendText(m_edit2, s);
	}
	Promenade.m_roleproperty.Object.Distance = mfun.caclDistance(*r.m_roleproperty.Object.X, *r.m_roleproperty.Object.Y, *Promenade.m_roleproperty.Object.X, *Promenade.m_roleproperty.Object.Y);
	s.Format("距主体距离:%f", Promenade.m_roleproperty.Object.Distance);
	AppendText(m_edit2, s);

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
//		tools::getInstance()->message("错误:%d", er);
//	}
//
//	hookAPI(WSARecv, MyWSARecv);
//	return ret;
//}

BOOL CTestDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
 
	// TODO:  在此添加额外的初始化


  //初始化共享内存,取得共享内存索引
	if (!shareCli.openShareMemory())
	{
		tools::getInstance()->message("打开共享内存失败\n");
	}
	shareindex = shareCli.getIndexByPID(GetCurrentProcessId());

	//HOOK连接服务器失败代码
	hook.hookReg(0x5F8B69, 5, CallTest);

	CloseHandle(::CreateThread(NULL, NULL, LPTHREAD_START_ROUTINE(threadLogin), NULL, NULL, NULL));
	//登录成功之后设置 启动通讯线程,定时验证存活消息
	
	CloseHandle(::CreateThread(NULL, NULL, LPTHREAD_START_ROUTINE(threadAlive), NULL, NULL, NULL));


	//hookAPI(WSARecv, MyWSARecv);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

/*初始化打怪相关设置*/
bool CTestDlg:: initVariable()
{
	i_map = 0;
	s_ID = -1;
	tflag_attack = true;
	tflag_goto = true;
	tflag_pickup = true;
	/*载入拾取物品*/
	pick_goods_list.clear();
	pick_goods_list = tools::getInstance()->ReadTxt(tools::getInstance()->getParentPath(shareCli.m_pSMAllData->currDir) + "拾取物品.txt");
	if (!pick_goods_list.size())return false;
	/*设置技能*/
	if (!Set_Skill())return false;
	/*载入打怪地图路线*/
	if (!Load_coordinate())return false;
	/*载入攻击怪物列表*/
	attack_monlist.clear();
	attack_monlist = tools::getInstance()->ReadTxt(tools::getInstance()->getParentPath(shareCli.m_pSMAllData->currDir) + "逆魔大殿.txt");
	if (!attack_monlist.size())return false;

	return true;
}

/*
函数功能:自动回收物品，将军团物品回收为经验，逆魔物品回收为元宝
*/
void CTestDlg::AutoRecvGoods(void)
{
	/*回收经验*/
	if (r_bag.caclGoodsNumber("幽冥项链") > 0) {
		mfun.RecovryGoods_To_Exp(*(r_bag.m_bag[r_bag.getGoodsIndex("幽冥项链")].ID),*r.m_roleproperty.VIP_Level);
		Sleep(200);
	}
	if (r_bag.caclGoodsNumber("阎罗手套") > 0) {
		mfun.RecovryGoods_To_Exp(*(r_bag.m_bag[r_bag.getGoodsIndex("阎罗手套")].ID), *r.m_roleproperty.VIP_Level);
		Sleep(200);
	}
	if (r_bag.caclGoodsNumber("光芒手套") > 0) {
		mfun.RecovryGoods_To_Exp(*(r_bag.m_bag[r_bag.getGoodsIndex("光芒手套")].ID), *r.m_roleproperty.VIP_Level);
		Sleep(200);
	}
	if (r_bag.caclGoodsNumber("天珠项链") > 0) {
		mfun.RecovryGoods_To_Exp(*(r_bag.m_bag[r_bag.getGoodsIndex("天珠项链")].ID), *r.m_roleproperty.VIP_Level);
		Sleep(200);
	}
	if (r_bag.caclGoodsNumber("心灵护腕") > 0) {
		mfun.RecovryGoods_To_Exp(*(r_bag.m_bag[r_bag.getGoodsIndex("心灵护腕")].ID), *r.m_roleproperty.VIP_Level);
		Sleep(200);
	}
	if (r_bag.caclGoodsNumber("白金戒指") > 0) {
		mfun.RecovryGoods_To_Exp(*(r_bag.m_bag[r_bag.getGoodsIndex("白金戒指")].ID), *r.m_roleproperty.VIP_Level);
		Sleep(200);
	}
	if (r_bag.caclGoodsNumber("生命项链") > 0) {
		mfun.RecovryGoods_To_Exp(*(r_bag.m_bag[r_bag.getGoodsIndex("生命项链")].ID), *r.m_roleproperty.VIP_Level);
		Sleep(200);
	}
	if (r_bag.caclGoodsNumber("魔魂手镯") > 0) {
		mfun.RecovryGoods_To_Exp(*(r_bag.m_bag[r_bag.getGoodsIndex("魔魂手镯")].ID), *r.m_roleproperty.VIP_Level);
		Sleep(200);
	}
	if (r_bag.caclGoodsNumber("红宝戒指") > 0) {
		mfun.RecovryGoods_To_Exp(*(r_bag.m_bag[r_bag.getGoodsIndex("红宝戒指")].ID), *r.m_roleproperty.VIP_Level);
		Sleep(200);
	}
	if (r_bag.caclGoodsNumber("龙戒") > 0) {
		mfun.RecovryGoods_To_Exp(*(r_bag.m_bag[r_bag.getGoodsIndex("龙戒")].ID), *r.m_roleproperty.VIP_Level);
		Sleep(200);
	}
	if (r_bag.caclGoodsNumber("记忆项链") > 0) {
		mfun.RecovryGoods_To_Exp(*(r_bag.m_bag[r_bag.getGoodsIndex("记忆项链")].ID), *r.m_roleproperty.VIP_Level);
		Sleep(200);
	}
	if (r_bag.caclGoodsNumber("记忆头盔") > 0) {
		mfun.RecovryGoods_To_Exp(*(r_bag.m_bag[r_bag.getGoodsIndex("记忆头盔")].ID), *r.m_roleproperty.VIP_Level);
		Sleep(200);
	}
	if (r_bag.caclGoodsNumber("战神(男)") > 0) {
		mfun.RecovryGoods_To_Exp(*(r_bag.m_bag[r_bag.getGoodsIndex("战神(男)")].ID), *r.m_roleproperty.VIP_Level);
		Sleep(200);
	}
	if (r_bag.caclGoodsNumber("战神(女)") > 0) {
		mfun.RecovryGoods_To_Exp(*(r_bag.m_bag[r_bag.getGoodsIndex("战神(女)")].ID), *r.m_roleproperty.VIP_Level);
		Sleep(200);
	}
	if (r_bag.caclGoodsNumber("幽灵战甲(男)") > 0) {
		mfun.RecovryGoods_To_Exp(*(r_bag.m_bag[r_bag.getGoodsIndex("幽灵战甲(男)")].ID), *r.m_roleproperty.VIP_Level);
		Sleep(200);
	}
	if (r_bag.caclGoodsNumber("幽灵战甲(女)") > 0) {
		mfun.RecovryGoods_To_Exp(*(r_bag.m_bag[r_bag.getGoodsIndex("幽灵战甲(女)")].ID), *r.m_roleproperty.VIP_Level);
		Sleep(200);
	}
	if (r_bag.caclGoodsNumber("恶魔长袍(男)") > 0) {
		mfun.RecovryGoods_To_Exp(*(r_bag.m_bag[r_bag.getGoodsIndex("恶魔长袍(男)")].ID), *r.m_roleproperty.VIP_Level);
		Sleep(200);
	}
	if (r_bag.caclGoodsNumber("恶魔长袍(女)") > 0) {
		mfun.RecovryGoods_To_Exp(*(r_bag.m_bag[r_bag.getGoodsIndex("恶魔长袍(女)")].ID), *r.m_roleproperty.VIP_Level);
		Sleep(200);
	}
	if (r_bag.caclGoodsNumber("离别钩") > 0) {
		mfun.RecovryGoods_To_Exp(*(r_bag.m_bag[r_bag.getGoodsIndex("离别钩")].ID), *r.m_roleproperty.VIP_Level);
		Sleep(200);
	}
	if (r_bag.caclGoodsNumber("方天画戢") > 0) {
		mfun.RecovryGoods_To_Exp(*(r_bag.m_bag[r_bag.getGoodsIndex("方天画戢")].ID), *r.m_roleproperty.VIP_Level);
		Sleep(200);
	}
	if (r_bag.caclGoodsNumber("井中月") > 0) {
		mfun.RecovryGoods_To_Exp(*(r_bag.m_bag[r_bag.getGoodsIndex("井中月")].ID), *r.m_roleproperty.VIP_Level);
		Sleep(200);
	}
	if (r_bag.caclGoodsNumber("铜锤") > 0) {
		mfun.RecovryGoods_To_Exp(*(r_bag.m_bag[r_bag.getGoodsIndex("铜锤")].ID), *r.m_roleproperty.VIP_Level);
		Sleep(200);
	}
	if (r_bag.caclGoodsNumber("无极") > 0) {
		mfun.RecovryGoods_To_Exp(*(r_bag.m_bag[r_bag.getGoodsIndex("无极")].ID), *r.m_roleproperty.VIP_Level);
		Sleep(200);
	}
	if (r_bag.caclGoodsNumber("魔法权杖") > 0) {
		mfun.RecovryGoods_To_Exp(*(r_bag.m_bag[r_bag.getGoodsIndex("魔法权杖")].ID), *r.m_roleproperty.VIP_Level);
		Sleep(500);
	}

	/*回收元宝*/
	if (r_bag.caclGoodsNumber("恶魔铃") > 0) {
		mfun.RecovryGoods_To_Gold(*(r_bag.m_bag[r_bag.getGoodsIndex("恶魔铃")].ID), *r.m_roleproperty.VIP_Level);
		Sleep(200);
	}
	if (r_bag.caclGoodsNumber("龙鳞手镯") > 0) {
		mfun.RecovryGoods_To_Gold(*(r_bag.m_bag[r_bag.getGoodsIndex("龙鳞手镯")].ID), *r.m_roleproperty.VIP_Level);
		Sleep(200);
	}
	if (r_bag.caclGoodsNumber("碧螺戒指") > 0) {
		mfun.RecovryGoods_To_Gold(*(r_bag.m_bag[r_bag.getGoodsIndex("碧螺戒指")].ID), *r.m_roleproperty.VIP_Level);
		Sleep(200);
	}
	if (r_bag.caclGoodsNumber("魔龙腰带") > 0) {
		mfun.RecovryGoods_To_Gold(*(r_bag.m_bag[r_bag.getGoodsIndex("魔龙腰带")].ID), *r.m_roleproperty.VIP_Level);
		Sleep(200);
	}
	if (r_bag.caclGoodsNumber("魔龙法靴") > 0) {
		mfun.RecovryGoods_To_Gold(*(r_bag.m_bag[r_bag.getGoodsIndex("魔龙法靴")].ID), *r.m_roleproperty.VIP_Level);
		Sleep(200);
	}
	if (r_bag.caclGoodsNumber("黑铁头盔") > 0) {
		mfun.RecovryGoods_To_Gold(*(r_bag.m_bag[r_bag.getGoodsIndex("黑铁头盔")].ID), *r.m_roleproperty.VIP_Level);
		Sleep(200);
	}
	if (r_bag.caclGoodsNumber("绿色项链") > 0) {
		mfun.RecovryGoods_To_Gold(*(r_bag.m_bag[r_bag.getGoodsIndex("绿色项链")].ID), *r.m_roleproperty.VIP_Level);
		Sleep(200);
	}
	if (r_bag.caclGoodsNumber("猛士手镯") > 0) {
		mfun.RecovryGoods_To_Gold(*(r_bag.m_bag[r_bag.getGoodsIndex("猛士手镯")].ID), *r.m_roleproperty.VIP_Level);
		Sleep(200);
	}
	if (r_bag.caclGoodsNumber("力量戒指") > 0) {
		mfun.RecovryGoods_To_Gold(*(r_bag.m_bag[r_bag.getGoodsIndex("力量戒指")].ID), *r.m_roleproperty.VIP_Level);
		Sleep(200);
	}
	if (r_bag.caclGoodsNumber("雷霆腰带") > 0) {
		mfun.RecovryGoods_To_Gold(*(r_bag.m_bag[r_bag.getGoodsIndex("雷霆腰带")].ID), *r.m_roleproperty.VIP_Level);
		Sleep(200);
	}
	if (r_bag.caclGoodsNumber("雷霆战靴") > 0) {
		mfun.RecovryGoods_To_Gold(*(r_bag.m_bag[r_bag.getGoodsIndex("雷霆战靴")].ID), *r.m_roleproperty.VIP_Level);
		Sleep(200);
	}
	if (r_bag.caclGoodsNumber("灵魂项链") > 0) {
		mfun.RecovryGoods_To_Gold(*(r_bag.m_bag[r_bag.getGoodsIndex("灵魂项链")].ID), *r.m_roleproperty.VIP_Level);
		Sleep(200);
	}
	if (r_bag.caclGoodsNumber("三眼护腕") > 0) {
		mfun.RecovryGoods_To_Gold(*(r_bag.m_bag[r_bag.getGoodsIndex("三眼护腕")].ID), *r.m_roleproperty.VIP_Level);
		Sleep(200);
	}
	if (r_bag.caclGoodsNumber("泰坦戒指") > 0) {
		mfun.RecovryGoods_To_Gold(*(r_bag.m_bag[r_bag.getGoodsIndex("泰坦戒指")].ID), *r.m_roleproperty.VIP_Level);
		Sleep(200);
	}
	if (r_bag.caclGoodsNumber("风云腰带") > 0) {
		mfun.RecovryGoods_To_Gold(*(r_bag.m_bag[r_bag.getGoodsIndex("风云腰带")].ID), *r.m_roleproperty.VIP_Level);
		Sleep(200);
	}
	if (r_bag.caclGoodsNumber("风云道靴") > 0) {
		mfun.RecovryGoods_To_Gold(*(r_bag.m_bag[r_bag.getGoodsIndex("风云道靴")].ID), *r.m_roleproperty.VIP_Level);
		Sleep(200);
	}
}

/*
函数功能:回城处理
*/
void CTestDlg::AutoReturnToCity()
{
	// 检查背包中是否有回城卷轴
	if (r_bag.caclGoodsNumber("永久回城神石") > 0)
	{
		// 使用回城卷轴
		mfun.useGoods(r_bag.getGoodsIndex("永久回城神石"));
		Sleep(2000); // 等待回城操作完成

		// 回城后处理包裹
		AutoRecvGoods();

		// 检查背包空间
		if (r_bag.getBagSpace() < 10)
		{
			// 背包空间不足，处理多余物品
			if (r_bag.caclGoodsNumber("强效太阳神水") > REMAIN_TAIYANG)
			{
				mfun.useGoods(r_bag.getGoodsIndex("强效太阳神水"));
				Sleep(100);
			}
			if (r_bag.caclGoodsNumber("治疗药水") > REMAIN_TAIYANG)
			{
				mfun.useGoods(r_bag.getGoodsIndex("治疗药水"));
				Sleep(200);
			}
		}
	}
	else
	{
		// 背包中没有回城卷轴，提示用户
		AfxMessageBox("背包中没有回城卷轴，请补充！");
	}
}

void CTestDlg::OnBnClickedButton2()
{
	// TODO: 遍历周围对象 地面 怪物NPC
	if (!r.init())return;
	m_mon.m_monsterList.clear();
	m_mon.m_groundList.clear();
	CString s;
	if (!r.Get_Envionment(m_mon.pOb_list))
	{
		s.Format("遍历周围错误：\n");
		AppendText(m_edit2, s);
		return;
	}	
	if (!r.Get_Ground(m_mon.pGr_list))
	{
		s.Format("遍历地面错误：\n");
		AppendText(m_edit2, s);
		return;
	}
	m_mon.init();
	s.Format("周围对象列表（怪物、NPC、其他玩家、宠物）：\n");
	AppendText(m_edit2, s);
	for (size_t i = 0; i < m_mon.m_monsterList.size(); i++)
	{
		s.Format("%s ID: %x, 坐标(x,y)：%d, %d   距离：%.2f\n",
			m_mon.m_monsterList[i].pName, 
			*(m_mon.m_monsterList[i].ID),
			*(m_mon.m_monsterList[i].X),
			*(m_mon.m_monsterList[i].Y),
			mfun.caclDistance(*(r.m_roleproperty.Object.X), *(r.m_roleproperty.Object.Y), *(m_mon.m_monsterList[i].X), *(m_mon.m_monsterList[i].Y)));
		AppendText(m_edit2, s);
	}

	s.Format("地面：\n");
	AppendText(m_edit2, s);
	for (size_t i = 0; i < m_mon.m_groundList.size(); i++)
	{
		s.Format(" %s : %d/%d\n", m_mon.m_groundList[i].pName, *m_mon.m_groundList[i].X, *m_mon.m_groundList[i].Y);
		AppendText(m_edit2, s);
	}

}


void CTestDlg::OnBnClickedButton3()
{
	// TODO: 技能遍历
	if (!r.init())return;
	CString s;
	m_skill.m_skillList.clear();
	m_skill.skillBase = (DWORD)r.m_roleproperty.p_Skill_Base;
	if (!m_skill.init())
	{
		s.Format("遍历技能错误：\n");
		AppendText(m_edit2, s);
		return;
	}
	s.Format("技能：\n");
	AppendText(m_edit2, s);

	for (size_t i = 0; i < m_skill.m_skillList.size(); i++)
	{
		s.Format("%s  等级:%d  ID: %d\n", m_skill.m_skillList[i].pName, *(m_skill.m_skillList[i].level), *(m_skill.m_skillList[i].ID));
		AppendText(m_edit2, s);
	}

}


void CTestDlg::OnBnClickedButton8()
{
	// TODO: 背包遍历
	if (!r.init())return;
	CString s;
	r_bag.maxSize = *r.m_roleproperty.Bag_Size;
	r_bag.bagBase = (DWORD)r.m_roleproperty.p_Bag_Base;
	if(!r_bag.init())
	{
		s.Format("遍历背包错误：\n");
		AppendText(m_edit2, s);
		return;
	}
	s.Format("大小:%d   剩余: %d\n", r_bag.maxSize, r_bag.getBagSpace());
	AppendText(m_edit2, s);

	for (size_t i=0;i<r_bag.maxSize;i++)
	{
		if (*(r_bag.m_bag[i].ID))
		{
			s.Format("第%d格:%s   ID: %x\n", i, r_bag.m_bag[i].pName, *r_bag.m_bag[i].ID);
			AppendText(m_edit2, s);
		}
	}


}


void CTestDlg::OnBnClickedButton5()
{
	// TODO: 停止脚本
	tflag_attack = false;
	//tflag_goto= false;
	tflag_pickup= false;
	//线程开关置为假
	//WaitForSingleObject(m_threadGoto, 100);
	WaitForSingleObject(m_threadAttack, 60000);
	WaitForSingleObject(m_threadPickup, 60000);
}




/*
函数功能:选择打怪目标
参数一:角色结构体
参数二:攻击怪物列表
返回值：选中怪物对象指针
*/
MONSTER_PROPERTY Choose_Moster(CTestDlg* pDlg, std::vector<std::string>& vec)
{
	MONSTER_PROPERTY ret;
	if(*r.m_roleproperty.p_Target_ID == 0)
	{
		std::vector<MONSTER_PROPERTY> near_atak_mon_list = mfun.sort_aroud_monster(r, vec);
		if (near_atak_mon_list.size() > 0)
		{
			ret = near_atak_mon_list[0];
			*r.m_roleproperty.p_Target_ID = *ret.ID;
		}
		else
		{
			/*边寻路边找怪*/
			if (pDlg->i_map == pDlg->map_xy.size())pDlg->i_map = 0;
			MapXY xy = pDlg->map_xy[pDlg->i_map];
			mfun.CurrentMapMove(xy.x, xy.y);//寻路0.8s
			Sleep(800);
			mfun.CurrentMapMove(*r.m_roleproperty.Object.X, *r.m_roleproperty.Object.Y);//停止
			if (mfun.caclDistance(*r.m_roleproperty.Object.X, *r.m_roleproperty.Object.Y,xy.x,xy.y)<4)pDlg->i_map++;
		}
	}
	return ret;
}

/*
函数功能:自动打怪
参数一:角色结构体
参数二:攻击怪物列表
参数三:使用技能ID
返回值：true为已经打死怪物，fasle为位置错误（或者600次没打死怪物）
*/
bool Auto_Attack(CTestDlg* pDlg, std::vector<std::string>& vec, DWORD s_ID)
{
	*r.m_roleproperty.p_Target_ID = 0;
	MONSTER_PROPERTY att_mon = Choose_Moster(pDlg, vec);
	if (nullptr == att_mon.ID)return false;
	unsigned i = 0;/*记录攻击次数*/
	while( (*att_mon.HP> 0) && (0 != *r.m_roleproperty.p_Target_ID) && (i < 600))
	{
		if (mfun.caclDistance(*r.m_roleproperty.Object.X, *r.m_roleproperty.Object.Y, *att_mon.X, *att_mon.Y) > 7)
		{
			mfun.CurrentMapMove(*att_mon.X, *att_mon.Y);
			Sleep(500);
		}
		DWORD s_posion = m_skill.getSkillId("施毒术");
		if( (s_posion!=-1))//道士
		{
			if ((r_bag.ifHasPoison()>0)&& (*att_mon.IsPosion < 0x40))//是否中毒 0没毒，0x40红毒，0x80绿毒，0xc0红绿毒,
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
函数功能:设置打怪技能
参数一:角色结构体
返回值：选中怪物对象指针
*/
bool CTestDlg::Set_Skill()
{
	/*技能设置*/
	if (*r.m_roleproperty.Job == 0)/*战士*/
	{
		return false;
	}
    if (*r.m_roleproperty.Job == 1)/*法师*/
	{
		DWORD  s_tmp = m_skill.getSkillId("狂龙紫电");
		if (s_tmp != -1)
		{
			s_ID = s_tmp;
			return true;
		}
		s_tmp = m_skill.getSkillId("雷电术");
		if (s_tmp != -1)
		{
			s_ID = s_tmp;
			return true;
		}
		s_tmp = m_skill.getSkillId("冰箭术");
		if (s_tmp != -1)
		{
			s_ID = s_tmp;
			return true;
		}
		s_tmp = m_skill.getSkillId("小火球");
		if (s_tmp != -1)
		{
			s_ID = s_tmp;
			return true;
		}
		return false;
	}
	if (*r.m_roleproperty.Job == 2)/*道士*/
	{
		return false;
	}
	 return false;

}


/*
函数功能:载入寻路打怪坐标
参数一:角色结构体
返回值：选中怪物对象指针
*/
bool CTestDlg::Load_coordinate()
{
	map_xy.clear();
	std::vector<std::string> map_coordinate= tools::getInstance()->ReadTxt(tools::getInstance()->getParentPath(shareCli.m_pSMAllData->currDir) + "\\逆魔大殿1.txt");
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
函数功能:初始化组队Timer
参数一:角色结构体
返回值：选中怪物对象指针
*/
bool CTestDlg::init_team()
{
	m_team.init();
	m_team_check_id = 0;
	pBtn = (CButton*)GetDlgItem(IDC_CHK_TEAM);  //获得组队复选框控件的句柄
	team_list.clear();
	team_list = tools::getInstance()->ReadTxt(tools::getInstance()->getParentPath(shareCli.m_pSMAllData->currDir) + "\\队伍人员.txt");
	if (pBtn->GetCheck())
	{
		if (*r.m_roleproperty.Team_is_allow != 1)mfun.team_open_close(1);//允许组队
		m_team_check_id = SetTimer(11111, 30000, NULL);
	}
	return true;
}

/*
函数功能:读取组队文件并组队
参数一:角色结构体
返回值：选中怪物对象指针
*/
//void  TimerProc(HWND hWnd, UINT uMsg, UINT uID, DWORD dwTimew)
void  CTestDlg::MakeTeam(CTestDlg* pDlg)
{		
	m_team.init();
	if (pDlg->team_list.size()<2)return;
	if (pDlg->team_list.size() == m_team.m_team_list.size())return;//组队完毕
	if (strcmp(pDlg->team_list[0].c_str(),r.m_roleproperty.Object.pName)==0)//我是队长
	{
		for (size_t i = 1; i < pDlg->team_list.size(); i++)
		{
			size_t temp = i;
			for (size_t j = 0; j < m_team.m_team_list.size(); j++)
			{
				if (strcmp(pDlg->team_list[i].c_str(), m_team.m_team_list[j].pName) == 0)
				{
					temp = i + 1;
					break;
				}			
			}
			if (temp==i) mfun.maketeam(pDlg->team_list[i]);
		}
	}
	else //我是队员
	{
		if (shareCli.m_pSMAllData->team_info == 0)
		{
			pBtn->SetCheck(0);
			pDlg->OnBnClickedChkTeam();
			return;
		}
		for (size_t j = 0; j < m_team.m_team_list.size(); j++)
		{
			if (strcmp(r.m_roleproperty.Object.pName, m_team.m_team_list[j].pName) == 0)
			{
				return;
			}
		}
	  mfun.allowteam(pDlg->team_list[0]);//同意组队
	}
}


/*寻路线程*/
UINT __cdecl CTestDlg::threadGoto(LPVOID p)
{
	CTestDlg* pDlg = (CTestDlg*)p;
	size_t i = 0;
	while (pDlg->tflag_goto)
	{
		MapXY xy = pDlg->map_xy[i];
		shareCli.m_pSMAllData->m_sm_data[shareindex].cscript = std::string( "正在寻路到");
		//MapGoto_Point(pDlg->map_xy[i].x, pDlg->map_xy[i].y, pDlg->tflag_goto);
		i++;
		if (i = pDlg->map_xy.size())i = 0;
	}
	return 0;
}

/*打怪线程*/
UINT __cdecl CTestDlg::threadAttack(LPVOID p)
{
	CTestDlg* pDlg = (CTestDlg*)p;
	CString s;
	s.Format("打怪线程开始");
	AppendText(pDlg->m_edit2, s);
	while (pDlg->tflag_attack)
	{
		shareCli.m_pSMAllData->m_sm_data[shareindex].cscript = std::string("打怪");
		//Auto_Attack(pDlg, pDlg->attack_monlist, pDlg->s_ID);
		mfun.start_end_AutoAttack(pDlg->tflag_attack);
		//Sleep(ATTACK_SLEEP);
	}
	mfun.start_end_AutoAttack(pDlg->tflag_attack);
	shareCli.m_pSMAllData->m_sm_data[shareindex].cscript = std::string("空闲");
	s.Format("打怪线程停止");
	AppendText(pDlg->m_edit2, s);
	return 0;
}

/*包裹处理*/
UINT __cdecl CTestDlg::threadBagPocess(LPVOID p)
{
	CString s;
	CTestDlg* pDlg = (CTestDlg*)p;
	s.Format("处理包裹线程开始");
	AppendText(pDlg->m_edit2, s);
	while (pDlg->tflag_attack)
	{
		if (r_bag.getBagSpace() < 35)
		{
			pDlg->AutoRecvGoods();
			if (r_bag.caclGoodsNumber("1个绑定元宝") > 0) {
				mfun.useGoods(r_bag.getGoodsIndex("1个绑定元宝"));
				Sleep(200);
			}
			if (r_bag.caclGoodsNumber("2个绑定元宝") > 0) {
				mfun.useGoods(r_bag.getGoodsIndex("2个绑定元宝"));
				Sleep(200);
			}
			if (r_bag.caclGoodsNumber("5个绑定元宝") > 0) {
				mfun.useGoods(r_bag.getGoodsIndex("5个绑定元宝"));
				Sleep(200);
			}
			if (r_bag.caclGoodsNumber("10个绑定元宝") > 0) {
				mfun.useGoods(r_bag.getGoodsIndex("10个绑定元宝"));
				Sleep(200);
			}
			if (r_bag.caclGoodsNumber("20个绑定元宝") > 0)
			{
				mfun.useGoods(r_bag.getGoodsIndex("20个绑定元宝"));
				Sleep(200);
			}
	    }
		if (r_bag.getBagSpace() < 10)
		{
			if (r_bag.caclGoodsNumber("强效太阳神水") > REMAIN_TAIYANG)
			{
				mfun.useGoods(r_bag.getGoodsIndex("强效太阳神水"));
				Sleep(100);
			}
			if (r_bag.caclGoodsNumber("治疗药水") > REMAIN_TAIYANG) {
				mfun.useGoods(r_bag.getGoodsIndex("治疗药水"));
				Sleep(200);
			}
		}
		Sleep(2000);
	}
	s.Format("处理包裹线程停止");
	AppendText(pDlg->m_edit2, s);
	return 0;
}


/*捡物线程*/
UINT __cdecl CTestDlg::threadPickup(LPVOID p)
{
	CString s;
	CTestDlg* pDlg = (CTestDlg*)p;
	s.Format("捡物线程开始");
	AppendText(pDlg->m_edit2, s);
	while (pDlg->tflag_pickup)
	{
		std::vector<GROUND_GOODS> need2pick_list = mfun.sort_groud_goods(r, pDlg->pick_goods_list);
		if (need2pick_list.size())
		{
			GROUND_GOODS pick_temp = need2pick_list[0];
			std::vector<MONSTER_PROPERTY> near_mon = mfun.sort_aroud_monster(r, pDlg->attack_monlist, 6);
			if (near_mon.size() < 5)
			{
				pDlg->m_threadAttack->SuspendThread();
				s.Format("正在拾取物品:%s 坐标：%d,%d", pick_temp.pName, *pick_temp.X, *pick_temp.Y);
				AppendText(pDlg->m_edit2, s);
				int pick_try_accounts = 0;
				do
				{
					mfun.Run_or_Step_To(*pick_temp.X, *pick_temp.Y, 1);
					Sleep(800);
					mfun.Run_or_Step_To(*pick_temp.X, *pick_temp.Y, 1);
					Sleep(800);
					mfun.Run_or_Step_To(*pick_temp.X, *pick_temp.Y, 1);
					Sleep(800);
					pick_try_accounts++;
					if ((*r.m_roleproperty.Object.X == *pick_temp.X) && (*r.m_roleproperty.Object.X == *pick_temp.Y))
					{
						mfun.pickupGoods(*pick_temp.X, *pick_temp.Y);
					}
					need2pick_list.clear();
					need2pick_list = mfun.sort_groud_goods(r, pDlg->pick_goods_list);
					if (!need2pick_list.size())break;
				} while ((need2pick_list[0].X == pick_temp.X) && (need2pick_list[0].Y == pick_temp.Y) && (pick_try_accounts < 4));
				pDlg->m_threadAttack->ResumeThread();
			}
			else
			{
				Sleep(10000);//打怪
			}
		}
		Sleep(1000);
	}
	s.Format("捡物线程停止");
	AppendText(pDlg->m_edit2, s);
	return 0;
}

void CTestDlg::OnBnClickedButton9()
{
	// TODO: 脚本测试

	/*自动打怪 需要启动：①打怪线程优先级正常 ②遍历周围对象、地面并拾取线程 优先级中高 ③寻路线程、智能闪避 优先级最高
	*
	*          ****选怪
	*自动打怪***   打怪：判断打死
	*          ****继续打怪
	* */
	r.init();
	r.Get_Envionment(m_mon.pOb_list);
	r.Get_Ground(m_mon.pGr_list);
	m_mon.init();
	r_bag.maxSize = *r.m_roleproperty.Bag_Size;
	r_bag.bagBase = (DWORD)r.m_roleproperty.p_Bag_Base;
	m_skill.skillBase = (DWORD)r.m_roleproperty.p_Skill_Base;
	r_bag.init();
	m_skill.init();

	initVariable();
	//CString s;
	//std::vector<GROUND_GOODS> need2pick_list = mfun.sort_groud_goods(r, pick_goods_list);
	//for (auto i=0;i< need2pick_list.size();i++)
	//{
	//	s.Format("%s %d/%d  %f", need2pick_list[i].pName, *need2pick_list[i].X, *need2pick_list[i].Y, need2pick_list[i].Distance);
	//	AppendText(m_edit2, s);
	//}

	//if (need2pick_list.size())
	//{
	//	GROUND_GOODS pick_temp = need2pick_list[0];
	//	s.Format("%s %d/%d  %f", pick_temp.pName, *pick_temp.X, *pick_temp.Y, pick_temp.Distance);
	//	AppendText(m_edit2, s);
	//	mfun.CurrentMapMove(*pick_temp.X, *pick_temp.Y);
	//	Sleep(2000);
	//	//mfun.pickupGoods(*pick_temp.X, *pick_temp.Y);
	//}
	//m_threadGoto=AfxBeginThread(threadGoto, (LPVOID)this, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
	m_threadAttack= AfxBeginThread(threadAttack, (LPVOID)this);
	m_threadPickup= AfxBeginThread(threadPickup, (LPVOID)this, THREAD_PRIORITY_ABOVE_NORMAL);
		
}

void CTestDlg::OnBnClickedButton4()
{
	// 队伍遍历+任务遍历
	r.init();
	m_team.team_Base = r.m_roleproperty.Team_pointer;
	m_team.m_team_list.clear();
	m_team.init();
	CString s;
	s.Format("%p \n", m_team.team_Base);
	AppendText(m_edit2, s);
	if (m_team.m_team_list.size())
	{
		for (size_t i = 0; i < m_team.m_team_list.size(); i++)
		{
			s.Format("名字:%s NEXT:%p Pre:%p \n", m_team.m_team_list[i].pName, m_team.m_team_list[i].Next, m_team.m_team_list[i].Previous);
			AppendText(m_edit2, s);
		}
	}
}


void CTestDlg::OnBnClickedChkTeam()
{
	// TODO: 组队CHECK单机事件
	pBtn = (CButton*)GetDlgItem(IDC_CHK_TEAM);  //获得组队复选框控件的句柄
	CString s;

	if (pBtn->GetCheck()&&(m_team_check_id==0))
	{
		if (*r.m_roleproperty.Team_is_allow != 1)mfun.team_open_close(1);//允许组队
		m_team_check_id = SetTimer(11111, 30000, NULL);
	}
	else
	{
		if (m_team_check_id != 0)
		{			
			KillTimer(11111);
			m_team_check_id = 0;
			if (strcmp(team_list[0].c_str(),r.m_roleproperty.Object.pName)!=0)mfun.team_open_close(0);//队员直接关闭队伍开关
			else//队长 逐个删除队员之后再关闭队伍开关
			{
				shareCli.m_pSMAllData->team_info = 0;
				s.Format("你是队长,正在逐个关闭队员队伍开关,稍等10s");
				AppendText(m_edit2, s);
			}
		}
	}
}


void CTestDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	switch (nIDEvent)
	{
	case 11111:
		MakeTeam(this);
		break;
	default:
		break;
	}
	CDialogEx::OnTimer(nIDEvent);

}



void CTestDlg::OnBnClickedBtnGj()
{
	// TODO: 使用内置自动打怪挂机
	if (!r.init())return;
	r_bag.maxSize = *r.m_roleproperty.Bag_Size;
	r_bag.bagBase = (DWORD)r.m_roleproperty.p_Bag_Base;
	r_bag.init();
    tflag_attack = !tflag_attack;
	if (tflag_attack)
	{
		m_threadAttack = AfxBeginThread(threadAttack, (LPVOID)this);
		m_threadBagProcess = AfxBeginThread(threadBagPocess, (LPVOID)this);
	}
	else
	{
		WaitForSingleObject(m_threadAttack, 60000);
		WaitForSingleObject(m_threadBagProcess, 60000);
	}	
}


void CTestDlg::OnBnClickedBtnTestcall()
{
	// TODO: 在此添加控件通知处理程序代码


}
