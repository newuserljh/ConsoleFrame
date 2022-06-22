#pragma once
#include "baseaddr.h"
#include "role.h"
/*
��Ϸ����CALL��
*/
class gamecall
{
public:
	gamecall(){}
	~gamecall(){}
	bool loginGame(const char* user, const char* pass);
	bool useGoods(int goodsIndex);
	bool useSkillTo(DWORD skillId, DWORD x, DWORD y, DWORD targetId = 0);
	bool CurrentMapMove(DWORD x, DWORD y);
	bool OpendNPC(DWORD npcID);
	bool ChooseCmd(char* command);
	bool pickupGoods(DWORD x,DWORD y);
	bool presskey(int vkcode);
	bool presskey(DWORD pid, int vkcode = VK_RETURN);
	HWND GetHwndByPid(DWORD dwProcessID);
	unsigned caclDistance(DWORD x1, DWORD y1, DWORD x2, DWORD y2);
	DWORD Find_T_Monster(role& r, std::vector<std::string>& vec);
	MapXY splitXY(std::string str);
private:

};


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
			mov edx,CALL_USEGOODS
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
bool gamecall::useSkillTo(DWORD skillId,DWORD x,DWORD y,DWORD targetId)
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
��������:Ѱ·����ǰ��ͼָ������
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
bool gamecall::ChooseCmd(char* command)
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
bool gamecall::pickupGoods(DWORD x,DWORD y)
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
bool gamecall::loginGame(const char* user,const char* pass)
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
��������:ģ�ⰴ�� ��֧�ֻس���
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
��������:��HWND���������ӿؼ�����ָ��������Ϣ
����һ:ָ���ĸ����ھ��
������:��Ϣ ȱʡΪ�س���
*/

/*
��������:postMessageģ�ⰴ�� ֧�ֻس���
����һ:����PID
����һ:������ ȱʡΪ�س�
*/
bool gamecall::presskey(DWORD pid,int vkcode)
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
unsigned gamecall::caclDistance(DWORD x1,DWORD y1, DWORD x2, DWORD y2)
{
	int x, y;
	x = x1 - x2;
	if (x < 0)x = 0 - x;
	y = y1 - y2;
	if (y < 0)y = 0 - y;
	return (x > y) ? x : y;
}


/*
��������:�ҵ�Ҫ�����Ĺ���
����һ:��ɫ�ṹ��
������:���������б�
����ֵ�������ҵ��Ĺ������ָ��
*/
DWORD gamecall::Find_T_Monster(role& r, std:: vector<std::string>& vec)
{
	std::vector<DWORD> near_Mon_3, near_Mon_7;
	r.Get_Envionment(near_Mon_3, 3); /*�ҵ�3�����ڵĹ���*/
	if (near_Mon_3.size())
	{
		for (unsigned i = 0; i < near_Mon_3.size(); i++)
		{
			for (unsigned j = 0; j < vec.size(); j++){
			
				if (strcmp((CHAR*)(near_Mon_3[i] + 0x10), vec[j].c_str()) == 0) /*strcmp����0�������*/
					return near_Mon_3[i];
			}
		}
	}
	r.Get_Envionment(near_Mon_7, 7); /*�ҵ�7�����ڵĹ���*/
	if (near_Mon_7.size())
	{
		for (unsigned i = 0; i < near_Mon_7.size(); i++)
		{
			for (unsigned j = 0; j < vec.size(); j++)
			{
				if (strcmp((CHAR*)(near_Mon_7[i] + 0x10), vec[j].c_str()) == 0) /*strcmp����0�������*/
					return near_Mon_7[i];
			}
		}
	}
	return 0;
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
	if (temp.size()!=2) 
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