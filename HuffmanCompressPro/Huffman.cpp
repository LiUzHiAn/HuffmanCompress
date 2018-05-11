#include"Huffman.h"
using namespace std;
// ����Ȩֵ���鹹��huffman��
HuffmanNode* generateHuffmanTree(int *w, int n)
{
	HuffmanNode* tree = new HuffmanNode[2 * n - 1];
	for (int i = 0;i < n;i++)  // ǰn��ΪҶ�ӽ��
	{
		tree[i].weight = w[i];
		tree[i].left_node = -1;
		tree[i].right_node = -1;
		tree[i].parent = -1;
	}
//	printTreeInfo(tree, 0, 255);
	for (int i = n;i < 2 * n - 1;i++)  //  ��n-1��Ϊ�м丸�ڵ�
	{
		int index1 = -1, index2 = -1;
		// ���ʼ���Ѿ�����Ľ��������С�ʹ�С
		selectMin(tree, i - 1, index1, index2);
		// �޸ĵ�ǰ�¹���Ľ��
		tree[i].left_node = index1;
		tree[i].right_node = index2;
		tree[i].weight = tree[index1].weight + tree[index2].weight;
		tree[i].parent = -1;
		// ������Ҷ�ӽ����Ϊ��ǰ�����ӽڵ�
		tree[index1].parent = i;
		tree[index2].parent = i;
	}
	return tree;
}

// ѡ��Ȩֵ��С�ʹ�С���������
void selectMin(HuffmanNode* tree, int end_index, int &index1, int &index2)
{
	int min1 = INT_MAX, min2 = INT_MAX;
	for (int j = 0;j <= end_index;j++)
	{
		if (tree[j].parent == -1)  // û��ѡ���Ľ��
		{
			// ���ĳ���Ȩֵ����Сֵ��С
			if (tree[j].weight < min1)
			{
				index2 = index1;
				min2 = min1;
				index1 = j;
				min1 = tree[j].weight;
			}
			// ���ĳ���Ȩֵ����Сֵ���󣬵��ȴ�СֵС
			else if (tree[j].weight < min2)
			{
				min2 = tree[j].weight;
				index2 = j;
			}
		}
	}
}
// ��ӡ����begin~end�����Ϣ
void printTreeInfo(HuffmanNode* tree, int begin, int end)
{
	cout << "Huffman Tree info : " << endl;
	cout << "Index\t" << "Weight\t" << "Parent\t" << "left_child\t" << "right_child\t" << endl;
	for (int i = begin;i <= end;i++)
	{
		cout << i <<"\t" << tree[i].weight << "\t" << tree[i].parent << "\t" 
			<< tree[i].left_node << "\t" << tree[i].right_node << "\t" << endl;
	}
}

// ��ӡHuffman���Ľ����Ϣ���������
void printNodeInfo(HuffmanNode* tree, int root)
{
	cout << tree[root].weight << " ";
	if (tree[root].left_node != -1)  // ������,�������������
	{
		printNodeInfo(tree, tree[root].left_node);
	}
	if (tree[root].right_node != -1)   // ���Һ���,�������������
	{
		printNodeInfo(tree, tree[root].right_node);
	}
	// ���Һ��Ӷ������ڣ�Ҳ���ǵ���Ҷ�ӽ�㣬���
}

// ����Huffman�����б��룬��0��1���������
char** huffmanCoding(HuffmanNode* tree)
{
	// ������ô�Ҷ�ӽ�㷴������������ķ��������
	// N��Ҷ�ӽ���Huffman�������ΪN-1λ
	// 256��Ҷ�ӽ�㣬����һ���ַ�����¼���Ӧ�Ĺ���������
	char **code = new char*[256];
	for (int i = 0;i < 256;i++)
	{
		code[i] = new char[256];
	}

	int start,p,c;
	// ��Huffman���У�0-255ΪҶ�ӽ��
	for (int i = 0;i < 256;i++)
	{
		// ĩβһλΪ\0,ͬʱ����һλָ���־λstart����ʾ����Ŀ�ʼ
		char *ch = new char[256];
		ch[255] = '\0';
		start= 255;
		c = i;
		while ((p= tree[c].parent )!=-1)
		{
			//��Ҷ�ӽ��Ϊ��һ��ֲ������ӣ���0
			if (tree[p].left_node == c) 
			{
				ch[--start] = '0';
			}
			//��Ҷ�ӽ��Ϊ��һ��ֲ����Һ��ӣ���0
			else if (tree[p].right_node == c)
			{
				ch[--start] = '1';
			}
			c = p;  // �������ϻ���
		}
		// ��start��'\0'������code[i]����Ҷ�ӽڵ�i�ı���
		
		strcpy(code[i], &ch[start]);
		delete ch;
	}
	return code;
}

// ��ӡHuffman������Ϣ
void printHuffmanCodeInfo(char** code)
{
	cout <<"Byte\t" << "����\t" << "����\t" << endl;
	for (int i = 0;i < 256;i++)
	{
		cout  << i << "\t" << code[i]<<"\t"<<strlen(code[i]) << "\t" <<endl;
	}
}

