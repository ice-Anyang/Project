#include"ScanManager.h"

size_t g_FileCount = 0;
size_t g_ScanCount = 0;

ScanManager::ScanManager()
{}

void ScanManager::StartScan(const string &path)
{
	while(1)
	{
		//this_thread::sleep_for(chrono::seconds(2)); //

		unique_lock<mutex> lock(m_mutex);
		m_cond.wait(lock);

		ScanDirectory(path);
	}
}
void ScanManager::StartWatch(const string &path)
{
	//�������������
	g_FileCount = GetFileCount(path);
	while(1)
	{
		//���pathĿ¼���ļ������Ƿ��б仯
		g_ScanCount = 0;
		bool ischange = DirectoryWatch(path);
		if(ischange)
		{
			//����ɨ���ź�
			m_cond.notify_one();
			g_FileCount = g_ScanCount;
		}
		this_thread::sleep_for(chrono::seconds(3));
	}
}

ScanManager& ScanManager::CreateInstance(const string &path)
{
	static ScanManager inst;
	//����ɨ���߳�
	thread scan_thread(&StartScan, &inst, path);
	scan_thread.detach();

	//��������߳�
	thread watch_thread(&StartWatch, &inst, path);
	watch_thread.detach();
	return inst;
}

void ScanManager::ScanDirectory(const string &path)
{
	//1 ɨ�豾���ļ� �����д洢
	vector<string> local_files;
	vector<string> local_dirs;
	DirectoryList(path, local_files, local_dirs);
	set<string> local_set;
	local_set.insert(local_files.begin(), local_files.end());
	local_set.insert(local_dirs.begin(), local_dirs.end());

	DataManager &m_db = DataManager::CreateInstance();

	//2 ɨ�����ݿ��ļ� �����д洢
	set<string> db_set;
	//��ѯ�������ݿ�
	m_db.GetDocs(path, db_set);

	//3 �����ļ��Ա�
	auto local_it = local_set.begin();
	auto db_it = db_set.begin();
	while(local_it!=local_set.end() && db_it!=db_set.end())
	{
		if(*local_it < *db_it)
		{
			//�����ļ����ڣ����ݿ��ļ������ڣ������ݿ������ļ�
			//�����ļ�
			m_db.InsertDoc(path, *local_it);
			++local_it;
		}
		else if(*local_it > *db_it)
		{
			//�����ļ������ڣ����ݿ��ļ����ڣ������ݿ�ɾ���ļ�
			//ɾ���ļ�
			m_db.DeleteDoc(path, *db_it);
			++db_it;
		}
		else
		{
			//�����ļ����ڣ����ݿ��ļ�Ҳ���ڣ������ݿⲻ��
			++local_it;
			++db_it;
		}
	}

	//�����ļ����ڣ� �������ݿ��ļ�
	while(local_it != local_set.end())
	{
		//�����ļ�
		m_db.InsertDoc(path, *local_it);
		++local_it;
	}

	//���ݿ��ļ����ڣ� ɾ�����ݿ��ļ�
	while(db_it != db_set.end())
	{
		//ɾ���ļ�
		m_db.DeleteDoc(path, *db_it);
		++db_it;
	}

	//�ݹ������Ŀ¼
	for(auto &dir : local_dirs)
	{
		//E:\\Users\\Documents\\�����רҵ�μ�
		string dir_path = path;
		dir_path += '\\';
		dir_path += dir;

		ScanDirectory(dir_path);
	}
}