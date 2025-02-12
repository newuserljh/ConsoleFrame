#pragma once
#include "../GameData/bag.h"
#include "../GameData/role.h"
#include "../GameData/m_object.h"
#include "../GameData/skill.h"
#include "../GameData/team.h"
#include "../Common/utils.h"
#include "../GameData/gamecall.h"
#include "../luajit/src/lua.hpp"
#include "../Common/shareMemoryCli.h"
#include "LuaBridge/LuaBridge.h"
#include <unordered_map>
#include <queue>


// 声明外部对象 
extern shareMemoryCli shareCli; 
extern int shareindex; 
extern role r; 
extern role Promenade; 
extern m_object m_obj; 
extern skill m_skill; 
extern bag r_bag; 
extern gamecall mfun; 
extern team m_team; 

struct Position {
	int x, y;
};

using TransitionMap = std::unordered_map<std::string, std::vector<Position>>;
using MapNames = std::unordered_map<std::string, std::string>;
using Transitions = std::unordered_map<std::string, TransitionMap>;

class lua_interface
{
public:
	lua_interface();
	~lua_interface();

	void registerClasses();
	lua_State* getLuaState() const;

private:
	lua_State* L;
	role m_role;
	bag m_bag;
	gamecall m_gcall;
	tools &m_tools; // 单例类的引用

public:
	//导出本类函数
	bool presskey(int vkcode);
	bool find_path(const MapNames& map_names, const Transitions& transitions, const std::string& start_name, const std::string& end_name, std::vector<std::string>& path);
	void load_and_store_map_data(lua_State* L, const std::string& file_path, MapNames& map_names, Transitions& transitions);
	std::vector<std::pair<std::string, std::vector<Position>>> get_positions_for_path(const Transitions& transitions, const std::vector<std::string>& path);


public:
	//游戏功能封装
	void applySJLP();//提交赏金令牌
	bool storeGoods(std::vector <DWORD*>& p_vec_store);//保存物品
	bool buyMedicine(std::string med_name, BYTE num);//买药
	bool sellMedicine(std::vector<DWORD*>& med_sell);//卖药
	bool sellClothes(std::vector <DWORD*>& clo_sell);//卖衣服
	bool sellJewelry(std::vector <DWORD*>& je_sell);//卖首饰
private:
	DWORD getEviroNPCIdByName(std::string npcName);//通过名字获取NPCID



public:
	void parseMyConfig(std::vector<std::string>& store, std::vector<std::string>& weapon, std::vector<std::string>& clothes,
		std::vector<std::string>& jewelry, std::map<std::string, DWORD>& medicine);//解析读取到配置文件到各自列表

private:
	void  getStoreAndSell_Pointer_vec();//获取待存物品 待卖衣服 首饰 武器 药品的指针容器 std::vector<DWORD*>
	std::vector<DWORD*> pStoreVec; //
	std::vector<DWORD*> pSellWeaponVec;
	std::vector<DWORD*> pSellClothesVec;
	std::vector<DWORD*> pSellJewelryVec;
	std::vector<DWORD*> pMedicineVec;
};

