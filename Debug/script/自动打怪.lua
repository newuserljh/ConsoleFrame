-- 创建 lua_interface 对象
local game = LuaInterface()

-- 打印分隔线
local function printSeparator()
    print("--------------------------------------------------")
end

-- 回城整理背包
local function goHomeAndOrganize()
    printSeparator()
    print("回城整理背包...")
    game:回城整理背包()

    -- 获取待存和待卖物品数量
    local storeCount = game:待存物品数量()
    local sellJewelryCount = game:待卖首饰数量()
    local sellClothesCount = game:待卖衣服数量()
    local sellWeaponCount = game:待卖武器数量()

    print("待存物品数量: " .. storeCount)
    print("待卖首饰数量: " .. sellJewelryCount)
    print("待卖衣服数量: " .. sellClothesCount)
    print("待卖武器数量: " .. sellWeaponCount)

    -- 存仓库
    if storeCount > 0 then
        print("正在存仓库...")
        if game:存仓库() then
            print("存仓库成功")
        else
            print("存仓库失败")
        end
    else
        print("没有待存物品")
    end

    -- 卖物品
    if sellJewelryCount > 0 then
        print("正在卖首饰...")
        if game:卖首饰() then
            print("卖首饰成功")
        else
            print("卖首饰失败")
        end
    else
        print("没有待卖首饰")
    end

    if sellClothesCount > 0 then
        print("正在卖衣服...")
        if game:卖衣服() then
            print("卖衣服成功")
        else
            print("卖衣服失败")
        end
    else
        print("没有待卖衣服")
    end

    if sellWeaponCount > 0 then
        print("正在卖武器...")
        if game:卖武器() then
            print("卖武器成功")
        else
            print("卖武器失败")
        end
    else
        print("没有待卖武器")
    end

    -- 卖药
    print("正在卖药...")
    if game:卖药() then
        print("卖药成功")
    else
        print("卖药失败")
    end
end

-- 购买药品
local function buyMedicines()
    printSeparator()
    print("购买药品...")

    -- 购买红药
    local hpMedicineCount = game:红药数量()
    if hpMedicineCount < 10 then
        print("红药数量不足，正在购买...")
        if game:买药("红药", 10 - hpMedicineCount) then
            print("购买红药成功")
        else
            print("购买红药失败")
        end
    else
        print("红药数量充足: " .. hpMedicineCount)
    end

    -- 购买蓝药
    local mpMedicineCount = game:蓝药数量()
    if mpMedicineCount < 10 then
        print("蓝药数量不足，正在购买...")
        if game:买药("蓝药", 10 - mpMedicineCount) then
            print("购买蓝药成功")
        else
            print("购买蓝药失败")
        end
    else
        print("蓝药数量充足: " .. mpMedicineCount)
    end
end

-- 移动到目标点
local function moveToTarget(x, y)
    printSeparator()
    print("正在移动到目标点 (" .. x .. ", " .. y .. ")...")

    -- 计算距离
    local distance = game:计算距离(x, y)
    print("与目标点的距离: " .. distance)

    -- 寻路到目标点
    if distance > 0 then
        if game:寻路到(x, y) then
            print("寻路到目标点成功")
        else
            print("寻路到目标点失败")
        end
    else
        print("已经在目标点")
    end
end

-- 战斗
local function startCombat()
    printSeparator()
    print("开始战斗...")

    -- 开始战斗
    if game:开始战斗() then
        print("战斗开始成功")
    else
        print("战斗开始失败")
    end

    -- 模拟攻击目标
    local targetX, targetY = 100, 200
    local targetId = 12345 -- 假设目标 ID 是 12345
    print("对目标使用技能...")
    if game:对目标使用技能("火球术", targetX, targetY, targetId) then
        print("对目标使用技能成功")
    else
        print("对目标使用技能失败")
    end

    -- 结束战斗
    if game:结束战斗() then
        print("战斗结束成功")
    else
        print("战斗结束失败")
    end
end

-- 主函数
local function main()
    print("脚本开始运行")

    -- 回城整理背包
    goHomeAndOrganize()

    -- 购买药品
    buyMedicines()

    -- 移动到目标点
    local targetX, targetY = 150, 250
    moveToTarget(targetX, targetY)

    -- 开始战斗
    startCombat()

    print("脚本运行结束")
end

-- 运行主函数
main()
