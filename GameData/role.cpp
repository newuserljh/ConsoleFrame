#include "role.h"
#include "baseaddr.h"
#include "../Common/utils.h"

role::role()
{
}

role::~role()
{
}

bool role::init()
{
	try
	{
		m_roleproperty.Object.ID = (DWORD*)(*(DWORD*)RoleBase + 8);
		m_roleproperty.Object.pName = (char*)(*(DWORD*)RoleBase + 0x10);
		m_roleproperty.Object.HP = (DWORD*)(*(DWORD*)RoleBase + 0x80);
		m_roleproperty.Object.HP_MAX = (DWORD*)(*(DWORD*)RoleBase + 0x84);
		m_roleproperty.Object.MP = (DWORD*)(*(DWORD*)RoleBase + 0x88);
		m_roleproperty.Object.MP_MAX = (DWORD*)(*(DWORD*)RoleBase + 0x8c);
		m_roleproperty.Object.X = (DWORD*)(*(DWORD*)RoleBase +0xac);
		m_roleproperty.Object.Y = (DWORD*)(*(DWORD*)RoleBase + 0xb0);
		m_roleproperty.Object.IsPosion = (BYTE*)(*(DWORD*)RoleBase + 0x34b  );
		m_roleproperty.Job = (DWORD*)(*(DWORD*)RoleBase + 0xe4);
		m_roleproperty.Level = (DWORD*)(*(DWORD*)RoleBase + 0xec);
		m_roleproperty.Sex = (DWORD*)(*(DWORD*)RoleBase + 0xdc);
		m_roleproperty.GJL_L = (DWORD*)(*(DWORD*)RoleBase + 0xA5C);
		m_roleproperty.GJL_H = (DWORD*)(*(DWORD*)RoleBase +0xA60);
		m_roleproperty.MFL_L= (DWORD*)(*(DWORD*)RoleBase  +0xA64);
		m_roleproperty.MFL_H = (DWORD*)(*(DWORD*)RoleBase + 0xA68);
		m_roleproperty.DSL_L = (DWORD*)(*(DWORD*)RoleBase + 0xA6c);
		m_roleproperty.DSL_H = (DWORD*)(*(DWORD*)RoleBase + 0xA70);
		m_roleproperty.FY_L = (DWORD*)(*(DWORD*)RoleBase + 0xA7C);
		m_roleproperty.FY_H= (DWORD*)(*(DWORD*)RoleBase + 0xA80);
		m_roleproperty.MF_L= (DWORD*)(*(DWORD*)RoleBase + 0xA84);
		m_roleproperty.MF_H= (DWORD*)(*(DWORD*)RoleBase +0xA88);
		m_roleproperty.BAG_W= (WORD*)(*(DWORD*)RoleBase +0xAA8);
		m_roleproperty.BAG_W_MAX = (WORD*)(*(DWORD*)RoleBase + 0xAAA);
		m_roleproperty.SW = (DWORD*)(*(DWORD*)RoleBase  +0xAD0);
		m_roleproperty.p_Bag_Base = (DWORD*)(*(DWORD*)RoleBase+ 0xC54);
		m_roleproperty.Bag_Size = (DWORD*)(*(DWORD*)RoleBase +0xc58);
		m_roleproperty.p_LR_Bag_Base = (DWORD*)(*(DWORD*)RoleBase + 0xc60);
		m_roleproperty.LR_Bag_Size = (DWORD*)(*(DWORD*)RoleBase + 0xc64);
		m_roleproperty.p_Target_ID = (DWORD*)(*(DWORD*)RoleBase + 0x458);
		m_roleproperty.p_Skill_Base = (DWORD*)(*(DWORD*)RoleBase+0x1358);
		m_roleproperty.p_ZB = (DWORD*)(*(DWORD*)RoleBase +0xc48);
		m_roleproperty.LL = (DWORD*)(*(DWORD*)RoleBase + 0x139C);
		m_roleproperty.p_Current_Map = (char*)(*(DWORD*)RoleBase + 0x8299C8);
		m_roleproperty.PatCount = (DWORD*)(*(DWORD*)RoleBase + 0xA24);
		if (!*m_roleproperty.Object.HP_MAX)
		{
			return false;
		}
	}
	catch (...)
	{
		return false;
	}
	if(!init_equip())return false;
	return true;
}

//初始化身上装备
bool role::init_equip()
{
	if (!m_roleproperty.p_ZB)return false;
	DWORD bagBase = *m_roleproperty.p_ZB;
	try
	{
		for (auto i=0;i<21;i++)
		{
			//if (!(*(DWORD*)(bagBase + i * 0x688 + 0x2c)))//判断无物品
			//{
			//	m_euip[i].ID = 0;
			//	continue;
			//}
			m_euip[i].pName = (char*)(bagBase + i * 0x688 + 1);
			m_euip[i].WD_low = (BYTE*)(bagBase + i * 0x688 + 0x1a);
			m_euip[i].WD_high = (BYTE*)(bagBase + i * 0x688 + 0x1b);
			m_euip[i].MD_low = (BYTE*)(bagBase + i * 0x688 + 0x1c);
			m_euip[i].WD_high = (BYTE*)(bagBase + i * 0x688 + 0x1d);
			m_euip[i].PA_low = (BYTE*)(bagBase + i * 0x688 + 0x1e);
			m_euip[i].PA_high = (BYTE*)(bagBase + i * 0x688 + 0x1f);
			m_euip[i].Magic_low = (BYTE*)(bagBase + i * 0x688 + 0x20);
			m_euip[i].Magic_high = (BYTE*)(bagBase + i * 0x688 + 0x21);
			m_euip[i].Tao_low = (BYTE*)(bagBase + i * 0x688 + 0x22);
			m_euip[i].Tao_high = (BYTE*)(bagBase + i * 0x688 + 0x23);
			m_euip[i].Need_what = (BYTE*)(bagBase + i * 0x688 + 0x24);
			m_euip[i].Need_Num = (BYTE*)(bagBase + i * 0x688 + 0x25);
			m_euip[i].ID = (DWORD*)(bagBase + i * 0x688 + 0x2c);
			m_euip[i].Use_Num = (WORD*)(bagBase + i * 0x688 + 0x30);
			m_euip[i].Use_Num_Max = (WORD*)(bagBase + i * 0x688 + 0x32);
		}
	}
	catch (...)
	{
		return false;
	}
	return true;
}



/*遍历周围对象及地面物品*/
bool role:: Get_Envionment(DWORD x, DWORD y, std::vector<DWORD> &vec,DWORD get_offset, DWORD g_range)
{
	vec.clear();
	DWORD p_temp=0;
	for (int i = x - g_range; i < x + g_range; i++)
	{
		if (i <= 0)i = 1;
		for (int j = y - g_range; j < y + g_range; j++)
		{
			if (j <= 0)j = 1;
			try
			{
				_asm
				{
					pushad
					mov ecx, RoleBase
					mov ecx, [ecx]
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
					mov p_temp, eax
					popad
				}
				if (p_temp != 0)
				{
					if (get_offset == Ground_Offset)vec.push_back(p_temp);/*地面*/
					if ((get_offset == Envi_Offset)&&(*(DWORD*)(p_temp + 0x80)!=0))vec.push_back(p_temp);/*周围其他对象*/
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