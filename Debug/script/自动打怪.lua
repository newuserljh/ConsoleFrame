-- ���� lua_interface ����
local game = LuaInterface()

-- ��ӡ�ָ���
local function printSeparator()
    print("--------------------------------------------------")
end

-- �س�������
local function goHomeAndOrganize()
    printSeparator()
    print("�س�������...")
    game:�س�������()

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
        print("���ڴ�ֿ�...")
        if game:��ֿ�() then
            print("��ֿ�ɹ�")
        else
            print("��ֿ�ʧ��")
        end
    else
        print("û�д�����Ʒ")
    end

    -- ����Ʒ
    if sellJewelryCount > 0 then
        print("����������...")
        if game:������() then
            print("�����γɹ�")
        else
            print("������ʧ��")
        end
    else
        print("û�д�������")
    end

    if sellClothesCount > 0 then
        print("�������·�...")
        if game:���·�() then
            print("���·��ɹ�")
        else
            print("���·�ʧ��")
        end
    else
        print("û�д����·�")
    end

    if sellWeaponCount > 0 then
        print("����������...")
        if game:������() then
            print("�������ɹ�")
        else
            print("������ʧ��")
        end
    else
        print("û�д�������")
    end

    -- ��ҩ
    print("������ҩ...")
    if game:��ҩ() then
        print("��ҩ�ɹ�")
    else
        print("��ҩʧ��")
    end
end

-- ����ҩƷ
local function buyMedicines()
    printSeparator()
    print("����ҩƷ...")

    -- �����ҩ
    local hpMedicineCount = game:��ҩ����()
    if hpMedicineCount < 10 then
        print("��ҩ�������㣬���ڹ���...")
        if game:��ҩ("��ҩ", 10 - hpMedicineCount) then
            print("�����ҩ�ɹ�")
        else
            print("�����ҩʧ��")
        end
    else
        print("��ҩ��������: " .. hpMedicineCount)
    end

    -- ������ҩ
    local mpMedicineCount = game:��ҩ����()
    if mpMedicineCount < 10 then
        print("��ҩ�������㣬���ڹ���...")
        if game:��ҩ("��ҩ", 10 - mpMedicineCount) then
            print("������ҩ�ɹ�")
        else
            print("������ҩʧ��")
        end
    else
        print("��ҩ��������: " .. mpMedicineCount)
    end
end

-- �ƶ���Ŀ���
local function moveToTarget(x, y)
    printSeparator()
    print("�����ƶ���Ŀ��� (" .. x .. ", " .. y .. ")...")

    -- �������
    local distance = game:�������(x, y)
    print("��Ŀ���ľ���: " .. distance)

    -- Ѱ·��Ŀ���
    if distance > 0 then
        if game:Ѱ·��(x, y) then
            print("Ѱ·��Ŀ���ɹ�")
        else
            print("Ѱ·��Ŀ���ʧ��")
        end
    else
        print("�Ѿ���Ŀ���")
    end
end

-- ս��
local function startCombat()
    printSeparator()
    print("��ʼս��...")

    -- ��ʼս��
    if game:��ʼս��() then
        print("ս����ʼ�ɹ�")
    else
        print("ս����ʼʧ��")
    end

    -- ģ�⹥��Ŀ��
    local targetX, targetY = 100, 200
    local targetId = 12345 -- ����Ŀ�� ID �� 12345
    print("��Ŀ��ʹ�ü���...")
    if game:��Ŀ��ʹ�ü���("������", targetX, targetY, targetId) then
        print("��Ŀ��ʹ�ü��ܳɹ�")
    else
        print("��Ŀ��ʹ�ü���ʧ��")
    end

    -- ����ս��
    if game:����ս��() then
        print("ս�������ɹ�")
    else
        print("ս������ʧ��")
    end
end

-- ������
local function main()
    print("�ű���ʼ����")

    -- �س�������
    goHomeAndOrganize()

    -- ����ҩƷ
    buyMedicines()

    -- �ƶ���Ŀ���
    local targetX, targetY = 150, 250
    moveToTarget(targetX, targetY)

    -- ��ʼս��
    startCombat()

    print("�ű����н���")
end

-- ����������
main()
