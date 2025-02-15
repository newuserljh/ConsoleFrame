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
	tools &m_tools; // �����������

public:
	//�������ຯ��
	bool presskey(int vkcode);
	bool find_path(const MapNames& map_names, const Transitions& transitions, const std::string& start_name, const std::string& end_name, std::vector<std::string>& path);
	void load_and_store_map_data(lua_State* L, const std::string& file_path, MapNames& map_names, Transitions& transitions);
	std::vector<std::pair<std::string, std::vector<Position>>> get_positions_for_path(const Transitions& transitions, const std::vector<std::string>& path);


public:
	//��Ϸ���ܷ�װ
	void applySJLP();//�ύ�ͽ�����
	bool storeGoods(std::vector <DWORD*>& p_vec_store);//������Ʒ
	bool buyMedicine(std::string med_name, BYTE num);//��ҩ
	bool sellMedicine(std::vector<DWORD*>& med_sell);//��ҩ
	bool sellClothes(std::vector <DWORD*>& clo_sell);//���·�
	bool sellJewelry(std::vector <DWORD*>& je_sell);//������
private:
	DWORD getEviroNPCIdByName(std::string npcName);//ͨ�����ֻ�ȡNPCID



public:
	void parseMyConfig(std::vector<std::string>& store, std::vector<std::string>& weapon, std::vector<std::string>& clothes,
		std::vector<std::string>& jewelry, std::map<std::string, DWORD>& medicine);//������ȡ�������ļ��������б�

private:
	void  getStoreAndSell_Pointer_vec();//��ȡ������Ʒ �����·� ���� ���� ҩƷ��ָ������ std::vector<DWORD*>
	std::vector<DWORD*> pStoreVec; //
	std::vector<DWORD*> pSellWeaponVec;
	std::vector<DWORD*> pSellClothesVec;
	std::vector<DWORD*> pSellJewelryVec;
	std::vector<DWORD*> pMedicineVec;
};

