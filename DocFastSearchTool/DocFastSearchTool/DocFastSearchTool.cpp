#include"Common.h"
#include"Sysutil.h"

#include"ScanManager.h"
#include"DataManager.h"

char *title = "文档快速搜索工具";

void main()
{
	//创建扫描实例
	const string path = "C:\\Users\\baoso\\Desktop\\55班\\test\\阶段性考试试卷\\my_dir";
	//ScanManager sm;
	ScanManager::CreateInstance(path).ScanDirectory(path);

	//创建搜索实例
	string key;
	vector<pair<string, string>> doc_path; //用于保存搜索的结果
	DataManager &dm = DataManager::CreateInstance();

	int row = 5;
	int count;
	while(1)
	{
		DrawFrame(title);
		DrawMenu();
		cin>>key;
		if(key == string("exit"))
			break;
		dm.Search(key, doc_path);

		//显示结果
		count = 0;
		for(auto &e : doc_path)
		{
			string &name = e.first; //
			string &path = e.second;

			//高亮处理
			string prefix, hightlight, suffix;
			DataManager::SplitHightLight(name, key, prefix, hightlight, suffix);

			SetCurPos(row+count++, 2);
			printf("%s",prefix.c_str());
			ColourPrintf(hightlight);
			printf("%-20s", suffix.c_str());

			SetCurPos(row+count-1, 2+30);
			printf("%-85s\n", path.c_str());
		}
		doc_path.clear();
		
		SystemEnd();
		system("pause");
	}

	SystemEnd();
}


/*
void Test_GetFileCount()
{
	const string path = "C:\\Users\\baoso\\Desktop\\55班\\test\\阶段性考试试卷\\my_dir";
	size_t count = GetFileCount(path);
	cout<<"count = "<<count<<endl;
}

void main()
{
	Test_GetFileCount();
}

mutex mt;
condition_variable cond;
condition_variable cond1;
condition_variable cond2;
//scan_thread
void thread_fun1()
{
	while(1)
	{
		printf("1111111111111111\n");
		unique_lock<mutex> lock(mt);  
		cond.wait(lock);//阻塞等待

		printf("This is thread_fun1().\n");
	}
}
//watch_thread
void thread_fun2()
{
	int count = 0;
	while(1)
	{
		count++;
		if(count == 5)
		{
			count = 0;
			printf("send wake up signal.\n");
			cond1.notify_one();
		}
		else
			printf("This is thread_fun2().\n");
		Sleep(1000);	
	}
}

void Test_Thread()
{
	thread th1(thread_fun1);
	thread th2(thread_fun2);

	th1.join();
	th2.join();
}

void main()
{
	Test_Thread();
}


void Test_HightLight()
{
	string str = "123比特科技,让就业更简单666"; 
	//string key = "就业你好";
	//string key = "keji";
	string key = "jy";
	string prefix, hightlight, suffix;
	DataManager::SplitHightLight(str, key, prefix, hightlight, suffix);
	cout<<prefix;
	ColourPrintf(hightlight);
	cout<<suffix<<endl;
}
void main()
{
	Test_HightLight();
}


/*
void Test_DirectoryList()
{
	string path = "C:\\Users\\baoso\\Desktop\\55班\\test\\阶段性考试试卷";
	vector<string> subfile, subdir;
	DirectoryList(path, subfile, subdir);

	for(auto &e : subfile)
	{
		//cout<<typeid(e).name()<<endl; //string
		cout<<e<<endl;
	}
	for(auto &e : subdir)
		cout<<e<<endl;
}

void Test_Search()
{
	//创建扫描实例
	const string path = "C:\\Users\\baoso\\Desktop\\55班\\test\\阶段性考试试卷";
	ScanManager sm;
	sm.ScanDirectory(path);

	//创建搜索实例
	string key;
	vector<pair<string, string>> doc_path; //用于保存搜索的结果

	DataManager dm;

	//
	while(1)
	{
		cout<<"请输入要搜索的关键字:>";
		cin>>key;
		dm.Search(key, doc_path);
		//显示结果
		printf("%-30s%-50s\n","名称","路径");
		for(auto &e : doc_path)
		{
			string &name = e.first; //
			string &path = e.second;
			printf("%-30s%-50s\n", name.c_str(), path.c_str());
		}

		doc_path.clear();
	}
}

int main(int argc, char *argv[])
{
	//Test_DirectoryList();
	Test_Search();
	return 0;
}
*/