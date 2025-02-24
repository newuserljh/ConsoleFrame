#pragma once
#include <unordered_map>
#include <queue>
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
#include "scriptManager.h"


// �����ⲿ���� 
extern shareMemoryCli shareCli; 
extern int shareindex; 
extern role r; 
extern role Promenade; 
extern m_object m_obj; 
extern skill m_skill; 
extern bag r_bag; 
extern gamecall mfun; 
extern team m_team; 


// �洢��ͼ���ƺ�ת����ϵ��ӳ�� ��Ҫ�� �ṹ�� �� ����
struct Position {
	int x, y;
};
using TransitionMap = std::unordered_map<std::string, std::vector<Position>>;
using MapNames = std::unordered_map<std::string, std::string>;
using Transitions = std::unordered_map<std::string, TransitionMap>;
extern MapNames map_names; //��ͼ����ӳ��
extern Transitions transitions; //��ͼת��


//�洢NPC��Ϣ�� ��Ҫ�� �ṹ�� �� ����
struct NPC {
	std::string mapName;
	std::string npcName;
	int x;
	int y;
	bool operator==(const NPC& other) const {
		return mapName == other.mapName && npcName == other.npcName && x == other.x && y == other.y;
	}
};
// ���� std::unordered_map �Ĺ�ϣ����
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
	lua_interface() : L(luaL_newstate(), lua_close)
	{
		if (L) {
			luaL_openlibs(L.get());

			// �滻 Lua �� print ����
			lua_pushcfunction(L.get(), &lua_interface::lua_print);
			lua_setglobal(L.get(), "print");
			registerClasses();
		} 
	}
	~lua_interface() = default;

	// �ض��� Lua �� print ����
	static int lua_print(lua_State* L);
	void registerClasses(); //ע���ൽ Lua ��
	lua_State* getLuaState() const { return L.get(); }
	// ���ÿ������캯���͸�ֵ������
	lua_interface(const lua_interface&) = delete;
	lua_interface& operator=(const lua_interface&) = delete;
private:
	std::unique_ptr<lua_State, decltype(&lua_close)> L;


public:
	//��ͼ��Ϣ
	void load_and_store_map_data(lua_State* L, const std::string& file_path);
	std::vector<std::pair<std::string, std::vector<Position>>> find_path_with_positions(const std::string& start_name, const std::string& end_name);	
	bool find_path(const std::string& start_name, const std::string& end_name, std::vector<std::string>& path);//��ȡ���������·��

	// NPC��Ϣ�洢
	void  saveNPCsToLua(const std::vector<NPC>& npcs, const std::string& filePath);
	std::map<std::string, std::unordered_map<NPC, bool, NPCHash>> existingNPCs;

public:
	//��Ϸ���ܷ�װ
	bool presskey(int vkcode);
	void applySJLP();//�ύ�ͽ�����
	void getGoodsProcessIndex() { r_bag.getGoodsProcessIndex(); } //�س�֮�� ��ȡ��Ʒ����ʽ������ д��index_vec_store .index_vec_sell_jewel...
	int getStoreGoodsNumber() { return r_bag.index_vec_store.size(); }//��ȡ������Ʒ����
	int getSellJewelryNumber() { return r_bag.index_vec_sell_jewel.size(); }//��ȡ������������
	int getSellClothesNumber() { return r_bag.index_vec_sell_cloth.size(); }//��ȡ�����·�����
	int getSellWeaponNumber() { return r_bag.index_vec_sell_weapon.size(); }//��ȡ������������
	//const DWORD getSellMedicineNumber();//���ü�����ҩƷ����  Ҫ��ҩ ��ʱ������
	// �����޲ΰ汾�ķ���
	bool storeGoods() { return storeGoods(r_bag.index_vec_store); } //������Ʒ
	bool sellMedicine() { return sellMedicine(r_bag.index_vec_sell_medci); }//��ҩ
	bool sellClothes() { return sellClothes(r_bag.index_vec_sell_cloth); }//���·�
	bool sellJewelry() { return sellJewelry(r_bag.index_vec_sell_jewel); }//������
	bool sellWeapon() { return sellWeapon(r_bag.index_vec_sell_weapon); }//������
	bool buyMedicine(const std::string& med_name, BYTE num);//��ҩ
	int getCurrentX() { return *r.m_roleproperty.Object.X; }//��ȡ��ǰ����X
	int getCurrentY() { return *r.m_roleproperty.Object.Y; }// ��ȡ��ǰ����Y
	std::string getCurrentMapName() { return r.m_roleproperty.p_Current_Map; }//��ȡ��ǰ��ͼ����
	int getBagSpace() { return r_bag.getBagSpace(); }//��ȡ����ʣ���������
	int getBagWeightRemain() { return *r.m_roleproperty.BAG_W; }//��ȡ������ʹ�ø���
	int getBagWeightMax() { return *r.m_roleproperty.BAG_W_MAX; }//��ȡ���������
	int getBagGoodsNumber(std::string& name) { return r_bag.caclGoodsNumber(name); }//��ȡ������Ʒ����
	int getBagHpMedcine() { return r_bag.caclHpGoodsNumer(); }//��ȡ������ҩ����
	int getBagMpMedcine() { return r_bag.caclMpGoodsNumer(); }//��ȡ������ҩ����
	int getGoldNumber(){return *r.m_roleproperty.p_Role_Gold;}//��ȡ�������
	int getYbNumber() { return *r.m_roleproperty.p_Role_YB; }//��ȡԪ������
	int getBindGoldNumber() { return *r.m_roleproperty.p_Role_GoldBind; }//��ȡ�󶨽������
	int getBindYbNumber() { return (int)*r.m_roleproperty.p_Role_YB_Bind; }//��ȡ��Ԫ������
	bool useBagGoods(const std::string& goodsName) { return mfun.useGoods(r_bag.getGoodsIndex(goodsName)); }//ʹ�ñ�����Ʒ
	bool useSkillToMyself(std::string skillname) { return mfun.useSkillTo(m_skill.getSkillId(skillname.c_str()), getCurrentX(), getCurrentY(), 0); }//���Լ�ʹ�ü���
	bool useSkillToTarget(std::string skillname, int x, int y,DWORD tId) { return mfun.useSkillTo(m_skill.getSkillId(skillname.c_str()), x, y, tId); }//��Ŀ��ʹ�ü���
	bool smallExit() { return mfun.small_exit(); }//С��
	bool immdiaRebirth() { return mfun.immdia_rebirth(); }//��������
	bool runTo(int x, int y){ return mfun.Run_or_Step_To(x, y,2);}//�ܵ�Ŀ��� һ��2��
    bool walkTo(int x, int y) { return mfun.Run_or_Step_To(x, y, 1); }//�ߵ�Ŀ��� һ��1��
    bool gotoMapXY(int x, int y) { return mfun.CurrentMapMove(x, y); }//Ѱ·����ͼ����
	bool startAttack() { return mfun.start_end_AutoAttack(1); }//��ʼս��
	bool endAttack() { return mfun.start_end_AutoAttack(0); }//����ս��
	float getDistance(int x, int y) { return mfun.caclDistance(getCurrentX(), getCurrentY(), x, y); }	//����������(x,y)�ľ��� ,ֱ�߾��� �޷�����ʵ�ʾ���
	bool interactNPC(const std::string& npcName, const std::string& command);//�Ի�NPC ��ѡ��һ������
	bool chooseNpcCommand(const std::string& command) { return mfun.ChooseCmd(command.c_str()); }//ѡ��NPC�Ի�

private:
	//������ָ�������
	DWORD getEviroNPCIdByName(std::string npcName);//ͨ�����ֻ�ȡNPCID
	bool interactWithNPC(const std::string& npcName, const std::string& command, const std::function<bool(int)>& action);
	bool sellItems(const std::vector<DWORD>& bagIndexSell, int npcId);
	bool storeGoods(const std::vector<DWORD>& bagIndexStore = r_bag.index_vec_store);//������Ʒ
	bool sellClothes(const std::vector<DWORD>& bagIndexCloSell = r_bag.index_vec_sell_cloth);//���·�
	bool sellJewelry(const std::vector<DWORD>& bagIndexJeSell = r_bag.index_vec_sell_jewel);//������
	bool sellWeapon(const std::vector<DWORD>& bagIndexWpSell = r_bag.index_vec_sell_weapon);//������
	bool sellMedicine(const std::vector<DWORD>& bagIndexMedSell = r_bag.index_vec_sell_medci);//��ҩ
};

