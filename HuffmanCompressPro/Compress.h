#pragma once
#include<string>
#include<iostream>
#include<fstream>
#define FILE_IO_ERROR -1
#define ALLOCATE_ERROR -2
#define HEAD_ORERATE_ERROR -3
#define SUCCESS 1
using namespace std;

struct Head
{
	char type[4];  // �ļ�����
	int length;	   // ԭ�ļ�����
	int weight[256];  // Ȩ����Ϣ
};
// ��ȡ�ļ���ͳ�Ƹ��ַ�����Ƶ��
int countFrequency(const char* fileName, int *weight);
// ��һ��ԭ�ļ���������ѹ����ָ��Ŀ���ļ�
int compress(const char* fileSoruceName);
// ����λ0-1�ַ���ת��Ϊ��Ӧ���ַ�
unsigned char str2byte(const char* pBinStr);
// ����Huffman������ļ�����ѹ������
unsigned char* encode(const char* fileName, char** code,  long nSize);
// ����ԭ�ļ������ļ�ͷ������ѹ������ļ����ײ�
int initHead(const char* fileName, Head* head);
// ��ѹ���õ����ݱ��浽�ļ�
int writeToFile(const char* fileName, Head *head, unsigned char* buffer,  long  nSize);
// ��ѹ���ļ�
int deCompress(const char* zipFileName, const char* desFileName);
// ͳ���ļ���С���ɹ�ʱ����ֵΪͳ�ƽ��
int countFileSize(const char* fileName);