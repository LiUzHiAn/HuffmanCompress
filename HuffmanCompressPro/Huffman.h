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
// 打印Huffman树信息
void printHuffmanCodeInfo(char** code);
// 根据权值数组构造huffman树
HuffmanNode* generateHuffmanTree(int *w, int n);
// 选择权值最小和次小的两个结点
void selectMin(HuffmanNode* tree, int end_index, int &index1, int &index2);
// 打印Huffman树信息
void printTreeInfo(HuffmanNode* tree, int begin, int end);
// 打印Huffman结点信息
void printNodeInfo(HuffmanNode* tree, int root);
// 根据Huffman树进行编码
char** huffmanCoding(HuffmanNode* tree);


