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
		cout << "=================Huffman压缩小程序=========================" << endl;
		cout << "1.压缩文件" << endl;
		cout << "2.解压文件" << endl;
		cout << "3.退出" << endl;

		cout << "请输入功能选项：";
		cin >> choice;
		if (choice == 1)
		{
			char fileName[256] = { 0 };
			cout << "请输入所要压缩的文件名(包括后缀名)："<<endl;
			cin >> fileName;
			compress(fileName);
		}
		else if(choice==2)
		{
			char zippedFileName[256] = { 0 };
			char deCompressFileName[256] = { 0 };
			cout << "请输入所要解压的文件名(包括后缀名)：" << endl;
			cin >> zippedFileName;
			cout << "请输入解压后的文件名(包括后缀名)：" << endl;
			cin >> deCompressFileName;
			deCompress(zippedFileName,deCompressFileName);
		}
		else if(choice==3)
		{
			cout << "退出成功!" << endl;
			break;
		}
	}
	return 0;
}