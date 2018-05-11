#include"Compress.h"
#include"Huffman.h"
using namespace std;

// 统计文件大小，成功时返回值为统计结果
int countFileSize(const char* fileName)
{
	FILE *in = fopen(fileName, "rb");
	// 文件打开失败
	if (in == NULL)
		return FILE_IO_ERROR;
	int iRead;
	unsigned char ch;
	long cnt = 0;
	while (true)
	{
		iRead = fread(&ch, 1, sizeof(unsigned char), in);
		if (iRead == 0)
			break;
		cnt++;
	}
	fclose(in);
	in = NULL;
	return cnt;
}

// 统计文件中各字符出现频率
int countFrequency(const char* fileName,int *weight)
{
	FILE *in = fopen(fileName, "rb");
	// 文件打开失败
	if (in==NULL) 
		return FILE_IO_ERROR;
	int iRead;
	unsigned char ch;
	while (true)
	{
		iRead = fread(&ch, 1, sizeof(unsigned char), in);
		if (iRead == 0)
			break;
		weight[ch]++;
	}
	fclose(in);
	in = NULL;
	return SUCCESS;
}

// 将0-1字符串前八位转化为对应的字符
unsigned char str2byte(const char* pBinStr)
{
	unsigned char b = 0x00;
	for (int i = 0;i < 8;i++)
	{
		b = b << 1;// 左移一位
		if (pBinStr[i] == '1')
			b = b | 0x01;
	}
	return b;
}

// 压缩指定文件名的文件，并生成.huf后缀的压缩文件
int compress(const char* fileSoruceName)
{
	int weight[256] = { 0 };
	// 读取文件，统计得到权重数值
	int countReslt = countFrequency(fileSoruceName, weight);
	// 文件不存在
	if (countReslt != 1)
	{
		cout << "文件打开失败，请检查文件是否存在！" << endl;
		return FILE_IO_ERROR;
	}
	// 根据权值生成Huffman树
	HuffmanNode *tree = generateHuffmanTree(weight, 256);
	// 打印Huffman树的信息
//	printTreeInfo(tree,0,510);
	// 根据Huffman树生成对应的Huffman编码
	char **code = huffmanCoding(tree);
//	printTreeInfo(tree, 0, 510);
	// 打印Huffman编码信息
//	printHuffmanCodeInfo(code);
	// 计算编码所需缓冲区大小
	long nSize = 0;
	for (int i = 0;i < 256;i++)
	{
		nSize += weight[i] * strlen(code[i]);
	}
	// 如果所有位数被8整除还有余数，最后一个添0
	nSize = (nSize % 8) ? nSize / 8 + 1 : nSize / 8;
	
	unsigned char *buffer =encode(fileSoruceName, code, nSize);
	if (buffer==NULL) return ALLOCATE_ERROR;
	
	Head *head = new Head;
	initHead(fileSoruceName, head);
	writeToFile(fileSoruceName, head, buffer, nSize);

	// 释放new创建的空间
	//delete tree;
	/*for (int i = 0;i < 256;i++)
		delete code[i];*/
	//delete buffer;
	return SUCCESS;
}

// 根据nSize，创建一个char数组作为缓冲区，将压缩后的数据保存到缓冲区并返回该缓冲区
unsigned char* encode(const char* fileName,  char** code,  long  nSize)
{
	FILE *in = fopen(fileName, "rb");
	if (!in) return NULL;
	unsigned char *pBuffer = new unsigned char[nSize];
	char cd[256] = { 0 }; // 工作区，Huffman编码最长为256位
	unsigned char ch,trans_ch;
	int pos = 0;
	int sourceFileLen = 0;
	int iRead;
	// 边读取源文件边编码
	while (true)
	{
		iRead = fread(&ch, 1, sizeof(unsigned char), in);
		if (iRead == 0) break;
		sourceFileLen++;
		strcat(cd, code[ch]);
		//压缩编码
		while (strlen(cd) >= 8)
		{
			// 截取字符串左边8位，转化为对应的字符，放在缓冲区
			trans_ch= str2byte(cd);
			pBuffer[pos++] = trans_ch;
			
			// 整体左移8位
			for (int i = 0;i < 256 - 8;i++)
			{
				cd[i] = cd[i + 8];
			}
		}
	}
	// 最后操作完还有几位0-1字符剩余，相当于在最后补0至8位
	if (strlen(cd) > 0)
	{
		trans_ch = str2byte(cd);
		pBuffer[pos++] = trans_ch;
	}
	fclose(in);
	
	return pBuffer;
}

// 根据原文件信息，生成一个压缩文件头
int initHead(const char* fileName, Head* head)
{
	// 文件类型
	strcpy(head->type, "huf");
	// 原文件长度
	head->length = countFileSize(fileName);
	// 初始化原文件字符权重
	for (int i = 0; i < 256; i++)
	{
		head->weight[i] = 0;
	}
	// 统计文件字符出现频率
	int countResult=countFrequency(fileName, head->weight);
	if (countResult != 1)
	{
		cout << "初始化编码文件头信息失败！" << endl;
		return FILE_IO_ERROR;
	}
	return SUCCESS;

}

// 根据压缩后的编码，压缩头生成压缩后的文件
int writeToFile(const char* fileName, Head *head, unsigned char* buffer,  long  nSize)
{
	// 生成文件名
	char saveFileName[256] = { 0 };
	strcpy(saveFileName, fileName);
	strcat(saveFileName, ".huf");

	FILE *out = fopen(saveFileName, "wb");
	if (out == NULL)
	{
		cout << "写入文件时打开文件失败！" << endl;
		return FILE_IO_ERROR;
	}
	// 写头文件
	fwrite(head, sizeof(Head), 1, out);
	// 写压缩后的编码
	fwrite(buffer, sizeof(unsigned char), nSize, out);
	// 关闭文件
	fclose(out);
	out = NULL;
	cout << "生成压缩文件" << saveFileName << "成功！" << endl;
	cout << "压缩后文件大小：" << sizeof(Head)+nSize << endl;
	cout << "压缩前文件大小：" << head->length << endl;
	cout << "压缩比：" << (double)(sizeof(Head) + nSize) / head->length << endl;
	return SUCCESS;
}

// 根据压缩文件名，解压文件，生成还原后的文件
int deCompress(const char* zipFileName,const char* desFileName)
{
	Head *head = new Head;
	FILE *in = fopen(zipFileName, "rb");
	if (!in)
	{
		cout << "打开文件失败！" << endl;
		return FILE_IO_ERROR;
	}
	int iRead = fread(head, sizeof(Head), 1, in);
	if (iRead==0)
	{
		cout << "读取头文件信息失败！" << endl;
		return HEAD_ORERATE_ERROR;
	}
	HuffmanNode *tree = generateHuffmanTree(head->weight, 256);
	//printTreeInfo(tree, 0, 510);
	char ** code = huffmanCoding(tree);
	unsigned char ch;
	unsigned char b = 0x80;
	// 用于指示是否解压到底
	int pos = 0;
	int p = 510;
	FILE* out = fopen(desFileName,"wb");
	while (true)
	{
		iRead=fread(&ch, sizeof(unsigned char), 1, in);
		if (iRead == 0)
			break;
		for (int i = 0;i < 8;i++)
		{
			if ((ch&b) != 0)  // 当前位为1，往树的右边走
			{
				p = tree[p].right_node;
			}
			else   // 当前位为0，往树的左边走
			{
				p = tree[p].left_node;
			}
			// 如果到达了叶子结点，就向文件中写
			if (tree[p].left_node == -1 && tree[p].right_node == -1)
			{
				unsigned char b=p;
				fwrite(&b, sizeof(unsigned char), 1, out);
				pos++;
				if (pos == head->length)  // 达到了原文件的长度
					break;
				p = 510;  // 回到根节点
			}
			ch = ch << 1;  // 左移一位
		}
		if (pos == head->length)
			break;
	}
	fclose(in);
	fclose(out);
	cout << "解压成功！" << endl;
	return SUCCESS;
}