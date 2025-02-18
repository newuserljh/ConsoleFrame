#include "stdafx.h"
#include "lua_interface.h"
#include "TestDlg.h"


void lua_interface::registerClasses()
{
	using namespace luabridge;

	// �ർ����lua
	getGlobalNamespace(L.get())
		.beginClass<bag>("bag") //bag��
		.addConstructor<void(*)()>()
		.addFunction("init", &bag::init)
		.addFunction("ifHasPoison", &bag::ifHasPoison)
		.endClass()

		.beginClass<tools>("tools")//tools��
		.addStaticFunction("getInstance", &tools::getInstance)
		//.addFunction("message", &tools::message)
		.addFunction("improveProcPriv", &tools::improveProcPriv)
		//.addFunction("findPattern", &tools::findPattern)
		.addFunction("getModuleInfo", &tools::getModuleInfo)
		.addFunction("log2file", &tools::log2file)
		.addFunction("write2file", &tools::write2file)
		.addFunction("write2file_c", &tools::write2file_c)
		//.addFunction("read", &tools::read)
		//.addFunction("write", &tools::write)
		//.addFunction("byte2hex", &tools::byte2hex)
		//.addFunction("lower2upper", &tools::lower2upper)
		//.addFunction("hexstr2upper", &tools::hexstr2upper)
		//.addFunction("hex2byte", &tools::hex2byte)
		//.addFunction("bytes2hexstr", &tools::bytes2hexstr)
		//.addFunction("hexstr2bytes", &tools::hexstr2bytes)
		.addFunction("fileIsexist", &tools::fileIsexist)
		.addFunction("ReadTxt", &tools::ReadTxt)
		.addFunction("splitString", &tools::splitString)
		.addFunction("GetCurrDir", &tools::GetCurrDir)
		.addFunction("getParentPath", &tools::getParentPath)
		.addFunction("char2wchar", &tools::char2wchar)
		.addFunction("wchar2char", &tools::wchar2char)
		.addFunction("ReleaseResource", &tools::ReleaseResource)
		.addFunction("eipinjectDll", &tools::eipinjectDll)
		.addStaticFunction("exeload", &tools::exeload)
		.endClass()

		.beginClass<gamecall>("gamecall") //gamecall��
		.addConstructor<void(*)()>()
		.addFunction("OpendNPC", &gamecall::OpendNPC)
		.addFunction("ChooseCmd", &gamecall::ChooseCmd)
		.addFunction("pickupGoods", &gamecall::pickupGoods)
		.addFunction("team_open_close", &gamecall::team_open_close)
		.addFunction("maketeam", &gamecall::maketeam)
		.addFunction("allowteam", &gamecall::allowteam)
		.addFunction("OpenRecovry", &gamecall::OpenRecovry)
		.addFunction("RecovryGoods", &gamecall::RecovryGoods)
		.addFunction("RecovryGoods_To_Exp", &gamecall::RecovryGoods_To_Exp)
		.addFunction("RecovryGoods_To_Gold", &gamecall::RecovryGoods_To_Gold)
		//.addFunction("SubmitInputbox", &gamecall::SubmitInputbox)
		.endClass()
				    
		.beginClass<lua_interface>("LuaInterface") // ע�᱾�෽���� Lua
		.addConstructor<void(*)(void)>()
		.addFunction("ģ�ⰴ��", &lua_interface::presskey)
		.addFunction("���ͽ�", &lua_interface::applySJLP)
		.addFunction("��ҩ", &lua_interface::buyMedicine)
		.addFunction("�س�������", &lua_interface::getGoodsProcessIndex) //�سǵ��� ��ȡ��Ʒ����ʽд��index_vec_store .. ֮����ܵ�������Ĵ��������Ʒ
		.addFunction("������Ʒ����", &lua_interface::getStoreGoodsNumber)
		.addFunction("������������", &lua_interface::getSellJewelryNumber)
		.addFunction("�����·�����", &lua_interface::getSellClothesNumber)
		.addFunction("������������", &lua_interface::getSellWeaponNumber)
		.addFunction("��ֿ�", static_cast<bool(lua_interface::*)()>(&lua_interface::storeGoods))
		.addFunction("���·�", static_cast<bool(lua_interface::*)()>(&lua_interface::sellClothes))
		.addFunction("������", static_cast<bool(lua_interface::*)()>(&lua_interface::sellJewelry))
		.addFunction("������", static_cast<bool(lua_interface::*)()>(&lua_interface::sellWeapon))
		.addFunction("��ҩ", static_cast<bool(lua_interface::*)()>(&lua_interface::sellMedicine))
		.addFunction("��ǰ����X", &lua_interface::getCurrentX)
		.addFunction("��ǰ����Y", &lua_interface::getCurrentY)
		.addFunction("��ǰ��ͼ��", &lua_interface::getCurrentMapName)
		.addFunction("����ʣ�����", &lua_interface::getBagSpace)
		.addFunction("��ǰ��������", &lua_interface::getBagWeightRemain)
		.addFunction("��󱳰�����", &lua_interface::getBagWeightMax)
		.addFunction("������Ʒ����", &lua_interface::getBagGoodsNumber)
		.addFunction("��ҩ����", &lua_interface::getBagHpMedcine)
		.addFunction("��ҩ����", &lua_interface::getBagMpMedcine)
		.addFunction("��ǰ���", &lua_interface::getGoldNumber)
		.addFunction("��ǰԪ��", &lua_interface::getYbNumber)
		.addFunction("��ǰ�󶨽��", &lua_interface::getBindGoldNumber)
		.addFunction("��ǰ��Ԫ��", &lua_interface::getBindYbNumber)
		.addFunction("ʹ����Ʒ", &lua_interface::useBagGoods)
		.addFunction("���Լ�ʹ�ü���", &lua_interface::useSkillToMyself)
		.addFunction("��Ŀ��ʹ�ü���", &lua_interface::useSkillToTarget)
		.addFunction("С��", &lua_interface::smallExit)
		.addFunction("��������", &lua_interface::immdiaRebirth)
		.addFunction("�ܵ�Ŀ���", &lua_interface::runTo)
		.addFunction("�ߵ�Ŀ���", &lua_interface::walkTo)
		.addFunction("Ѱ·��", &lua_interface::gotoMapXY)
		.addFunction("��ʼս��", &lua_interface::startAttack)
		.addFunction("����ս��", &lua_interface::endAttack)
		.addFunction("�������", &lua_interface::getDistance)
		.endClass();
}


//�ض���lua��print����
int lua_interface::lua_print(lua_State* L) {
	int nargs = lua_gettop(L); // ��ȡ��������
	for (int i = 1; i <= nargs; ++i) {
		if (lua_isstring(L, i)) {
			std::cout << lua_tostring(L, i) << "\t"; // �����C++��std::cout
		}
	}
	std::cout << std::endl;
	return 0;
}

//��װpresskey
bool lua_interface::presskey(int vkcode)
{
	return mfun.presskey(::GetCurrentProcessId(),vkcode);
}


// ���ز�������lua map�ļ� ������ͼ��������ֵ�ӳ���ϵ �Լ���ͼ֮���·����ϵ
void lua_interface::load_and_store_map_data(lua_State* L, const std::string& file_path)
{
	// ���� Lua �ļ�
	int result = luaL_loadfile(L, file_path.c_str());
	if (result != 0) {
		std::cerr << "Lua error: " << lua_tostring(L, -1) << std::endl;
		lua_pop(L, 1); // �Ƴ�������Ϣ
		return;
	}

	// ִ�� Lua �ļ�
	result = lua_pcall(L, 0, LUA_MULTRET, 0);
	if (result != 0) {
		std::cerr << "Lua error: " << lua_tostring(L, -1) << std::endl;
		lua_pop(L, 1); // �Ƴ�������Ϣ
		return;
	}

	// ȷ������ֵ��һ����
	if (!lua_istable(L, -1)) {
		std::cerr << "Ԥ�ڷ���һ����" << std::endl;
		return;
	}

	// ��ȡȫ�ֱ�
	lua_pushnil(L); // ��һ����
	while (lua_next(L, -2) != 0) {
		// 'key' ����ջ�� (-1)��'value' ����ջ������һ��λ�� (-2)
		if (lua_type(L, -2) == LUA_TSTRING) {
			const char* key = lua_tostring(L, -2);

			if (std::string(key) == "map_names") {
				if (!lua_istable(L, -1)) {
					std::cerr << "map_names ���Ǳ�����" << std::endl;
					lua_pop(L, 1); // �Ƴ� 'value'
					continue;
				}
				lua_pushnil(L); // ��һ����
				while (lua_next(L, -2) != 0) {
					if (lua_type(L, -2) != LUA_TSTRING || lua_type(L, -1) != LUA_TSTRING) {
						std::cerr << "map_names ����ֵ�����ַ�������" << std::endl;
						lua_pop(L, 1); // �Ƴ� 'value'������ 'key' �Ա��������
						continue;
					}
					const char* map_id = lua_tostring(L, -2);
					const char* map_name = lua_tostring(L, -1);
					map_names[map_id] = map_name;
					lua_pop(L, 1); // �Ƴ� 'value'������ 'key' �Ա��������
				}
			}
			else if (std::string(key) == "transitions") {
				if (!lua_istable(L, -1)) {
					std::cerr << "transitions ���Ǳ�����" << std::endl;
					lua_pop(L, 1); // �Ƴ� 'value'
					continue;
				}
				lua_pushnil(L); // ��һ����
				while (lua_next(L, -2) != 0) {
					if (lua_type(L, -2) != LUA_TSTRING || !lua_istable(L, -1)) {
						std::cerr << "transitions �������ַ������ͻ�ֵ���Ǳ�����" << std::endl;
						lua_pop(L, 1); // �Ƴ� 'value'������ 'key' �Ա��������
						continue;
					}
					const char* from_map = lua_tostring(L, -2);
					TransitionMap transition_map;

					lua_pushnil(L); // ��һ����
					while (lua_next(L, -2) != 0) {
						if (lua_type(L, -2) != LUA_TSTRING || !lua_istable(L, -1)) {
							std::cerr << "transitions �ڲ��������ַ������ͻ�ֵ���Ǳ�����" << std::endl;
							lua_pop(L, 1); // �Ƴ� 'value'������ 'key' �Ա��������
							continue;
						}
						const char* to_map = lua_tostring(L, -2);
						std::vector<Position> positions;

						lua_pushnil(L); // ��һ����
						while (lua_next(L, -2) != 0) {
							if (!lua_istable(L, -1)) {
								std::cerr << "positions ֵ���Ǳ�����" << std::endl;
								lua_pop(L, 1); // �Ƴ� 'value'������ 'key' �Ա��������
								continue;
							}
							Position pos;
							lua_pushnil(L); // ��һ����
							while (lua_next(L, -2) != 0) {
								if (lua_type(L, -2) != LUA_TSTRING || lua_type(L, -1) != LUA_TNUMBER) {
									std::cerr << "positions ����ֵ������������" << std::endl;
									lua_pop(L, 1); // �Ƴ� 'value'������ 'key' �Ա��������
									continue;
								}
								const char* coord_key = lua_tostring(L, -2);
								if (std::string(coord_key) == "x") {
									pos.x = static_cast<int>(lua_tointeger(L, -1));
								}
								else if (std::string(coord_key) == "y") {
									pos.y = static_cast<int>(lua_tointeger(L, -1));
								}
								lua_pop(L, 1); // �Ƴ� 'value'������ 'key' �Ա��������
							}

							positions.push_back(pos);

							lua_pop(L, 1); // �Ƴ� 'value'������ 'key' �Ա��������
						}

						transition_map[to_map] = positions;
						lua_pop(L, 1); // �Ƴ� 'value'������ 'key' �Ա��������
					}
					transitions[from_map] = transition_map;

					// ���û���ҵ��κ� 'to_map'�������ʾ��Ϣ
					if (transition_map.empty()) {
						std::cout << "Warning: No transitions found for From map: " << from_map << std::endl;
					}
					lua_pop(L, 1); // �Ƴ� 'value'������ 'key' �Ա��������
				}
			}
		}
		lua_pop(L, 1); // �Ƴ� 'value'������ 'key' �Ա��������
	}
}

//�ҵ���ͼ·��
bool lua_interface::find_path(const std::string& start_name, const std::string& end_name, std::vector<std::string>& path) {
	std::unordered_map<std::string, std::string> came_from;
	std::queue<std::string> queue;

	// ���ݵ�ͼ�����ҵ���Ӧ��ID
	std::string start_id, end_id;
	for (const auto& pair : map_names) {
		if (pair.second == start_name) {
			start_id = pair.first;
		}
		if (pair.second == end_name) {
			end_id = pair.first;
		}
	}

	if (start_id.empty() || end_id.empty()) {
		std::cerr << "δ�ҵ�ָ���ĵ�ͼ����" << std::endl;
		return false;
	}

	queue.push(start_id);
	came_from[start_id] = "";

	while (!queue.empty()) {
		std::string current = queue.front();
		queue.pop();

		if (current == end_id) {
			// ����·��
			std::string node = end_id;
			while (!node.empty()) {
				path.insert(path.begin(), node);
				node = came_from[node];
			}
			return true;
		}

		auto it = transitions.find(current);
		if (it != transitions.end()) {
			for (const auto& transition : it->second) {
				const std::string& to_map = transition.first;
				if (came_from.find(to_map) == came_from.end()) {
					queue.push(to_map);
					came_from[to_map] = current;
				}
			}
		}
	}

	return false;
}

//�ҵ���ͼ·���͹�ͼ����
std::vector<std::pair<std::string, std::vector<Position>>> lua_interface::find_path_with_positions(const std::string& start_name, const std::string& end_name)
{
	std::vector<std::pair<std::string, std::vector<Position>>> path_with_positions; // �洢·���͹�ͼ����
	std::vector<std::string> path;// �洢·��
	if (!find_path(start_name, end_name, path))
	{
		std::cerr << "Failed to find path from " << start_name << " to " << end_name << std::endl;
		return path_with_positions;
	}
	for (size_t i = 0; i < path.size() - 1; ++i) {
		const std::string& from_map = path[i];
		const std::string& to_map = path[i + 1];

		auto it_from = transitions.find(from_map);
		if (it_from != transitions.end()) {
			auto it_to = it_from->second.find(to_map);
			if (it_to != it_from->second.end()) {
				path_with_positions.push_back({ to_map, it_to->second });
			}
			else {
				std::cerr << "δ�ҵ��� " << from_map << " �� " << to_map << " �Ĺ��ɵ�" << std::endl;
			}
		}
		else {
			std::cerr << "δ�ҵ���ͼ " << from_map << " �Ĺ�����Ϣ" << std::endl;
		}
	}

	return path_with_positions;
}

//ͨ�����ֻ�ȡ�����ΧNPCID
DWORD lua_interface::getEviroNPCIdByName(std::string npcName)
{
	if (!r.Get_Envionment(m_obj.p_pets, m_obj.p_npcs, m_obj.p_monster, m_obj.p_players))
	{
		std::cout << "������Χ����!!!" << std::endl;
		return -1;
	}

	for (size_t i = 0; i < m_obj.p_npcs.size(); i++)
	{
		if (npcName.compare((char*)(m_obj.p_npcs[i]+0x10)) == 0) return *(DWORD*)(m_obj.p_npcs[i] + 8);  // ����ƥ���NPC��ID
	}
		return -1;
}

// ͨ�� NPC ��������
bool lua_interface::interactWithNPC(const std::string& npcName, const std::string& command, const std::function<bool(int)>& action) {
	auto npcId = getEviroNPCIdByName(npcName);
	if (npcId == -1) {
		std::cerr << "�޷��ҵ� NPC: " << npcName << std::endl;
		return false;
	}

	if (!mfun.OpendNPC(npcId)) {
		std::cerr << "�� NPC �Ի���ʧ��: " << npcName << std::endl;
		return false;
	}

	Sleep(50);

	if (!mfun.ChooseCmd(command.c_str())) {
		std::cerr << "ѡ������ʧ��: " << command << std::endl;
		return false;
	}

	Sleep(50);

	return action(npcId);
}

// ͨ������Ʒ����
bool lua_interface::sellItems(const std::vector<DWORD>& bagIndexSell, int npcId) {
	for (auto index : bagIndexSell) {
		const auto& item = r_bag.m_bag[index];
		if (!mfun.sellGoods(std::string(item.pName), *item.ID, npcId)) {
			std::cerr << "������Ʒʧ��: " << item.pName << std::endl;
			return false;
		}
		memset((DWORD*)item.Name_Length, 0, 0x688); // �������ֶ�
		Sleep(50);
	}
	return true;
}

/*��װ��Ϸ����------------------------------------------------------------------------------------------------*/

//�ύ�ͽ�����
void lua_interface::applySJLP()
{
	int num = r_bag.caclGoodsNumber("�ͽ�����");
	std::cout <<"�ͽ����Ƶ�����Ϊ��" << num << std::endl;
	if (num > 0)
	{
		auto npcid = getEviroNPCIdByName("�ͽ�����");
		if (npcid == -1)return;
		mfun.OpendNPC(npcid);
		for (auto i = 0; i < num; ++i)
		{
			Sleep(50);
			mfun.ChooseCmd("@givepai");
			Sleep(50);
			mfun.ChooseCmd("@main");
		}
	}
}

//��ҩ  ����һ������  ������������ ����bool
bool lua_interface::buyMedicine(const std::string& medName, BYTE num) {
	return interactWithNPC("ҩ���ƹ�", "@bindbuy", [&](int npcId) {
		if (*r.m_roleproperty.p_Role_GoldBind > 100000) { // ʹ�ð󶨽��
			if (!mfun.buyGoods(medName, npcId, 1, num)) {
				std::cerr << "����ҩƷʧ��: " << medName << std::endl;
				return false;
			}
			return true;
		}
		else if (*r.m_roleproperty.p_Role_Gold > 100000) { // ʹ�ý��
			if (!mfun.buyGoods(medName, npcId, 0, num)) {
				std::cerr << "����ҩƷʧ��: " << medName << std::endl;
				return false;
			}
			return true;
		}
		else {
			std::cerr << "��ҺͰ󶨽�Ҳ��㣬����ʧ��: " << medName << std::endl;
			return false;
		}
		});
}

//������Ʒ ����:������Ʒ�ı�����������
bool lua_interface::storeGoods(const std::vector<DWORD>& bagIndexStore) {
	return interactWithNPC("�ֿⱣ��Ա", "@storage", [&](int npcId) {
		for (auto index : bagIndexStore) {
			const auto& item = r_bag.m_bag[index];
			if (!mfun.storeGoods(std::string(item.pName), *item.ID, npcId)) {
				std::cerr << "������Ʒʧ��: " << item.pName << std::endl;
				return false;
			}
			memset((DWORD*)item.Name_Length, 0, 0x688); // �������ֶ�
			Sleep(50);
		}
		return true;
		});
}

//��ҩ ����:������Ʒ�ı�����������
bool lua_interface::sellMedicine(const std::vector<DWORD>& bagIndexMedSell) {
	return interactWithNPC("ҩ���ƹ�", "@sell", [&](int npcId) {
		for (auto index : bagIndexMedSell) {
			const auto& item = r_bag.m_bag[index];
			if (!mfun.sellGoods(std::string(item.pName), *item.ID, npcId)) {
				std::cerr << "����ҩƷʧ��: " << item.pName << std::endl;
				return false;
			}
			memset((DWORD*)item.Name_Length, 0, 0x688); // �������ֶ�
			Sleep(50);
		}

		// ����ʣ��ҩƷ
		for (const auto& kv : r_bag.m_bag) {
			if (kv.howProcess == 4 && kv.goods_type == 4 && kv.remainNumbers < (DWORD)r_bag.caclGoodsNumber(kv.pName)) {
				if (!mfun.sellGoods(std::string(kv.pName), *kv.ID, npcId)) {
					std::cerr << "����ʣ��ҩƷʧ��: " << kv.pName << std::endl;
					return false;
				}
				memset((DWORD*)kv.Name_Length, 0, 0x688); // �������ֶ�
				Sleep(50);
			}
		}
		return true;
		});
}

//���·� ������Ʒ�ı�����������
bool lua_interface::sellClothes(const std::vector<DWORD>& bagIndexCloSell) {
	return interactWithNPC("��װ���ƹ�", "@sell", [&](int npcId) {
		return sellItems(bagIndexCloSell, npcId);
		});
}

//������ ������Ʒ�ı�����������
bool lua_interface::sellJewelry(const std::vector<DWORD>& bagIndexJeSell) {
	return interactWithNPC("���ε��ƹ�", "@sell", [&](int npcId) {
		return sellItems(bagIndexJeSell, npcId);
		});
}

//������ ������Ʒ�ı�����������
bool lua_interface::sellWeapon(const std::vector<DWORD>& bagIndexWpSell) {
	return interactWithNPC("����", "@sell", [&](int npcId) {
		return sellItems(bagIndexWpSell, npcId);
		});
}


//����storeANDsell.ini ������Ҫ��ֺ���������Ʒ�����б�
std::vector<std::string>  bag::StoreVec, bag::SellWeaponVec, bag::SellClothesVec, bag::SellJewelryVec;//�ֱ�洢 �����Ʒ ������ �·� ���� ����
std::map<std::string, DWORD> bag::SellMedicineVec;//�洢 ��ҩƷ ������ ��ʣ������
bool bag::initGoodsProcWayList()
{
	static bool initialized = false;
	if (!initialized) {
		std::string cfgpath = std::string(shareCli.m_pSMAllData->currDir) + "cfg\\" + r.m_roleproperty.Object.pName + "\\" + "storeANDsell.ini";
		auto data = tools::getInstance()->parseIniFile(cfgpath);
		if (data.empty())std::cerr << "OpenFile error����" << std::endl;
		for (const auto& section : data)
		{
			if (section.first == std::string("�ֿ�"))
			{
				for (const auto& kv : section.second)
				{
					StoreVec.push_back(kv.first);
					}
			}
			else if (section.first == std::string("�·�"))
			{
				for (const auto& kv : section.second)
				{
					SellClothesVec.push_back(kv.first);
				}
			}
			else if (section.first == std::string("����"))
			{
				for (const auto& kv : section.second)
				{
					SellJewelryVec.push_back(kv.first);
					}
			}
			else if (section.first == std::string("����"))
			{
				for (const auto& kv : section.second)
				{
					SellWeaponVec.push_back(kv.first);

				}
			}
			else if (section.first == std::string("ҩƷ"))
			{
				for (const auto& kv : section.second) {
					try {
						DWORD value = std::stoul(kv.second);
						SellMedicineVec[kv.first] = value;
					}
					catch (const std::invalid_argument& e) {
						std::cerr << "Invalid argument: " << e.what() << std::endl;
					}
					catch (const std::out_of_range& e) {
						std::cerr << "Out of range: " << e.what() << std::endl;
					}
				}
			}

		}
		initialized = true;
		return true;
	}
	else return false;
}

//׷�Ӵ洢NPC ��Ϣ��Lua�ļ���
void lua_interface::saveNPCsToLua(const std::vector<NPC>& npcs, const std::string& filePath) {
	std::ofstream outputFile(filePath, std::ios::app); // ʹ��׷��ģʽ
	if (outputFile.is_open()) {
		std::map<std::string, std::unordered_map<NPC, bool, NPCHash>> mapNPCs;

		// �� NPC ����ͼ���鲢ȥ��
		for (const auto& npc : npcs) {
			mapNPCs[npc.mapName][npc] = true;
		}

		// ����ļ��Ƿ�Ϊ�գ����Ϊ����д���ͷ
		outputFile.seekp(0, std::ios::end);
		bool isEmpty = (outputFile.tellp() == 0);
		if (isEmpty) {
			outputFile << "npcs = {\n";
		}

		for (const auto& mapEntry : mapNPCs) {
			const std::string& mapName = mapEntry.first;
			const std::unordered_map<NPC, bool, NPCHash>& mapNPCList = mapEntry.second;

			// ���õ�ͼ�Ƿ��Ѿ�����
			outputFile.seekp(0, std::ios::end);
			bool mapExists = false;
			std::string line;
			std::ifstream checkFile(filePath);
			if (checkFile.is_open()) {
				while (std::getline(checkFile, line)) {
					if (line.find("[\"" + mapName + "\"]") != std::string::npos) {
						mapExists = true;
						break;
					}
				}
				checkFile.close();
			}

			if (!mapExists) {
				outputFile << "    [\"" << mapName << "\"] = {\n";
			}

			for (const auto& npcEntry : mapNPCList) {
				const NPC& npc = npcEntry.first;

				// ���� NPC �Ƿ��Ѿ�����
				if (existingNPCs[mapName].find(npc) == existingNPCs[mapName].end()) {
					outputFile << "        [\"" << npc.npcName << "\"] = { x = " << npc.x << ", y = " << npc.y << " },\n";
					existingNPCs[mapName][npc] = true;
				}
			}

			if (!mapExists) {
				outputFile << "    },\n";
			}
		}

		// ����ļ�Ϊ�գ�д���β
		if (isEmpty) {
			outputFile << "}\n";
		}

		outputFile.close();
		std::cout << "NPCs saved to " << filePath << std::endl;
	}
	else {
		std::cerr << "Unable to open file " << filePath << std::endl;
	}
}