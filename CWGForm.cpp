// CWGForm.cpp: 实现文件
//

#include "pch.h"
#include "CWGForm.h"
#include <vector>
#include <fstream>
#include<string>
#include <regex>

/*==========进程通信=========*/
//#define _MAP_ "gyarmy" 
//HANDLE g_hModule;
//HANDLE g_hMapFile;
//LPTSTR lpBuff;
//DWORD dwType;

using namespace std;
CWinThread* m_pThread_Environment; //遍历周围NPC 怪物
CWinThread* m_pThread_AutoAttack; //自动打怪
CWinThread* m_pThread_Goto; //打怪寻路
CWinThread* m_pThread_Mapping; //打怪寻路

CCriticalSection g_criticalsection;//*边界锁 线程同步用 
BOOL tflag/*遍历*/, tflag_auto/*打怪*/, tflag_goto/*寻路*/,tflag_map/*进程通信*/; //线程开关
HUMAN role1, role_YS; //主角色,元神; //人物结构体
GOODS role_Bag[125], ls_Bag[59], ys_Bag[59], role_ZB[19]; //主体包裹,灵兽,元神，主体装备


vector<DWORD> p_Moster; //存储怪物NPC
vector<DWORD> p_Ground_Goods;//存储地面物品
vector<SKILL> role_Skill,YS_Skill;//存储技能


CWGForm::CWGForm(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DLG_MAIN, pParent)
{

}

CWGForm::~CWGForm()
{
}

void CWGForm::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_Monster_Listctrl);
}


BEGIN_MESSAGE_MAP(CWGForm, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_CALLTEST, &CWGForm::OnBnClickedBtnCalltest)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_STOP, &CWGForm::OnBnClickedButtonStop)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BTN_PAUSE, &CWGForm::OnBnClickedBtnPause)
END_MESSAGE_MAP()

const UINT ID_TIMER_ZHILIAO = 0x001;
const UINT ZHILIAO_ELAPSE = 1 * 1000;

const UINT ID_TIMER_HUISHOU = 0x002;
const UINT HUISHOU_ELAPSE = 240 * 1000;
// CWGForm 消息处理程序

OBJECT Get_Object(DWORD obj_BASE)/*获取对象信息*/
{
	OBJECT temp;
	_asm
	{
		pushad
		mov ecx, obj_BASE
		mov ecx, [ecx]
		add ecx, 8
		mov eax, [ecx]
		mov temp.ID, eax

		add ecx, 8
		mov temp.Name, ecx
		add ecx, 0x70 // 
		mov eax, [ecx]
		mov temp.HP, eax
		add ecx, 4// 
		mov eax, [ecx]
		mov temp.HP_MAX, eax
		add ecx, 4 // 
		mov eax, [ecx]
		mov temp.MP, eax
		add ecx, 4 // 
		mov eax, [ecx]
		mov temp.MP_MAX, eax
		add ecx, 0x20 // 
		mov eax, [ecx]
		mov temp.XY.X, eax
		add ecx, 4 // 
		mov eax, [ecx]
		mov temp.XY.Y, eax
		add ecx, 0x29b
		mov eax, [ecx]
		mov temp.IsPosion, al
		popad
	}
	return temp;
}
void Get_Skill(DWORD s_Base/*技能基址*/,vector<SKILL>&vec )/*获取角色技能列表*/
{
	if (!s_Base)return;
	SKILL temp = { 0 };
	int i = 0;
	int iaddr = 0;
	while ((*(DWORD*)(s_Base + i * 0x88 + 0x14))) // 
	{
		iaddr = i * 0x88 + 0x14;
		_asm {
			mov ecx, s_Base;
			add ecx, iaddr // 
				mov temp.Skill_Name, ecx
				sub ecx, 6
				mov eax, [ecx]
				mov temp.Skill_ID, ax
		}
		i++;
		vec.push_back(temp);
	}
}

HUMAN Get_Human_State(DWORD role_base)//遍历人物属性
{
	HUMAN temp;
	temp.Object = Get_Object(role_base);
	_asm
	{
		pushad
		mov ecx, role_base
		mov ecx, [ecx] 	 
		add ecx, 0xe4 
		mov eax, [ecx]
		mov temp.Job, eax
		add ecx, 8 // 
		mov eax, [ecx]
		mov temp.Level, eax
		sub ecx, 0x10 // 
		mov eax, [ecx]
		mov temp.Sex, eax

		add ecx, 0x980 
		mov eax, [ecx]
		mov temp.GJL_L, eax
		add ecx, 4 // 
		mov eax, [ecx]
		mov temp.GJL_H, eax
		add ecx, 4 // 
		mov eax, [ecx]
		mov temp.MFL_L, eax
		add ecx, 4 // 
		mov eax, [ecx]
		mov temp.MFL_H, eax
		add ecx, 4 // 
		mov eax, [ecx]
		mov temp.DSL_L, eax
		add ecx, 4 // 
		mov eax, [ecx]
		mov temp.DSL_H, eax
		add ecx, 0xc //
		mov eax, [ecx]
		mov temp.FY_L, eax
		add ecx, 4 // 
		mov eax, [ecx]
		mov temp.FY_H, eax
		add ecx, 4 // 
		mov eax, [ecx]
		mov temp.MF_L, eax
		add ecx, 4 // 
		mov eax, [ecx]
		mov temp.MF_H, eax

		add ecx, 0x20 // 
		mov eax, [ecx]
		mov temp.BAG_W, ax
		add ecx, 2 //
		mov eax, [ecx]
		mov temp.BAG_W_MAX, ax

		add ecx, 0x26 // 
		mov eax, [ecx]
		mov temp.SW, eax
		add ecx, 0x178 // 
		mov eax, [ecx]
		mov temp.p_ZB, eax
		add ecx, 0xc // 
		mov eax, [ecx]
		mov temp.p_Bag_Base, eax
		add ecx, 4 // 
		mov eax, [ecx]
		mov temp.Bag_Size, eax
		add ecx, 8 // 
		mov eax, [ecx]
		mov temp.p_LR_Bag_Base, eax
		add ecx, 4 // 
		mov eax, [ecx]
		mov temp.LR_Bag_Size, eax
		sub ecx, 0x80c // 
		mov temp.p_Target_ID, ecx
		add ecx, 0xf00 // 
		mov eax, [ecx]
		mov temp.p_Skill_Base, eax
		add ecx, 0x828670
		mov temp.p_Current_Map, ecx;
		popad
	}
	return temp;
}//end Get

void Get_Bag_ZB(GOODS* b_goods/*物品结构体数组*/,DWORD b_Base/*背包格子指针/身上装备基址*/, 
	DWORD b_Size/*背包大小/装备数量*/)//遍历格子物品属性,身上装备
{
	if((b_Base)&&(b_Size))return;
	GOODS temp = { 0 };
	int iaddr = 0;
	for (unsigned i = 0; i < b_Size; i++) // 
	{
		 iaddr = i * 0x688 + 1;
		 if (!(*(DWORD*)(b_Base+iaddr))) //判断是否有物品
		 {
			 b_goods[i] = { 0 };
			 continue;
		 }
		 _asm{
			    mov ecx, b_Base;
				add ecx, iaddr // 
				mov temp.Goods_Name, ecx
				add ecx, 0x19
				mov eax, [ecx]
				mov temp.Goods_WD_low, al
				add ecx, 1
				mov eax, [ecx]
				mov temp.Goods_WD_high, al
				add ecx, 1
				mov eax, [ecx]
				mov temp.Goods_MD_low, al
				add ecx, 1
				mov eax, [ecx]
				mov temp.Goods_MD_high, al
				add ecx, 1
				mov eax, [ecx]
				mov temp.Goods_PA_low, al
				add ecx, 1
				mov eax, [ecx]
				mov temp.Goods_PA_high, al
				add ecx, 1
				mov eax, [ecx]
				mov temp.Goods_Magic_low, al
				add ecx, 1
				mov eax, [ecx]
				mov temp.Goods_Magic_high, al
				add ecx, 1
				mov eax, [ecx]
				mov temp.Goods_Tao_low, al
				add ecx, 1
				mov eax, [ecx]
				mov temp.Goods_Tao_high, al
				add ecx, 1
				mov eax, [ecx]
				mov temp.Goods_Need_what, al
				add ecx, 1
				mov eax, [ecx]
				mov temp.Goods_Need_Num, al
				add ecx, 7
				mov eax, [ecx]
				mov temp.Goods_ID, eax
				add ecx, 4
				mov eax, [ecx]
				mov temp.Goods_Use_Num, ax
				add ecx, 2
				mov eax, [ecx]
				mov temp.Goods_Use_Num_Max, ax
		}
		 b_goods[i] = temp;
	}
}//end GetGoodsIndex;


void Get_Envionment(DWORD x/*人物坐标x*/, DWORD y/*坐标Y*/,vector<DWORD> &vec/*存储对象容器*/, 
					DWORD get_offset = Envi_Offset/*遍历偏移*/, DWORD g_range = 15/*遍历范围*/)/*遍历角色周围NPC 怪物，地面*/
{
	DWORD ID_temp;
	for (int i=x- g_range;i<x+ g_range;i++)
	{
		if (i <= 0)i = 1;
		for (int j = y - g_range; j<y + g_range; j++)
		{
			if (j <= 0)j = 1;
			_asm
			{
				pushad
				mov ecx, RoleBase
				mov ecx,[ecx]
				add ecx, 0x1D38
				mov edi, i
				mov esi, j
				mov eax, 0x66666667
				imul edi
				sar edx, 0x4
				mov eax, edx
				shr eax, 0x1F
				add eax, edx
				lea eax, dword ptr ds : [eax + eax * 4]
				add eax, eax
				add eax, eax
				add eax, eax
				mov ebx, eax
				mov eax, 0x99999999
				imul esi
				sar edx, 0x4
				mov eax, edx
				shr eax, 0x1F
				add eax, edx
				sub eax, ebx
				add eax, edi
				lea edx, dword ptr ds : [eax + eax * 4]
				lea eax, dword ptr ds : [esi + edx * 8]
				imul eax, eax, 0xA70
				add ecx, get_offset
				mov eax, dword ptr ds : [eax + ecx]
				mov ID_temp, eax
				popad				
			}
			__try
			{
				DWORD ptry = 0;
			    if(ID_temp)ptry = *(DWORD*)ID_temp;//对非法地址赋值，会造成Access Violation 异常
				if (ptry&& (get_offset == Ground_Offset)) vec.push_back(ID_temp);/*地面*/
				else if (*(DWORD*)(ID_temp + 0x80) && ptry && (get_offset == Envi_Offset)) vec.push_back(ID_temp);/*周围其他对象*/
		
			}
			__except (EXCEPTION_EXECUTE_HANDLER)
			{
			}			
		}
	}

}


//int use_goods_index = -1;

//int Get_Goods_num(const char* name) //获得背包指定物品数量,最后一个物品下标赋值到use_goods_index;
//{
	//Get_Bag();
//	int goods_number = 0;
//	use_goods_index = -1;
//	for (int i = 0; i < 125; i++)
//	{
//		if (memcmp(name, baggoods[i].name, strlen(name)) == 0)
//		{
//			goods_number = goods_number + 1;
//			use_goods_index = i;
//		}
//	}
//	return goods_number;
//}

//int Find_Goods_index(const char* name) //获得背包指定物品数量,最后一个物品下标赋值到use_goods_index;
//{
//	Get_Bag();
//	for (int i = 0; i < 125; i++)
//	{
//		if (memcmp(name, baggoods[i].name, strlen(name)) == 0)return i;
//	}
//	return -1;
//}

std::vector<string> ReadTxt(const CHAR* file_Path/*文件路径*/)/*按行读取，返回vector*/
{
	fstream f(file_Path);
	vector<string> temp;
	string line;
	if (!f.is_open())
	{
		CString s = "文件打开失败！";
		AfxMessageBox(s, MB_OK);
	}
	while (!f.eof())//会自动把换行符去掉
	{		
		std::getline(f, line);
		temp.push_back(line);
	}
	f.close();
	return temp;
}

void usebaggoods(int bag_index) //包裹格子下标
{
	_asm
	{
		push 0x1
		push 0x0
		push 0x0
		push bag_index  //背包格子下标从0开始，快捷栏1-6为60-65  有VIP包裹 60-119 快捷栏120-125                      
		mov ecx, dword ptr ds : [0x112F580]
		mov eax, 0x6D7F70
		call eax
	}
}

VOID CALLBACK 自动治疗(HWND hWnd, UINT nMsg, UINT_PTR nIDEvent, DWORD dwTime)
{
	//if (Get_Goods_num("治疗神水") > 1)
	//{
	//	usebaggoods(use_goods_index);
	//}
}
void GoTo_Current_Map(DWORD x,DWORD y)/*当前地图寻路*/
{
	_asm {
		pushad
		mov ecx, dword ptr ds : [0x4E072C4]
		push 0x0
		push y
		push x
		mov eax, GOTOBASE
		call eax
		popad
	}

}
void UseSkill_To(DWORD skill_ID/*技能ID*/,DWORD p_Target/*目标指针*/) /*对对象使用技能*/
{
	DWORD x = *(DWORD*)(p_Target + 0xac);
	DWORD y = *(DWORD*)(p_Target + 0xb0);
	DWORD t_ID = *(DWORD*)(p_Target + 8);
	_asm
	{
		pushad
		mov edx, edx
		mov eax, SKILLBASE
		mov ecx, dword ptr ds : [CALLBASE]
		push skill_ID
		push y
		push x
		push t_ID
		call eax
		popad
	}

}
BOOL If_Has_Skill(const CHAR* name/*技能名字*/)/*判断是否有某个技能*/
{
	for (unsigned i=0;i<role_Skill.size();i++)
	{
		if(!memcmp(name,(CHAR*)(role_Skill[i].Skill_Name), strlen(name)))return TRUE;
	}
    return FALSE;
}
BOOL If_Has_Skill(DWORD s_ID/*技能ID*/)/*判断是否有某个技能*/
{
	for (unsigned i = 0; i <role_Skill.size(); i++)
	{
		if (s_ID == (DWORD)(role_Skill[i].Skill_ID))return TRUE;
	}
	return FALSE;
}
unsigned Cacl_Distance(HUMAN role, DWORD t_x, DWORD t_y)/*计算角色与某坐标的距离*/
{
	int x, y;
	unsigned a, b;
	x = role.Object.XY.X - t_x;
	if (x<0)x = 0 - x;
	y = role.Object.XY.Y - t_y;
	if (y < 0)y = 0 - y;
	a = (unsigned)x;
	b = (unsigned)y;
	return (a > b) ? a : b;       

}
DWORD Find_T_Monster/*找到要攻击的怪物*/(HUMAN role/*角色结构体*/, vector<CHAR*>& vec/*攻击怪物列表*/)/*返回找到的怪物对象指针*/
{
	vector<DWORD> near_Mon_3, near_Mon_7;
	Get_Envionment(role.Object.XY.X, role.Object.XY.Y, near_Mon_3, Envi_Offset, 3); /*找到3格已内的怪物*/
	if (near_Mon_3.size())
	{
		for (unsigned i = 0; i < near_Mon_3.size(); i++)
		{
			for (unsigned j = 0; j < vec.size(); j++)
			{
				if (strcmp((CHAR*)(near_Mon_3[i]+0x10), vec[j])==0) /*strcmp返回0代表不相等*/
					return near_Mon_3[i];
			}
		}
	}
	Get_Envionment(role.Object.XY.X, role.Object.XY.Y, near_Mon_7, Envi_Offset, 7); /*找到7格已内的怪物*/
	if (near_Mon_7.size())
	{
		for (unsigned i = 0; i < near_Mon_7.size(); i++)
		{
			for (unsigned j = 0; j < vec.size(); j++)
			{
				if (strcmp((CHAR*)(near_Mon_7[i] + 0x10), vec[j]) == 0) /*strcmp返回0代表不相等*/
					return near_Mon_7[i];
			}
		}
	}
	return 0;
}
DWORD Choose_Moster(HUMAN role/*角色结构体*/, vector<CHAR*>& vec/*攻击怪物列表*/)/*返回选中怪物对象指针*/
{
	DWORD Object_ID = *(DWORD*)(role.p_Target_ID);
	while (Object_ID==0)
	{
		Object_ID = Find_T_Monster(role, vec);
		if (Object_ID)
		{
			*(DWORD*)(role.p_Target_ID) = *(DWORD*)(Object_ID + 0x8);
			m_pThread_Goto->SuspendThread();
		}
		else
		{
			/*边寻路边找怪*/
			m_pThread_Goto->ResumeThread();	
			Sleep(500);
		}
	}
	return Object_ID;
}

BOOL Auto_Attack/*自动打怪*/(HUMAN role/*角色结构体*/, vector<CHAR*>& vec/*攻击怪物列表*/, DWORD s_ID/*使用技能ID*/)
{
	*(DWORD*)role.p_Target_ID = 0;
	DWORD p_Target/*对象指针*/ =  Choose_Moster(role, vec);
	unsigned i = 0;/*记录攻击次数*/
	while((*(DWORD*)(p_Target + 0x80)>0)&&(*(DWORD*)(p_Target+0x8)==*(DWORD*)role.p_Target_ID)&&(i<600))
	{
	
		if (Cacl_Distance(role, *(DWORD*)(p_Target + 0xac), *(DWORD*)(p_Target + 0xb0)) > 9)return FALSE;
		UseSkill_To(s_ID, p_Target);
		i++;
		Sleep(ATTACK_SLEEP);
	}
	return true;
}
DWORD Load_Settings(HUMAN role/*角色结构体*/) /*返回使用技能ID*/
{
	/*技能设置*/
	DWORD s_ID = 0;
	if (If_Has_Skill("狂龙紫电"))s_ID = 0x47;
	else if (If_Has_Skill("雷电术"))s_ID = 11;
	else if (If_Has_Skill("冰箭术"))s_ID = 49;
	else if (If_Has_Skill("小火球"))s_ID = 1;
	else s_ID = 0;
	return s_ID;
}

vector<MAP_XY> split_XY(vector<string>& vec)/*分割坐标字符串转化位坐标结构体*/
{
	vector<MAP_XY> rvec;
	MAP_XY xy_temp;
	for (unsigned i=0;i<vec.size();i++)
	{
		std::regex delimiters("\\s+");
		vector<string> temp(sregex_token_iterator(vec[i].begin(),vec[i].end(),delimiters,-1), sregex_token_iterator());

		xy_temp.X = stoi(temp[0]);
		xy_temp.Y = stoi(temp[1]);
		rvec.push_back(xy_temp);
	}
	return rvec;
}

void 使用物品(const char* name) //物品名字
{
	//int index = Find_Goods_index(name);
	//usebaggoods(index);

}

void 点击对话框(const char* dlg_name) //对话内容
{
	_asm
	{
		mov edx, dword ptr ds : [0x00EED594]
		mov ecx, dword ptr ds : [0x0112F580]
		push 0xFFFF
		push 0xFFFF
		push 0x0
		push 0x0
		push dlg_name         //存储对话框命令地址 @获取神御    
		push edx
		mov eax, 0x670B70
		call  eax
	}
}

void 自动回收()
{
	const char* dlg_cmd_shenyu = "@获取神御"; //神於
	const char* dlg_cmd_sanbei = "@获得三倍经验"; //三倍
	const char* dlg_cmd_yijian = "@一件回收路"; //一键回收
	const char* dlg_cmd_jiangjun = "@旭日.一键&将军系列&1&灵力"; //一键回收将军
	const char* dlg_cmd_nimo = "@旭日.一键&逆魔系列&5&灵力"; //一键回收逆魔
	const char* dlg_cmd_tiexue = "@旭日.一键&铁血技能系列&4&灵力"; //一键回收铁血
	使用物品("赞助包天卡");
	Sleep(2000);
	点击对话框(dlg_cmd_shenyu);
	Sleep(500);
	点击对话框(dlg_cmd_sanbei);
	Sleep(500);
	点击对话框(dlg_cmd_yijian);
	Sleep(500);;
	点击对话框(dlg_cmd_jiangjun);
	Sleep(500);;
	点击对话框(dlg_cmd_nimo);
	Sleep(500);;
	点击对话框(dlg_cmd_tiexue);
}





BOOL CWGForm::OnInitDialog()
{
	/*========初始化人物，元神状态技能============*/
	CDialogEx::OnInitDialog();
	tflag = true;
	tflag_auto = true;
	tflag_goto = true;

	m_Monster_Listctrl.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES); // 整行选择、网格线
	m_Monster_Listctrl.InsertColumn(0, _T(""), LVCFMT_LEFT, 0);
	m_Monster_Listctrl.InsertColumn(1, _T("序号"), LVCFMT_LEFT, 100); // 插入第2列的列名
	m_Monster_Listctrl.InsertColumn(2, _T("名字"), LVCFMT_LEFT, 100); // 插入第3列的列名
	m_Monster_Listctrl.InsertColumn(3, _T("指针"), LVCFMT_LEFT, 100); // 插入第4列的列名

	role1 = Get_Human_State(RoleBase);
	role_YS = Get_Human_State(YS_Base);
	Get_Skill(role1.p_Skill_Base, role_Skill);
	Get_Skill(role_YS.p_Skill_Base, YS_Skill);
	/*=============启动线程3s遍历一次人物状态和环境地面==================*/
	m_pThread_Environment = AfxBeginThread(ThreadProc_Environmrnt, this);
	//m_pThread_Environment = AfxBeginThread(ThreadProc_Mapping, this);
	//m_pThread_Environment->ResumeThread();
	// TODO:  在此添加额外的初始化

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

UINT __cdecl CWGForm::ThreadProc_Mapping(LPVOID nParam) /*进程通信*/
{
	//dwType = 0;
	//g_hMapFile = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, _MAP_);	//打开共享内存
	//if (g_hMapFile == NULL)
	//{
	//	AfxMessageBox("OpenFileMapping Error: %d\n",MB_OK);
	//	return -1;
	//}	
	//lpBuff = (LPTSTR)MapViewOfFile(g_hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, BUFSIZ);//映射内存
	//if (lpBuff == NULL)
	//{
	//	AfxMessageBox("OpenFileMapping Error: %d\n", MB_OK);
	//	return -1;
	//}
	//while (tflag_map)
	//{
	//	CHAR* lpContent = "Test!!!";
	//	//dwType = 1234;	
	//	//strcpy((char*)lpBuff, lpContent);
	//	CopyMemory(lpBuff, &hwnd_count,sizeof(hwnd_count));
	//	Sleep(1000);
	//}
	//return 0;   // thread completed successfully
}
UINT __cdecl CWGForm::ThreadProc_Goto(LPVOID nParam)
{
	vector<MAP_XY> *xy = (vector<MAP_XY>*)nParam;
	auto it = xy->begin();
	while (tflag_goto&&(it!=xy->end()))
	{

		while (Cacl_Distance(role1, it->X, it->Y) > 3)
		{
			GoTo_Current_Map(it->X, it->Y);
			Sleep(500);
		}
		++it;
		if (it== xy->end())it= xy->begin();
	}
	return 0;   // thread completed successfully
}
UINT __cdecl CWGForm::ThreadProc_AutoAttack(LPVOID nParam) //自动打怪
{
	CWGForm* threadol = (CWGForm*)nParam;
	vector<CHAR*> attack_monster;
	/*========读取打怪列表=============================================================*/
	vector<string> vec_t = ReadTxt("G:\\VS_Projects\\GameAuto\\Release\\AttackMonster.txt");
	for (unsigned i = 0; i < vec_t.size(); i++)
	{
		attack_monster.push_back((CHAR*)vec_t[i].data());
	}
	/*========读取当前地图寻路点坐标=============================================================*/
	vector<string> vec_xy = ReadTxt("G:\\VS_Projects\\GameAuto\\Release\\逆魔大殿.txt");
	vector<MAP_XY>map_Auto_Goto = split_XY(vec_xy);
    /*=================设置打怪技能=========================================================*/
	DWORD Skill_ID=Load_Settings(role1);

	while (tflag_goto)
	{
		g_criticalsection.Lock();  //边界锁锁定 边界锁中间添加需要保护的代码，如读写文件的能

		g_criticalsection.Unlock(); //边界锁解除

		Auto_Attack(role1, attack_monster,Skill_ID);

	}
	return 0;   // thread completed successfully
}
UINT __cdecl CWGForm::ThreadProc_Environmrnt(LPVOID nParam) //遍历周围怪物NPC
{

	CWGForm* threadol = (CWGForm*)nParam;
	while (tflag)
	{

		role1 = Get_Human_State(RoleBase);
		role_YS = Get_Human_State(YS_Base);	
		Get_Envionment(role1.Object.XY.X, role1.Object.XY.Y, p_Moster, Envi_Offset);//遍历NPC怪物
		Get_Envionment(role1.Object.XY.X, role1.Object.XY.Y, p_Ground_Goods, Ground_Offset);//遍历地面
		Get_Bag_ZB(role_Bag, role1.p_Bag_Base, role1.Bag_Size); //包裹
		Get_Bag_ZB(ls_Bag, role1.p_LR_Bag_Base, role1.LR_Bag_Size);//灵兽包裹
		Get_Bag_ZB(role_ZB, role1.p_ZB, 19);//主体装备	
		CString strName, str指针, strNum,st;
		g_criticalsection.Lock();  //边界锁锁定 边界锁中间添加需要保护的代码，如读写文件的能
		g_criticalsection.Unlock(); //边界锁解除
		threadol->m_Monster_Listctrl.DeleteAllItems();
		for (size_t i = 0; i < p_Moster.size(); i++)
		{
			strNum.Format(_T("%d"), i + 1);
			str指针.Format(_T("%x"), p_Moster[i]);
			strName.Format(_T("%s"), (CHAR*)(p_Moster[i] + 0x10));
			threadol->m_Monster_Listctrl.InsertItem(i, _T("")); // 插入行
			threadol->m_Monster_Listctrl.SetItemText(i, 1, strNum); // 设置第2列(序号)
			threadol->m_Monster_Listctrl.SetItemText(i, 2, strName); // 设置第3列(名字)
			threadol->m_Monster_Listctrl.SetItemText(i, 3, str指针); // 设置第4列(坐标)
			strName = str指针 = strNum = "";
		}	
		Sleep(3000);
		p_Moster.clear();
		p_Ground_Goods.clear();
	}
	return 0;   // thread completed successfully
}

void CWGForm::OnDestroy()
{
	CDialogEx::OnDestroy();
	// TODO: 在此处添加消息处理程序代码
	tflag = FALSE;/*遍历*/
	tflag_goto = FALSE;/*寻路*/
	tflag_auto = FALSE;/*打怪*/
	WaitForSingleObject(m_pThread_Environment, 100);
	WaitForSingleObject(m_pThread_Goto, 100);
	WaitForSingleObject(m_pThread_AutoAttack, 100);

	if (m_pThread_Goto->m_hThread)
	{
		TerminateThread(m_pThread_Goto->m_hThread, EXIT_FAILURE);
		CloseHandle(m_pThread_Goto->m_hThread);
	}

	if (m_pThread_Environment->m_hThread)
	{
		TerminateThread(m_pThread_Environment->m_hThread, EXIT_FAILURE);
		CloseHandle(m_pThread_Environment->m_hThread);
	}
	if (m_pThread_AutoAttack->m_hThread)
	{
		TerminateThread(m_pThread_AutoAttack->m_hThread, EXIT_FAILURE);
		CloseHandle(m_pThread_AutoAttack->m_hThread);
	}

	//g_criticalsection.Lock();
	////file.Close();
	//g_criticalsection.Unlock();
}

void CWGForm::OnBnClickedBtnCalltest()
{
	// TODO: 在此添加控件通知处理程序代码
	//SetTimer(ID_TIMER_ZHILIAO, ZHILIAO_ELAPSE, &自动治疗);
	//AfxMessageBox(str坐标, MB_OK);
	//SetTimer(ID_TIMER_HUISHOU, HUISHOU_ELAPSE, &自动回收);
	CString s, st;
	/*=================启动打怪寻路线程=========================================================*/
	//m_pThread_AutoAttack = AfxBeginThread(ThreadProc_AutoAttack, this, THREAD_PRIORITY_NORMAL, 0, 0);

	///*========读取当前地图寻路点坐标=============================================================*/
	//vector<string> vec_t = ReadTxt("G:\\VS_Projects\\GameAuto\\Release\\逆魔大殿.txt");
	//vector<MAP_XY> map_Auto_Goto = split_XY(vec_t);

	///*=================启动寻路线程=========================================================*/
	//m_pThread_Goto = AfxBeginThread(ThreadProc_Goto, (LPVOID)(&map_Auto_Goto), THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);


	//for (unsigned i = 0; i < p_Moster.size(); i++)
	//{
	//	st.Format(_T("%s\n"), (CHAR*)(p_Moster[i] + 0x10));
	//	s = st + s;
	//}
	//st.Format(_T("%d,%d\n"), role1.Object.XY.X, role1.Object.XY.Y);


}	




CHAR t = 1;
void CWGForm::OnBnClickedBtnPause()
{
	// TODO: 在此添加控件通知处理程序代码
	if (t == 1)
	{
		m_pThread_Environment->SuspendThread();
		t = 0;
		GetDlgItem(IDC_BTN_PAUSE)->SetWindowText("继续");
	}
	else
	{
		m_pThread_Environment->ResumeThread();
		t = 1;
		GetDlgItem(IDC_BTN_PAUSE)->SetWindowText("暂停");
	}
	//KillTimer(ID_TIMER_ZHILIAO);

}
void CWGForm::OnBnClickedButtonStop()
{
	// TODO: 在此添加控件通知处理程序代码
//KillTimer(ID_TIMER_ZHILIAO);
//KillTimer(ID_TIMER_HUISHOU);
//Resum寄存器((LPVOID)H_Monster_NPC_Address, HOOK_Monster_NPC, 6, 钩子空间);
}


void CWGForm::MessageBox(DWORD dw)
{
	CString s;
	s.Format("%d", dw);
	AfxMessageBox(s, MB_OK);
}



