#pragma once
#include "Compiler.h"
#include "Lexical_Analysis.h"
#include "Grammar_Analysis.h"
#include "Semantics_Analysis.h"
#include "Code_Process.h"
#include "Code_Generation.h"

int main()
{
	Lexical_Analysis LA;
	Semantics_Analysis SA;
	Grammar_Analysis GA;
	Code_Process OP;
	Code_Generation CG;
	//LA.Generate_Token();
	//LA.print_token_list();
	system("color f0");
	//GA.scanf_information();//输入文法及属性文法
	//GA.get_first_list();//求文法各非终结符first集
	//GA.get_follow_list();//求文法各非终结符follow集
	//GA.get_select_list();//求文法各产生式select集
	//GA.get_LL1_list();//求LL1语法分析分析表
	//GA.print_information();//输出文法相关信息
	//GA.Generate_Token();//词法分析+语法分析+语义分析
	SA.scanf_information();//输入文法及属性文法
	SA.get_first_list();//求文法各非终结符first集
	SA.get_follow_list();//求文法各非终结符follow集
	SA.get_select_list();//求文法各产生式select集
	SA.get_LL1_list();//求LL1语法分析分析表
	SA.print_information();//输出文法相关信息
	SA.Generate_Token();//词法分析+语法分析+语义分析
	SA.print_token_list();//输出词法分析过程中保存的token串
	OP.get_const_symbol(SA.return_const_symbol());//将前端生成的标识符信息传给后端
	OP.get_four_item_production(SA.return_four_item_production());//将前端生成的中间代码四元式传给后端
	OP.get_symbol_table_list(SA.return_symbol_table_list());//将前端生成的符号表传给后端
	OP.code_optomization();//DAG块内代码优化
	OP.print_DAG_information();//输出DAG的信息
	OP.print_old_four_item_production();//输出未优化的四元式及变量活跃信息
	OP.print_new_four_item_production();//输出优化后的四元式及变量活跃信息
	CG.get_const_symbol(OP.return_const_symbol());//标识符赋值
	CG.get_intermediate_code_list(OP.return_new_four_item_production_list());//优化后的中间代码赋值
	CG.get_symbol_table_list(OP.return_symbol_table_list());//符号表赋值
	CG.get_sizeof_symbol_list(OP.return_sizeof_symbol_list());//符号对应的长度赋值
	CG.generate_code_list();//生成目标代码
	CG.write_in_file();
	system("pause");
}