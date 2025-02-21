-- 创建全局 lua_interface 对象
game=LuaInterface 
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
    -- 获取所有子目录
    local subdirs = get_subdirectories(directory)
    -- 添加到 package.path
    for _, subdir in ipairs(subdirs) do
        local sep = package.config:sub(1, 1) -- 获取路径分隔符
        local custom_path = subdir .. sep .. "?.lua"
        if not package.path:find(custom_path, 1, true) then
            package.path = package.path .. ";" .. custom_path
        end
    end
end

-- 触发器协程
local function trigger_coroutine()
    while true do
        -- 模拟触发器逻辑
        local event = wait_for_event() -- 假设有一个函数 wait_for_event 来等待事件
        if event == "trigger_event" then
            print("Trigger event detected!")
            -- 执行触发器逻辑
            -- 例如：trigger:trigger("my_trigger", "my_label")
        end
        coroutine.yield() -- 暂停协程，等待下一次恢复
    end
end

-- 自动打怪协程
local function auto_combat_coroutine()
        if(game:开始战斗()) then
            print("开始自动打怪...")
        else
            print("开启自动打怪失败.正在结束脚本...")
        end
    while true do
        -- 模拟自动打怪逻辑
        if not luaStopFlag then
            -- 执行自动打怪逻辑
            -- 例如：attack_nearest_monster()
        else
            game:停止战斗()
            print("停止打怪...")
            break -- 停止协程
        end
        coroutine.yield() -- 暂停协程，等待下一次恢复
    end
end

-- 执行脚本的协程
local function script_executor_coroutine()
    while true do
        -- 接收脚本路径
        local script_path = coroutine.yield()
        if not luaStopFlag then
            if script_path then
                print("Script executor: Running script...")
                -- 执行脚本逻辑
                local status, err = pcall(dofile, script_path)
                if not status then
                    print("Script execution failed: " .. err)
                end
            else
                print("No script path provided.")
            end
        else
            print("Script executor stopped.")
            break -- 停止协程
        end
    end
end

-- 启动协程
local function start_coroutines()
    coroutine.resume(trigger_co) -- 启动触发器协程
    coroutine.resume(auto_combat_co) -- 启动自动打怪协程）
    coroutine.resume(script_executor_co) -- 启动脚本执行协程-- 启动协程（第一次恢复时不传递参数)
end

-- 动态传递脚本路径
local function 运行脚本(path)
    coroutine.resume(script_executor_co, path)
end

-- 停止协程
local function 停止脚本()
    luaStopFlag = true -- 设置停止标志
    print("All coroutines stopped.")
end

-- 初始化
local function init()
    -- 获取当前目录
    local currentDir = get_current_directory()
    print("Current directory: " .. currentDir)

    -- 动态添加目录到 package.path
    local sep = package.config:sub(1, 1) -- 获取路径分隔符
    local custom_path = currentDir .. sep .. "?.lua"
    if not package.path:find(custom_path, 1, true) then
        package.path = package.path .. ";" .. custom_path
    end
    add_directories_to_package_paths(currentDir)

    -- 启动协程
    start_coroutines()
end

-- 创建协程
local trigger_co = coroutine.create(trigger_coroutine) --触发器
local auto_combat_co = coroutine.create(auto_combat_coroutine) --自动打怪

-- 创建执行脚本的协程
local script_executor_co = coroutine.create(script_executor_coroutine)

-- 运行初始化
init()