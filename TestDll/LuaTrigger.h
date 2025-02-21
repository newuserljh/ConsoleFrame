#include <lua.hpp>
#include <unordered_map>
#include <string>
#include <iostream>
#include <memory>
#include <mutex>

// LuaTrigger �ඨ��
class LuaTrigger {
public:
    LuaTrigger() = default;
    ~LuaTrigger() = default;

    // ע���ǩ�������� Lua ���ã�
    void register_label(const std::string& triggerName, const std::string& label) {
        std::lock_guard<std::mutex> lock(triggerMutex); // ����

        if (triggerMap.find(triggerName) == triggerMap.end()) {
            throw std::runtime_error("Trigger does not exist");
        }

        auto& labelMap = triggerMap[triggerName];
        if (labelMap.find(label) != labelMap.end()) {
            throw std::runtime_error("Label already exists for trigger");
        }

        labelMap[label] = luaState; // ����ǰ Lua ״̬���ǩ����
    }

    // ������ת�������� Lua ���ã�
    std::string trigger(const std::string& triggerName, const std::string& targetLabel) {
        std::lock_guard<std::mutex> lock(triggerMutex); // ����

        auto it = triggerMap.find(triggerName);
        if (it == triggerMap.end()) {
            throw std::runtime_error("Trigger does not exist");
        }

        auto& labelMap = it->second;
        auto labelIt = labelMap.find(targetLabel);
        if (labelIt == labelMap.end()) {
            throw std::runtime_error("Undefined label for trigger");
        }

        lua_State* targetState = labelIt->second;

        // ���Ŀ��״̬�Ƿ���Ч
        if (!targetState) {
            throw std::runtime_error("Invalid target state");
        }

        // ���Ŀ��״̬�Ƿ���Ч
        if (lua_status(targetState)) {
            throw std::runtime_error("Target state is not in a valid state");
        }

        return "triggered"; // ����һ����־��ʾ�����ɹ�
    }

    // ���� Lua ״̬
    void setLuaState(lua_State* L) {
        luaState = L;
    }

private:
    std::unordered_map<std::string, std::unordered_map<std::string, lua_State*>> triggerMap;
    std::mutex triggerMutex;
    lua_State* luaState = nullptr; // ��ǰ Lua ״̬
};

