#include"Common.h"
#include"Sysutil.h"

#include"ScanManager.h"
#include"DataManager.h"

char *title = "�ĵ�������������";

int main()
{
	//����ɨ��ʵ��
	const string path = "E:\\Users\\Documents\\�����רҵ�μ�";
	//ScanManager sm;
	ScanManager::CreateInstance(path).ScanDirectory(path);

	//��������ʵ��
	string key;
	vector<pair<string, string>> doc_path; //���ڱ��������Ľ��
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

		//��ʾ���
		count = 0;
		for(auto &e : doc_path)
		{
			string &name = e.first; //
			string &path = e.second;

			//��������
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
	return 0;
}



