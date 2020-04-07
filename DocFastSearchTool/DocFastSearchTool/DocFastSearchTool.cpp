#include"Common.h"
#include"Sysutil.h"

#include"ScanManager.h"
#include"DataManager.h"

char *title = "�ĵ�������������";

void main()
{
	//����ɨ��ʵ��
	const string path = "E:\\Ice_bitwork\\bit-githup\\�����רҵ�μ�";
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

			SetCurPos(row+count++, 2);
			//��������
			printf("%-30s%-85s\n", name.c_str(), path.c_str());
		}
		doc_path.clear();
		
		SystemEnd();
		system("pause");
	}

	SystemEnd();
}

