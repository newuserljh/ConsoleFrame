#include <lua.hpp>
#include <unordered_map>
#include <string>
#include <iostream>
#include <memory>
#include <mutex>

// LuaTrigger 类定义
class LuaTrigger {
public:
    LuaTrigger() = default;
    ~LuaTrigger() = default;

    // 注册标签函数（供 Lua 调用）
    void register_label(const std::string& triggerName, const std::string& label) {
        std::lock_guard<std::mutex> lock(triggerMutex); // 加锁

        if (triggerMap.find(triggerName) == triggerMap.end()) {
            throw std::runtime_error("Trigger does not exist");
        }

        auto& labelMap = triggerMap[triggerName];
        if (labelMap.find(label) != labelMap.end()) {
            throw std::runtime_error("Label already exists for trigger");
        }

        labelMap[label] = luaState; // 将当前 Lua 状态与标签关联
    }

    // 触发跳转函数（供 Lua 调用）
    std::string trigger(const std::string& triggerName, const std::string& targetLabel) {
        std::lock_guard<std::mutex> lock(triggerMutex); // 加锁

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

        // 检查目标状态是否有效
        if (!targetState) {
            throw std::runtime_error("Invalid target state");
        }

        // 检查目标状态是否有效
        if (lua_status(targetState)) {
            throw std::runtime_error("Target state is not in a valid state");
        }

        return "triggered"; // 返回一个标志表示触发成功
    }

    // 设置 Lua 状态
    void setLuaState(lua_State* L) {
        luaState = L;
    }

private:
    std::unordered_map<std::string, std::unordered_map<std::string, lua_State*>> triggerMap;
    std::mutex triggerMutex;
    lua_State* luaState = nullptr; // 当前 Lua 状态
};

