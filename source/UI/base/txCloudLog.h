#ifndef txCloud_h_
#define txCloud_h_

#ifdef iLiveSDKLogExp
#define LogAPI __declspec(dllexport)
#else
#define LogAPI 
#endif
#include <string>

using namespace std;

	enum E_iLiveLogLevel
	{
		E_LogNone = 0,	///< ����־
		E_LogError,		///< ����
		E_LogWarn,		///< ����
		E_LogInfo,		///< ��Ϣ
		E_LogDebug		///< ����
	};

	/**
	@brief ������־Ŀ¼��
	@param [in] szLogPath ��־Ŀ¼;
	@note Ĭ��·��Ϊ��ǰĿ¼; ����ʾ��: SetiLiveLogPath("./txsdklog/");
	@return �����true�ɹ���falseʧ��
	*/
	bool SetiLiveLogPath(std::string szLogPath);
	/**
	@brief ��ȡ��־Ŀ¼·��
	@return ��־·��
	*/
	const std::string& GetiLiveLogPath();
	/**
	@brief ������־����.
	@param [in] logLevel ��־����
	@note ������־�����ֻ�и��ڴ˼������־�Ż�������磬��־����ΪE_LogWarnʱ��ֻ���ӡE_LogError
	��E_LogWarn�������־��Ĭ����־����ΪE_LogDebug;
	*/
	void SetiLiveLogLevel(E_iLiveLogLevel logLevel);

	void ClearLog();
	void DoClearLog(const std::string& szPath, const std::string& szPre, uint32_t day);
	
	/**
	@brief ���Error��־��Ϣ.
	@param [in] szModule ģ����.
	@param [in] szFormat ��־���ݸ�ʽ.
	@note ʹ�÷�������prinf()����.
	*/
	LogAPI void txCloudEduLog_e(const char* szModule, const char* szFormat, ...);
	/**
	@brief ���Error��־��Ϣ(˫�ֽڰ汾).
	@param [in] szModule ģ����.
	@param [in] szFormat ��־���ݸ�ʽ.
	@note ʹ�÷�������wprintf()����.
	*/
	LogAPI void txCloudEduLog_e(const wchar_t* szModule, const wchar_t* szFormat, ...);

	/**
	@brief ���Warn��־��Ϣ.
	@param [in] szModule ģ����.
	@param [in] szFormat ��־���ݸ�ʽ.
	@note ʹ�÷�������prinf()����.
	*/
	LogAPI void txCloudEduLog_w(const char* szModule, const char* szFormat, ...);
	/**
	@brief ���Warn��־��Ϣ(˫�ֽڰ汾).
	@param [in] szModule ģ����.
	@param [in] szFormat ��־���ݸ�ʽ.
	@note ʹ�÷�������wprintf()����.
	*/
	LogAPI void txCloudEduLog_w(const wchar_t* szModule, const wchar_t* szFormat, ...);

	/**
	@brief ���Info��־��Ϣ.
	@param [in] szModule ģ����.
	@param [in] szFormat ��־���ݸ�ʽ.
	@note ʹ�÷�������prinf()����.
	*/
	LogAPI void txCloudEduLog_i(const char* szModule, const char* szFormat, ...);
	/**
	@brief ���Info��־��Ϣ(˫�ֽڰ汾).
	@param [in] szModule ģ����.
	@param [in] szFormat ��־���ݸ�ʽ.
	@note ʹ�÷�������wprintf()����.
	*/
	LogAPI void txCloudEduLog_i(const wchar_t* szModule, const wchar_t* szFormat, ...);

	/**
	@brief ���Debug��־��Ϣ.
	@param [in] szModule ģ����.
	@param [in] szFormat ��־���ݸ�ʽ.
	@note ʹ�÷�������prinf()����.
	*/
	LogAPI void txCloudEduLog_d(const char* szModule, const char* szFormat, ...);
	/**
	@brief ���Debug��־��Ϣ(˫�ֽڰ汾).
	@param [in] szModule ģ����.
	@param [in] szFormat ��־���ݸ�ʽ.
	@note ʹ�÷�������wprintf()����.
	*/
	LogAPI void txCloudEduLog_d(const wchar_t* szModule, const wchar_t* szFormat, ...);

	LogAPI void txCloudEduLog_k(const char* szModule, const char* szFormat, ...);
	LogAPI void txCloudEduLog_k(const wchar_t* szModule, const wchar_t* szFormat, ...);


#endif 