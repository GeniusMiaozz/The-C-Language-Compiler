#pragma once
#include "Compiler.h"

class Lexical_Analysis
{
protected:
	vector<string> token_list;//token串集合
	vector<string> token_type;//token串的属性
	vector<int> token_number;//token串在表内的序号
	vector<string> key_word = { "\0","int", "main", "void","if" , "else", "char",
	"default", "do", "double","const" , "enum", "extern",
	"float", "for", "goto", "continue", "auto", "long",
	"register", "return", "short", "signed", "sizeof", "static",
	"struct", "switch", "typedef", "union", "unsigned", "case",
	"volatile", "while", "include", "printf","string","break","bool" }; //关键字
	vector<string> key_symbol = { "\0","+", "-", "*", "/", "<", "<=", ">", ">=", "=", "==",
		"!=", ";", "(", ")", "^", ",", "\"", "\\'", "#", "&",
		"&&", "|", "||", "%", "~","@","$","?","<<", ">>", "[", "]", "{",
		"}", "//", ".", ":", "!","_","\\","/*","*/","::","->" ,"++","--" }; //关键符号
	vector<string> const_symbol = { "\0" };//标识符,如a_b
	vector<string> const_number = { "\0" };//常数,如123
	vector<string> const_char = { "\0" };//字符,如'c'
	vector<string> const_string = { "\0" };//字符串,如"def"
	fstream code_file;
public:
	Lexical_Analysis();
	~Lexical_Analysis();
	void Generate_Token();//词法分析主体
	int change_state(int state, char ch);//状态转换自动机
	void check_code(int state, char* token);//根据终结态生成单词
	int search_key(char* token, vector<string>* key, string type);//查找、匹配、加入token序列和查找表
	void print_token_list();//输出token序列
};