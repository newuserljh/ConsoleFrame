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
�������ܣ�stringд���ļ�+����
����1���ļ���
����2������
����3����ģʽ
����ֵ��BOOL
*/
bool tools::write2file(std::string fileName, std::string data, std::ios_base::open_mode _model/*= std::ios::app*/)
{
	std::ofstream fout;
	fout.open(fileName, _model);
	if (!fout)
	{
		return false;
	}
	fout << data << std::endl;
	fout.close();
	return true;
}

/*
�������ܣ�c��ʽд�ļ�������¼��NPC��ID �ͶԻ�����
����1���ļ���
����2������
����3��NPC��ID
����4����ģʽ
����ֵ��BOOL
*/
bool tools::write2file_c(const char* fileName, const char* data, DWORD npc_id, const char* _model)
{
	FILE* fp;
	errno_t err = fopen_s(&fp, fileName, _model); // ʹ�� fopen_s ���� fopen
	if (err != 0 || fp == NULL) {
		perror("Error opening file");
		return false;
	}
	fprintf(fp, "NPC ID: %x  %s\n", npc_id, data); // ����ʽ��������д���ļ�
	fclose(fp); // �ر��ļ�
	return true;
}


/*
�������ܣ�д�ڴ��ֽڼ�
����1: �ڴ��ַ
����2���ֽڼ�
����3���ߴ�
����ֵ��BOOL
*/
bool tools::write(DWORD _addr, const char* _data, int _size)
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
bool tools::read(DWORD _addr, const char* _data, int _size)
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
	/* ������ʹ��GetFileAttributesA ���� Windows API���������ڻ�ȡָ���ļ���Ŀ¼�����ԡ�
	    �������ļ����Ե�һ��λ���롣
		INVALID_FILE_ATTRIBUTES����� GetFileAttributesA �������ֵ����ʾ�ļ���Ŀ¼�����ڡ�
        ����ļ������ԣ�ͨ����� fileAttributes �Ƿ���� INVALID_FILE_ATTRIBUTES����ȷ��������һ��Ŀ¼��ʹ�� FILE_ATTRIBUTE_DIRECTORY ��־�������ж��ļ��Ƿ���ڡ�
	//DWORD fileAttributes = GetFileAttributesA(fileName.c_str());*/
	//return (fileAttributes != INVALID_FILE_ATTRIBUTES && !(fileAttributes & FILE_ATTRIBUTE_DIRECTORY));
}


/*
�������ܣ����ж�ȡ�ļ�
����1���ļ���
����ֵ���洢���ݵ�vector
*/
std::vector<std::string> tools::ReadTxt(std::string file_Path/*�ļ�·��*/)
{
	std::fstream f(file_Path);
	std::vector<std::string> temp;
	std::string line;
	if (!f.is_open())
	{
		tools::getInstance()->message("�ļ���ʧ�ܣ�\n");
	}
	while (!f.eof())//���Զ��ѻ��з�ȥ��
	{
		std::getline(f, line);
		if(line.size())temp.push_back(line); //���в�push
	}
	f.close();
	return temp;
}


/*
�������ܣ��ָ������ַ���
����1���ַ���
����ֵ���洢���ݵ�vector
*/
std::vector<std::string> tools::splitString(std::string str)
{
	std::regex delimiters("\\s+");
	std::vector<std::string> temp(std::sregex_token_iterator(str.begin(), str.end(), delimiters, -1), std::sregex_token_iterator());
	return temp;
}

/*
�������ܣ���ȡ��ǰexe����dll��·��
����1���ַ���
����ֵ����ǰexe����·��
*/
std::string tools::GetCurrDir()
{	
	TCHAR szFull[_MAX_PATH];
	TCHAR szDrive[_MAX_DRIVE];
	TCHAR szDir[_MAX_DIR];
	::GetModuleFileName(NULL, szFull, sizeof(szFull) / sizeof(TCHAR));
	_splitpath_s(szFull, szDrive, sizeof(szDrive) / sizeof(TCHAR), szDir, sizeof(szDir) / sizeof(TCHAR), NULL, 0, NULL, 0);
	_tcscpy_s(szFull, szDrive);
	_tcscat_s(szFull, szDir);
	std::string strPath(szFull);
	return strPath;
}

/*
�������ܣ�ȡ�ļ�������,ȡ��һ·��
����1��·���ַ��� " X:\\DIR1\\DIR2\\DIR3\\DIR4\\"
����ֵ����һ��Ŀ¼ " X:\\DIR1\\DIR2\\DIR3\\"
*/
 std::string tools::getParentPath(std::string str)
{
	std::string::size_type idx = str.rfind('\\', str.length());
	std::string folder = str.substr(0, idx);
	idx = folder.rfind('\\', folder.length());
	std::string folder1 = folder.substr(0, idx + 1);
	return folder1;
}


 /*
�������ܣ� char* ת wchar_t*
����1��
����ֵ��
*/
 wchar_t* tools::char2wchar(const char* cchar)
 {
	 wchar_t* m_wchar;
	 int len = MultiByteToWideChar(CP_ACP, 0, cchar, strlen(cchar), NULL, 0);
	 m_wchar = new wchar_t[len + 1];
	 MultiByteToWideChar(CP_ACP, 0, cchar, strlen(cchar), m_wchar, len);
	 m_wchar[len] = '\0';
	 return m_wchar;
 }


 /*
�������ܣ� wchar_t* ת char_t*
����1��
����ֵ��
*/
 char* tools::wchar2char(const wchar_t* wchar)
 {
	 char* m_char;
	 int len = WideCharToMultiByte(CP_ACP, 0, wchar, wcslen(wchar), NULL, 0, NULL, NULL);
	 m_char = new char[len + 1];
	 WideCharToMultiByte(CP_ACP, 0, wchar, wcslen(wchar), m_char, len, NULL, NULL);
	 m_char[len] = '\0';
	 return m_char;
 }

 /*
�������ܣ� �ͷ���Դ��ָ��Ŀ¼
����1����ԴID
����2���ͷŵ�����·��������Ŀ¼+�ļ�������"C:\windwos\test.dll"��
����3���ͷ���Դ�����ͣ�����Դ�ļ��п��Կ���
����4���Ƿ������ͷŵ��ļ�����Ϊϵͳ�����أ�bool�ͣ�Ĭ��false
����ֵ��
*/
 bool tools::ReleaseResource(int resourceId, const std::string& outputPath, 
	 const std::string& resourceType, bool hiddenSystem) 
 {
	 HMODULE hModule =  GetModuleHandle(NULL);
	 HRSRC hResource = FindResource(hModule, MAKEINTRESOURCE(resourceId), resourceType.c_str());
	 if (hResource == NULL) {
		 return false;
	 }

	 HGLOBAL hLoadedResource = LoadResource(hModule, hResource);
	 if (hLoadedResource == NULL) {
		 return false;
	 }

	 LPVOID pLockedResource = LockResource(hLoadedResource);
	 DWORD dwResourceSize = SizeofResource(hModule, hResource);
	 if (pLockedResource == NULL || dwResourceSize == 0) {
		 return false;
	 }

	 std::ofstream outFile(outputPath, std::ios::binary);
	 outFile.write((const char*)pLockedResource, dwResourceSize);
	 outFile.close();

	 if (hiddenSystem) {
		 // �����ļ�Ϊ���غ�ϵͳ�ļ�
		 SetFileAttributes(outputPath.c_str(), FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_SYSTEM);
	 }

	 return true;
 }


 /*
�������ܣ� �ٳ� EIP��ָ��ָ��Ĵ�������ע�� DLL ��Ŀ�������
����1��dll·��
����2��ע��Ŀ����̵ĵĽ�����Ϣ PROCESS_INFORMATION �ṹ��
����ֵ��
*/
 bool tools::eipinjectDll(WCHAR* dllname, PROCESS_INFORMATION pi) {

	 SuspendThread(pi.hThread); //�����߳� 
	 CONTEXT ct = { 0 };
	 ct.ContextFlags = CONTEXT_CONTROL;
	 GetThreadContext(pi.hThread, &ct); //��ȡ�������̼߳Ĵ������ 

	 DWORD dwSize = sizeof(WCHAR) * 1024; //0-0x100 д���� ֮��д���� 
	 BYTE* pProcessMem = (BYTE*)::VirtualAllocEx(pi.hProcess, NULL, dwSize, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	 if (NULL == pProcessMem)return  false;
	 DWORD dwWrited = 0;
	 if (!::WriteProcessMemory(pi.hProcess, (pProcessMem + 0x100), (LPVOID)dllname, //�Ȱ�·�������ݣ�д���ڴ����0x100��ʼ 
		 (wcslen(dllname) + 1) * sizeof(WCHAR), &dwWrited))return false;

	 // ��ȡ LoadLibraryW ������ַ
	 FARPROC pLoadLibraryW = (FARPROC)::GetProcAddress(::GetModuleHandle("Kernel32"), "LoadLibraryW");
	 if (NULL == pLoadLibraryW)return false;

	 // ���� ShellCode
	 BYTE ShellCode[32] = { 0 };
	 DWORD* pdwAddr = NULL;

	 ShellCode[0] = 0x60; // pushad 
	 ShellCode[1] = 0x9c; // pushfd 
	 ShellCode[2] = 0x68; // push 
	 pdwAddr = (DWORD*)&ShellCode[3]; // ShellCode[3/4/5/6] 
	 *pdwAddr = (DWORD)(pProcessMem + 0x100);
	 ShellCode[7] = 0xe8;//call 
	 pdwAddr = (DWORD*)&ShellCode[8]; // ShellCode[8/9/10/11] 
	 *pdwAddr = (DWORD)pLoadLibraryW - ((DWORD)(pProcessMem + 7) + 5); // ��Ϊֱ��call��ַ�ˣ����Զ�Ӧ��������Ҫת��������VA 
	 ShellCode[12] = 0x9d; // popfd 
	 ShellCode[13] = 0x61; // popad 
	 ShellCode[14] = 0xe9; // jmp 
	 pdwAddr = (DWORD*)&ShellCode[15]; // ShellCode[15/16/17/18] 
	 *pdwAddr = ct.Eip - ((DWORD)(pProcessMem + 14) + 5); //��Ϊֱ��jmp��ַ�ˣ����Զ�Ӧ��������Ҫת��������VA 

	 // �� ShellCode д��Ŀ������ڴ�
	 if (!::WriteProcessMemory(pi.hProcess, pProcessMem, ShellCode, sizeof(ShellCode), &dwWrited))return false;

	 // �޸� EIP ָ�� ShellCode
	 ct.Eip = (DWORD)pProcessMem;
	 ::SetThreadContext(pi.hThread, &ct);
	 ::ResumeThread(pi.hThread);
	 return true;
 }


 /*
�������ܣ� ����Ŀ�����
����1����ִ���ļ�����+·��
����2������ִ��Ŀ¼
����3�����̽ṹ��
����ֵ��
*/
 bool tools::exeload(const std::string& filename, const std::string& excutedirectory, PROCESS_INFORMATION& pi)
 {
	 STARTUPINFO si; // ������Ϣ
	 memset(&si, 0, sizeof(STARTUPINFO));
	 si.cb = sizeof(STARTUPINFO);
	 si.dwFlags = STARTF_USESHOWWINDOW;
	 si.wShowWindow = SW_SHOW;

	 if (!CreateProcess(
		 NULL, // Ӧ�ó�������
		 const_cast<char*>(filename.c_str()), // �����в���
		 NULL, // ���̰�ȫ����
		 NULL, // �̰߳�ȫ����
		 FALSE, // �Ƿ�̳о��
		 0, // ������־
		 NULL, // �½��̵Ļ�������
		 const_cast<char*>(excutedirectory.c_str()), // ��ǰĿ¼
		 &si, // ������Ϣ
		 &pi // ������Ϣ
	 )) return false;

	 return true;
 }


 // ���������ļ�������һ�� map�����м����б�����ֵ���б�����
 std::unordered_map<std::string, std::unordered_map<std::string, std::string>> tools::parseIniFile(const std::string& filename)
 {
	 std::ifstream file(filename);
	 if (!file.is_open()) {
		 throw std::runtime_error("Could not open file: " + filename);
	 }
	 std::unordered_map<std::string, std::unordered_map<std::string, std::string>> config;
	 std::string line;
	 std::string currentSection;

	 while (std::getline(file, line)) {
		 // Trim whitespace
		 line.erase(0, line.find_first_not_of(" \t"));
		 line.erase(line.find_last_not_of(" \t") + 1);

		 if (line.empty() || line[0] == ';') continue; // Skip empty lines and comments

		 if (line[0] == '[' && line[line.size() - 1] == ']') {
			 // Section header
			 currentSection = line.substr(1, line.size() - 2);
		 }
		 else {
			 size_t delimiterPos = line.find('=');
			 if (delimiterPos != std::string::npos) {
				 std::string key = line.substr(0, delimiterPos);
				 std::string value = line.substr(delimiterPos + 1);
				 config[currentSection][key] = value;
			 }
		 }
	 }
	 return config;
 }

 void tools::copyFile(const std::string& srcPath, const std::string& destPath) {
	 std::ifstream src(srcPath, std::ios::binary);
	 std::ofstream dest(destPath, std::ios::binary);

	 if (!src) {
		 std::cerr << "�޷���Դ�ļ�: " << srcPath << std::endl;
		 return;
	 }
	 if (!dest) {
		 std::cerr << "�޷�����Ŀ���ļ�: " << destPath << std::endl;
		 return;
	 }

	 // ʹ�û��������и�Ч����
	 dest << src.rdbuf();
 }