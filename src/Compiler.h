#pragma once
#include <bits/stdc++.h>
#define token_max_size 1000            //单个token串的最大长度

using namespace std;

/*中间代码的定义，前端与后端间的接口*/

/*符号表的定义*/
typedef struct
{
	string type_code;				  //种类代码，如“整形”“布尔型”
	void* information_pointer = NULL; //信息表指针，基本数据类型指向NULL，数组指向数组表等等
} type_property;					  //种类的总体属性

typedef struct
{
	int bottom_number;//数组下界
	int top_number;//数组上界
	type_property* type_information;//数组中每个元素的的类型信息，如int a[10]指向int，int a[5][10]指向后面长度为10的数组，
	int array_size;//数组长度
} array_table;//数组表

typedef struct constant_table
{
	union {
		int int_value;
		float float_value;
		double double_value;
		char char_value;
		bool bool_value;
	}value;
	constant_table* next = NULL;
} constant_table; //常量表

typedef struct
{
	string ID;			//结构体中变量的标识符	
	int OFF = 0;			//偏移量
	type_property* tp = NULL;//类型指针
	constant_table* vp = NULL;//数值指针，指向常量表
} structure_property;//结构体变量属性

typedef struct
{
	int num;			//结构体内变量个数
	int total_OFF;       //总偏移量
	vector<structure_property> structure_property_list; //结构体变量表
} structure_table;//结构表

typedef struct
{

} function_table;//函数表

typedef struct
{
	string name;		  //标识符的名字
	type_property* type = NULL;   //标识符的类型指针
	string kind;		  //标识符的种类信息，如“常量”“函数”
	int offset = 0;//地址偏移量
	void* address = NULL; //标识符的解释表，如常量指向常量表，函数指向函数表
} symbol_property;		  //符号表总表中每个标识符的属性

typedef struct symbol_table
{
	symbol_table* header = NULL;                   //每张函数符号表的头指针，指向调用该函数的函数的符号表
	int total_size = 0;                            //一张函数符号表的总长度
	vector<symbol_property> symbol_property_table; //符号表总表中每个标识符的属性表
} symbol_table;                                    //符号表，每个函数一张符号表

/*四元式的定义*/
typedef struct
{
	string operation_target_value;  //第一个运算对象的值
	bool active_information = false;//第一个运算对象的活跃信息
} first_operation_target;		    //第一个运算对象的属性

typedef struct
{
	string operation_target_value;  //第二个运算对象的值
	bool active_information = false;//第二个运算对象的活跃信息
} second_operation_target;		    //第二个运算对象的属性

typedef struct
{
	string result_target_value;     //运算结果对象的值
	bool active_information = false;//运算结果对象的活跃信息
} operation_result;				    //运算结果对象的属性

typedef struct
{
	string basic_block_flag = "顺序块";    //标记基本块的入口或出口
	string operation_symbol;			   //运算符号
	first_operation_target first_target;   //第一个运算对象的属性
	second_operation_target second_target; //第二个运算对象的属性
	operation_result result_target;		   //运算结果的属性
} four_item_production;		               //四元式序列的属性