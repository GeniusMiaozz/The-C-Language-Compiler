#include "Code_Process.h"

class Code_Generation :public Code_Process
{
private:
	vector<vector<four_item_production>> intermediate_code_list;//优化后的中间代码
	vector<vector<string>> object_code_list;//生成的目标代码

	vector<string> reg_8 = { "AL", "AH", "BL", "BH", "CL", "CH","DL", "DH" };//8位寄存器
	vector<string> reg_16 = { "AX", "BX", "CX", "DX" };//16位寄存器
	map <string, string>symbol_to_reg;//变量所在的寄存器
	map <string, string>reg_to_symbol;//寄存器存的变量
public:
	Code_Generation();
	~Code_Generation();
	void get_const_symbol(vector<string> c) { this->const_symbol = c; }
	void get_symbol_table_list(vector<symbol_table> s) { this->symbol_table_list = s; }
	void get_intermediate_code_list(vector<vector<four_item_production>> f) { this->intermediate_code_list = f; }
	void get_sizeof_symbol_list(vector<map<string, int>> g) { this->get_symbol_size = g; }
	void generate_code_list();//生成目标代码
	void write_in_file();//写入文件
};