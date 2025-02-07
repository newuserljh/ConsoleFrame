#pragma once
#include "baseaddr.h"
#include "role.h"
#include<algorithm>
#include <regex> 
/*
游戏功能CALL类
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
	bool ChooseCmd(const char* command);
	bool pickupGoods(DWORD x,DWORD y);
	bool presskey(int vkcode);
	bool presskey(DWORD pid, int vkcode = VK_RETURN);
	bool Run_or_Step_To(DWORD x, DWORD y, DWORD run_step_flag);
	HWND GetHwndByPid(DWORD dwProcessID);
	float caclDistance(DWORD x1, DWORD y1, DWORD x2, DWORD y2);
	//DWORD Find_T_Monster(role& r, std::vector<std::string>& vec);
	std::vector<MONSTER_PROPERTY> sort_aroud_monster(role& r, std::vector<std::string>& vec, DWORD e_range=15);
	std::vector<GROUND_GOODS> sort_groud_goods(role& r, std::vector<std::string>& vec);
	MapXY splitXY(std::string str);
	bool team_open_close(DWORD tflag);
	bool maketeam(std::string pName);
	bool allowteam(std::string pName);
	bool release_Promenade(void);
	bool start_end_AutoAttack(int turn_flag);
	bool OpenRecovry(unsigned vip_level);
	bool RecovryGoods(DWORD goodsId);
	bool RecovryGoods_To_Exp(DWORD goodsId, unsigned	vip_level);
	bool RecovryGoods_To_Gold(DWORD goodsId, unsigned	vip_level);
	bool SubmitInputbox(const char* content);
	DWORD* getTargetP(role &r);
	bool immdia_rebirth(void);
	bool small_exit(void);
	DWORD  get_buy_goods_ID(DWORD goods_index=0);
	bool buyGoods(std::string& goodsName, DWORD npcId,DWORD isBind=0, unsigned number=1);

private:
	static bool comp(const MONSTER_PROPERTY& a, const MONSTER_PROPERTY& b);
	static bool comp_groud(const GROUND_GOODS& a, const GROUND_GOODS& b);
};



