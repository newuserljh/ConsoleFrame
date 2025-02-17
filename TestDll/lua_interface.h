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


// 存储地图名称和转换关系的映射 需要的 结构体 和 声明
struct Position {
	int x, y;
};
using TransitionMap = std::unordered_map<std::string, std::vector<Position>>;
using MapNames = std::unordered_map<std::string, std::string>;
using Transitions = std::unordered_map<std::string, TransitionMap>;
extern MapNames map_names; //地图名称映射
extern Transitions transitions; //地图转换


//存储NPC信息的 需要的 结构体 和 声明
struct NPC {
	std::string mapName;
	std::string npcName;
	int x;
	int y;
	bool operator==(const NPC& other) const {
		return mapName == other.mapName && npcName == other.npcName && x == other.x && y == other.y;
	}
};
// 用于 std::unordered_map 的哈希函数
struct NPCHash {
	std::size_t operator()(const NPC& npc) const {
		std::size_t h1 = std::hash<std::string>{}(npc.mapName);
		std::size_t h2 = std::hash<std::string>{}(npc.npcName);
		std::size_t h3 = std::hash<int>{}(npc.x);
		std::size_t h4 = std::hash<int>{}(npc.y);
		return h1 ^ (h2 << 1) ^ (h3 << 2) ^ (h4 << 3);
	}
};

class lua_interface
{
public:
	lua_interface();
	~lua_interface();
	static int lua_print(lua_State* L);//重定向lua的print函数
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
	void load_and_store_map_data(lua_State* L, const std::string& file_path);
	std::vector<std::pair<std::string, std::vector<Position>>> find_path_with_positions(const std::string& start_name, const std::string& end_name);
	void  saveNPCsToLua(const std::vector<NPC>& npcs, const std::string& filePath);

public:
	//游戏功能封装
	void applySJLP();//提交赏金令牌
	bool storeGoods(const std::vector<DWORD>& bagIndexStore);//保存物品
	bool buyMedicine(const std::string &med_name, BYTE num);//买药
	bool sellMedicine(const std::vector<DWORD>& bagIndexMedSell);//卖药
	bool sellClothes(const std::vector<DWORD>& bagIndexCloSell);//卖衣服
	bool sellJewelry(const std::vector<DWORD>& bagIndexJeSell);//卖首饰
	bool sellWeapon(const std::vector<DWORD>& bagIndexWpSell);//卖武器
private:
	DWORD getEviroNPCIdByName(std::string npcName);//通过名字获取NPCID
	bool interactWithNPC(const std::string& npcName, const std::string& command, const std::function<bool(int)>& action);
	bool sellItems(const std::vector<DWORD>& bagIndexSell, int npcId);
	bool find_path(const std::string& start_name, const std::string& end_name, std::vector<std::string>& path);//获取不带坐标的路径
	std::map<std::string, std::unordered_map<NPC, bool, NPCHash>> existingNPCs;
};

