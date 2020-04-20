#include"DataManager.h"

#pragma comment(lib, "./sqlite/sqlite3.lib")

SqliteManager::SqliteManager() :m_db(nullptr)
{}
SqliteManager::~SqliteManager()
{
	Close();
}
void SqliteManager::Open(const string &path)
{
	int rc = sqlite3_open(path.c_str(), &m_db);
	if(rc != SQLITE_OK)
	{
		ERROR_LOG("Can't open database");
		return;
	}
	else
		TRACE_LOG("Opened database successfully")
}

void SqliteManager::Close()
{
	if(m_db)
	{
		int rc = sqlite3_close(m_db);
		if(rc != SQLITE_OK)
		{
			ERROR_LOG("Close Database failed.");
			return;
		}
		else
			TRACE_LOG("Close Database successfully.");
		m_db = nullptr;
	}
}

void SqliteManager::ExecuteSql(const string &sql)
{
	char *zErrMsg = 0;
	int rc = sqlite3_exec(m_db, sql.c_str(), 0, 0, &zErrMsg);
	if (rc != SQLITE_OK)
	{
		ERROR_LOG("SQL error(sql: %s): %s\n",sql.c_str(), zErrMsg);
		sqlite3_free(zErrMsg);
	}
	else
	{
		TRACE_LOG("Exec Sql(sql:%s) successfully\n", sql.c_str());
	}
}

void SqliteManager::GetResTable(const string& sql, int &row, int &col, char**&ppRet)
{
	char *zErrMsg = 0;
	int rc = sqlite3_get_table(m_db, sql.c_str(), &ppRet, &row, &col, &zErrMsg);

	if (rc != SQLITE_OK)
	{
		ERROR_LOG("GetResTable error(sql: %s): %s\n",sql.c_str(), zErrMsg);
		sqlite3_free(zErrMsg);
	}
	else
	{
		TRACE_LOG("GetResTable  successfully");
	}
}

//////////////////////////////////////////////////////////////////////
//�Զ���ȡ������ ʵ��
AutoGetResTable::AutoGetResTable(SqliteManager *db, const string &sql, int &row, int &col, char **&ppRet)
:m_db(db),m_ppRet(nullptr)
{
	m_db->GetResTable(sql, row, col, ppRet);
	m_ppRet = ppRet;

}
AutoGetResTable::~AutoGetResTable()
{
	if(m_ppRet)
		sqlite3_free_table(m_ppRet);
}

/////////////////////////////////////////////////////////////////////

DataManager& DataManager::CreateInstance()
{
	static DataManager inst;
	return inst;
}

DataManager::DataManager()
{
	m_dbmgr.Open(DOC_DB);
	
	//�������ݿ��
	InitSqlite();

	//���
	ClearDoc();
}
DataManager::~DataManager()
{}

void DataManager::InitSqlite()
{
	char sql[MAX_SQL_SIZE] = {0};
	sprintf(sql,  "create table if not exists %s (id INTEGER PRIMARY KEY autoincrement, doc_path text, doc_name text,doc_name_pinyin text, doc_name_initials text)", DOC_TABLE);
	m_dbmgr.ExecuteSql(sql);
}

//�����ݿ��ȡ����
void DataManager::GetDocs(const string &path, set<string>& docs)
{
	//select doc_name from doc_tb where doc_path = path.c_str()
	char sql[MAX_SQL_SIZE] = {0};
	sprintf(sql, "select doc_name from %s where doc_path='%s'", DOC_TABLE, path.c_str());

	int row=0, col=0;
	char **ppRet = 0;

	//��ȡ�����
	//m_dbmgr.GetResTable(sql, row, col, ppRet);
	AutoGetResTable at(&m_dbmgr, sql, row, col, ppRet);

    //��������浽set
	for(int i=1; i<=row; ++i)
		docs.insert(ppRet[i]);

	//�ͷŽ����
	//sqlite3_free_table(ppRet);
}
//�����ݿ�����ĵ�
void DataManager::InsertDoc(const string &path, string doc)
{
	char sql[MAX_SQL_SIZE] = {0};
	string pinyin = ChineseConvertPinYinAllSpell(doc);
	string initials = ChineseConvertPinYinInitials(doc);

	sprintf(sql, "insert into %s values(NULL, '%s', '%s', '%s', '%s')",
			DOC_TABLE, path.c_str(), doc.c_str(), pinyin.c_str(), initials.c_str());
	m_dbmgr.ExecuteSql(sql);
}

//�����ݿ�ɾ���ĵ�
void DataManager::DeleteDoc(const string &path, string doc)
{
	char sql[MAX_SQL_SIZE] = {0};
	sprintf(sql, "delete from  %s where doc_path='%s' and doc_name='%s'", DOC_TABLE, path.c_str(), doc.c_str());
	m_dbmgr.ExecuteSql(sql);

	//�ݹ�ɾ����Ŀ¼
	//E:\\Users\\Documents\\�����רҵ�μ�
	string doc_path = path;
	doc_path += "\\";
	//E:\\Users\\Documents\\�����רҵ�μ�
	doc_path += doc;
	//E:\\Users\\Documents\\�����רҵ�μ�\AA
	sprintf(sql, "delete from %s where doc_path like '%s%%'", DOC_TABLE, doc_path.c_str());
	m_dbmgr.ExecuteSql(sql);
}

void DataManager::ClearDoc()
{
	char sql[MAX_SQL_SIZE] = {0};
	sprintf(sql, "delete from %s", DOC_TABLE);
	m_dbmgr.ExecuteSql(sql);
}

void DataManager::Search(const string &key, vector<pair<string, string>> &doc_path)
{
	char sql[MAX_SQL_SIZE] = {0};
	int row=0, col=0;
	char **ppRet = 0;
	
	//�Ƽ�   %keji%
	string key_pinyin = "%";
	key_pinyin += ChineseConvertPinYinAllSpell(key);
	key_pinyin +="%";

	//�Ƽ�   %kj%
	string key_initials = "%";
	key_initials += ChineseConvertPinYinInitials(key);
	key_initials += "%";

	//select dco_name, doc_path from doc_tb where doc_name like '%%s%';
	sprintf(sql, "select doc_name, doc_path from %s where doc_name_pinyin like '%s' or doc_name_initials like '%s'",
			DOC_TABLE, key_pinyin.c_str(), key_initials.c_str());

	//m_dbmgr.GetResTable(sql, row, col, ppRet);
	AutoGetResTable at(&m_dbmgr, sql, row, col, ppRet);


	for(int i=1; i<=row; ++i)
	{   										//name            path
		doc_path.push_back(make_pair(ppRet[i*col+0], ppRet[i*col+1]));
	}
	
	//sqlite3_free_table(ppRet);  //Դ���뼶��
}

void DataManager::SplitHightLight(const string &str, const string &key, 
								  string &prefix, string &hightlight, string &suffix)
{
	//str = "123���ؿƼ�,�þ�ҵ����666"; 
	//key = "��ҵ";

	string strlower = str;
	string keylower = key;
	transform(str.begin(), str.end(), strlower.begin(), tolower);
	transform(key.begin(), key.end(), keylower.begin(), tolower);

	//1 ��������, ���ƥ�䣬ֱ�ӷָ�
	size_t pos = strlower.find(keylower);
	if(pos != string::npos)
	{
		prefix = str.substr(0, pos);
		hightlight = str.substr(pos, keylower.size());
		suffix = str.substr(pos+keylower.size(), string::npos);
		return;
	}

	//2 ƴ��ȫƴ�����������ƥ�䣬����Ҫƥ�����ĺ��ֺ�ƴ��

	string str_pinyin = ChineseConvertPinYinAllSpell(strlower);
	string key_pinyin = ChineseConvertPinYinAllSpell(keylower);
	pos = str_pinyin.find(key_pinyin);
	if(pos != string::npos)
	{
		size_t str_index = 0;
		size_t pinyin_index = 0;

		size_t hightlight_index = 0;
		size_t hightlight_len = 0;

		while(str_index < strlower.size())
		{
			if(pinyin_index == pos)
			{
				hightlight_index = str_index;
			}
			if(pinyin_index >= pos + key_pinyin.size())
			{
				hightlight_len = str_index - hightlight_index + 1;
				break;
			}
			//�ж�ԭ�ַ���str_index�±���ָ�Ƿ�Ϊ�ַ�
			if(strlower[str_index]>=0 && strlower[str_index]<=127)
			{
				//���ַ�
				str_index++;
				pinyin_index++;
			}
			else
			{
				//�Ǻ���
				string  word(strlower, str_index, 2); //��ȡһ������
				string  subpinyin = ChineseConvertPinYinAllSpell(word);
				str_index += 2;
				pinyin_index += subpinyin.size();
			}
		}
		prefix = str.substr(0, hightlight_index);
		hightlight = str.substr(hightlight_index, hightlight_len);
		suffix = str.substr(hightlight_index+hightlight_len, string::npos);
		return;
	}

	//3 ����ĸ���� �����ƥ�䣬����Ҫƥ����뺺�ֺ�����ĸ
	std::string key_initials = ChineseConvertPinYinInitials(keylower);
	std::string str_initials = ChineseConvertPinYinInitials(strlower);
	pos = str_initials.find(key_initials);
	if (pos != std::string::npos)
	{
		size_t str_index = 0;
		size_t initials_index = 0;

		size_t highlight_index = 0;
		size_t highlight_len = 0;

		while (str_index < strlower.size())
		{
			// ���ƴ��λ���Ѿ��ߵ��Ӵ�ƥ���λ�ã���ԭ����λ�þ��Ǹ�������ʼλ��
			if (initials_index == pos)
			{
				highlight_index = str_index;
			}

			// ���ƴ��λ���Ѿ�����keylower��λ�ã���ԭ����λ�þ��Ǹ����Ľ���λ��
			if (initials_index >= pos + key_initials.size())
			{
				highlight_len = str_index - highlight_index + 1;
				break;
			}

			if (strlower[str_index] >= 0 && strlower[str_index] <= 127)
			{
				// ������ַ������һ��λ��
				++str_index;
				++initials_index;
			}
			else
			{
				// ����Ǻ���ԭ���ĺ����������ֽ�(gbk�ĺ����������ַ�)������ĸ��һ��
				str_index += 2;
				++initials_index;
			}
		}

		prefix = str.substr(0, highlight_index);
		hightlight = str.substr(highlight_index, highlight_len);
		suffix = str.substr(highlight_index + highlight_len, std::string::npos);
		return;
	}

	//����ʧ�ܣ����ղ���������
	prefix = str;
	hightlight.clear();
	suffix.clear();
}