#include "utils.h"

tools* tools::m_pInstance = nullptr;
std::mutex tools::m_mutex;
/*
�������ܣ�
����ģʽָ���ȡ
����ֵ��thisָ�룬ָ�򹤾���
*/
tools* tools::getInstance()
{
	if (m_pInstance == nullptr)
	{
		m_mutex.lock();
		if (nullptr == m_pInstance)
		{
			m_pInstance = new tools;
		}
		m_mutex.unlock();
	}
	return m_pInstance;
}

tools::~tools()
{

}

tools::tools()
{

}

tools::tools(const tools&)
{

}

tools& tools::operator=(const tools&)
{
	return *this;
}


/*
�������ܣ���ӡ������Ϣ
����1�����ݸ�ʽ
����2������ɱ�Ĳ���
����ֵ����
*/
void tools::message(char* fmt, ...)
{
	__try{
		char szbufFormat[0x1000];//�ַ�����������Ϊvsprintf_s׼��
		char szBufFormat_Game[0x1000] = "HXL:";//��8�ֽ���Ϊ�˼�Game���ǰ׺
		va_list argList;
		va_start(argList, fmt);//�����б��ʼ��
		vsprintf_s(szbufFormat, fmt, argList);
		strcat_s(szBufFormat_Game, szbufFormat);//׷���ַ���
		OutputDebugString(szBufFormat_Game);
		va_end(argList);//������
	}
	__except (1)
	{
		return;
	}
}
/*
�������ܣ�����Ȩ��
����ֵ��BOOL 
*/
bool tools::improveProcPriv()
{
	HANDLE token;
	//����Ȩ��
	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &token))
	{
		return false;
	}
	TOKEN_PRIVILEGES tkp;
	tkp.PrivilegeCount = 1;
	::LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &tkp.Privileges[0].Luid);
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	if (!AdjustTokenPrivileges(token, FALSE, &tkp, sizeof(tkp), NULL, NULL))
	{
		return false;
	}
	CloseHandle(token);
	return true;
}

/*
�������ܣ�����������
����1����ʼ��ַ
����2����ַ��С
����3��������
����4������������
����ֵ��������Ŀ�ʼ��ַ
*/
DWORD tools::findPattern(DWORD startAddress, DWORD fileSize, unsigned char* pattern, char mask[])
{
	DWORD pos = 0;
	int searchLen = strlen(mask) - 1;
	//���ڴ�������ֽڽ��бȽ�
	for (DWORD retAddress = startAddress; retAddress < startAddress + fileSize; retAddress++)
	{
		//�жϵ�ǰ��ַ�Ƿ���Ч
		if (IsBadReadPtr((const void *)retAddress, 1))
		{
			pos = 0;
			continue;
		}
		if (*(PBYTE)retAddress == pattern[pos] || mask[pos] == '?')
		{
			if (mask[pos + 1] == '\0')
			{
				return (retAddress - searchLen);
			}

			pos++;
		}
		else
		{
			pos = 0;
		}
	}
	return NULL;
}
/*
�������ܣ�����ģ����Ϣ
������ģ������
����ֵ��ģ����Ϣ
*/
MODULEINFO tools::getModuleInfo(const char* moduleName)
{
	MODULEINFO moudleInfo = { NULL };
	HMODULE hMoudle = GetModuleHandle(moduleName);
	if (hMoudle)
	{
		GetModuleInformation(GetCurrentProcess(), hMoudle, &moudleInfo, sizeof(MODULEINFO));
	}
	return moudleInfo;
}
/*
�������ܣ���¼��־�ļ�
����1���ļ���
����2������
����3����ģʽ
����ֵ��BOOL
*/
bool tools::log2file(std::string fileName, std::string data, std::ios_base::open_mode _model/*= std::ios::app*/)
{
	std::ofstream fout;
	fout.open(fileName, _model);
	if (!fout)
	{
		return false;
	}
	std::stringstream ss;
	time_t nowTime = time(NULL);
	ss << "time: " << nowTime<<" ";
	fout << ss.str() <<data << std::endl;
	fout.close();
	return true;
}
/*
�������ܣ�д�ڴ��ֽڼ�
����1: �ڴ��ַ
����2���ֽڼ�
����3���ߴ�
����ֵ��BOOL
*/
bool tools::write(DWORD _addr, unsigned char* _data, int _size)
{
	if (!improveProcPriv())
	{
		return false;
	}
	DWORD oldProtect = 0;
	if (!VirtualProtect((void*)_addr, _size, PAGE_READWRITE, &oldProtect))
	{
		tools::getInstance()->message("erroe1\n");
		return false;
	}
	memcpy((void*)_addr, _data, _size);
	if (!VirtualProtect((void*)_addr, _size, oldProtect, &oldProtect))
	{
		tools::getInstance()->message("erroe1\n");
		return false;
	}
	return  true;
}
/*
�������ܣ����ڴ��ֽڼ�
����1: �ڴ��ַ
����2���ֽڼ�
����3���ߴ�
����ֵ��BOOL
*/
bool tools::read(DWORD _addr, unsigned char* _data, int _size)
{
	unsigned long oldProtect = 0;
	if ( S_OK != VirtualProtect((void*)_addr, _size, PAGE_READONLY, &oldProtect))
	{
		return false;
	}
	memcpy((void*)_data, (void*)_addr, _size);
	if (S_OK !=VirtualProtect((void*)_addr, _size, oldProtect, 0))
	{
		return false;
	}
	return  true;
}

bool tools::byte2hex(BYTE src, char* desc)
{
	byte low = 0, high = 0;
	//ȡ�ߵ�λ ��λȡ����
	low = src % 16;
	high = src / 16;

	if (high > 9)
	{
		//�жϸ�λΪA -F
		desc[0] = 'A' + high - 10;
	}
	else
	{
		desc[0] = '0' + high;
	}

	//�Ե�λ���и�ֵ
	if (low > 9)
	{
		desc[1] = 'A' + low - 10;
	}
	else
	{
		desc[1] = '0' + low;
	}
	desc[2] = ',';
	desc[3] = '\0';
	return true;
}

bool tools::lower2upper(char& ch)
{
	if (ch >= 'a' && ch <= 'z')
	{
		ch = ch + 'A' - 'a';
	}
	return true;
}

bool tools::hexstr2upper(char* src)
{
	DWORD ndLen = strlen(src);
	for (DWORD i = 0; i != ndLen; i++)
	{
		lower2upper(src[i]);
	}
	return true;
}

char tools::hex2byte(const char& high, const char& low)
{
	int result = 0;

	//�����λΪ����
	//���㷽������   high - '0'
	if (high >= '0' && high <= '9')
	{
		result = high - '0';
	}
	else if (high >= 'A' && 'F')
	{
		//�����λΪ��ĸ
		//���㷽��Wie  high - 'A' + 10
		result = high - 'A' + 10;
	}
	else
	{
		return -1;
	}
	//��λ���㷽��
	if (low >= '0' && low <= '9')
	{
		return result * 16 + low - '0';
	}
	else if (low >= 'A' && low <= 'F')
	{
		return result * 16 + low - 'A' + 10;
	}
	else
	{
		return -1;
	}
}

bool tools::bytes2hexstr(BYTE* src, DWORD ndSize, char* desc)
{

	//�����ǽ������ַ�ת��Ϊ��д��ĸ
	__try{

		for (int i = 0; i != ndSize; i++)
		{
			byte2hex(src[i], &desc[i * 3]);
		}
	}
	__except (1)
	{

		tools::getInstance()->message("�ֽ�ת���쳣��\n");
	}
	//DWORD ndLen = strlen(src);

	return hexstr2upper(desc);
}

bool tools::hexstr2bytes(char* src, char* desc)
{
	hexstr2upper(src);
	DWORD ndLen = strlen(src);
	for (int i = 0; i != ndLen; i++)
	{
		desc[i] = hex2byte(src[i * 2], src[i * 2 + 1]);
	}
	return hexstr2upper(desc);
}

/*
�������ܣ��ж��ļ��Ƿ����
����1���ļ���
����ֵ��bool
*/
bool tools::fileIsexist(std::string fileName)
{
	std::fstream ss;
	ss.open(fileName,std::ios::in);
	bool result;
	if (ss)
		result = true;
	else
		result = false;
	ss.close();
	return result;
}

