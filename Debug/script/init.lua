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
    local sep = package.config:sub(1, 1) -- ��ȡ·���ָ���
    local subdirs = get_subdirectories(directory)   -- ��ȡ������Ŀ¼
    -- ��ӵ� package.path
    for _, subdir in ipairs(subdirs) do

        local custom_path = subdir .. sep .. "?.lua"
        if not package.path:find(custom_path, 1, true) then
            package.path = package.path .. ";" .. custom_path
        end
    end

-- ��ӵ� package.cpath
    for _, subdir in ipairs(subdirs) do
        local custom_dll_path = subdir .. sep .. "?" .. (sep == "\\" and ".dll" or ".so")
        if not package.cpath:find(custom_dll_path, 1, true) then
            package.cpath = package.cpath .. ";" .. custom_dll_path
        end
    end
end

add_directories_to_package_paths(currentDir) --��ʼ��Ŀ¼

socket = require("socket")  -- ���� LuaSocket

sleepers = {}
-- ������ sleep ������ʹ�� socket.gettime ��߾��ȣ�
sleep=function(ms)
    -- ������ת��Ϊ�룬��Ϊ socket.gettime ���ص�������
    local resume_time = socket.gettime() + (ms / 1000.0)
    table.insert(sleepers, {coroutine.running(), resume_time})
    coroutine.yield()
end



-- ����ȫ�ִ�������
TriggerSystem = {}
TriggerSystem.__index = TriggerSystem

-- ���캯��
function TriggerSystem:new()
    local obj = {
        triggers = {},  -- ��������
        isRunning = false  -- ��ʱ������״̬
    }
    setmetatable(obj, self)
    return obj
end

-- ��Ӵ������ĺ���
function TriggerSystem:addTrigger(conditionFunc, label)
    table.insert(self.triggers, { condition = conditionFunc, label = label })
end

-- ������д������ĺ���
function TriggerSystem:clearTriggers()
    self.triggers = {}  -- ��մ�������
	self.isRunning = false  -- ֹͣ��ʱ��
    print("��������д�����!")
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


-- ����ȫ�� lua_interface ����
game=LuaInterface()
ScriptFLag=false --���ƽű�ִ�е�Э�̱�־
AutoFlag=false --�����Զ���ֵ�Э�̱�־


-- ����һ����Э�̣���������ִ�л���Ϊȫ�ֻ���
local function create_global_coroutine(func)
    local co_func = function(...)
        if setfenv then
            setfenv(1, _G) -- ���õ�ǰ�����Ļ���Ϊȫ�ֻ���
        else
            _ENV = _G -- ���� Lua 5.2 �����ϰ汾
        end
        return func(...)
    end
    return coroutine.create(co_func)
end


-- �Զ����Э��
function auto_combat_coroutine()
    while not AutoFlag do
	game:��ʼս��()
        sleep(1000)
    end
 game:ֹͣս��()
 coroutine.yield() -- ��ͣЭ�̣��ȴ���һ�λָ�
end

-- ִ�нű���Э��
function script_executor_coroutine()
    while true do
        -- ���սű�·��
        local script_path = coroutine.yield()
		if script_path then
			if not ScriptFLag then
					-- ���ؽű�
					local chunk, err = loadfile(script_path)
					if not chunk then
						print("Failed to load script: " .. (err or "no error message"))
					else
						-- ִ�нű�
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



-- ����ȫ�� ������
 ������ = TriggerSystem:new()

��Ӵ�����=function(condition, label)
	������:addTrigger(condition, label)  -- ��Ӵ�����1
end


-- �������������
����������=function()
	������.isRunning = true  -- ������ʱ��
	������:checkTriggers()
	print("���������������...")
end

-- �������������
���������= function()
	������:clearTriggers()
	print("��������д�����...")
end


-- ��̬���ݽű�·��
function ���нű�(path)
    ScriptFLag = false -- ����ֹͣ��־
    if coroutine.status(script_executor_co) == "dead" then
        -- ���Э����ֹͣ�����´���
        script_executor_co = create_global_coroutine(script_executor_coroutine)
    end

	--coroutine.resume(script_executor_co)

    -- ���ű�·�����ݸ�Э��
    local success, err = coroutine.resume(script_executor_co, path)
    if not success then
        print("Э��ִ��ʧ��: " .. tostring(err))
    end
end

-- ֹͣ�ű�
ֹͣ�ű�=function()
    ScriptFLag = true -- ����ֹͣ��־
    print("�ű�����ֹͣ...")
end

-- �Զ�ս��
�Զ�ս��=function()
    AutoFlag = false -- ����ֹͣ��־
    if coroutine.status(auto_combat_co) == "dead" then
        -- ���Э����ֹͣ�����´���
          auto_combat_co = create_global_coroutine(auto_combat_coroutine)
    end
    local status, err = coroutine.resume(auto_combat_co)
    if not status then
        print("Failed to start auto combat: " .. err)
        print("Stack trace:", debug.traceback())
    end
end

-- ֹͣս��
ֹͣս��=function()
    AutoFlag = true -- ����ֹͣ��־
    print("ֹͣս��...")
end


-- ����Э��
function init()
auto_combat_co = create_global_coroutine(auto_combat_coroutine) --�Զ����Э��
script_executor_co = create_global_coroutine(script_executor_coroutine) --ִ�нű�Э��


-- ����Э��
  --coroutine.resume(auto_combat_co)
  coroutine.resume(script_executor_co)
  --coroutine.resume(res_chedul_co)

end

-- ���г�ʼ��
init()

print(package.path)
print("\n")
print(package.cpath)

-- ��������ѭ��
-- ���Ե���
���нű�("D:\\LJH\\VS_PROJECT\\ConsoleFrame\\Debug\\script\\1.lua")
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
                print("Э�̴���:", err)
            end
            processed = processed + 1
            if processed >= 100 then break end -- ��ֹһ���Դ���̫������
        end
    end
    socket.sleep(0.01) -- ʹ�� socket.sleep �������CPUռ��
end

