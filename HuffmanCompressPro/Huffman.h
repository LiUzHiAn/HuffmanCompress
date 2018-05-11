#pragma once
#include<fstream>
#include<string>
#include<iostream>
#include"Huffman.h"
using namespace std;

// Node type in huffman tree
struct HuffmanNode
{
	int weight;		 // weight of node
	int parent;		 // parent node index
	int left_node;   // left child node index
	int right_node;	 // right child node index
};
// ��ӡHuffman����Ϣ
void printHuffmanCodeInfo(char** code);
// ����Ȩֵ���鹹��huffman��
HuffmanNode* generateHuffmanTree(int *w, int n);
// ѡ��Ȩֵ��С�ʹ�С���������
void selectMin(HuffmanNode* tree, int end_index, int &index1, int &index2);
// ��ӡHuffman����Ϣ
void printTreeInfo(HuffmanNode* tree, int begin, int end);
// ��ӡHuffman�����Ϣ
void printNodeInfo(HuffmanNode* tree, int root);
// ����Huffman�����б���
char** huffmanCoding(HuffmanNode* tree);


