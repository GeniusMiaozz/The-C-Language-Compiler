#pragma once
#include "Compiler.h"
class Code_Process
{
protected:
	typedef struct
	{
		int node_number = 0;//结点序号
		vector<int> front_node;//前驱结点
		vector<int> rear_node;//后继结点
		string operate_symbol;//运算符
		string main_mark;//主标记
		vector<string> other_mark;//其他标记
		int symbol_size;//该节点所代表标识符的字节长度，1/2
	} DAG_property;

	vector<symbol_table> symbol_table_list;//符号表
	vector<vector<four_item_production>> four_item_production_list;     //四元式序列
	vector<string> const_symbol = { "\0" };//标识符,如a_b

	/*代码优化部分*/
	vector<vector<four_item_production>> new_four_item_production_list;     //优化后的四元式序列 
	vector<map<string, bool>> symbol_active_list;               //活跃信息初始化时用到的辅助表
	map<string, int> DAG_symbol_to_node;//得到一个常量、非临时变量或临时变量在DAG图的哪一个节点上
	vector<DAG_property> DAG_map;//DAG图
	vector<vector<DAG_property>> out_DAG;//输出的DAG图
	vector<four_item_production> temp_f;//临时四元式表
	four_item_production temp_item;//临时四元式

	/*提供给代码生成*/
	vector<map<string, int>> get_symbol_size;//一个标识符的长度，提供给生成代码使用
	int function_number = -1;//标记第几个函数
public:
	Code_Process();
	~Code_Process();
	void get_symbol_table_list(vector<symbol_table> s);
	void get_four_item_production(vector<vector<four_item_production>> f);//四元式赋值
	void get_const_symbol(vector<string> c);//标识符赋值
	vector<string> return_const_symbol() { return this->const_symbol; }
	vector<vector<four_item_production>> return_new_four_item_production_list() { return this->new_four_item_production_list; }//返回优化后的四元式
	vector<symbol_table> return_symbol_table_list() { return this->symbol_table_list; }
	vector<map<string, int>> return_sizeof_symbol_list() { return this->get_symbol_size; }
	void print_old_four_item_production();//输出未优化的四元式
	void print_new_four_item_production();//输出优化后的四元式
	void print_DAG_information();//输出优化过程中产生的DAG
	int block_part_variable_flag(string s);//判断四元式中的一个标识符变量是否是块内的局部变量
	int const_symbol_flag(string s);//判断一个字符是不是标识符
	int const_number_flag(string s);//判断一个字符是不是常量
	int get_symbol_number(string s, int i);//得到一个标识符在符号表中的位置
	void divide_basic_block(vector<vector<four_item_production>>* four_item_production_list);//划分基本块
	void initialize_active_information(vector<vector<four_item_production>>* four_item_production_list);//活跃信息初始化前初始化辅助表
	void change_active_information(vector<vector<four_item_production>>* four_item_production_list);//修改活跃信息
	void code_optomization();//代码优化
	int get_A_B_node_number(string s);//DAG中处理A=B的点
	int get_A_C1wC2_node_number(string c1, string c2, string c3);//DAG中处理A=C1wC2的点
	int get_A_wB_node_number(string B, string w);//DAG中处理A=wB的点
	int get_A_BwC_node_number(string B, string C, string w);//DAG中处理A=BwC的点
	void handle_main_other_order(int node);//对DAG中点的主、附加标记顺序交换处理
	void handle_DAG();//根据DAG生成块内优化四元式
};