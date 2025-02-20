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


-- print("currentDir: " ..currentDir)

-- ��ӡ��ǰ�� package.path
-- print("Current package.path: " .. package.path)

-- ��̬��ӵ�ǰĿ¼�� package.path
-- ��ȡ��ǰ�ļ�����Ŀ¼
local dir = currentDir
local sep = package.config:sub(1, 1) -- ��ȡ·���ָ���
local custom_path = dir .. sep .. "?.lua"
if not package.path:find(custom_path, 1, true) then
	package.path = package.path .. ";" .. custom_path
end
add_directories_to_package_paths(dir)

-- ��ӡ���º�� package.path
-- print("Updated package.path: " .. package.path)


--ֹͣ�ű�����
function stopScript()
    if not luaStopFlag then
		luaStopFlag=true
    end
end

