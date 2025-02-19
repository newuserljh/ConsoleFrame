-- ���� lua_interface ����
local game = LuaInterface()

function checkStopFlag()
    if stopScript then
	game:ֹͣս��()
        error("Script termination requested")
    end
end

-- ģ�� Sleep ��������λ�����룩
local function sleep(ms)
    local start = os.clock()
    while os.clock() - start < ms / 1000 do end
end

-- ��ӡ�ָ���
local function printSeparator()
    print("--------------------------------------------------")
end

-- �س�
local function goHome()
    printSeparator()
    print("�س�������...")
    game:ʹ����Ʒ("���ûس���ʯ")
    sleep(2000);
	if (game:��ǰ��ͼ��()=="����") and (game:�������(468, 226)<20) then
		game:�س�������()
	else
		goHome()
	end
end

-- ��ֿ�
local function storeGoods()
    local maxAttempts = 5
    local attempt = 1

    while attempt <= maxAttempts do
        print("���ڴ�ֿ�... (���Դ���: " .. attempt .. ")")

        if game:��ֿ�() then
            print("��ֿ�ɹ�")
            game:�س�������()

            -- ����Ƿ���ʣ����Ʒ��Ҫ�洢
            local remainingStoreCount = game:������Ʒ����()
            if remainingStoreCount == 0 then
                print("������Ʒ�ѳɹ�����ֿ�")
                break
            else
                print("���� " .. remainingStoreCount .. " ����Ʒ��Ҫ�洢")
            end
        else
            print("��ֿ�ʧ��")
        end

        attempt = attempt + 1
        if attempt > maxAttempts then
            print("�Ѵﵽ����Դ��� (" .. maxAttempts .. ")����δ�ܳɹ�����ֿ�")
            break
        end
    end
end

-- ����Ʒ��ͨ�ú���
local function sellItems(itemType, maxAttempts, npcName, npcLocation)
    local attempt = 1
    while attempt <= maxAttempts do
        print("������" .. itemType .. "... (���Դ���: " .. attempt .. ")")
        if game["��" .. itemType]() then
            print("��" .. itemType .. "�ɹ�")
            break
        else
            print("��" .. itemType .. "ʧ��")
        end
        attempt = attempt + 1
        if attempt > maxAttempts then
            print("�Ѵﵽ����Դ��� (" .. maxAttempts .. ")����δ�ܳɹ�����" .. itemType)
            break
        end
    end
end

-- ��ҩ
local function sellMedicine()
    print("������ҩ...")
    if game:��ҩ() then
	    sleep(1000)
		game:��ҩ()
		sleep(1000)
		game:��ҩ()
		sleep(1000)
		game:��ҩ()
        print("��ҩ�ɹ�")
    else
        print("��ҩʧ��")
    end
end

-- ����ҩƷ
local function buyMedicines(name, count)
    local maxAttempts = 5
    local attempt = 1

    -- ��ҩ
    local bagCount = game:������Ʒ����(name)
    if bagCount < count then
        while attempt <= maxAttempts do
            print(name.."�������㣬���ڹ���,���Դ���: ("..attempt..")")

            -- ������Ҫ���������
            local needToBuy = count - bagCount
            if game:��ҩ(name, needToBuy) then
                print("����"..name.."�ɹ�")
                bagCount = game:������Ʒ����(name)  -- ���µ�ǰ����
                if bagCount >= count then
                    print(name.."��������������: " .. count)
                    break  -- �ɹ����˳�ѭ��
                end
            else
                print("��ҩʧ��")
            end

            attempt = attempt + 1
            if attempt > maxAttempts then
                print("�Ѵﵽ����Դ��� (" .. maxAttempts .. ")����δ�ܹ����㹻��ҩ")
                break
            end
        end
    else
        print(name.."��������: " .. count)
    end

end

-- �ƶ���Ŀ��㣨�첽��⣩
local function moveToTargetAsync(x, y)
    printSeparator()
    print("�����ƶ���Ŀ��� (" .. x .. ", " .. y .. ")...")

    -- �����ʼ����
    local distance = game:�������(x, y)
    print("��Ŀ���ľ���: " .. distance)
    -- Ѱ·��Ŀ���
	local threshold = 5 -- �������Χ
    if distance > threshold  then
        if game:Ѱ·��(x, y) then
            print("Ѱ·��Ŀ���ɹ�")

            -- �첽����Ƿ񵽴�Ŀ���

            while  not stopScript do
                sleep(500) -- ÿ 500 ������һ��
                local currentX = game:��ǰ����X()
                local currentY = game:��ǰ����Y()
                local currentDistance = game:�������(x, y)
                print("��ǰλ��: (" .. currentX .. ", " .. currentY .. "), ʣ�����: " .. currentDistance)

                if currentDistance <= threshold then
                    print("����Ŀ���")
                    break
                end
            end
        else
            print("Ѱ·��Ŀ���ʧ��")
        end
    else
        print("�Ѿ���Ŀ���")
    end
end


-- ��ͼ
local function passMap(currentMapName,targetMapName,x,y,x1,y1)
    local maxAttempts = 5
    local attempt = 1
	 -- ����Ĭ��ֵ
	x1 = x1 or 0
    y1 = y1 or 0
          moveToTargetAsync(x, y)
    while attempt <= maxAttempts do
        print("���ڹ�ͼ... (���Դ���: " .. attempt .. ")")
		game:�ߵ�Ŀ���(x,y)
		sleep(1000)

        if game:��ǰ��ͼ��()==targetMapName then
            print("��ͼ�ɹ�")
            break  -- �ɹ����˳�ѭ��
        else
			if(x1~=0 and y1~=0) then
				game:�ߵ�Ŀ���(x,y)
				sleep(1000)
			end
        end

        attempt = attempt + 1
        if attempt > maxAttempts then
            print("�Ѵﵽ����Դ��� (" .. maxAttempts .. ")����δ�ܳɹ���ͼ")
            break
        end
    end
end

-- ս��
function startCombat()
    while not stopScript do
	-- ����Ƿ���Ҫ��ͣ����ֹ
        if stopScript then
            if game:ֹͣս��() then
                print("ս��ֹͣ�ɹ�")
            else
                print("ս��ֹͣʧ��")
            end
            break
        end

        checkStopFlag()
        sleep(5000)  -- ÿ 5000 ������һ��ֹͣ��־
    end
end

local function ����()
	if game:��ǰ��ͼ��()=="����" and game:�������(470,221)<20 and not stopScript then
	 	moveToTargetAsync(470,223)
		game:�Ի�NPCѡ������("�ϱ�","@main1")
		sleep(500)
		game:ѡ������("@west")
		sleep(500)
		game:ѡ������("@west1")
		sleep(1000)
	 end

	 if game:��ǰ��ͼ��()=="�����澳" and game:�������(463,110)<20 and not stopScript  then
	 	moveToTargetAsync(463,110)
		game:�Ի�NPCѡ������("�ϱ�","@main1")
		sleep(500)
		game:ѡ������("@ttta")
		sleep(500)
		game:ѡ������("@ta")
		sleep(1000)
	 end

	if game:��ǰ��ͼ��()=="�����澳" and game:�������(140, 107)<20 and not stopScript  then
	 	passMap("�����澳","һ������", 137, 102 , 137, 103)
	 end

	if stopScript then
	   return
	end

	if game:��ǰ��ͼ��()=="һ������"  and not stopScript then
    -- ��ʼս��
        print("����ս����ͼ")
	else
		goHome()
	end
end


-- ������
local function ���ﲹ��()
        -- �س�������
        goHome()
        -- ��ȡ����ʹ�����Ʒ����
        local storeCount = game:������Ʒ����()
        local sellJewelryCount = game:������������()
        local sellClothesCount = game:�����·�����()
        local sellWeaponCount = game:������������()

        print("������Ʒ����: " .. storeCount)
        print("������������: " .. sellJewelryCount)
        print("�����·�����: " .. sellClothesCount)
        print("������������: " .. sellWeaponCount)

        -- ��ֿ�
        if storeCount > 0 then
            moveToTargetAsync(455, 228)
            game:�Ի�NPCѡ������("��ݴ�����", "@Shop_GO&0&378&217")
            sleep(1000)
            moveToTargetAsync(375, 211)
            storeGoods()
            game:ʹ����Ʒ("���ûس���ʯ")
            sleep(1000)
        end

        -- ����Ʒ
    if sellJewelryCount > 0 then
        moveToTargetAsync(438, 249)
        sellItems("����", 5, "���ε��ϰ�", {438, 249})
        game:ʹ����Ʒ("���ûس���ʯ")
        sleep(1000)
    end

    if sellClothesCount > 0 then
        moveToTargetAsync(417, 206)
        passMap("����", "��װ��", 418, 205)
        sellItems("�·�", 5, "��װ���ϰ�", {418, 205})
        game:ʹ����Ʒ("���ûس���ʯ")
        sleep(1000)
    end

    if sellWeaponCount > 0 then
        moveToTargetAsync(419, 246)
        sellItems("����", 5, "�������ϰ�", {419, 246})
        game:ʹ����Ʒ("���ûس���ʯ")
        sleep(1000)
    end

        -- ����ҩƷ
        moveToTargetAsync(455, 228)
        game:�Ի�NPCѡ������("��ݴ�����", "@Shop_GO&0&502&266")
        sleep(1000)
        moveToTargetAsync(498, 268)
        sellMedicine()
        sleep(1000)
        buyMedicines("�ؼ���ҩ��", 6)
        buyMedicines("�ؼ�ħ��ҩ��", 6)
        game:ʹ����Ʒ("���ûس���ʯ")
        sleep(1000)

	-- ���ֹͣ��־
        checkStopFlag()
end

-- ������ѭ��
local combatCoroutine = nil
while not stopScript do
    print("Running...")
   ���ﲹ��()

    -- �ƶ���ս����
    ����()
    ---��ʼս��
   if game:��ʼս��() then
   	print("��ʼս��--------")
   else
	print("��ʼս��ʧ��---���ڽ����ű�...")
	break
   end

-- ����ս��Э�̣������û��������
    if not combatCoroutine or coroutine.status(combatCoroutine) == "dead" then
        combatCoroutine = coroutine.create(startCombat)
    end

    -- �ָ�ս��Э��
    if coroutine.status(combatCoroutine) ~= "dead" then
        coroutine.resume(combatCoroutine)
    end

    -- ���ֹͣ��־
    checkStopFlag()

    -- ģ��ű�ִ��һ��ʱ��
    sleep(5000)
end
-- ����Э��
if combatCoroutine and coroutine.status(combatCoroutine) ~= "dead" then
    coroutine.close(combatCoroutine)
end
