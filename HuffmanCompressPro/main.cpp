#include"Huffman.h"
#include"Compress.h"
#include<fstream>
#include<iostream>
using namespace std;
#pragma warning(disable:4996)

int main()
{
	int choice;

	while (true)
	{
		cout << "=================Huffmanѹ��С����=========================" << endl;
		cout << "1.ѹ���ļ�" << endl;
		cout << "2.��ѹ�ļ�" << endl;
		cout << "3.�˳�" << endl;

		cout << "�����빦��ѡ�";
		cin >> choice;
		if (choice == 1)
		{
			char fileName[256] = { 0 };
			cout << "��������Ҫѹ�����ļ���(������׺��)��"<<endl;
			cin >> fileName;
			compress(fileName);
		}
		else if(choice==2)
		{
			char zippedFileName[256] = { 0 };
			char deCompressFileName[256] = { 0 };
			cout << "��������Ҫ��ѹ���ļ���(������׺��)��" << endl;
			cin >> zippedFileName;
			cout << "�������ѹ����ļ���(������׺��)��" << endl;
			cin >> deCompressFileName;
			deCompress(zippedFileName,deCompressFileName);
		}
		else if(choice==3)
		{
			cout << "�˳��ɹ�!" << endl;
			break;
		}
	}
	return 0;
}