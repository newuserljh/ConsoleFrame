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


-- print("currentDir: " ..currentDir)

-- 打印当前的 package.path
-- print("Current package.path: " .. package.path)

-- 动态添加当前目录到 package.path
-- 获取当前文件所在目录
local dir = currentDir
local sep = package.config:sub(1, 1) -- 获取路径分隔符
local custom_path = dir .. sep .. "?.lua"
if not package.path:find(custom_path, 1, true) then
	package.path = package.path .. ";" .. custom_path
end
add_directories_to_package_paths(dir)

-- 打印更新后的 package.path
-- print("Updated package.path: " .. package.path)


--停止脚本功能
function stopScript()
    if not luaStopFlag then
		luaStopFlag=true
    end
end

