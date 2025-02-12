//#include "stdafx.h"
#include "gamecall.h"




/*
��������:ʹ�ñ�����Ʒ
����һ:��Ʒ�ڱ�����λ��(�±�)
*/
bool gamecall::useGoods(int goodsIndex)
{
	try
	{
		_asm
		{
			pushad
			push 1
			push 0
			push 0
			push goodsIndex   //���������±��0��ʼ�������1-6Ϊ60-65  ��VIP���� 60-119 �����120-125  
			mov ecx, dword ptr ds : [CALL_ECX]
			mov edx, CALL_USEGOODS
			call edx
			popad
		}
	}
	catch (...)
	{
		return false;
	}
	return true;
}

/*
��������:��Ŀ��(ָ������)ʹ�ü���
����һ:����ID
������:����x
������:����y
������:Ŀ��ID (����ȱʡΪ0)
*/
bool gamecall::useSkillTo(DWORD skillId, DWORD x, DWORD y, DWORD targetId)
{
	try
	{
		_asm
		{
			pushad
			push skillId
			push y
			push x
			push targetId
			mov ecx, dword ptr ds : [CALL_ECX]
			mov edx, CALL_USESKILL
			call edx
			popad
		}
	}
	catch (...)
	{
		return false;
	}
	return true;
}


/*
��������:Ѱ·����ǰ��ͼָ������4������
����һ:����x
������:����y
*/
bool gamecall::CurrentMapMove(DWORD x, DWORD y)
{
	try
	{
		_asm
		{
			pushad
			push 0
			push y
			push x
			mov ecx, dword ptr ds : [CALL_ECX_1]
			mov edx, CALL_CURRENT_MAPMOVE
			call edx
			popad
		}
	}
	catch (...)
	{
		return false;
	}
	return true;
}

/*
��������:�ߵ�ָ�����꣬ÿ����һ����Ҫ��������
����һ:����x
������:����y
�����������ܱ�ǣ�1Ϊ�ߣ�2Ϊ��
*/
bool gamecall::Run_or_Step_To(DWORD x, DWORD y, DWORD run_step_flag)
{
	DWORD call_addr;
	if (run_step_flag == 1)call_addr = CALL_STEP_TO_MAP;
	else if (run_step_flag == 2)call_addr = CALL_RUN_TO_MAP;
	else return false;
	try
	{
		_asm
		{
			pushad
			push y
			push x
			mov ecx, dword ptr ds : [CALL_ECX]
			mov edx, call_addr
			call edx
			popad
		}
	}
	catch (...)
	{
		return false;
	}
	return true;
}

/*
��������:��NPC
����һ:NPC��ID
*/
bool gamecall::OpendNPC(DWORD npcID)
{
	try
	{
		_asm
		{
			pushad
			push npcID
			mov edx, CALL_OPENNPC
			call edx
			popad
		}
	}
	catch (...)
	{
		return false;
	}
	return true;
}

/*
��������:ѡ��Ի�
����һ:�Ի�����
*/
bool gamecall::ChooseCmd(const char* command)
{
	try
	{
		_asm
		{
			pushad
			push 0xffff
			push 0xffff
			push 0
			push 0
			push command
			mov ecx, dword ptr ds : [CALL_ECX]
			mov edx, dword ptr ds : [CALL_EDX]
			push edx
			mov edi, CALL_CHOOSE_CMD
			call edi
			popad
		}
	}
	catch (...)
	{
		return false;
	}
	return true;
}

/*
��������:ʰȡ������Ʒ
����һ:����x
������:����y
*/
bool gamecall::pickupGoods(DWORD x, DWORD y)
{
	try
	{
		_asm
		{
			pushad
			push y
			push x
			mov ecx, dword ptr ds : [CALL_ECX]
			mov edx, CALL_PICKUP_GOODS
			call edx
			popad
		}
	}
	catch (...)
	{
		return false;
	}
	return true;
}

/*
��������:��¼��Ϸ
����һ:�û���
������:����
*/
bool gamecall::loginGame(const char* user, const char* pass)
{
	try
	{
		_asm
		{
			pushad
			push pass
			push user
			push ID_LONGIN
			mov ecx, dword ptr ds : [CALL_ECX]
			mov edx, CALL_LOGIN
			call edx
			popad
		}
	}
	catch (...)
	{
		return false;
	}
	return true;
}


/*
��������:ģ�ⰴ�� ��֧�ֻس���,������
����һ:������
*/
bool gamecall::presskey(int vkcode)
{
	int hcode = ::MapVirtualKey(vkcode, MAPVK_VK_TO_VSC);//ȡӲ��ɨ����
	try
	{
		_asm
		{
			pushad
			push hcode
			push vkcode
			mov ecx, dword ptr ds : [CALL_ECX_2]
			mov edx, CALL_PRESSKEY
			call edx
			popad
		}
	}
	catch (...)
	{
		return false;
	}
	return true;
}

/*
��������:���ݽ���pid��ȡ���ھ��
����һ:����PID
*/
HWND gamecall::GetHwndByPid(DWORD dwProcessID)
{
	HWND h = GetTopWindow(0);
	HWND retHwnd = NULL;
	while (h)
	{
		DWORD pid = 0;
		DWORD dwTheardId = GetWindowThreadProcessId(h, &pid);
		if (dwTheardId != 0)
		{
			if (pid == dwProcessID && GetParent(h) == NULL && ::IsWindowVisible(h))
			{
				retHwnd = h;    //���ж�����ֵ
			}
		}
		h = GetNextWindow(h, GW_HWNDNEXT);
	}
	return retHwnd;
}

/*
��������:postMessageģ�ⰴ�� ֧�ֻس��� ��HWND���������ӿؼ�����ָ��������Ϣ
����һ:����PID
����һ:������ ȱʡΪ�س�
*/
bool gamecall::presskey(DWORD pid, int vkcode)
{
	int hcode = ::MapVirtualKey(vkcode, MAPVK_VK_TO_VSC);//ȡӲ��ɨ����
	DWORD dwProcId;
	::GetWindowThreadProcessId(GetHwndByPid(pid), &dwProcId);
	HWND hWnd = GetForegroundWindow();
	//char buf[1000];
	while (hWnd)
	{
		DWORD ProcId;
		GetWindowThreadProcessId(hWnd, &ProcId);
		if (ProcId == dwProcId)
		{
			::PostMessage(hWnd, WM_KEYDOWN, vkcode, hcode);
			Sleep(10);
			::PostMessage(hWnd, WM_KEYUP, vkcode, hcode);
		}
		hWnd = GetNextWindow(hWnd, GW_HWNDNEXT);
	}
	return true;
}

/*
��������:��������(x1,y1)������(x2,y2)�ľ���
����һ:x1
����һ:y1
��������x2
�����ģ�y2
����ֵ������
*/
float gamecall::caclDistance(DWORD x1, DWORD y1, DWORD x2, DWORD y2)
{
	int x, y;
	x = x1 - x2;
	y = y1 - y2;
	return (float)sqrt(abs(x * x) + abs(y * y));
}

///*
//��������:�ҵ�Ҫ�����Ĺ���
//����һ:��ɫ�ṹ��
//������:���������б�
//����ֵ�������ҵ��Ĺ������ָ��
//*/
//DWORD gamecall::Find_T_Monster(role& r, std:: vector<std::string>& vec)
//{
//	std::vector<DWORD>  near_Mon_7;
//	r.Get_Envionment(near_Mon_7, 7); /*�ҵ�9�����ڵĹ���*/
//	if (near_Mon_7.size())
//	{
//		for (unsigned i = 0; i < near_Mon_7.size(); i++)
//		{
//			for (unsigned j = 0; j < vec.size(); j++)
//			{
//				if (strcmp((CHAR*)(near_Mon_7[i] + 0x10), vec[j].c_str()) == 0) /*strcmp����0�������*/
//					return near_Mon_7[i];
//			}
//		}
//	}
//	return 0;
//}


/*
��������:�������sort������
����һ:�������
������:�������
����ֵ��bool
*/
bool gamecall::comp(const MONSTER_PROPERTY& a, const MONSTER_PROPERTY& b)
{
	if (a.Distance < b.Distance) {
		return true;
	}
	else if (a.Distance == b.Distance && a.ID < b.ID) {
		return true;
	}
	else {
		return false;
	}
}
/*
��������:ˢ����Χ���ﰴ��������
����һ:��ɫ�ṹ��
������:���������б�
������:������ΧĬ��15
����ֵ�����ذ����������15�����ڵĹ�����������б�
*/
std::vector<MONSTER_PROPERTY> gamecall::sort_aroud_monster(role& r, std::vector<std::string>& vec, DWORD e_range)
{
	std::vector<MONSTER_PROPERTY> ret;
	std::vector<DWORD>  near_Mon,npcs,players,pets;
	r.Get_Envionment(pets,npcs,near_Mon,players,e_range); /*�ҵ�7�����ڵĹ���*/
	if (near_Mon.size())
	{
		for (unsigned i = 0; i < near_Mon.size(); i++)
		{
			for (unsigned j = 0; j < vec.size(); j++)
			{
				if (strcmp((CHAR*)(near_Mon[i] + 0x10), vec[j].c_str()) == 0) /*strcmp����0�������*/
				{
					MONSTER_PROPERTY temp((DWORD*)near_Mon[i]);
					temp.Distance = caclDistance(*r.m_roleproperty.Object.X, *r.m_roleproperty.Object.Y, *temp.X, *temp.Y);
					ret.push_back(temp);
				}
			}
		}
	}
	std::sort(ret.begin(), ret.end(), &comp);
	return ret;
}


/*
��������:������Ʒ����sort������
����һ:�������
������:�������
����ֵ��bool
*/
bool gamecall::comp_groud(const GROUND_GOODS& a, const GROUND_GOODS& b)
{
	if (a.Distance < b.Distance) {
		return true;
	}
	else {
		return false;
	}
}
/*
��������:ˢ�µ�����Ʒ����������
����һ:��ɫ�ṹ��
������:ʰȡ��Ʒ�б�
����ֵ�����ذ����������15�����ڵ���Ҫʰȡ�ĵ�����Ʒ�б�
*/
std::vector<GROUND_GOODS> gamecall::sort_groud_goods(role& r, std::vector<std::string>& vec)
{
	std::vector<GROUND_GOODS> ret;
	std::vector<DWORD>  near_groud;
	r.Get_Ground(near_groud, 15);
	if (near_groud.size())
	{
		for (unsigned i = 0; i < near_groud.size(); i++)
		{
			for (unsigned j = 0; j < vec.size(); j++)
			{
				if (strcmp((char*)(near_groud[i] + 0x18), vec[j].c_str()) == 0) /*strcmp����0�������*/
				{
					GROUND_GOODS temp((DWORD*)near_groud[i]);
					temp.Distance = caclDistance(*r.m_roleproperty.Object.X, *r.m_roleproperty.Object.Y, *temp.X, *temp.Y);
					ret.push_back(temp);
					break;
				}
			}
		}
	}
	if (ret.size() > 1)
	{
		std::sort(ret.begin(), ret.end(), &comp_groud);
	}

	return ret;
}

/*
��������:�ָ������ַ���
����һ:�����ַ���
����һ:���ص�����ָ��
����ֵ����������ָ��,���x,y��Ϊ-1 ��ָ����
*/
MapXY gamecall::splitXY(std::string str)
{
	MapXY XYtemp;
	std::regex delimiters("\\s+");
	std::vector<std::string> temp(std::sregex_token_iterator(str.begin(), str.end(), delimiters, -1), std::sregex_token_iterator());
	if (temp.size() != 2)
	{
		XYtemp.x = -1;
		XYtemp.y = -1;
	}
	else
	{
		XYtemp.x = stoi(temp[0]); //x
		XYtemp.y = stoi(temp[1]); //y
	}
	return XYtemp;
}
/*
��������:�����������
����һ:���ر�־ 1�� 0��
����ֵ��bool
*/
bool gamecall::team_open_close(DWORD tflag)
{
	try
	{
		_asm
		{
			pushad
			push tflag
			mov ecx, dword ptr ds : [CALL_ECX]
			mov edx, CALL_TEAM_OPEN_CLOSE
			call edx
			popad
		}
	}
	catch (...)
	{
		return false;
	}
	return true;
}


/*
��������:�����������
����һ:��Ա����
����ֵ��bool
*/
bool gamecall::maketeam(std::string pName)
{
	const char* cname = pName.c_str();
	try
	{
		_asm
		{
			pushad
			push cname
			mov ecx, dword ptr ds : [CALL_ECX]
			mov edx, CALL_MAKE_TEAM
			call edx
			popad
		}
	}
	catch (...)
	{
		return false;
	}
	return true;
}

/*
��������:ͬ���������
����һ:�ӳ�����
����ֵ��bool
*/
bool gamecall::allowteam(std::string pName)
{
	const char* cname = pName.c_str();
	try
	{
		_asm
		{
			pushad
			push 1
			push cname
			mov ecx, dword ptr ds : [CALL_ECX]
			mov edx, CALL_ALLOW_TEAM
			call edx
			popad
		}
	}
	catch (...)
	{
		return false;
	}
	return true;
}

/*
��������:�ͷ�Ԫ��
����һ:0
����ֵ��bool
*/
bool  gamecall::release_Promenade(void)
{
	try
	{
		_asm
		{
			pushad
			mov ecx, dword ptr ds : [CALL_ECX]
			mov edx, CALL_RELEASE_PROMENADE
			call edx
			popad
		}
	}
	catch (...)
	{
		return false;
	}
	return true;
}



/*
��������:�����Զ��һ�
����һ:�һ���־��1Ϊ����0Ϊ��
����ֵ��bool
*/
bool  gamecall::start_end_AutoAttack(int turn_flag)
{
	try
	{
		_asm
		{
			pushad
			push turn_flag
			mov ecx, 0x3D6B4F0
			mov edx, CALL_START_END_ATTACK
			call edx
			popad
		}
	}
	catch (...)
	{
		return false;
	}
	return true;
}

/*
��������:���Զ����նԻ���
����һ:VIP�ȼ�, vip�ȼ�>=1ʱ��ʹ������������գ�VIP�ȼ�Ϊ0ʱ��ʹ��NPC����
����ֵ��bool
*/
bool  gamecall::OpenRecovry(unsigned vip_level)
{
	if (vip_level > 0)
	{
		try
		{
			_asm
			{
				pushad
				push 0xFFFF
				push 0xFFFF
				push 0
				push 0
				push 0xF14DE0
				push 0xFFFFFFEC
				mov ecx, dword ptr ds : [CALL_ECX]
				mov edx, CALL_OPEN_BAG_REC
				call edx
				popad
			}
		}
		catch (...)
		{
			return false;
		}
	}
	else
	{
		if (OpendNPC(NPC_ZJ_RCV))
		{
			if ((*(DWORD*)P_CURRENT_NPC_DLG == NPC_ZJ_RCV))return true;
		}
		return false;
	}
	return true;
}

/*
��������:��Ʒ���գ����ݶԻ���ѡ��Ļ��վ������Ԫ�����
����һ:��ƷID
����ֵ��bool
*/
bool  gamecall::RecovryGoods(DWORD goodsId)
{
	try
	{
		_asm
		{
			pushad
			push goodsId
			mov ecx, P_CURRENT_NPC_DLG
			mov ecx, dword ptr ds : [ecx]
			push ecx
			mov ecx, dword ptr ds : [CALL_ECX]
			mov edx, CALL_RCV_GOODS
			call edx
			popad
		}
	}
	catch (...)
	{
		return false;
	}
	return true;
}


/*
��������:��Ʒ���վ���
����һ:��ƷID
������:VIP�ȼ�
����ֵ��bool
*/
bool  gamecall::RecovryGoods_To_Exp(DWORD goodsId, unsigned vip_level)
{
	OpenRecovry(vip_level);
	ChooseCmd("@Recovery_Exp_KaiQu");
	bool rtn = RecovryGoods(goodsId);
	ChooseCmd("@Recovery_Exp_KaiQu");
	return rtn;
}

/*
��������:��Ʒ����Ԫ������
����һ:��ƷID
������:VIP�ȼ�
����ֵ��bool
*/
bool  gamecall::RecovryGoods_To_Gold(DWORD goodsId, unsigned vip_level)
{
	OpenRecovry(vip_level);
	ChooseCmd("@Recovery_Gold_KaiQu");
	bool rtn = RecovryGoods(goodsId);
	ChooseCmd("@Recovery_Gold_KaiQu");
	return	rtn;
}

/*
��������:�ύ���������
����һ:�ύ������
����ֵ��bool
*/
bool gamecall::SubmitInputbox(const char* content)
{
	try
	{
		_asm
		{
			pushad
			push 1
			push content
			mov ecx, dword ptr ds : [CALL_EDX]
			push ecx
			mov ecx, dword ptr ds : [CALL_ECX]
			mov edx, CALL_SUBMIT_INPUTBOX
			call edx
			popad
		}
	}
	catch (...)
	{
		return false;
	}
	return true;
}

/*
��������:��ȡ��ɫ ��Ŀ��Ķ���ָ��
����һ:role����
����ֵ������ָ��
*/
DWORD* gamecall::getTargetP(role &r)
{
	if (*r.m_roleproperty.p_Target_ID)
	{
		DWORD id = *r.m_roleproperty.p_Target_ID;
		std::vector<DWORD> vec,npcs,pets,players;
		r.Get_Envionment(pets,npcs,vec,players);	

		for (auto it:vec)
		{
			if (*(DWORD*)(it + 0x8) == id) return (DWORD*)it;
		}
	}
	return nullptr;
}

/*
��������:��ɫ��������������
����һ:0
����ֵ��bool
*/
bool  gamecall::immdia_rebirth(void)
{
	try
	{
		_asm
		{
			pushad
			push 0x2
			mov ecx, dword ptr ds : [CALL_ECX]
			mov edx, CALL_IMMDIA_REBIRTH
			call edx
			popad
		}
	}
	catch (...)
	{
		return false;
	}
	return true;
}

/*
��������:С����Ϸ����ɫѡ�����
����һ:0
����ֵ��bool
*/
bool  gamecall::small_exit(void)
{
	try
	{
		_asm
		{
			pushad
			push 0x1
			mov ecx, dword ptr ds : [CALL_ECX]
			mov edx, CALL_SMALL_EXIT
			call edx
			popad
		}
	}
	catch (...)
	{
		return false;
	}
	return true;
}

/*
��������:��ȡ������Ʒ�������ƷID����Ҫ��ѡ����Ʒ֮��ſ��Ե���
����һ:������0��ʼ��Ĭ��0����һ����
����ֵ����ƷID
*/
DWORD  gamecall::get_buy_goods_ID(DWORD goods_index)
{
	DWORD goodsid = 0;
	try
	{
		_asm
		{
			pushad
			push goods_index
			mov ecx, 0x110D9E8
			mov edx, CALL_GET_BUY_GOODS_ID
			call edx
			mov eax, dword ptr ds : [eax + 0x20]
			mov goodsid,eax
			popad
		}
	}
	catch (...)
	{
		return false;
	}
	return goodsid;
}


/*
��������:����Ʒ
����һ:��Ʒ����
������:NPC����ID
������:�Ƿ�󶨣�Ĭ�ϲ��󶨣�ѡ��󶨽�ҹ���Ļ�������Ϊ1
������:������Ʒ����,Ϊ�յĻ���1��
����ֵ��bool
*/
bool gamecall::buyGoods(std::string& goodsName,DWORD npcId,DWORD isBind,unsigned number)
{
	/*  std::vector<std::string> drug = {"ǿЧ̫����ˮ", "̫����ˮ", "��ҩ(С��)", "��ҩ(����)","��ҩ(����)", "ħ��ҩ(С��)", "ħ��ҩ(����)", "ħ��ҩ(����)",
		"��ҩ��С����", "��ҩ���У���", "ħ��ҩ��С����", "ħ��ҩ���У���", "������ҩ", "����ħ��ҩ", "�ؼ���ҩ", "�ؼ�ħ��ҩ", "�ؼ���ҩ��", "�ؼ�ħ��ҩ��" ,
		"����ѩ˪","¹��","����ҩˮ" }; //0x64
	std::vector<std::string> books = { "��������", "ѪӰ����", "��Ӱ����", "��ҩ(����)","��ɱ����", "��������", "��ɱ����", "���µ�",
	"�����䵶", "Ұ����ײ", "ս������", "������", "�һ𽣷�", "�ƻ�����", "�ƶ�ն", "ͻն", "��ջ���", "������" ,"���λ�Ӱ" ,"С����" ,"���ܻ�" ,"�ջ�֮��" ,"��������" ,"�׵���" ,
	"������","˲���ƶ�","���׵�" ,"�����" ,"���ѻ���" ,"��ǽ" ,"�����Ӱ" ,"������" ,"������" ,"�����׹�" ,"��֮��" ,"ħ����" ,"ʥ����" ,"���׺�" ,"������" ,"��Ӱ��" ,"������" ,"������" ,
	"�����ϵ�","��������" ,"ħ����","���ǻ���" ,"������" ,"����ս��" ,"������" ,"��ʬ" ,"������" ,"������" ,"����������" ,"�����" ,"��ʥս����" ,"ʨ�Ӻ�" ,"��ħ��" ,"���ǽ" ,"��������" ,
		"Ⱥ��������" ,"�ٻ�����" ,"�����" ,"�ⶾ��" ,"��ڤ����" ,"ǿ��������" ,"�����ٻ�" ,"�ݵ�" };//0x32  */


		//�ȴ�����Ʒ���ֺ�npc��ȷ��call�ĵ��ĸ�����   0x64��ҩ��  0x32���飬���Σ������  0x1E(ɳ�أ����ΰ�) 1���سǣ� 0xA����������޸��ͣ�
	DWORD param_4 = 0;
	if (npcId==NPC_ZJ_YDZG) //����ҩ��
	{
		if ((goodsName.find("��ҩ") != std::string::npos) || (goodsName.find("������") != std::string::npos))
		{
			param_4 = get_buy_goods_ID(); //��ҩ
		}
		else
		{
			param_4 = 0x64;//��ҩ
		}
	}
	if (npcId==NPC_ZJ_ZHP)//�ӻ�
	{
		if (goodsName == "ɳ�ǻسǾ�" || goodsName == "�������Ѿ��")
		{
			param_4 = 0x32;
		}
		if (goodsName=="���ξ���"|| goodsName == "�������")
		{
			param_4 = 0x32;
		}
		if (goodsName == "�سǾ���" )
		{
			param_4 = 1;
		}
		if (goodsName == "������;��" || goodsName == "�޸���")
		{
			param_4 = 0xA;
		}
		if (goodsName == "����" || goodsName == "��������")
		{
			param_4 = get_buy_goods_ID();
		}
	}
	//if (npcId == NPC_ZJ_SDZG)param_4 = 0x32; //���飬����ϰ������� ��ʱδʵ��
	else
	{
		param_4 = get_buy_goods_ID();
	}
	if (param_4 == 0) return false;

	const char* gname = goodsName.c_str();

	if ((param_4< 0x65) && (param_4>0)) //����Ҫ ����� ����Ҫ��ƷID
	{
		for (unsigned i = 0; i < number; i++)
		{
			try
			{
				_asm
				{
					pushad
					push 0
					push isBind
					push gname
					push param_4
					push npcId
					mov ecx, dword ptr ds : [CALL_ECX]
					mov edx, CALL_BUY_GOODS
					call edx
					popad
				}
			}
			catch (...)
			{
				return false;
			}
		}
	}
	else //��ҪID
	{
		for (unsigned i = 0; i < number; i++)
		{
			param_4 = get_buy_goods_ID(i);
			try
			{
				_asm
				{
					pushad
					push 0
					push isBind
					push gname
					push param_4
					push npcId
					mov ecx, dword ptr ds : [CALL_ECX]
					mov edx, CALL_BUY_GOODS
					call edx
					popad
				}
			}
			catch (...)
			{
				return false;
			}
		}
	}
	return true;
}


/*
��������:����Ʒ
����һ:��Ʒ����
������:NPC����ID
������:��ƷID
����ֵ��bool
*/
bool gamecall::sellGoods(std::string& goodsName, DWORD npcId, DWORD goodsId)
{
	const char* name = goodsName.c_str();

			try
			{
				_asm
				{
					pushad
					push name
					push goodsId
					push npcId
					mov ecx, dword ptr ds : [CALL_ECX]
					mov edx, CALL_SELL_GOODS
					call edx
					popad
				}
			}
			catch (...)
			{
				return false;
			}
}


/*
��������:����Ʒ
����һ:��Ʒ����
������:NPC����ID
������:��ƷID
����ֵ��bool
*/
bool gamecall::storeGoods(std::string& goodsName, DWORD npcId, DWORD goodsId)
{
	const char* name = goodsName.c_str();

	try
	{
		_asm
		{
			pushad
			push 0
			push name
			push goodsId
			push npcId
			mov ecx, dword ptr ds : [CALL_ECX]
			mov edx, CALL_STORE_GOODS
			call edx
			popad
		}
	}
	catch (...)
	{
		return false;
	}
}