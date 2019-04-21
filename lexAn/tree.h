#pragma once
#include <iostream>
#include <string>
using namespace std;
template <class T>
struct treeNode//多叉树节点结构
{
	int childNum;
	T element;
	treeNode<T> *child[20];
	treeNode(int number, T ele) 
	{ 
		childNum = number; 
	    element = ele;
		
		for (int i = 0; i < 20; i++)
			child[i] = NULL;
	}
	treeNode(T ele) 
	{ 
		element = ele; 
		childNum = 20;
		for (int i = 0; i < 20; i++)
			child[i] = NULL;
	}
};
template <class T>
void print(treeNode<T> *root,int blk)
{
	treeNode<T> *t = root;
	
	if (t != NULL)
	{
		for (int i = 0; i<blk; i++)
		cout<<"|  ";//缩进
		cout <<"|--"<<t->element<<endl;
		for (int i = 0; i <20; i++)
		{
			print(t->child[i],blk+1);
		}
	}
}