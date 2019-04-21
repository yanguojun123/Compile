#pragma once
#include<iostream>
#include<string>
#include <fstream>
#include <sstream>
#include<Windows.h>
#include"recursion.h"
using namespace std;
char ch;//字符变量，存放最新读进的源程序字符
char strToken[100];//字符数组，存放构成单词符号的字符串
int len = 0;//记录字符串的长度
string klabel[29] = { "+","-","*","/","=","#","<","<=",">",">=",",",";",":=","(",")",".","const","var","procedure","begin","end","odd","if","then","call"
,"while","do","read","write" };
string slabel[100] = {};
string  clabel[100] = {};
string result[200] = {};
int ip = 0;
int slen = 0;//标识符表的下标
int clen = 0;//常量表的下标
			 //判断是否为数字
const char *input;
int index = 0;
bool isDigit()
{
	if (ch < 58 && ch >= 48)
		return true;
	else
		return false;
}
//判断是否为小写字母
bool issLetter()
{
	if (ch <= 122 && ch >= 97)
		return true;
	else
		return false;
}
//判断是否为大写字母
bool islLetter()
{
	if (ch <= 90 && ch >= 65)
		return true;
	else
		return false;
}
//将下一输入字符读到ch中
void GetChar()
{
	ch = input[index];
	index++;

}
//判断ch中是否是空白，若是，则调用GetChar直至读入下一个非空白的字符
void GetBC()
{
	while (ch == 32)
		GetChar();
}
//将ch中的字符连接到strToken之后
void concat()
{
	strToken[len] = ch;
	len++;
}
//对strToken中的字符串查找保留字表，若他是一个保留字则返回则返回他的编码，否则返回-1值
int Reserve()
{

	for (int i = 0; i < 26; i++)
	{
		/*const char *p = klabel[i].data();
		if (strcmp(p,strToken)==0)
		{
		return i;
		}*/
		if (klabel[i] == strToken)
			return i;
	}
	return -1;
}
//将搜索指示器回调一个字符位置
void Retract()
{
	index--;
	ch = 32;
}
//将strToken中的标识符插入到符号表
int insertId()
{
	slabel[slen] = strToken;
	slen++;
	return slen - 1;
}
//将strToken中的常数插入到常数表
int insertConst()
{
	clabel[clen] = strToken;
	clen++;
	return clen - 1;
}
string readFileIntoString(char * filename)
{
	ifstream ifile(filename);
	//将文件读入到ostringstream对象buf中
	ostringstream buf;
	char ch;
	while (buf&&ifile.get(ch))
		buf.put(ch);
	//返回与流对象buf关联的字符串
	return buf.str();
}
//检测是否已经存在此标识符
int isexists()
{
	for (int i = 0; i < slen; i++)
		if (strToken == slabel[i])
			return i;

	return -1;
}
//检测是否已经存在此常量
int  isexistc()
{
	for (int i = 0; i < clen; i++)
		if (strToken == clabel[i])
			return i;
	return -1;
}
//名字表的结构体
struct symbel
{
	string name;
	string kind;
	int level;
	int addr;
	int value;
}symlabel;
//主程序
int main()
{
	int code, value, num = 0;
	/*cout << "请输入单词长度"<<endl;
	cin >> num;
	cout << "请输入单词："<<endl;
	cin >> input;*/
	//char filename[] = "test.txt";
	//const char *file = readFileIntoString(filename).data();
	std::ifstream in("test.txt");
	std::ostringstream tmp;
	tmp << in.rdbuf();
	std::string str = tmp.str();
	cout << str << endl;
	for (int i = 0; i < str.length(); i++)//如果输入中有大写字母，则转换为小写字母
		if (str.at(i) <= 90 && str.at(i) >= 65)
			str.at(i) = str.at(i) + 32;
	const char *file = str.data();
	input = file;

	while (input[index] != 0)
	{
		len = 0;
		//strcpy(strToken,"");
		memset(strToken, 0, sizeof(strToken));
		//cout << strToken << endl;
		//Sleep(1000);
		GetChar();
		GetBC();
		if (issLetter())//判断是否是字母
		{
			while (issLetter() || isDigit())
			{
				concat();
				GetChar();
			}
			Retract();
			result[ip] = strToken;
			ip++;
			code = Reserve();
			if (code == -1)//不在关键字表里，即为标识符
			{
				insertId();
				int index1 = isexists();//检测此标识符是否存在
				if (index1 != -1)
					cout << "(" << strToken << ",id," << index1 << ")" << endl;
				else
				{
					value = insertId();
					cout << "(" << strToken << ",id," << value << ")" << endl;//输出标识符的三元组

				}//cout << strToken<<endl;
			}
			else
			{
				cout << "(" << klabel[code] << "," << klabel[code] << ",-)" << endl;//输出关键字的二元组
			}
		}
		else if (isDigit())//如果是数字，即为常量
		{
			while (isDigit())
			{
				concat();
				GetChar();
			}
			Retract();
			result[ip] = strToken;
			ip++;
			insertConst();
			int index2 = isexistc();
			if (index2 != -1)
				cout << "(" << strToken << ",int," << index2 << ")" << endl;
			else
			{
				value = insertConst();
				cout << "(" << strToken << ",int," << value << ")" << endl;
			}
		}
		/*else if (islLetter())
		{
		while(islLetter())
		{
		concat();
		GetChar();
		}
		Retract();
		code = Reserve();
		if (code == -1)
		cout << "拼写错误"<<endl;
		else
		cout << "(" << code << ",-)" << endl;//输出关键字的二元组
		}*/
		else//其余情况
		{
			switch (ch)
			{
			case '+':
				cout << "(+,+," << "-)" << endl;
				result[ip] = "+";
				ip++;
				break;
			case '-':
				cout << "(-,-," << "-)" << endl;
				result[ip] = "-";
				ip++;
				break;
			case '*':
				cout << "(*,*," << "-)" << endl;
				result[ip] = "*";
				ip++;
				break;
			case '/':
				cout << "(/,/," << "-)" << endl;
				result[ip] = "/";
				ip++;
				break;
			case '=':
				cout << "(=,=," << "-)" << endl;
				result[ip] = "=";
				ip++;
				break;
			case '#':
				cout << "(#,#," << "-)" << endl;
				result[ip] = "#";
				ip++;
				break;
			case '<':
				GetChar();
				if (ch == '=')
				{
					cout << "(<=,<=," << "-)" << endl;
					result[ip] = "<=";
					ip++;
				}
				else
				{
					Retract();
					cout << "(<,<," << "-)" << endl;
					result[ip] = "<";
					ip++;
				}
				break;
			case '>':
				GetChar();
				if (ch == '=')
				{
					cout << "(>=,>=," << "-)" << endl;
					result[ip] = ">=";
					ip++;
				}
				else
				{
					Retract();
					cout << "(>,>," << "-)" << endl;
					result[ip] = ">";
					ip++;
				}
				break;
			case ',':
				cout << "(,,,," << "-)" << endl;
				result[ip] = ",";
				ip++;
				break;
			case ';':
				cout << "(;,;," << "-)" << endl;
				result[ip] = ";";
				ip++;
				break;
			case ':':
				GetChar();
				if (ch == '=')
				{
					cout << "(:=,:=," << "-)" << endl;
					result[ip] = ":=";
					ip++;
				}
				else
				{
					Retract();
					cout << "(:,:," << "-)" << endl;
					result[ip] = ":";
					ip++;
				}
				break;
			case '(':
				cout << "((,(," << "-)" << endl;
				result[ip] = "(";
				ip++;
				break;
			case ')':
				cout << "(),)," << "-)" << endl;
				result[ip] = ")";
				ip++;
				break;
			case '.':
				cout << "(.,.," << "-)" << endl;
				result[ip] = ".";
				ip++;
				break;
			case 10://排除掉换行符
				break;
			default:
				cout << "拼写错误:" << ch << endl;
				break;
			}
		}
	}
	for (int i = 0; i < 100; i++)
		cout << result[i] << " ";
	cout << endl;
	for (int i = 0; i < 100; i++)
		cout << slabel[i] << " ";
	cout << endl;
	for (int i = 0; i < 100; i++)
		cout << clabel[i] << " ";
	system("pause");
	return 0;
}