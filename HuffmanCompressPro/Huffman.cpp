#include"Huffman.h"
using namespace std;
// 根据权值数组构造huffman树
HuffmanNode* generateHuffmanTree(int *w, int n)
{
	HuffmanNode* tree = new HuffmanNode[2 * n - 1];
	for (int i = 0;i < n;i++)  // 前n个为叶子结点
	{
		tree[i].weight = w[i];
		tree[i].left_node = -1;
		tree[i].right_node = -1;
		tree[i].parent = -1;
	}
//	printTreeInfo(tree, 0, 255);
	for (int i = n;i < 2 * n - 1;i++)  //  后n-1个为中间父节点
	{
		int index1 = -1, index2 = -1;
		// 从最开始到已经构造的结点中找最小和次小
		selectMin(tree, i - 1, index1, index2);
		// 修改当前新构造的结点
		tree[i].left_node = index1;
		tree[i].right_node = index2;
		tree[i].weight = tree[index1].weight + tree[index2].weight;
		tree[i].parent = -1;
		// 将两个叶子结点设为当前结点的子节点
		tree[index1].parent = i;
		tree[index2].parent = i;
	}
	return tree;
}

// 选择权值最小和次小的两个结点
void selectMin(HuffmanNode* tree, int end_index, int &index1, int &index2)
{
	int min1 = INT_MAX, min2 = INT_MAX;
	for (int j = 0;j <= end_index;j++)
	{
		if (tree[j].parent == -1)  // 没被选过的结点
		{
			// 如果某结点权值比最小值还小
			if (tree[j].weight < min1)
			{
				index2 = index1;
				min2 = min1;
				index1 = j;
				min1 = tree[j].weight;
			}
			// 如果某结点权值比最小值还大，但比次小值小
			else if (tree[j].weight < min2)
			{
				min2 = tree[j].weight;
				index2 = j;
			}
		}
	}
}
// 打印树的begin~end结点信息
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

// 打印Huffman树的结点信息，先序遍历
void printNodeInfo(HuffmanNode* tree, int root)
{
	cout << tree[root].weight << " ";
	if (tree[root].left_node != -1)  // 有左孩子,先序遍历左子树
	{
		printNodeInfo(tree, tree[root].left_node);
	}
	if (tree[root].right_node != -1)   // 有右孩子,先序遍历右子树
	{
		printNodeInfo(tree, tree[root].right_node);
	}
	// 左右孩子都不存在，也就是到了叶子结点，输出
}

// 根据Huffman树进行编码，左0右1，先序遍历
char** huffmanCoding(HuffmanNode* tree)
{
	// 这里采用从叶子结点反向回溯至树根的方法求编码
	// N个叶子结点的Huffman树编码最长为N-1位
	// 256个叶子结点，各有一个字符串记录其对应的哈夫曼编码
	char **code = new char*[256];
	for (int i = 0;i < 256;i++)
	{
		code[i] = new char[256];
	}

	int start,p,c;
	// 在Huffman树中，0-255为叶子结点
	for (int i = 0;i < 256;i++)
	{
		// 末尾一位为\0,同时设置一位指针标志位start，表示编码的开始
		char *ch = new char[256];
		ch[255] = '\0';
		start= 255;
		c = i;
		while ((p= tree[c].parent )!=-1)
		{
			//该叶子结点为上一层分叉点的左孩子，添0
			if (tree[p].left_node == c) 
			{
				ch[--start] = '0';
			}
			//该叶子结点为上一层分叉点的右孩子，添0
			else if (tree[p].right_node == c)
			{
				ch[--start] = '1';
			}
			c = p;  // 继续向上回溯
		}
		// 从start到'\0'拷贝到code[i]，即叶子节点i的编码
		
		strcpy(code[i], &ch[start]);
		delete ch;
	}
	return code;
}

// 打印Huffman树的信息
void printHuffmanCodeInfo(char** code)
{
	cout <<"Byte\t" << "编码\t" << "长度\t" << endl;
	for (int i = 0;i < 256;i++)
	{
		cout  << i << "\t" << code[i]<<"\t"<<strlen(code[i]) << "\t" <<endl;
	}
}

