-- ��ӡ��ǰ��package.cpathֵ�Ա��˽�Ĭ������
print(package.cpath)

-- ����Զ���·���� package.path
package.path = package.path .. ";D:/LJH/VS_PROJECT/ConsoleFrame/Debug/map/?.lua"

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
