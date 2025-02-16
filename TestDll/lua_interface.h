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

extern MapNames map_names; //��ͼ����ӳ��
extern Transitions transitions; //��ͼת��

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
	void load_and_store_map_data(lua_State* L, const std::string& file_path);
	std::vector<std::pair<std::string, std::vector<Position>>> find_path_with_positions(const std::string& start_name, const std::string& end_name);


public:
	//��Ϸ���ܷ�װ
	void applySJLP();//�ύ�ͽ�����
	bool storeGoods(std::vector <DWORD>& bag_index_store);//������Ʒ
	bool buyMedicine(std::string med_name, BYTE num);//��ҩ
	bool sellMedicine(std::vector<DWORD>& bag_index_med_sell);//��ҩ
	bool sellClothes(std::vector <DWORD>& bag_index_clo_sell);//���·�
	bool sellJewelry(std::vector <DWORD>& bag_index_je_sell);//������
	bool sellWeapon(std::vector <DWORD>& bag_index_wp_sell);//������
private:
	DWORD getEviroNPCIdByName(std::string npcName);//ͨ�����ֻ�ȡNPCID
	bool find_path(const std::string& start_name, const std::string& end_name, std::vector<std::string>& path);//��ȡ���������·��
};

