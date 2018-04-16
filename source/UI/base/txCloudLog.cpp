#include "txCloudLog.h"
#include <ATLComTime.h> //COleDateTime类的定义
#include <direct.h>
#include <io.h>
#include <locale>
#define CLEAR_Log_Day		7					//自动清理日志的天数
#define DefaultLogPath		"./txsdklog/"		//日志文件默认路径(av、im、ilive)
#define MinQuaParTime		1000				//默认质量回调计时器的时间间隔

	struct iLiveLogSetting
	{
		iLiveLogSetting()
		{
			m_logLevel = E_LogDebug;
			m_logFilePath = DefaultLogPath;

			SYSTEMTIME st = {0};
			GetLocalTime(&st);
			char szData[32];
			sprintf_s( szData, sizeof(szData)-1, "cloudedu_%d%02d%02d.log", st.wYear, st.wMonth, st.wDay);
			m_logFileName = szData;

			m_bCanSetLogPath = true;
		}

		std::string getLogFileName();

		int			m_logLevel;
		std::string m_logFilePath;
		std::string m_logFileName;
		bool		m_bCanSetLogPath;
	};

	std::string iLiveLogSetting::getLogFileName()
	{
		return m_logFilePath + m_logFileName;
	}
	
	static iLiveLogSetting s_setting;

	bool SetiLiveLogPath( std::string szLogPath )
	{
		if ( !s_setting.m_bCanSetLogPath )
		{
			return false;
		}
		if ( szLogPath[szLogPath.size()-1] != '\\' && szLogPath[szLogPath.size()-1] != '/' )
		{
			szLogPath += '/';
		}
		s_setting.m_logFilePath = szLogPath;
		return true;
	}

	const std::string& GetiLiveLogPath()
	{
		return s_setting.m_logFilePath;
	}

	void SetiLiveLogLevel(E_iLiveLogLevel logLevel)
	{
		s_setting.m_logLevel = logLevel;
	}

	void ClearLog()
	{
		DoClearLog( s_setting.m_logFilePath, "imsdk_", CLEAR_Log_Day);
		DoClearLog( s_setting.m_logFilePath, "QAVSDK_", CLEAR_Log_Day);
		DoClearLog( s_setting.m_logFilePath, "ilivesdk_", CLEAR_Log_Day);

		//TODO 下面3行，为了清除之前版本遗留下的日志，后面可以删除掉;
		DoClearLog( "./", "ilivesdk_", 0);
		DoClearLog( "./", "QAVSDK_", 0);
		DoClearLog( "./", "imsdk_", 0);
	}

	void DoClearLog( const std::string& szPath, const std::string& szPre, uint32_t day )
	{
		const time_t tLimitSeconds = 24*60*60*day; //day天的秒数
		HANDLE hFind = INVALID_HANDLE_VALUE;
		WIN32_FIND_DATA fileData;
		std::string path;
		std::string fileName;
		char file_name[MAX_PATH] = { 0 };

		//CString filePath;
		//filePath.Format(L"%s", path.assign(szPath).append("*.log").c_str());
		//hFind = FindFirstFile(filePath, &fileData );
		//if (hFind != INVALID_HANDLE_VALUE)
		//{
		//	do
		//	{
		//		if( fileData.dwFileAttributes != FILE_ATTRIBUTE_DIRECTORY )
		//		{
		//			int len = WideCharToMultiByte(CP_ACP, 0, fileData.cFileName, wcslen(fileData.cFileName), NULL, 0, NULL, NULL);
		//			WideCharToMultiByte(CP_ACP, 0, fileData.cFileName, wcslen(fileData.cFileName), file_name, MAX_PATH, NULL, NULL);
		//			path = szPath + file_name;
		//			fileName = file_name;

		//			if ( !fileName.compare( 0, szPre.length(), szPre.c_str() ) )
		//			{
		//				time_t nowTime = time(NULL), logTime = 0;

		//				const int nStart = fileName.length() - 12;
		//				tm tmLogTime;
		//				memset(&tmLogTime, 0, sizeof(tm));
		//				tmLogTime.tm_year = atoi( fileName.substr(nStart, 4).c_str() ) - 1900;
		//				tmLogTime.tm_mon = atoi( fileName.substr(nStart+4, 2).c_str() ) - 1;
		//				tmLogTime.tm_mday = atoi( fileName.substr(nStart+6, 2).c_str() );
		//				logTime = mktime(&tmLogTime);

		//				time_t tSecSpan = nowTime - logTime;

		//				if ( tSecSpan >= tLimitSeconds ) //超过day天的log文件
		//				{
		//					remove( path.c_str() );
		//				}
		//			}
		//		}
		//	}while( FindNextFile(hFind, &fileData) );

		//	FindClose(hFind);
		//}
	}

	static inline void iLiveCreateLogDir()
	{
		static bool bHaveRun = false;
		if (bHaveRun) return;
		bHaveRun = true;
		if ( _access(s_setting.m_logFilePath.c_str(), 2) )//如果不存在此目录，则创建
		{
			_mkdir(s_setting.m_logFilePath.c_str());
		}
		s_setting.m_bCanSetLogPath = false;
	}

	static void iLiveLog( const char* Tag, const char* Module, const char* szFormat, va_list args )
	{
		iLiveCreateLogDir();

		char szData[1024] = {0};
		std::string str;

		//时间
		SYSTEMTIME st = {0};
		GetLocalTime(&st);
		sprintf_s( szData, sizeof(szData)-1, "%04d-%02d-%02d %02d:%02d:%02d", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond );
		str += szData;
		str += "|";

		//线程ID
		DWORD tid = GetCurrentThreadId();
		sprintf_s( szData, sizeof(szData)-1, "%d", tid );
		str += szData;
		str += "|";
		
		//Tag
		str += Tag;
		str += "|";

		//模块(Module)
		str += Module;
		str += "|";

		//内容
		_vsnprintf_s(szData, sizeof(szData)-1, szFormat, args );
		str += szData;
		str += "\n";

		//打印到Output窗口
		OutputDebugStringA( str.c_str() );
		
		//写入log文件
		FILE* pFile;
		fopen_s( &pFile, s_setting.getLogFileName().c_str(), "a" );
		if (pFile)
		{
			fprintf( pFile, "%s", str.c_str() );
			fclose( pFile );
		}
	}

	static void iLiveLog( const wchar_t* Tag, const wchar_t* Module, const wchar_t* sz, va_list args )
	{	
		iLiveCreateLogDir();

		wchar_t szData[1024] = {0};
		std::wstring wstr;

		//时间
		SYSTEMTIME st = {0};
		GetLocalTime(&st);
		swprintf_s( szData, sizeof(szData)/sizeof(wchar_t)-1, L"%d-%02d-%02d %02d:%02d:%02d", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond );
		wstr += szData;
		wstr += L"|";

		//线程ID
		DWORD tid = GetCurrentThreadId();
		swprintf_s( szData, sizeof(szData)/sizeof(wchar_t)-1, L"%d", tid );
		wstr += szData;
		wstr += L"|";

		//Tag
		wstr += Tag;
		wstr += L"|";

		//模块(Module)
		wstr += Module;
		wstr += L"|";

		//内容
		_vsnwprintf_s( szData, sizeof(szData)/sizeof(wchar_t)-1, sz, args );
		wstr += szData;
		wstr += L"\n";

		//打印到Output窗口
		OutputDebugStringW( wstr.c_str() );

		//写入log文件
		FILE* pFile;
		fopen_s( &pFile, s_setting.getLogFileName().c_str(), "a" );
		if (pFile)
		{
			fwprintf( pFile, L"%s", wstr.c_str() );
			fclose( pFile );
		}
	}

	static void iLiveLogForBigData(const char* Tag, const char* Module, const char* szFormat, va_list args)
	{
		iLiveCreateLogDir();

		static char szDataBig[100*1024] = { 0 };
		std::string str;

		//时间
		SYSTEMTIME st = { 0 };
		GetLocalTime(&st);
		sprintf_s(szDataBig, sizeof(szDataBig) - 1, "%04d-%02d-%02d %02d:%02d:%02d", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
		str += szDataBig;
		str += "|";

		//线程ID
		DWORD tid = GetCurrentThreadId();
		sprintf_s(szDataBig, sizeof(szDataBig) - 1, "%d", tid);
		str += szDataBig;
		str += "|";

		//Tag
		str += Tag;
		str += "|";

		//模块(Module)
		str += Module;
		str += "|";

		//内容
		_vsnprintf_s(szDataBig, sizeof(szDataBig) - 1, szFormat, args);
		str += szDataBig;
		str += "\n";

		//打印到Output窗口
		OutputDebugStringA(str.c_str());

		//写入log文件
		FILE* pFile;
		fopen_s(&pFile, s_setting.getLogFileName().c_str(), "a");
		if (pFile)
		{
			fprintf(pFile, "%s", str.c_str());
			fclose(pFile);
		}
	}

	static void iLiveLogForBigData(const wchar_t* Tag, const wchar_t* Module, const wchar_t* sz, va_list args)
	{
		iLiveCreateLogDir();

		static wchar_t szwDataBig[100*1024] = { 0 };
		std::wstring wstr;

		//时间
		SYSTEMTIME st = { 0 };
		GetLocalTime(&st);
		swprintf_s(szwDataBig, sizeof(szwDataBig) / sizeof(wchar_t) - 1, L"%d-%02d-%02d %02d:%02d:%02d", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
		wstr += szwDataBig;
		wstr += L"|";

		//线程ID
		DWORD tid = GetCurrentThreadId();
		swprintf_s(szwDataBig, sizeof(szwDataBig) / sizeof(wchar_t) - 1, L"%d", tid);
		wstr += szwDataBig;
		wstr += L"|";

		//Tag
		wstr += Tag;
		wstr += L"|";

		//模块(Module)
		wstr += Module;
		wstr += L"|";

		//内容
		_vsnwprintf_s(szwDataBig, sizeof(szwDataBig) / sizeof(wchar_t) - 1, sz, args);
		wstr += szwDataBig;
		wstr += L"\n";

		//打印到Output窗口
		OutputDebugStringW(wstr.c_str());

		//写入log文件
		FILE* pFile;
		fopen_s(&pFile, s_setting.getLogFileName().c_str(), "a");
		if (pFile)
		{
			fwprintf(pFile, L"%s", wstr.c_str());
			fclose(pFile);
		}
	}

	void txCloudEduLog_e( const char* szModule, const char* szFormat, ... )
	{
		if (s_setting.m_logLevel < E_LogError) return;
		va_list args;
		va_start(args, szFormat);
		iLiveLog("Error", szModule, szFormat, args);
		va_end(args);
	}

	void txCloudEduLog_e( const wchar_t* szModule, const wchar_t* szFormat, ... )
	{
		if (s_setting.m_logLevel < E_LogError) return;
		va_list args;
		va_start(args, szFormat);
		iLiveLog(L"Error", szModule, szFormat, args);
		va_end(args);
	}

	void txCloudEduLog_w( const char* szModule, const char* szFormat, ... )
	{
		if (s_setting.m_logLevel < E_LogWarn) return;
		va_list args;
		va_start(args, szFormat);
		iLiveLog("Warn ", szModule, szFormat, args);
		va_end(args);
	}

	void txCloudEduLog_w( const wchar_t* szModule, const wchar_t* szFormat, ... )
	{
		if (s_setting.m_logLevel < E_LogWarn) return;
		va_list args;
		va_start(args, szFormat);
		iLiveLog(L"Warn ", szModule, szFormat, args);
		va_end(args);
	}

	void txCloudEduLog_i( const char* szModule, const char* szFormat, ... )
	{
		if (s_setting.m_logLevel < E_LogInfo) return;
		va_list args;
		va_start(args, szFormat);
		iLiveLog("Info ", szModule, szFormat, args);
		va_end(args);
	}

	void txCloudEduLog_i( const wchar_t* szModule, const wchar_t* szFormat, ... )
	{
		if (s_setting.m_logLevel < E_LogInfo) return;
		va_list args;
		va_start(args, szFormat);
		iLiveLog(L"Info ", szModule, szFormat, args);
		va_end(args);
	}

	void txCloudEduLog_d( const char* szModule, const char* szFormat, ... )
	{
		if (s_setting.m_logLevel < E_LogDebug) return;
		va_list args;
		va_start(args, szFormat);
		iLiveLogForBigData("Debug", szModule, szFormat, args);
		va_end(args);
	}

	void txCloudEduLog_d( const wchar_t* szModule, const wchar_t* szFormat, ... )
	{
		if (s_setting.m_logLevel < E_LogDebug) return;
		va_list args;
		va_start(args, szFormat);
		iLiveLogForBigData(L"Debug", szModule, szFormat, args);
		va_end(args);
	}

	void txCloudEduLog_k(const char* szModule, const char* szFormat, ...)
	{
		va_list args;
		va_start(args, szFormat);
		iLiveLog("Key_Procedure", szModule, szFormat, args);
		va_end(args);
	}

	void txCloudEduLog_k(const wchar_t* szModule, const wchar_t* szFormat, ...)
	{
		va_list args;
		va_start(args, szFormat);
		iLiveLog(L"Key_Procedure", szModule, szFormat, args);
		va_end(args);
	}

