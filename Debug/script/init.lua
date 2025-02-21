-- ����ȫ�� lua_interface ����
game=LuaInterface 
-- ��ȡ��ǰ�ļ���·��
function get_current_directory()
    local path = debug.getinfo(1, "S").source:sub(2)
    local directory = path:match("(.*[/\\])")
    return directory or ""
end

-- ��ȡָ��Ŀ¼�µ�������Ŀ¼�������� Windows��
function get_subdirectories(directory)
    local subdirs = {}
    local handle = io.popen('dir /AD /B /S "' .. directory .. '"')
    for dir in handle:lines() do
        table.insert(subdirs, dir)
    end
    handle:close()
    return subdirs
end

-- ���Ŀ¼������Ŀ¼�� package.path �� package.cpath
function add_directories_to_package_paths(directory)
    -- ��ȡ������Ŀ¼
    local subdirs = get_subdirectories(directory)
    -- ��ӵ� package.path
    for _, subdir in ipairs(subdirs) do
        local sep = package.config:sub(1, 1) -- ��ȡ·���ָ���
        local custom_path = subdir .. sep .. "?.lua"
        if not package.path:find(custom_path, 1, true) then
            package.path = package.path .. ";" .. custom_path
        end
    end
end

-- ������Э��
local function trigger_coroutine()
    while true do
        -- ģ�ⴥ�����߼�
        local event = wait_for_event() -- ������һ������ wait_for_event ���ȴ��¼�
        if event == "trigger_event" then
            print("Trigger event detected!")
            -- ִ�д������߼�
            -- ���磺trigger:trigger("my_trigger", "my_label")
        end
        coroutine.yield() -- ��ͣЭ�̣��ȴ���һ�λָ�
    end
end

-- �Զ����Э��
local function auto_combat_coroutine()
        if(game:��ʼս��()) then
            print("��ʼ�Զ����...")
        else
            print("�����Զ����ʧ��.���ڽ����ű�...")
        end
    while true do
        -- ģ���Զ�����߼�
        if not luaStopFlag then
            -- ִ���Զ�����߼�
            -- ���磺attack_nearest_monster()
        else
            game:ֹͣս��()
            print("ֹͣ���...")
            break -- ֹͣЭ��
        end
        coroutine.yield() -- ��ͣЭ�̣��ȴ���һ�λָ�
    end
end

-- ִ�нű���Э��
local function script_executor_coroutine()
    while true do
        -- ���սű�·��
        local script_path = coroutine.yield()
        if not luaStopFlag then
            if script_path then
                print("Script executor: Running script...")
                -- ִ�нű��߼�
                local status, err = pcall(dofile, script_path)
                if not status then
                    print("Script execution failed: " .. err)
                end
            else
                print("No script path provided.")
            end
        else
            print("Script executor stopped.")
            break -- ֹͣЭ��
        end
    end
end

-- ����Э��
local function start_coroutines()
    coroutine.resume(trigger_co) -- ����������Э��
    coroutine.resume(auto_combat_co) -- �����Զ����Э�̣�
    coroutine.resume(script_executor_co) -- �����ű�ִ��Э��-- ����Э�̣���һ�λָ�ʱ�����ݲ���)
end

-- ��̬���ݽű�·��
local function ���нű�(path)
    coroutine.resume(script_executor_co, path)
end

-- ֹͣЭ��
local function ֹͣ�ű�()
    luaStopFlag = true -- ����ֹͣ��־
    print("All coroutines stopped.")
end

-- ��ʼ��
local function init()
    -- ��ȡ��ǰĿ¼
    local currentDir = get_current_directory()
    print("Current directory: " .. currentDir)

    -- ��̬���Ŀ¼�� package.path
    local sep = package.config:sub(1, 1) -- ��ȡ·���ָ���
    local custom_path = currentDir .. sep .. "?.lua"
    if not package.path:find(custom_path, 1, true) then
        package.path = package.path .. ";" .. custom_path
    end
    add_directories_to_package_paths(currentDir)

    -- ����Э��
    start_coroutines()
end

-- ����Э��
local trigger_co = coroutine.create(trigger_coroutine) --������
local auto_combat_co = coroutine.create(auto_combat_coroutine) --�Զ����

-- ����ִ�нű���Э��
local script_executor_co = coroutine.create(script_executor_coroutine)

-- ���г�ʼ��
init()