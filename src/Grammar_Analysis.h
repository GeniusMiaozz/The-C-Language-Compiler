#pragma once
#include "Lexical_Analysis.h"

class Grammar_Analysis : public Lexical_Analysis
{
protected:
	vector<int>::size_type production_number;					 //产生式个数
	string head_symbol;						 //文法的初始符号
	vector<string> production_left;			 //文法产生式左部
	vector<vector<string>> production_right; //文法产生式右部
	vector<vector<string>> first_list;		 //first集，从1开始计数
	vector<vector<string>> follow_list;		 //follow集，从1开始计数
	vector<vector<string>> select_list;		 //select集，从1开始计数
	vector<vector<string>> LL1_list;		 //LL1分析表，从1开始计数
	stack<string> grammar_stack;			 //语法分析栈

	//终结符和非终结符表
	vector<string> end_symbol = { "\0", "+", "-", "*", "/", "(", ")","[","]","#","void","main","{","}","int","float","char","string","double","bool","break",
					  "&&","||","(",")",",",">=" ,"<=" ,"==" ,"!=","<",">","while" ,"if","else","for","=","++","--","标识符",";","数值","continue","switch",".",
					  "case","default",":","字符","字符串","struct","return","*","&" };
	vector<string> not_end_symbol = { "\0","程序","函数定义","类型说明","函数声明","参数列表", "数组", "复合语句","声明语句","标识符赋值","声明数组","声明赋值语句",
						  "赋值语句", "多个赋值","表达式语句","表达式","表达式右","表达式一","表达式右二","表达式二","条件语句","条件执行","声明赋值","单操作符","多个初始赋值",
						  "多条件","多条件判断","循环语句","判断语句","判断或语句","判断或","判断与语句","判断与","比较符","非空复合语句","空产生式","初始条件","结束操作",
						  "结束多个赋值","跳转语句","条件选择","声明标识符","函数声明二","函数定义二","标识符后缀","赋值标识符","赋值标识符后缀","单目操作符","结构体复合语句",
						  "结构体标识符","多变量","返回语句","标识符前缀","声明类型","赋值表达式","多表达式" };

public:
	Grammar_Analysis();
	~Grammar_Analysis();
	void Generate_Token();																	  //语法分析中的词法分析
	void get_first_list();																	  //求first集
	void get_follow_list();																	  //求follow集
	void get_select_list();																	  //求select集
	void get_LL1_list();																	  //求LL1分析表
	int LL1_sentence_analysis(string s);													  //语法分析中的LL1语法分析;
	void scanf_information();																  //输入信息
	void print_information();																  //输出信息
	int get_not_end_symbol_number(string s);												  //得到非终结符在查找表中的位置
	int get_end_symbol_number(string s);													  //得到终结符在查找表中的位置
	string get_next_w_type(string s);														  //返回一个读入字符串的类型
	int new_symbol_flag(vector<vector<string>>* symbol, string s, vector<char>::size_type i); //判断s是否存在于（*symbol）[i]中
	int empty_symbol_flag(string s);														  //判断s是否可推出空产生式
};