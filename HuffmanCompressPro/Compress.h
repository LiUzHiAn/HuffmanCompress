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
	char type[4];  // 文件类型
	int length;	   // 原文件长度
	int weight[256];  // 权重信息
};
// 读取文件，统计各字符出现频率
int countFrequency(const char* fileName, int *weight);
// 打开一个原文件，并进行压缩到指定目的文件
int compress(const char* fileSoruceName);
// 将八位0-1字符串转化为对应的字符
unsigned char str2byte(const char* pBinStr);
// 根据Huffman编码对文件进行压缩编码
unsigned char* encode(const char* fileName, char** code,  long nSize);
// 根据原文件生成文件头，放在压缩后的文件的首部
int initHead(const char* fileName, Head* head);
// 将压缩好的内容保存到文件
int writeToFile(const char* fileName, Head *head, unsigned char* buffer,  long  nSize);
// 解压缩文件
int deCompress(const char* zipFileName, const char* desFileName);
// 统计文件大小，成功时返回值为统计结果
int countFileSize(const char* fileName);