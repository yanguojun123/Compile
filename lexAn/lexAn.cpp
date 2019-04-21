
#include"recursion.h"
using namespace std;
//主程序
int main()
{
	lex("test.txt");//词法分析
	for (int i = 0; i < 100; i++)
		cout << result[i] << " ";
	cout << endl;
	for (int i = 0; i < 100; i++)
		cout << slabel[i] << " ";
	cout << endl;
	for (int i = 0; i < 100; i++)
		cout << clabel[i] << " ";
	cout << endl;
	program();//语法分析，主程序入口
	cout << endl;
	print(Tprogram,0);//打印语法树
	//打印符号表
	cout << "\t index"<<" \t name: " << "\t kind: "  << "\t level/value: "<< "\t addr: "  << endl;
	for (int i = 0; i <tableId; i++)
		cout <<" \t "<<i<<" \t "<< table[i].name << " \t " << table[i].kind << " \t " 
		<< table[i].attribute << " \t\t " << table[i].addr << endl;
	cout << endl;
	//打印目标代码
	cout << "\t index" << " \t f: " << "\t l: " << "\t a: " <<endl;
	for (int i = 0; i <codeId; i++)
		cout << " \t " << i << " \t " << code[i].funcCode << " \t " << code[i].levelDiff << " \t "
		<< code[i].displacement << "\t"<<code[i].content<<endl;
	explainToPerfrom();//解释执行
	system("pause");
	return 0;
}