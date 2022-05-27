#include "role.h"
#include "baseaddr.h"

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
		m_roleproperty.SW = (DWORD*)(*(DWORD*)RoleBase  +0xAA6);
		m_roleproperty.p_Bag_Base = (DWORD*)(*(DWORD*)RoleBase+ 0xC54);
		m_roleproperty.Bag_Size = (DWORD*)(*(DWORD*)RoleBase +0xc58);
		m_roleproperty.p_LR_Bag_Base = (DWORD*)(*(DWORD*)RoleBase + 0xc60);
		m_roleproperty.LR_Bag_Size = (DWORD*)(*(DWORD*)RoleBase + 0xc64);
		m_roleproperty.p_Target_ID = (DWORD*)(*(DWORD*)RoleBase + 0x458);
		m_roleproperty.p_Skill_Base = (DWORD*)(*(DWORD*)RoleBase+0x1358);
		m_roleproperty.p_ZB = (DWORD*)(*(DWORD*)RoleBase +0xc48);
		m_roleproperty.LL = (DWORD*)(*(DWORD*)RoleBase + 0x139C);
		m_roleproperty.p_Current_Map = (char*)(*(DWORD*)RoleBase + 0x8299C8);
		if (!*m_roleproperty.Object.HP_MAX)
		{
			return false;
		}
	}
	catch (...)
	{
		return false;
	}
	return true;
}
