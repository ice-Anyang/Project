#pragma once
#include"Common.h"
#include"Sysutil.h"
#include"./sqlite/sqlite3.h"

//���ݿ�sqlite ��װ
class SqliteManager
{
public :
	SqliteManager();
	~SqliteManager();
public:
	//�����ݿ�
	void Open(const string& path);
	//�ر����ݿ�
	void Close();
	//ִ��sql���
	void ExecuteSql(const string& sql);
	//��ȡ����
	void GetResTable(const string& sql, int& row, int& col, char**&ppRet);
public:
	//c++11
	//���������ݿ������п�������
	SqliteManager(const SqliteManager&) = delete; 
	//���������ݿ������и�ֵ
	SqliteManager& operator=(const SqliteManager&) = delete; 
private:
	sqlite3* m_db; // ���ݿ����
};

///////////////////////////////////////////////////////////////////////
//�Զ���ȡ������  ��Դ�ĳ�ʼ������ȡ   �Զ�
class AutoGetResTable
{
public:
	AutoGetResTable(SqliteManager *db, const string &sql, int &row, int &col, char **&ppRet);
	~AutoGetResTable();
public:
	AutoGetResTable(const AutoGetResTable &) = delete;
	AutoGetResTable& operator=(const AutoGetResTable &) = delete;
private:
	SqliteManager *m_db;
	char         **m_ppRet;
};

////////////////////////////////////////////////////////////////////////

#define DOC_DB     "doc.db"
#define DOC_TABLE  "doc_tb"

//���ݿ����ģ��
class DataManager
{
public:
	static DataManager& CreateInstance();
public:
	~DataManager();
public:
	void InitSqlite();
public:
	//�����ݿ��ȡ����
	void GetDocs(const string &path, set<string>& docs);
	//�����ݿ�����ĵ�
	void InsertDoc(const string &path, string doc);
	//�����ݿ�ɾ���ĵ�
	void DeleteDoc(const string &path, string doc);
public:
	void Search(const string &key, vector<pair<string, string>> &doc_path);
	static void SplitHightLight(const string &str, const string &key, string &prefix, string &hightlight, string &suffix);
private:
	DataManager();
private:
	SqliteManager m_dbmgr;
};

