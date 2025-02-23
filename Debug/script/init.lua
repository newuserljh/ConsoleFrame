-- 获取当前文件的路径
function get_current_directory()
    local path = debug.getinfo(1, "S").source:sub(2)
    local directory = path:match("(.*[/\\])")
    return directory or ""
end

-- 获取指定目录下的所有子目录（适用于 Windows）
function get_subdirectories(directory)
    local subdirs = {}
    local handle = io.popen('dir /AD /B /S "' .. directory .. '"')
    for dir in handle:lines() do
        table.insert(subdirs, dir)
    end
    handle:close()
    return subdirs
end

-- 添加目录及其子目录到 package.path 和 package.cpath
function add_directories_to_package_paths(directory)
    local sep = package.config:sub(1, 1) -- 获取路径分隔符
    local subdirs = get_subdirectories(directory)   -- 获取所有子目录
    -- 添加到 package.path
    for _, subdir in ipairs(subdirs) do

        local custom_path = subdir .. sep .. "?.lua"
        if not package.path:find(custom_path, 1, true) then
            package.path = package.path .. ";" .. custom_path
        end
    end

-- 添加到 package.cpath
    for _, subdir in ipairs(subdirs) do
        local custom_dll_path = subdir .. sep .. "?" .. (sep == "\\" and ".dll" or ".so")
        if not package.cpath:find(custom_dll_path, 1, true) then
            package.cpath = package.cpath .. ";" .. custom_dll_path
        end
    end
end

add_directories_to_package_paths(currentDir) --初始化目录

socket = require("socket")  -- 引入 LuaSocket

sleepers = {}
-- 非阻塞 sleep 函数（使用 socket.gettime 提高精度）
sleep=function(ms)
    -- 将毫秒转换为秒，因为 socket.gettime 返回的是秒数
    local resume_time = socket.gettime() + (ms / 1000.0)
    table.insert(sleepers, {coroutine.running(), resume_time})
    coroutine.yield()
end



-- 定义全局触发器类
TriggerSystem = {}
TriggerSystem.__index = TriggerSystem

-- 构造函数
function TriggerSystem:new()
    local obj = {
        triggers = {},  -- 触发器表
        isRunning = false  -- 定时器运行状态
    }
    setmetatable(obj, self)
    return obj
end

-- 添加触发器的函数
function TriggerSystem:addTrigger(conditionFunc, label)
    table.insert(self.triggers, { condition = conditionFunc, label = label })
end

-- 清除所有触发器的函数
function TriggerSystem:clearTriggers()
    self.triggers = {}  -- 清空触发器表
	self.isRunning = false  -- 停止定时器
    print("已清除所有触发器!")
end

function TriggerSystem:checkTriggers()
    while self.isRunning do
        for i, trigger in ipairs(self.triggers) do
            if trigger.condition() then
                goto(trigger.label)
            end
        end
        sleep(3000)
    end
end


-- 创建全局 lua_interface 对象
game=LuaInterface()
ScriptFLag=false --控制脚本执行的协程标志
AutoFlag=false --控制自动打怪的协程标志


-- 创建一个新协程，并设置其执行环境为全局环境
local function create_global_coroutine(func)
    local co_func = function(...)
        if setfenv then
            setfenv(1, _G) -- 设置当前函数的环境为全局环境
        else
            _ENV = _G -- 对于 Lua 5.2 及以上版本
        end
        return func(...)
    end
    return coroutine.create(co_func)
end


-- 自动打怪协程
function auto_combat_coroutine()
    while not AutoFlag do
	game:开始战斗()
        sleep(1000)
    end
 game:停止战斗()
 coroutine.yield() -- 暂停协程，等待下一次恢复
end

-- 执行脚本的协程
function script_executor_coroutine()
    while true do
        -- 接收脚本路径
        local script_path = coroutine.yield()
		if script_path then
			if not ScriptFLag then
					-- 加载脚本
					local chunk, err = loadfile(script_path)
					if not chunk then
						print("Failed to load script: " .. (err or "no error message"))
					else
						-- 执行脚本
						local status, err = pcall(chunk)
						if not status then
							print("Script execution failed: " .. err)
							print("Stack trace:", debug.traceback())
						else
							print("Script executed successfully.")
							return
						end
					end
			else
				print("Script executor stopped.")
				break
			end
		end
		coroutine.yield()
    end
end



-- 创建全局 触发器
 触发器 = TriggerSystem:new()

添加触发器=function(condition, label)
	触发器:addTrigger(condition, label)  -- 添加触发器1
end


-- 启动触发器检测
启动触发器=function()
	触发器.isRunning = true  -- 启动定时器
	触发器:checkTriggers()
	print("已启动触发器检测...")
end

-- 启动触发器检测
清除触发器= function()
	触发器:clearTriggers()
	print("已清除所有触发器...")
end


-- 动态传递脚本路径
function 运行脚本(path)
    ScriptFLag = false -- 设置停止标志
    if coroutine.status(script_executor_co) == "dead" then
        -- 如果协程已停止，重新创建
        script_executor_co = create_global_coroutine(script_executor_coroutine)
    end

	--coroutine.resume(script_executor_co)

    -- 将脚本路径传递给协程
    local success, err = coroutine.resume(script_executor_co, path)
    if not success then
        print("协程执行失败: " .. tostring(err))
    end
end

-- 停止脚本
停止脚本=function()
    ScriptFLag = true -- 设置停止标志
    print("脚本正在停止...")
end

-- 自动战斗
自动战斗=function()
    AutoFlag = false -- 设置停止标志
    if coroutine.status(auto_combat_co) == "dead" then
        -- 如果协程已停止，重新创建
          auto_combat_co = create_global_coroutine(auto_combat_coroutine)
    end
    local status, err = coroutine.resume(auto_combat_co)
    if not status then
        print("Failed to start auto combat: " .. err)
        print("Stack trace:", debug.traceback())
    end
end

-- 停止战斗
停止战斗=function()
    AutoFlag = true -- 设置停止标志
    print("停止战斗...")
end


-- 创建协程
function init()
auto_combat_co = create_global_coroutine(auto_combat_coroutine) --自动打怪协程
script_executor_co = create_global_coroutine(script_executor_coroutine) --执行脚本协程


-- 启动协程
  --coroutine.resume(auto_combat_co)
  coroutine.resume(script_executor_co)
  --coroutine.resume(res_chedul_co)

end

-- 运行初始化
init()

print(package.path)
print("\n")
print(package.cpath)

-- 调度器主循环
-- 测试调用
运行脚本("D:\\LJH\\VS_PROJECT\\ConsoleFrame\\Debug\\script\\1.lua")
print("1111111111111")

while true do
    local now = socket.gettime()
    local processed = 0
    for i = #sleepers, 1, -1 do
        if sleepers[i][2] <= now then
            local co = sleepers[i][1]
            table.remove(sleepers, i)
            local ok, err = coroutine.resume(co)
            if not ok then
                print("协程错误:", err)
            end
            processed = processed + 1
            if processed >= 100 then break end -- 防止一次性处理太多任务
        end
    end
    socket.sleep(0.01) -- 使用 socket.sleep 来避免高CPU占用
end

