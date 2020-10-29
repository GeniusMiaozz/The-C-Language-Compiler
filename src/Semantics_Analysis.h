#pragma once
#include "Grammar_Analysis.h"

class Semantics_Analysis : public Grammar_Analysis
{
protected:
	typedef void (Semantics_Analysis::* function_pointer)(string s);				    //函数指针重定义
	typedef unordered_map<string, Semantics_Analysis::function_pointer> function_map;   //语义动作和执行函数的关联图
	function_map get_semantic_action;													//定义语义动作具体对象
	function_pointer semantic_action = NULL;     										//执行语义动作函数

	typedef struct
	{
		string token_word;			   //token单词
		string token_type;			   //token串的属性
		int token_number = 0;		   //token串在表内的序号
		string* token_pointer = NULL;  //如果token是标识符，则指针指向符号表
	} token_property;				   //token的整体属性
	vector<token_property> token_list; //token信息列表

	/*语义分析过程，填写符号表*/
	unordered_map<string, int> end_symbol_name_to_number;//得到一个标识符在表中的位置
	vector<type_property> type_property_list;//种类表集合
	vector<array_table> array_table_list;//数组表集合
	vector<structure_table> structure_table_list;//结构表集合
	vector<function_table> function_table_list;//函数表集合
	vector<constant_table> constant_table_list;//常量表集合
	vector<symbol_table> symbol_table_list;//每张符号表集合

	/*语义分析过程，生成四元式*/
	vector<vector<string>> semantics_property;					//语义文法
	stack<string> semantic_data_stack;							//语义数据栈
	stack<string> temp_semantic_data_stack;						//语义动作参数暂存栈，对应的语义动作放入语法栈
	vector<four_item_production> four_item_production_list;     //四元式序列
	vector<vector<four_item_production>> function_four_item_production_list;//每个函数执行操作的四元式集合
	int temp_t_number = 1;//临时变量xt的x值

public:
	Semantics_Analysis();
	~Semantics_Analysis();
	vector<vector<four_item_production>> return_four_item_production() { return this->function_four_item_production_list; }//得到生成的四元式序列
	vector<symbol_table> return_symbol_table_list() { return this->symbol_table_list; }//得到符号表总表的集合
	vector<string> return_const_symbol(){ return this->const_symbol; }
	void scanf_information();
	void Generate_Token(); //语义分析的词法分析
	void check_code(int state, string token);
	int search_key(string one_token, vector<string>* key, string type);
	void print_token_list();
	int LL1_sentence_analysis(string token);//语义分析的语法分析+语义分析
	int LL1_sentence_analysis_with_print_step(string token);
	void add_production(string operate);
	void add_production(string operate, string first);
	void push_token(string s);
	void handle_token(string s);
	void handle_equal_symbol(string s);
	void handle_begin_if(string s);
	void handle_else_if(string s);
	void handle_else(string s);
	void handle_end_if(string s);
	void handle_judge_end(string s);
	void handle_begin_while(string s);
	void handle_do_while(string s);
	void handle_end_while(string s);
	void handle_compare_symbol(string s);
	void handle_array(string s);
	void handle_begin_for(string s);
	void handle_judge_for(string s);
	void handle_do_for(string s);
	void handle_end_for(string s);
	void handle_after_do(string s);
	void handle_auto_change(string s);
	void handle_break(string s);
	void handle_continue(string s);
	void handle_switch(string s);
	void handle_case(string s);
	void handle_case_end(string s);
	void handle_default(string s);
	void handle_switch_end(string s);
	void handle_function_begin(string s);  //函数开始
	void handle_function_end(string s);  //函数结束
	void handle_program_end(string s);   //程序结束
	void handle_jump_function(string s);  //跳转到执行函数操作四元式（有返回值）
	void handle_jump_void_function(string s); //跳转到执行函数操作四元式（无返回值）
	void handle_return(string s);      //return四元式
	void handle_dian_symbol(string s);   //处理点操作符
	void handle_before_identifier(string s); ////标识符前缀四元式（指针*和取地址&）

	int jilu_symbol_num = 0;//用来记录函数的符号表
	void save_assignment(string s);
	void save_array_assignment(string s);
	void save_array(string s);
	void save_struct(string s);
	void save_struct_name(string s);
	void save_struct_member(string s);
	void write_symbol_table(string s);
	void write_struct_table(string s);
	void write_function_table(string s);
	int duplicate_check(string s);
};
