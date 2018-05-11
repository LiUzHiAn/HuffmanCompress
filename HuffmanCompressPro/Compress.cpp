#include"Compress.h"
#include"Huffman.h"
using namespace std;

// ͳ���ļ���С���ɹ�ʱ����ֵΪͳ�ƽ��
int countFileSize(const char* fileName)
{
	FILE *in = fopen(fileName, "rb");
	// �ļ���ʧ��
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

// ͳ���ļ��и��ַ�����Ƶ��
int countFrequency(const char* fileName,int *weight)
{
	FILE *in = fopen(fileName, "rb");
	// �ļ���ʧ��
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

// ��0-1�ַ���ǰ��λת��Ϊ��Ӧ���ַ�
unsigned char str2byte(const char* pBinStr)
{
	unsigned char b = 0x00;
	for (int i = 0;i < 8;i++)
	{
		b = b << 1;// ����һλ
		if (pBinStr[i] == '1')
			b = b | 0x01;
	}
	return b;
}

// ѹ��ָ���ļ������ļ���������.huf��׺��ѹ���ļ�
int compress(const char* fileSoruceName)
{
	int weight[256] = { 0 };
	// ��ȡ�ļ���ͳ�Ƶõ�Ȩ����ֵ
	int countReslt = countFrequency(fileSoruceName, weight);
	// �ļ�������
	if (countReslt != 1)
	{
		cout << "�ļ���ʧ�ܣ������ļ��Ƿ���ڣ�" << endl;
		return FILE_IO_ERROR;
	}
	// ����Ȩֵ����Huffman��
	HuffmanNode *tree = generateHuffmanTree(weight, 256);
	// ��ӡHuffman������Ϣ
//	printTreeInfo(tree,0,510);
	// ����Huffman�����ɶ�Ӧ��Huffman����
	char **code = huffmanCoding(tree);
//	printTreeInfo(tree, 0, 510);
	// ��ӡHuffman������Ϣ
//	printHuffmanCodeInfo(code);
	// ����������軺������С
	long nSize = 0;
	for (int i = 0;i < 256;i++)
	{
		nSize += weight[i] * strlen(code[i]);
	}
	// �������λ����8�����������������һ����0
	nSize = (nSize % 8) ? nSize / 8 + 1 : nSize / 8;
	
	unsigned char *buffer =encode(fileSoruceName, code, nSize);
	if (buffer==NULL) return ALLOCATE_ERROR;
	
	Head *head = new Head;
	initHead(fileSoruceName, head);
	writeToFile(fileSoruceName, head, buffer, nSize);

	// �ͷ�new�����Ŀռ�
	//delete tree;
	/*for (int i = 0;i < 256;i++)
		delete code[i];*/
	//delete buffer;
	return SUCCESS;
}

// ����nSize������һ��char������Ϊ����������ѹ��������ݱ��浽�����������ظû�����
unsigned char* encode(const char* fileName,  char** code,  long  nSize)
{
	FILE *in = fopen(fileName, "rb");
	if (!in) return NULL;
	unsigned char *pBuffer = new unsigned char[nSize];
	char cd[256] = { 0 }; // ��������Huffman�����Ϊ256λ
	unsigned char ch,trans_ch;
	int pos = 0;
	int sourceFileLen = 0;
	int iRead;
	// �߶�ȡԴ�ļ��߱���
	while (true)
	{
		iRead = fread(&ch, 1, sizeof(unsigned char), in);
		if (iRead == 0) break;
		sourceFileLen++;
		strcat(cd, code[ch]);
		//ѹ������
		while (strlen(cd) >= 8)
		{
			// ��ȡ�ַ������8λ��ת��Ϊ��Ӧ���ַ������ڻ�����
			trans_ch= str2byte(cd);
			pBuffer[pos++] = trans_ch;
			
			// ��������8λ
			for (int i = 0;i < 256 - 8;i++)
			{
				cd[i] = cd[i + 8];
			}
		}
	}
	// �������껹�м�λ0-1�ַ�ʣ�࣬�൱�������0��8λ
	if (strlen(cd) > 0)
	{
		trans_ch = str2byte(cd);
		pBuffer[pos++] = trans_ch;
	}
	fclose(in);
	
	return pBuffer;
}

// ����ԭ�ļ���Ϣ������һ��ѹ���ļ�ͷ
int initHead(const char* fileName, Head* head)
{
	// �ļ�����
	strcpy(head->type, "huf");
	// ԭ�ļ�����
	head->length = countFileSize(fileName);
	// ��ʼ��ԭ�ļ��ַ�Ȩ��
	for (int i = 0; i < 256; i++)
	{
		head->weight[i] = 0;
	}
	// ͳ���ļ��ַ�����Ƶ��
	int countResult=countFrequency(fileName, head->weight);
	if (countResult != 1)
	{
		cout << "��ʼ�������ļ�ͷ��Ϣʧ�ܣ�" << endl;
		return FILE_IO_ERROR;
	}
	return SUCCESS;

}

// ����ѹ����ı��룬ѹ��ͷ����ѹ������ļ�
int writeToFile(const char* fileName, Head *head, unsigned char* buffer,  long  nSize)
{
	// �����ļ���
	char saveFileName[256] = { 0 };
	strcpy(saveFileName, fileName);
	strcat(saveFileName, ".huf");

	FILE *out = fopen(saveFileName, "wb");
	if (out == NULL)
	{
		cout << "д���ļ�ʱ���ļ�ʧ�ܣ�" << endl;
		return FILE_IO_ERROR;
	}
	// дͷ�ļ�
	fwrite(head, sizeof(Head), 1, out);
	// дѹ����ı���
	fwrite(buffer, sizeof(unsigned char), nSize, out);
	// �ر��ļ�
	fclose(out);
	out = NULL;
	cout << "����ѹ���ļ�" << saveFileName << "�ɹ���" << endl;
	cout << "ѹ�����ļ���С��" << sizeof(Head)+nSize << endl;
	cout << "ѹ��ǰ�ļ���С��" << head->length << endl;
	cout << "ѹ���ȣ�" << (double)(sizeof(Head) + nSize) / head->length << endl;
	return SUCCESS;
}

// ����ѹ���ļ�������ѹ�ļ������ɻ�ԭ����ļ�
int deCompress(const char* zipFileName,const char* desFileName)
{
	Head *head = new Head;
	FILE *in = fopen(zipFileName, "rb");
	if (!in)
	{
		cout << "���ļ�ʧ�ܣ�" << endl;
		return FILE_IO_ERROR;
	}
	int iRead = fread(head, sizeof(Head), 1, in);
	if (iRead==0)
	{
		cout << "��ȡͷ�ļ���Ϣʧ�ܣ�" << endl;
		return HEAD_ORERATE_ERROR;
	}
	HuffmanNode *tree = generateHuffmanTree(head->weight, 256);
	//printTreeInfo(tree, 0, 510);
	char ** code = huffmanCoding(tree);
	unsigned char ch;
	unsigned char b = 0x80;
	// ����ָʾ�Ƿ��ѹ����
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
			if ((ch&b) != 0)  // ��ǰλΪ1���������ұ���
			{
				p = tree[p].right_node;
			}
			else   // ��ǰλΪ0�������������
			{
				p = tree[p].left_node;
			}
			// ���������Ҷ�ӽ�㣬�����ļ���д
			if (tree[p].left_node == -1 && tree[p].right_node == -1)
			{
				unsigned char b=p;
				fwrite(&b, sizeof(unsigned char), 1, out);
				pos++;
				if (pos == head->length)  // �ﵽ��ԭ�ļ��ĳ���
					break;
				p = 510;  // �ص����ڵ�
			}
			ch = ch << 1;  // ����һλ
		}
		if (pos == head->length)
			break;
	}
	fclose(in);
	fclose(out);
	cout << "��ѹ�ɹ���" << endl;
	return SUCCESS;
}