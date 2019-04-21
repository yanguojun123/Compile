#pragma once
#include<iostream>
#include<string>
#include <fstream>
#include <sstream>
#include<Windows.h>
#include "tree.h"
#include<stack>
bool success = 1;
using namespace std;
char ch;//字符变量，存放最新读进的源程序字符
char strToken[100];//字符数组，存放构成单词符号的字符串
int len = 0;//记录字符串的长度
string klabel[29] = { "+","-","*","/","=","#","<","<=",">",">=",",",";",":=","(",")",".","const","var","procedure","begin","end","odd","if","then","call"
,"while","do","read","write" };
string slabel[100] = {};
string  clabel[100] = {};
string  result[1000] = {};//保存所有单词的结果数组
int procedureId = 0;//记录当前的过程层数
int tableId = 0;//当前名字表的下标
int dx = 3;//当前层数的偏移地址
int ip = 0;//全局符号表的指针
int slen = 0;//标识符表的下标
int clen = 0;//常量表的下标
			 //判断是否为数字
const char *input;
int index = 0;
int codeId = 0;//代码的下标
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
bool isDigit(char ch1)
{
	if (ch1 < 58 && ch1 >= 48)
		return true;
	else
		return false;
}
//把字符串转化为数字
int strToint(string str)
{
	int len = str.length();
	int count = 0;
	for (int i = 0; i < len; i++)
	{
		if (str.at(i) <= 57 && str.at(i) >= 48)
			count = count + (str.at(i) - 48)*pow(10, len - i - 1);
		else
			return -1;
	}
	return count;
}
//把数字转化为字符串
//string intTostr(int num)
//{

//}
//判断是否为小写字母
bool issLetter(char ch1)
{
	if (ch1 <= 122 && ch1 >= 97)
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
//对strToken中的字符串查找保留字表，若他是一个保留字则返回则返回他的编码，否则返回0值
int Reserve()
{

	for (int i = 0; i < 29; i++)
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
int isexists(string str)
{
	for (int i = 0; i < slen; i++)
		if (str == slabel[i])
			return 1;

	return 0;
}
//检测是否已经存在此常量
int  isexistc(string con)
{
	for (int i = 0; i < clen; i++)
		if (con == clabel[i])
			return 1;
	return -1;
}
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
struct symble
{
	string name;
	string kind;
	int addr;
	int attribute;//val/value 
}symlabel;
//判断是否退出
symble table[20];
//在符号表里查找某个标识符
int findSymble(string str)
{
	for (int i = 0; i < tableId; i++)
	{
		if (table[i].name == str)
			return i;
	}
	return -1;
}
//在符号表里根据地址查找变量的名字
string findSymble(int addr1)
{
	for (int i = 0; i < tableId; i++)
	{
		if (table[i].addr == addr1)
			return table[i].name;
	}
	return "";
}
//目标指令的结构体：
struct tarInstruc
{
	string funcCode=" ";
	int levelDiff = 0;
	int displacement = 0;
	string content = " ";
};
tarInstruc code[200];
//从关键字表查找关键字的下标
int findKlabel(string str)
{
	for (int i = 0; i < 29; i++)
	{
		if (str == klabel[i])
			return i+1;
	}
	return -1;
}
//添加code，运算类型
void codeAdd(string str,int lev,int place,string con)
{
	code[codeId].funcCode = str;
	code[codeId].levelDiff = lev;
	code[codeId].displacement = place;
	code[codeId].content = con;
	codeId++;
}
//添加code，非运算
void codeAdd(string str, int lev, int place)
{
	code[codeId].funcCode = str;
	code[codeId].levelDiff = lev;
	code[codeId].displacement = place;
	codeId++;
}
//主程序
void lex(string file1)
{
	int code, value, num = 0;
	/*cout << "请输入单词长度"<<endl;
	cin >> num;
	cout << "请输入单词："<<endl;
	cin >> input;*/
	//char filename[] = "test.txt";
	//const char *file = readFileIntoString(filename).data();
	std::ifstream in(file1);
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
}
//函数声明
void subProgram(treeNode<string> *tN);
void conExplain(treeNode<string> *tN);
void varExplain(treeNode<string> *tN);
void processExplain(treeNode<string> *tN);
void constDef(treeNode<string> *tN);
void statement(treeNode<string> *tN);
void varExplain(treeNode<string> *tN);
void id(treeNode<string> *tN);
void unsInt(treeNode<string> *tN);
void dig(treeNode<string> *tN,int index);

void processHead(treeNode<string> *tN);
void AssignmentStatement(treeNode<string> *tN);
void expression(treeNode<string> *tN);
void CompoundStatement(treeNode<string> *tN);
void conditions(treeNode<string> *tN);
void AddAndSubtract(treeNode<string> *tN);
void item(treeNode<string> *tN);
void factor(treeNode<string> *tN);
void MULAndDIV(treeNode<string> *tN);
void ConditionStatement(treeNode<string> *tN);
void relationship(treeNode<string> *tN);
void processCall(treeNode<string> *tN);
void readStatement(treeNode<string> *tN);
void dowhile(treeNode<string> *tN);
void writeStatement(treeNode<string> *tN);
void letter(treeNode<string> *tN,int index);
void digit(treeNode<string> *tN,int index);
treeNode<string> *Tprogram=new treeNode<string>(2,"<程序>");
/*treeNode<string> *TsubProgram=new treeNode<string>(4,"<分程序>");
treeNode<string> *TconExplain = new treeNode<string>("<常量说明部分>");
treeNode<string> *TvarExplain = new treeNode<string>("<变量说明部分>");
treeNode<string> *TprocessExplain = new treeNode<string>("<过程说明部分>");
treeNode<string> *Tstatement = new treeNode<string>(1,"<语句>");
treeNode<string> *TconstDef = new treeNode<string>(3,"<常量定义>");
treeNode<string> *TunsInt = new treeNode<string>("<无符号整数>");
treeNode<string> *Tpoint = new treeNode<string>(".");
treeNode<string> *TprocessHead = new treeNode<string>(3,"过程首部");
treeNode<string> *Tid = new treeNode<string>("标识符");
*/
void  program()//程序
{
	codeAdd("jmp", 0, -1);//暂时填一个
	ip = 0;
	treeNode<string> *TsubProgram = new treeNode<string>(4,"<分程序>");
	subProgram(TsubProgram);
	if (result[ip] == ".")
	{
		Tprogram->child[0] = TsubProgram;
		Tprogram->child[1] = new treeNode<string>(".");
		//**************目标代码的生成，最后的退出
		codeAdd("opr", 0, 0,"return");
		//*************
		cout << "成功";
	}
	else
		exit(0);
}

void subProgram(treeNode<string> *tN)//分程序
{

	int count = 4;
	int tempTableId = tableId-1;
	//常量说明部分
	if (result[ip] == "const")
	{   
		treeNode<string> *TconExplain = new treeNode<string>("<常量说明部分>");
		conExplain(TconExplain);
		count++;
		tN->child[0] = TconExplain;
	}
	//变量说明部分
	if (result[ip] == "var")
	{
		treeNode<string> *TvarExplain = new treeNode<string>("<变量说明部分>");
		varExplain(TvarExplain);
		count++;
		tN->child[1] = TvarExplain;
	}
	//过程说明部分
	if (result[ip] == "procedure")
	{
		treeNode<string> *TprocessExplain = new treeNode<string>("<过程说明部分>");
		processExplain(TprocessExplain);
		count++;
		tN->child[2] = TprocessExplain;
	}
		if (table[tempTableId].kind == "proc")
			table[tempTableId].addr = codeId;
	//**************目标代码的生成，开辟地址空间
	int varNumber = 0;
	for (int i = 0; i < tableId; i++)
		if (table[i].attribute == procedureId && table[i].kind == "var")
			varNumber++;
	codeAdd("int",0,varNumber+3);
	//*************
	treeNode<string> *Tstatement = new treeNode<string>(1,"<语句>");
	//***************目标代码生成,直接跳转到主程序
	if (procedureId == 0)
		code[0].displacement = codeId-1;
	//**********************
	statement(Tstatement);//语句
	
	tN->child[3]=Tstatement;
	
}
void conExplain(treeNode<string> *tN)//常量说明部分
{
	if (result[ip] == "const")
	{
		ip++;
		tN->child[0] = new treeNode<string>("const");
		treeNode<string> *TconstDef1 = new treeNode<string>(3, "常量定义");
		tN->child[1] = TconstDef1;
		constDef(TconstDef1);
		int count = 0;
		while (result[ip] == ",")
		{   
			count++;//树的孩子数
			tN->child[2*count] = new treeNode<string>(",");
			treeNode<string> *TconstDef2 = new treeNode<string>(3,"常量定义");
			tN->child[2*count+1] = TconstDef2;
			ip++;
			constDef(TconstDef2);
		}
		if (result[ip] == ";")
		{
			ip++;
			//增加树的结构
			tN->child[2*count + 2] = new treeNode<string>(";");
		}
		else
			exit(0);
	}
	else
		exit(0);
}
void varExplain(treeNode<string> *tN)//变量说明部分
{
	if (result[ip] == "var")
	{
		ip++;
		tN->child[0] = new treeNode<string>("var");
		treeNode<string> *Tid1 = new treeNode<string>(3, "<标识符>");
		tN->child[1] = Tid1;
		table[tableId].name = result[ip];
		table[tableId].attribute = procedureId;
		table[tableId].addr = dx;
		table[tableId].kind = "var";
		tableId++;
		dx++;
		id(Tid1);
		int count = 0;
		while (result[ip] == ",")
		{
			ip++;
			count++;
			tN->child[2 * count] = new treeNode<string>(",");
			treeNode<string> *Tid2 = new treeNode<string>(3, "<标识符>");
			tN->child[2 * count + 1] = Tid2;
			table[tableId].name = result[ip];
			table[tableId].attribute = procedureId;
			table[tableId].addr = dx;
			table[tableId].kind = "var";
			tableId++;
			dx++;
			id(Tid2);
		}
	
		if (result[ip] == ";")
		{
			ip++;
			tN->child[count + 1] = new treeNode<string>(";");
		}
	   else
		   exit(0);
	}
	else
		exit(0);
}
void processExplain(treeNode<string> *tN)//过程说明部分
{
	procedureId++;//层数加1
	dx = 3;//当前层数内地址变为1
	treeNode<string> *TprocessHead = new treeNode<string>(3,"<过程首部>");
	tN->child[0] = TprocessHead;
	treeNode<string> *TsubProgram = new treeNode<string>( "<分程序>");
	tN->child[1] = TsubProgram;
	processHead(TprocessHead);
	subProgram(TsubProgram);
	if (result[ip] == ";")
	{   
		tN->child[2] = new treeNode<string>(";");
		//**************目标代码的生成，过程说明结束
		codeAdd("opr", 0, 0,"return");
		//*************
		procedureId--;
		ip++;
		int count=0;
		while (result[ip]=="procedure")
		{
			count++;
			treeNode<string> *TprocessExplain = new treeNode<string>("<过程说明部分>");
			tN->child[count + 2] = TprocessExplain;
			processExplain(TprocessExplain);
		}

	}
	    else
			exit(0);
}
void statement(treeNode<string> *tN)//语句
{
	if (isexists(result[ip]))//赋值语句
	{
		treeNode<string> *TAssignmentStatement = new treeNode<string>(3,"<赋值语句>");
		tN->child[0] = TAssignmentStatement;
		AssignmentStatement(TAssignmentStatement);
	}
	else if (result[ip] == "if")//条件语句
	{
		treeNode<string> *TConditionStatement = new treeNode<string>(3,"<条件语句>");
		tN->child[0] = TConditionStatement;
		ConditionStatement(TConditionStatement);
	}
	else if (result[ip] == "while")//当型循环
	{
		treeNode<string> *Tdowhile = new treeNode<string>(3,"<当型循环语句>");
		tN->child[0] = Tdowhile;
		dowhile(Tdowhile);
	}
	else if (result[ip] == "call")//过程调用	
	{
		treeNode<string> *Tcall= new treeNode<string>(2, "<过程调用语句>");
		tN->child[0] = Tcall;
		processCall(Tcall);
	}
	else if (result[ip] == "read")//读语句
	{
		treeNode<string> *Tread = new treeNode<string>(2, "<读语句>");
		tN->child[0] = Tread;
		readStatement(Tread);
	}
	else if (result[ip] == "write")//写语句	
	{
		treeNode<string> *Twrite = new treeNode<string>(2, "<写语句>");
		tN->child[0] = Twrite;
		writeStatement(Twrite);
	}
	else if (result[ip] == "begin")//复合语句
	{
		treeNode<string> *TCompoundStatement = new treeNode<string>(2, "<复合语句>");
		tN->child[0] = TCompoundStatement;
		CompoundStatement(TCompoundStatement);
	}
	
}
void constDef(treeNode<string> *tN)//常量定义
{ 
	treeNode<string> *Tid = new treeNode<string>( "<标识符>");
	tN->child[0] = Tid;
	table[tableId].name = result[ip];
	table[tableId].kind = "const";
	id(Tid);
	if (result[ip] == "=")
	{
		tN->child[1] = new treeNode<string>("=");
		ip++;
		treeNode<string> *TunsInt = new treeNode<string>("<无符号整数>");
		tN->child[2] = TunsInt;
		table[tableId].attribute = strToint(result[ip]);
		tableId++;
		unsInt(TunsInt);
	}
	else
		exit(0);
}

void id(treeNode<string> *tN)//标识符
{
	/*if (isexists(result[ip]))
	{
		tN->child[0] = new treeNode<string>(result[ip]);
		ip++;
	}
	else
		exit(0);
		*/
	int index=0;
	treeNode<string> *Tletter1 = new treeNode<string>("<字母>");
	tN->child[0] = Tletter1;
	letter(Tletter1,0);
	index++;
	int max = result[ip].length();
	
	for (index=1; index < max && ((result[ip].at(index) >= 97 && result[ip].at(index) <= 122) ||
		(result[ip].at(index) >= 48 && result[ip].at(index) <= 57)); index++)
		{
			if (result[ip].at(index) >= 97 && result[ip].at(index) <= 122)
			{
				treeNode<string> *Tletter2 = new treeNode<string>("<字母>");
				tN->child[index] = Tletter2;
				letter(Tletter2, index);
			}
			else
			{
				treeNode<string> *Tdig = new treeNode<string>("<数字>");
				tN->child[index] = Tdig;
				dig(Tdig, index);
			}
		}
	ip++;
}
void unsInt(treeNode<string> *tN)//无符号整数
{
	/*if (isexistc(result[ip]))
	{
		tN->child[0] = new treeNode<string>(result[ip]);
		ip++;
	}
	else
		exit(0);*/
	int index = 0;
	treeNode<string> *Tdig1 = new treeNode<string>("<数字>");
	tN->child[0] = Tdig1;
	dig(Tdig1, 0);
	index++;
	int max = result[ip].length();

	for (index = 1; index < max && (result[ip].at(index) >= 48 && result[ip].at(index) <= 57); index++)
	{
		if (result[ip].at(index) >= 48 && result[ip].at(index) <= 57)
		{
			treeNode<string> *Tdig2 = new treeNode<string>("<数字>");
			tN->child[index] = Tdig2;
			dig(Tdig2, index);
		}
	}
	ip++;
}
void letter(treeNode<string> *tN,int index1)
{   
	if (result[ip].at(index1) >= 97 && result[ip].at(index1) <= 122)
	{
		string str;
		stringstream stream;
		stream << result[ip].at(index1);
		str = stream.str();//将char转换为string
		tN->child[0] = new treeNode<string>(str);
	}
	else
		exit(0);
}
void dig(treeNode<string> *tN,int index)//数字
{
	if (result[ip].at(index) <= 57 && result[ip].at(index) >= 48)
	{
		string str;
		stringstream stream;
		stream << result[ip].at(index);
		str = stream.str();//将char转换为string
		tN->child[0] = new treeNode<string>(str);
	}
	else
		exit(0);
}
void processHead(treeNode<string> *tN)//过程首部
{
	if (result[ip] == "procedure")
	{
		ip++;
		tN->child[0] = new treeNode<string>("procedure");
		treeNode<string> *Tid = new treeNode<string>("<标识符>");
		tN->child[1] = Tid;
		/******/
		table[tableId].kind = "proc";
		table[tableId].name = result[ip];
		table[tableId].addr = codeId;
		table[tableId].attribute = procedureId-1;
		tableId++;
		id(Tid);
		/****************/
		if (result[ip] == ";")
		{
			ip++;
			tN->child[1] = new treeNode<string>(";");
			
		}
		else
			exit(0);
	}
	else
		exit(0);
}
void AssignmentStatement(treeNode<string> *tN)//赋值语句
{
	treeNode<string> *Tid = new treeNode<string>("<标识符>");
	tN->child[0] = Tid;
	int tempIp = ip;//暂存ip
	id(Tid);
	if (result[ip] == ":=")
	{
		ip++;
		tN->child[1] = new treeNode<string>(":=");
		treeNode<string> *Texpression = new treeNode<string>("<表达式>");
		tN->child[3] = Texpression;
		expression(Texpression);
		//**************目标代码的生成，赋值语句
	int tableIndex = findSymble(result[tempIp]);
	if (tableIndex == -1)
	{
		cout << "此变量未定义" << endl;
		exit(0);
	}
	codeAdd("sto", abs(table[tableIndex].attribute - procedureId), table[tableIndex].addr);
	//*************
	}
	else
		exit(0);
}
void expression(treeNode<string> *tN)//表达式
{
	if (result[ip] == "+" || result[ip] == "-")
	{
		ip++;
		tN->child[0] = new treeNode<string>(result[ip]);
	}
	treeNode<string> *Titem = new treeNode<string>("<项>");
	tN->child[1]=Titem;
	item(Titem);
	int count = 0;
	while (result[ip] == "+" || result[ip] == "-")
	{
		count++;
		treeNode<string> *TAddAndSubtract = new treeNode<string>("<加减运算符>");
		tN->child[2*count] = TAddAndSubtract;
		treeNode<string> *Titem = new treeNode<string>("<项>");
		tN->child[2 * count+1] = Titem;
		int tempIp = ip;//暂存Ip
		AddAndSubtract(TAddAndSubtract);
		item(Titem);
		//**************目标代码的生成，加减运算
		codeAdd("opr", 0, findKlabel(result[tempIp]), result[tempIp]);
		//*************
	}
	
}
void CompoundStatement(treeNode<string> *tN)//复合语句
{
	if (result[ip] == "begin")
	{
		tN->child[0] = new treeNode<string>("begin");
		ip++;
		treeNode<string> *Tstatement1 = new treeNode<string>("<语句>");
		tN->child[1] = Tstatement1;
		statement(Tstatement1);
		int count = 0;
		while (result[ip] == ";")
		{
			ip++;
			count++;
			tN->child[2 * count] = new treeNode<string>(";");
			treeNode<string> *Tstatement2 = new treeNode<string>("<语句>");
			tN->child[2 * count + 1] = Tstatement2;
			statement(Tstatement2);

		}
		if (result[ip] == "end")
		{
			tN->child[2 * count+2] = new treeNode<string>("end");
			ip++;
		}
		else
			exit(0);
	}
	else
		exit(0);

}
void conditions(treeNode<string> *tN)//条件
{ 
	
	 if (result[ip] == "odd")
	 {
		 ip++;
		 tN->child[0] =new  treeNode<string>("odd");
		 treeNode<string> *Texpression = new treeNode<string>("<表达式>");
		 expression(Texpression);tN->child[1] = Texpression;
		 //***********目标代码生成，odd运算
		 codeAdd("opr",0,findKlabel("odd"),"odd");
		 //
	}
	 else 
	{
		 treeNode<string> *Texpression1 = new treeNode<string>("<表达式>");
		 tN->child[0] = Texpression1;
		expression(Texpression1);
		treeNode<string> *Trealationship = new treeNode<string>("<关系运算符>");
		tN->child[1] = Trealationship;
		int tempIp = ip;//保存ip
		relationship(Trealationship);
		treeNode<string> *Texpression2 = new treeNode<string>("<表达式>");
		tN->child[2] = Texpression2;
		expression(Texpression2);
		//**************目标代码的生成，加减运算
		codeAdd("opr", 0, findKlabel(result[tempIp]), result[tempIp]);
		//*************
	}
}
void item(treeNode<string> *tN)//项
{
	treeNode<string> *Tfactor1 = new treeNode<string>("<因子>");
	tN->child[0] = Tfactor1;
	factor(Tfactor1);
	int count = 0;
	while (result[ip]=="*"|| result[ip] == "/")
	{
		count++;
		treeNode<string> *TMULAndDIV = new treeNode<string>("<乘除运算符>");
		tN->child[2*count-1] = TMULAndDIV;
		int tempIp = ip;//暂存Ip
		MULAndDIV(TMULAndDIV);
		treeNode<string> *Tfactor2 = new treeNode<string>("<因子>");
		tN->child[2*count] = Tfactor2;
		factor(Tfactor2);
		//**************目标代码的生成，加减运算
		codeAdd("opr", 0, findKlabel(result[tempIp]), result[tempIp]);
		//*************
	}
}
void factor(treeNode<string> *tN)//因子
{
	if ((result[ip].at(0) <= 122 && result[ip].at(0) >= 97))
	{
		treeNode<string> *Tid = new treeNode<string>("<标识符>");
		tN->child[0] = Tid;
		//*********判断标识符为变量还是常量
		int tableIndex = findSymble(result[ip]);
		if (tableIndex != -1)
		{
			if (table[tableIndex].kind == "const")
			{
				codeAdd("lit", 0, table[tableIndex].attribute);
			}
			else if (table[tableIndex].kind == "var")
			{
				codeAdd("lod", abs(table[tableIndex].attribute - procedureId), table[tableIndex].addr);
			}
		}
		//****************/
		id(Tid);
	}
	else if ((result[ip].at(0) <= 57 && result[ip].at(0) >= 48))
	{
		treeNode<string> *TunsInt = new treeNode<string>("<无符号整数>");
		tN->child[0] = TunsInt;
	    //****************目标代码生成，常数
		codeAdd("lit",0,strToint(result[ip]));
		//
		unsInt(TunsInt);
	}
	else if ((result[ip] == "("))
	{
		tN->child[0] = new treeNode<string>("(");
		treeNode<string> *Texpression = new treeNode<string>("<表达式>");
		tN->child[1] = Texpression;
		expression(Texpression);
		if (result[ip] == ")")
		{
			tN->child[2] = new treeNode<string>(")");
			ip++;
		}
		else
			exit(0);
 	}
	else
		exit(0);

}
void AddAndSubtract(treeNode<string> *tN)//加减运算符
{
	if (result[ip] == "+" || result[ip] == "-")
	{
		tN->child[0] = new treeNode<string>(result[ip]);
		ip++;
	}
	else
		exit(0);
}
void MULAndDIV(treeNode<string> *tN)//乘除运算符
{
	if (result[ip] == "*" || result[ip] == "/")
	{
		tN->child[0] = new treeNode<string>(result[ip]);
		ip++;
	}
	else
		exit(0);
}
void relationship(treeNode<string> *tN)//关系运算符
{
	if (result[ip] == "=" || result[ip] == "#"||result[ip]=="<"|| result[ip] == "<="||result[ip] == ">"
		||result[ip] == ">=")
	{
		tN->child[0] = new treeNode<string>(result[ip]);
		ip++;
	}
	else
		exit(0);
}
void ConditionStatement(treeNode<string> *tN)//条件语句
{
	if (result[ip] == "if")
	{
		tN->child[0] = new treeNode<string>("if");
		ip++;
		treeNode<string> *Tconditions = new treeNode<string>("<条件>");
		tN->child[1] = Tconditions;
		conditions(Tconditions);
		//**************目标代码的生成，非真则跳转
		codeAdd("jpc", 0, 0);//暂时先填着地址0
		int tempId = codeId - 1;
		//*************
		if (result[ip] == "then")
		{
			ip++;
			tN->child[0] = new treeNode<string>("then");
			treeNode<string> *Tstatement = new treeNode<string>("<语句>");
			tN->child[1] = Tstatement;
			statement(Tstatement);
			//回填之前的jpc
			code[tempId].displacement = codeId;
			//*************
		}
		else
			exit(0);
	}
	else
		exit(0);
}
void processCall(treeNode<string> *tN)//过程调用语句
{
	if (result[ip] == "call")
	{
		ip++;
		treeNode<string> *Tid = new treeNode<string>("<标识符>");
		tN->child[0] = Tid;
		//****************目标代码生成，过程调用
		int tempAddr = 0;
		int tempId = 0;
		for (int i = 0; i < tableId; i++)
		{
			if (table[i].name == result[ip])
			{
				tempAddr = table[i].addr;
				tempId = i;
				break;
			}
		}
		codeAdd("cal",abs(procedureId-table[tempId].attribute),tempAddr);
		//**********************************8
		id(Tid);
	}
	else
		exit(0);
}
void dowhile(treeNode<string> *tN)//当型循环
{
	if (result[ip] == "while")
	{
		tN->child[0] = new treeNode<string>("while");
		ip++;
		int tempAddr = codeId+1;//while循环的入口地址
		treeNode<string> *Tconditions = new treeNode<string>("<条件>");
		tN->child[1] = Tconditions;
		conditions(Tconditions);
		//**************目标代码的生成，非真则跳转
		codeAdd("jpc", 0, 0);//暂时先填着地址0
		int tempId = codeId-1;
		//*************
		if (result[ip] == "do")
		{
			tN->child[2] = new treeNode<string>("do");
			ip++;
			treeNode<string> *Tstatement = new treeNode<string>("<语句>");
			tN->child[3] = Tstatement;
			statement(Tstatement);
			//**************目标代码的生成，无条件跳转
			codeAdd("jmp", 0, tempAddr);
			//回填之前的jpc
					code[tempId].displacement = codeId;
			//*************
		}
		else
			success = 0;
	}
	else
		exit(0);
}
void readStatement(treeNode<string> *tN)//读语句
{
	if (result[ip] == "read")
	{
		tN->child[0] = new treeNode<string>("read");
		//**************目标代码的生成，读语句
		codeAdd("opr", 0, findKlabel(result[ip]), result[ip]);
		//*************
		ip++;
		if (result[ip] == "(")
		{
			tN->child[1] = new treeNode<string>("（");
			ip++;
			treeNode<string> *Tid1 = new treeNode<string>("<标识符>");
			tN->child[2] = Tid1;
			//*******************目标代码的生成，结果的传递
			int tableIndex = findSymble(result[ip]);
			if (tableIndex == -1)
			{
				cout << "未定义此变量" << endl;
				exit(0);
			}
			codeAdd("sto", abs(table[tableIndex].attribute - procedureId), table[tableIndex].addr);
			//********************
			id(Tid1);
			int count = 0;
			while (result[ip] == ",")
			{
				count++;
				tN->child[2*count+1] = new treeNode<string>(",");
				ip++;
				//**************目标代码的生成，读语句
				codeAdd("opr", 0, findKlabel("read"), "read");
				//*************
				treeNode<string> *Tid2 = new treeNode<string>("<标识符>");
				tN->child[2*count+2] = Tid2;
				//*******************目标代码的生成，结果的传递
				 tableIndex = findSymble(result[ip]);
				codeAdd("sto", 0, table[tableIndex].addr);
				//********************
				id(Tid2);

			}
			if (result[ip] == ")")
			{
				tN->child[2*count+3] = new treeNode<string>(")");
				ip++;
			}
			else
				exit(0);

		}
		else
			exit(0);
	}
	else
		exit(0);
}
void writeStatement(treeNode<string> *tN)//写语句
{
	if (result[ip] == "write")
	{
		tN->child[0] = new treeNode<string>("write");
		ip++;
		if (result[ip] == "(")
		{
			ip++;
			tN->child[1] = new treeNode<string>("(");
			treeNode<string> *Texpression1 = new treeNode<string>("<表达式>");
			tN->child[2] = Texpression1;
			expression(Texpression1);
			//**************目标代码的生成，写语句
			codeAdd("opr", 0, findKlabel("write"), "write");
			//*************
			int count = 0;
			while (result[ip] == ",")
			{
				count++;
				ip++;
				
				tN->child[2 * count + 1] = new treeNode<string>(",");
				treeNode<string> *Texpression2 = new treeNode<string>("<表达式>");
				tN->child[2 * count + 2] = Texpression2;
				expression(Texpression2);
				//**************目标代码的生成，写语句
				codeAdd("opr", 0, findKlabel("write"), "write");
				//*************
			}
			if (result[ip] == ")")
			{
				tN->child[2 * count + 3] = new treeNode<string>(")");
				ip++;
			}
			else
				exit(0);

		}
		else
			exit(0);
	}
	else
		exit(0);
}


int findSymbleAddr(int addr1)
{
	for (int i = 0; i < tableId; i++)
		if (table[i].kind == "proc"&&table[i].addr == addr1)
			return table[i].attribute;
}
//解释执行函数
struct var 
{
	string name;
	int value;
};
var vartable[20];
int vartableid=0;

int findVar(string str)
{
	for (int i = 0; i < vartableid; i++)
	{
		if (vartable[i].name == str)
			return vartable[i].value;
	}
	return -1;
}
//根据变量的名字修改值
void changeVar(string str,int a)
{
	for (int i = 0; i < vartableid; i++)
	{
		if (vartable[i].name == str)
		{
			vartable[i].value = a;
			break;
		}
	}
}
//
struct action
{
	int moveList;
	int backAddr;
	int staticList;
	int varTable[5];//变量表
};
action Act[10];
int action1[200];
int moveId = 0;
int backId = 0;
int staticId = 0;
int actId = 0;
void explainToPerfrom()//目标代码的解释执行
{
	for (int i = 0; i < tableId; i++)
	{
		if (table[i].kind == "var")
		{
			vartable[vartableid].name = table[i].name;
			vartable[vartableid].value = 0;
		}
		vartableid++;
	}
	stack<int> com;//运算栈
	stack<action> actStack;//活动记录栈
	int temp = 0;//用户输入
	int cid = 0;//代码数组下标
	int tempCount = 0;//暂时保存
	int tempCode = 0;
	int tempBackId = 0;
	int tempMoveId = 0;
	int tempStaticId = 0;
	int lev = 0;//当前层数
	while (cid != codeId-1) 
	{
		if (code[cid].funcCode == "jmp")//跳转语句
		{
			if (cid == 0)//初始的入栈
			{
				action1[actId] =0;
				moveId = 0;//当前动态链
				backId = 1;
				staticId = 2;
				actId++;
				action1[actId] = 0;
				actId++;
				action1[actId] = 0;
				actId++;
			}
			cid = code[cid].displacement;
			
		}
	  else if (code[cid].funcCode == "opr")//运算语句
	  {
		  switch (code[cid].displacement)
		  {
		    case 0://退出过程
			{
				cid = action1[backId];
				actId = moveId;
				moveId = action1[moveId];
				backId = moveId+1;
				staticId =moveId+2;
				cid--;
				break;
			}
		    case 1://加法运算
				tempCount = com.top();
				com.pop();
				tempCount = tempCount + com.top();
				com.pop();
				com.push(tempCount);
				break;
			case 2://减法运算
				tempCount = com.top();
				com.pop();
				tempCount = com.top()-tempCount;
				com.pop();
				com.push(tempCount);
				break;
			case 3://乘法运算
				tempCount = com.top();
				com.pop();
				tempCount = com.top()*tempCount;
				com.pop();
				com.push(tempCount);
				break;
			case 4://除法运算
				tempCount = com.top();
				com.pop();
				tempCount = com.top()/tempCount;
				com.pop();
				com.push(tempCount);
				break;
			case 5://等于判断
				tempCount = com.top();
				com.pop();
				if (tempCount == com.top())
					tempCount = 1;
				else
					tempCount = 0;
				com.pop();
				com.push(tempCount);
				break;
			case 6://不等号运算
				tempCount = com.top();
				com.pop();
				if (tempCount == com.top())
					tempCount = 0;
				else
					tempCount = 1;
				com.pop();
				com.push(tempCount);
				break;
			case 7://小于
				tempCount = com.top();
				com.pop();
				if (com.top() < tempCount)
					tempCount = 1;
				else
					tempCount = 0;
				com.pop();
				com.push(tempCount);
				break;
			case 22://判断是否为奇数
				if (com.top() % 2 == 1)
					tempCount = 1;
				else
					tempCount = 0;
				com.pop();
				com.push(tempCount);
				break;
			case 28://输入
				cout << "请输入：";
				cin >> temp;
				//vartable[vartableid].
				com.push(temp);
				break;
			case 29://输出
				cout << com.top()<<"\n";
				break;
		  }
		  cid++;
	  }
	  else if (code[cid].funcCode == "lit")//常量声明
	  {
		  com.push(code[cid].displacement);
		  cid++;
	  }
	  else if (code[cid].funcCode == "lod")//加载变量
	  {  
		  //string tempName = findSymble(code[cid].displacement);
		  //com.push(findVar(tempName));
		  int tempId = 0;
		  if (code[cid].levelDiff == 0)
			  tempId = moveId;
		  else
		  {
			  tempId = staticId;
			  for (int i = 0; i < code[cid].levelDiff; i++)
			  {

				  tempId = action1[tempId];
			  }
		  }
		  com.push(action1[tempId + code[cid].displacement]);
		  cid++;
	  }
	  else if (code[cid].funcCode == "sto")//将栈顶结果送给变量
	  {
		  //string tempName = findSymble(code[cid].displacement);
		  //tempCount = findVar(tempName);
		  //changeVar(tempName,com.top());
		  int tempId = 0;
		  if(code[cid].levelDiff==0)
			   tempId = moveId;
		  else
		  { 
			   tempId = staticId;
			  for (int i = 0; i < code[cid].levelDiff; i++)
			  {
				  tempId = action1[tempId];
			  }
		  }
		  action1[tempId + code[cid].displacement] = com.top();
		  cid++;
	  }
	  else if (code[cid].funcCode == "cal")//过程调用语句
	  {
		  // lev = findSymbleAddr(code[cid].displacement);//更新层次
		  action1[actId] = moveId;
		  tempMoveId = moveId;
		  moveId = actId;
		  actId++;
		  action1[actId] = cid + 1;//修改返回地址
		  tempBackId = backId;
		  backId = actId;
		  actId++;

		  if (code[cid].levelDiff == 0)//层差为0
		  {
			  tempStaticId = staticId;
			  action1[actId] = action1[tempMoveId];//新的静态链
			  staticId = actId;
			  actId++;
		  }
		  if (code[cid].levelDiff == 1)//层差为1
		  {
			  tempStaticId = staticId;
			  tempCount = staticId;
			  //tempCount= action1[tempCount];
			  action1[actId] = action1[tempCount];//新的静态链
			  staticId = actId;
			  actId++;
		  } 
		  cid = code[cid].displacement;
	  }
	  else if (code[cid].funcCode == "jpc")//非真跳转语句
	  {
		  if (com.top() == 0)
			  cid = code[cid].displacement;
		  else
			  cid++;
	  }
	  else if (code[cid].funcCode == "int")//开辟空间
	  {
		  for (int i = 0; i < code[cid].displacement - 3; i++)//变量表初始化
		  {
			  action1[actId] = 0;
			  actId++;
		  }
		  cid++;
	  }
	}
}