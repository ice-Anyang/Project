#pragma once

#include"Common.h"

#ifndef __TRACE__
	//#define __TRACE__
#endif

#ifndef __DEBUG__
	#define __DEBUG__
#endif

///////////////////////////////////////////////////////////////////////
static std::string GetFileName(const std::string& path)
{
	char ch = '/';  //Linux   

#ifdef _WIN32      
	ch = '\\'; 
#endif 
	size_t pos = path.rfind(ch);
	if (pos == std::string::npos)
		return path;
	else
		return path.substr(pos + 1);
}

//���ڵ���׷�ݵ�trace log
inline static void __TraceDebug(const char* filename, int line, const char* function, const char* format, ...)
{
#ifdef __TRACE__
	//������ú�������Ϣ
	fprintf(stdout,"[TRACE][%s:%d:%s]:",GetFileName(filename).c_str(), line, function);

	//�ɱ����
	//����û����trace��Ϣ
	va_list args;
	va_start(args,format);
	vfprintf(stdout,format, args);
	va_end(args);

	fprintf(stdout,"\n");
#endif
}

inline static void __ErrorDebug(const char* filename, int line, const char* function, const char* format, ...)
{
#ifdef __DEBUG__
	//������ú�������Ϣ
	fprintf(stdout,"[ERROR][%s:%d:%s]:",GetFileName(filename).c_str(), line, function);

	//����û����trace��Ϣ
	va_list args;
	va_start(args,format);
	vfprintf(stdout,format, args);
	va_end(args);

	fprintf(stdout," errmsg:%s, errno:%d\n", strerror(errno), errno);
#endif
}

#define TRACE_LOG(...) \
	__TraceDebug(__FILE__, __LINE__, __FUNCTION__, __VA_ARGS__);

#define ERROR_LOG(...) \
	__ErrorDebug(__FILE__, __LINE__, __FUNCTION__, __VA_ARGS__);

////////////////////////////////////////////////////////////////////////////////////////////
//����ģ��

#define WIDTH 120
#define HEIGHT 30

void SetCurPos(int x, int y); //x ����������   y ����������
void HideCursor();

void DrawCol(int x, int y);
void DrawRow(int x, int y);

void DrawFrame(char *title);
void DrawMenu();

void SystemEnd();



/////////////////////////////////////////////////////////////////////////////////////////////
//ϵͳ���ܺ���ģ��
void DirectoryList(const string &path, vector<string> &subfile, vector<string> &subdir);
// ����תƴ��ȫƴ
/* CSDN��http://blog.csdn.net/csnd_ayo */
string ChineseConvertPinYinAllSpell(const string& dest_chinese);
// ����תƴ������ĸ
string ChineseConvertPinYinInitials(const string& name);
// ��ɫ������ʾһ���ַ���
void ColourPrintf(const string  &str);
