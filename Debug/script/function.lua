-- ��ȡ��ǰ�ļ���·��
function get_current_directory()
    local path = debug.getinfo(1, "S").source:sub(2)
    local directory = path:match("(.*[/\\])")
    return directory or ""
end

-- ��ȡ��ǰ�ļ�����Ŀ¼
local dir = get_current_directory()

-- ��ӡ��ǰ�� package.cpath �� package.path
print("Current package.cpath: " .. package.cpath)
print("Current package.path: " .. package.path)

-- ��̬��ӵ�ǰĿ¼�� package.path
local sep = package.config:sub(1, 1) -- ��ȡ·���ָ���
local custom_path = dir .. sep .. "?.lua"
if not package.path:find(custom_path, 1, true) then
    package.path = package.path .. ";" .. custom_path
end

-- ��ӡ���º�� package.path
print("Updated package.path: " .. package.path)

-- ���ص�ͼ����
local map = require("map_data")

-- ������������������
function bfs(startRegion, endRegion, transitions)
    local queue = {{region = startRegion, path = {region = startRegion}}}
    local visited = {}

    while #queue > 0 do
        local current = table.remove(queue, 1)
        local region = current.region
        local path = current.path

        if not visited[region] then
            visited[region] = true

            if region == endRegion then
                return path -- �����ҵ���·��
            end

            if transitions[region] then
                for nextRegion, coords in pairs(transitions[region]) do
                    if not visited[nextRegion] then
                        local newPath = {}
                        for _, v in ipairs(path) do
                            table.insert(newPath, v)
                        end
                        table.insert(newPath, {region = nextRegion, coords = coords})
                        table.insert(queue, {region = nextRegion, path = newPath})
                    end
                end
            end
        end
    end

    return nil -- ���û���ҵ�·�����򷵻�nil
end

-- ��ӡ·���͹�ͼ�㣨�����꣩
function printPathAndNodesWithCoordinates(path)
    if path then
        print("�ҵ�·��:")
        for i, node in ipairs(path) do
            print(string.format("%d. %s", i, node.region))
            if node.coords then
                for j, coord in ipairs(node.coords) do
                    print(string.format("   - ���� %d: (%d, %d)", j, coord.x, coord.y))
                end
            end
        end
    else
        print("δ�ҵ�·��")
    end
end

-- ʹ��ʾ��
local startRegion = "1"
local endRegion = "L004"
local path = bfs(startRegion, endRegion, map.transitions)

printPathAndNodesWithCoordinates(path)



