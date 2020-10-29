#include "Code_Generation.h"

Code_Generation::Code_Generation()
{
}

Code_Generation::~Code_Generation()
{
}

void Code_Generation::generate_code_list()//生成目标代码
{
	vector<string> line_code;//一行目标代码
	function_number = -1;
	for (int i = 0; i < intermediate_code_list.size(); i++)
	{//查看一个函数的优化后中间代码
		function_number++;
		object_code_list.resize(object_code_list.size() + 1);
		object_code_list[i].push_back("DATA\tSEGMENT");//数据段开始
		for (int j = 0; j < symbol_table_list[i].symbol_property_table.size(); j++)
		{
			string temp_s;
			if (symbol_table_list[i].symbol_property_table[j].type->type_code == "int")
			{//int设为DB一字节
				constant_table* value = (constant_table*)symbol_table_list[i].symbol_property_table[j].address;
				if (value == NULL)
				{
					temp_s = symbol_table_list[i].symbol_property_table[j].name + "\tDB\t?";
				}
				else
				{
					temp_s = symbol_table_list[i].symbol_property_table[j].name + "\tDB\t" + to_string(value->value.int_value);
				}
				//free(value);
			}
			else if (symbol_table_list[i].symbol_property_table[j].type->type_code == "double")
			{//double设为DW双字节
				constant_table* value = (constant_table*)symbol_table_list[i].symbol_property_table[j].address;
				if (value == NULL)
				{
					temp_s = symbol_table_list[i].symbol_property_table[j].name + "\tDW\t?";
				}
				else
				{
					temp_s = symbol_table_list[i].symbol_property_table[j].name + "\tDW\t" + to_string(value->value.double_value);
				}
				//free(value);
			}
			else if (symbol_table_list[i].symbol_property_table[j].type->type_code == "float")
			{//float设为DB一字节
				constant_table* value = (constant_table*)symbol_table_list[i].symbol_property_table[j].address;
				if (value == NULL)
				{
					temp_s = symbol_table_list[i].symbol_property_table[j].name + "\tDB\t?";
				}
				else
				{
					temp_s = symbol_table_list[i].symbol_property_table[j].name + "\tDB\t" + to_string(value->value.float_value);
				}
				//free(value);
			}
			else if (symbol_table_list[i].symbol_property_table[j].type->type_code == "char")
			{//char设为DB一字节
				constant_table* value = (constant_table*)symbol_table_list[i].symbol_property_table[j].address;
				if (value == NULL)
				{
					temp_s = symbol_table_list[i].symbol_property_table[j].name + "\tDB\t?";
				}
				else
				{
					temp_s = symbol_table_list[i].symbol_property_table[j].name + "\tDB\t" + to_string(value->value.char_value);
				}
				//free(value);
			}
			object_code_list[i].push_back(temp_s);//一条定义变量语句
		}
		object_code_list[i].push_back("DATA\tENDS");//数据段结束
		object_code_list[i].push_back("CODE\tSEGMENT");//数据段开始
		object_code_list[i].push_back("ASSUME\tCS:CODE,DS:DATA");//数据段开始
		object_code_list[i].push_back("START:\tMOV AX,DATA");
		object_code_list[i].push_back("\tMOV DS,AX");//设置基址
		string temp_code;
		int part_number = 0;
		stack<string> temp_position;
		string for_jump_position;
		string switch_jump_position;
		for (__int64 j = 0; j < intermediate_code_list[i].size(); j++)
		{//遍历第一个函数四元式表的每一条
			if (intermediate_code_list[i][j].basic_block_flag == "入口块")
			{
				symbol_to_reg.clear();
				reg_to_symbol.clear();//释放寄存器
			}

			/*跳过语句部分*/
			if ((intermediate_code_list[i][j].operation_symbol == "beginFunction") || (intermediate_code_list[i][j].operation_symbol == "endFunction"))
			{//函数的开头和结尾跳过语句继续遍历
				continue;
			}
			if ((intermediate_code_list[i][j].operation_symbol == "beginDo") || (intermediate_code_list[i][j].operation_symbol == "endDo"))
			{//跳过语句继续遍历
				continue;
			}

			/*if处理部分*/
			else if ((intermediate_code_list[i][j].operation_symbol == "ifJudge") || (intermediate_code_list[i][j].operation_symbol == "ifEndJudge"))
			{//if判断中的判断跳转语句
				object_code_list[i].push_back(temp_code);
				continue;
			}
			else if ((intermediate_code_list[i][j].operation_symbol == "ifBeginJudge") || (intermediate_code_list[i][j].operation_symbol == "else"))
			{//if判断中的跳转到语句
				string s = temp_position.top();
				temp_position.pop();
				object_code_list[i].push_back(s + ":\t");
				continue;
			}
			else if ((intermediate_code_list[i][j].operation_symbol == "ifBeginDo") || (intermediate_code_list[i][j].operation_symbol == "ifToEnd") || (intermediate_code_list[i][j].operation_symbol == "endIf"))
			{//if判断中的顺序执行语句
				continue;
			}

			/*while处理部分*/
			else if ((intermediate_code_list[i][j].operation_symbol == "whileDoIf"))
			{//while判断中的跳转语句
				object_code_list[i].push_back(temp_code);
				continue;
			}
			else if ((intermediate_code_list[i][j].operation_symbol == "endWhile"))
			{//while判断中的跳转到的语句
				string s = temp_position.top();
				temp_position.pop();
				object_code_list[i].push_back(s + ":\t");
				continue;
			}
			else if ((intermediate_code_list[i][j].operation_symbol == "beginWhile") || (intermediate_code_list[i][j].operation_symbol == "whileBeginDo"))
			{//while判断中的顺序执行语句
				continue;
			}

			/*for处理部分*/
			else if (intermediate_code_list[i][j].operation_symbol == "forDoIf")
			{//for判断中的跳转语句
				object_code_list[i].push_back(temp_code);
				continue;
			}
			else if (intermediate_code_list[i][j].operation_symbol == "forBeginJudge")
			{//for判断中转回判断条件
				for_jump_position = ("FUN" + to_string(part_number));
				object_code_list[i].push_back("FUN" + to_string(part_number) + ":");
				part_number++;
			}
			else if ((intermediate_code_list[i][j].operation_symbol == "endFor"))
			{//for判断中的跳转到的语句
				object_code_list[i].push_back("\tJMP " + for_jump_position);
				string s = temp_position.top();
				temp_position.pop();
				object_code_list[i].push_back(s + ":\t");
				continue;
			}
			else if ((intermediate_code_list[i][j].operation_symbol == "afterForDo") || (intermediate_code_list[i][j].operation_symbol == "forBegin") || (intermediate_code_list[i][j].operation_symbol == "forBeginEnd"))
			{//for判断中的顺序执行语句
				continue;
			}

			/*switch处理部分*/
			else if (intermediate_code_list[i][j].operation_symbol == "switch")
			{
				switch_jump_position = "FUN" + to_string(part_number);
				part_number++;
			}
			else if ((intermediate_code_list[i][j].operation_symbol == "case"))
			{//switch判断中的跳转语句
				object_code_list[i].push_back(temp_code);
				continue;
			}
			else if ((intermediate_code_list[i][j].operation_symbol == "caseEnd"))
			{//switch判断中的跳转到的语句
				string s = temp_position.top();
				temp_position.pop();
				object_code_list[i].push_back(s + ":\t");
				continue;
			}
			else if ((intermediate_code_list[i][j].operation_symbol == "caseBegin") || (intermediate_code_list[i][j].operation_symbol == "default") || (intermediate_code_list[i][j].operation_symbol == "caseBeginJudge"))
			{//switch判断中的顺序执行语句
				continue;
			}
			else if (intermediate_code_list[i][j].operation_symbol == "break")
			{
				object_code_list[i].push_back("\tJMP " + switch_jump_position);
				continue;
			}
			else if (intermediate_code_list[i][j].operation_symbol == "endSwitch")
			{
				object_code_list[i].push_back(switch_jump_position + ":");
			}

			/*赋值表达式处理*/
			else if (intermediate_code_list[i][j].operation_symbol == "=")
			{//赋值表达式(= B _ A)
				if (symbol_to_reg.count(intermediate_code_list[i][j].first_target.operation_target_value))
				{//当前有寄存器存储B的变量
					if (intermediate_code_list[i][j].first_target.active_information)
					{
						//object_code_list[i].push_back("\tMOV DS:[" + to_string(symbol_table_list[i].symbol_property_table[get_symbol_number(intermediate_code_list[i][j].first_target.operation_target_value, i)].offset) + "]," + intermediate_code_list[i][j].first_target.operation_target_value);
						////MOV DS:[B.offset],B
						object_code_list[i].push_back("\tMOV " + intermediate_code_list[i][j].result_target.result_target_value + "," + symbol_to_reg[intermediate_code_list[i][j].first_target.operation_target_value]);
						//MOV A,reg_B
						continue;
					}
				}
				else if (symbol_to_reg.count(intermediate_code_list[i][j].result_target.result_target_value))
				{//当前有寄存器存储A的变量
					object_code_list[i].push_back("\tMOV " + symbol_to_reg[intermediate_code_list[i][j].result_target.result_target_value] + "," + intermediate_code_list[i][j].first_target.operation_target_value);
					//MOV reg_A,B
					object_code_list[i].push_back("\tMOV " + intermediate_code_list[i][j].result_target.result_target_value + "," + symbol_to_reg[intermediate_code_list[i][j].result_target.result_target_value]);
					//MOV A,reg_A
					continue;
				}
				else
				{//既没有寄存器保存A,也没有寄存器保存B
				 //查看是否有寄存器空闲，有空闲使用空闲
					if (get_symbol_size[function_number][intermediate_code_list[i][j].result_target.result_target_value] == 8)
					{//double型需要16位寄存器
						int k, flag = 0;
						for (k = 0; k < reg_16.size(); k++)
						{
							if (!reg_to_symbol.count(reg_16[k]))
							{
								reg_to_symbol.insert(pair<string, string>(reg_16[k], intermediate_code_list[i][j].result_target.result_target_value));
								symbol_to_reg.insert(pair<string, string>(intermediate_code_list[i][j].result_target.result_target_value, reg_16[k]));
								object_code_list[i].push_back("\tMOV " + symbol_to_reg[intermediate_code_list[i][j].result_target.result_target_value] + "," + intermediate_code_list[i][j].first_target.operation_target_value);
								//MOV reg_A,B
								object_code_list[i].push_back("\tMOV " + intermediate_code_list[i][j].result_target.result_target_value + "," + symbol_to_reg[intermediate_code_list[i][j].result_target.result_target_value]);
								//MOV A,reg_A
								flag = 1;
								break;
							}
						}
						if (flag)//分配成功跳出本次循环
							continue;
					}
					else
					{//其他类型只需要8位寄存器
						int k, flag = 0;
						for (k = 0; k < reg_8.size(); k++)
						{
							if (!reg_to_symbol.count(reg_8[k]))
							{
								reg_to_symbol.insert(pair<string, string>(reg_8[k], intermediate_code_list[i][j].result_target.result_target_value));
								symbol_to_reg.insert(pair<string, string>(intermediate_code_list[i][j].result_target.result_target_value, reg_8[k]));
								object_code_list[i].push_back("\tMOV " + symbol_to_reg[intermediate_code_list[i][j].result_target.result_target_value] + "," + intermediate_code_list[i][j].first_target.operation_target_value);
								//MOV reg_A,B
								object_code_list[i].push_back("\tMOV " + intermediate_code_list[i][j].result_target.result_target_value + "," + symbol_to_reg[intermediate_code_list[i][j].result_target.result_target_value]);
								//MOV A,reg_A
								flag = 1;
								break;
							}
						}
						if (flag)//分配成功跳出本次循环
							continue;
					}
					if (get_symbol_size[function_number][intermediate_code_list[i][j].result_target.result_target_value] == 8)
					{//没有空闲寄存器，则需保存寄存器的值
						if ((get_symbol_number(reg_to_symbol["AX"], i)) != 0)
						{//当前AX寄存器变量需要保存
							object_code_list[i].push_back("\tMOV DS:[" + to_string(symbol_table_list[i].symbol_property_table[get_symbol_number(reg_to_symbol["AX"], i)].offset) + "]," + reg_to_symbol["AX"]);
							symbol_to_reg.erase(reg_to_symbol["AX"]);
							reg_to_symbol["AX"] = intermediate_code_list[i][j].first_target.operation_target_value;
						}
						//reg_to_symbol.insert(pair<string, string>("AX", intermediate_code_list[i][j].first_target.operation_target_value));
						symbol_to_reg.insert(pair<string, string>(intermediate_code_list[i][j].first_target.operation_target_value, "AX"));
						object_code_list[i].push_back("\tPUSH AX");
						object_code_list[i].push_back("\tMOV AX," + intermediate_code_list[i][j].first_target.operation_target_value);
						object_code_list[i].push_back("\tMOV " + intermediate_code_list[i][j].result_target.result_target_value + ",AX");
						object_code_list[i].push_back("\tPOP AX");
						continue;
					}
					else
					{
						if ((get_symbol_number(reg_to_symbol["AL"], i)) != 0)
						{//当前AX寄存器变量需要保存
							object_code_list[i].push_back("\tMOV DS:[" + to_string(symbol_table_list[i].symbol_property_table[get_symbol_number(reg_to_symbol["AL"], i)].offset) + "]," + reg_to_symbol["AL"]);
							symbol_to_reg.erase(reg_to_symbol["AL"]);
							reg_to_symbol["AL"] = intermediate_code_list[i][j].first_target.operation_target_value;
						}
						//reg_to_symbol.insert(pair<string, string>("AL", intermediate_code_list[i][j].first_target.operation_target_value));
						symbol_to_reg.insert(pair<string, string>(intermediate_code_list[i][j].first_target.operation_target_value, "AL"));
						object_code_list[i].push_back("\tPUSH AL");
						object_code_list[i].push_back("\tMOV AL," + intermediate_code_list[i][j].first_target.operation_target_value);
						object_code_list[i].push_back("\tMOV " + intermediate_code_list[i][j].result_target.result_target_value + ",AX");
						object_code_list[i].push_back("\tPOP AL");
						continue;
					}
				}
			}

			/*比较运算*/
			else if ((intermediate_code_list[i][j].operation_symbol == "==") || (intermediate_code_list[i][j].operation_symbol == "!=") || (intermediate_code_list[i][j].operation_symbol == "<=") || (intermediate_code_list[i][j].operation_symbol == ">=") || (intermediate_code_list[i][j].operation_symbol == "<") || (intermediate_code_list[i][j].operation_symbol == ">"))
			{//(w B C A)
				if (intermediate_code_list[i][j].operation_symbol == "==")
				{
					temp_code = ("\tJNE FUN" + to_string(part_number));
					temp_position.push("FUN" + to_string(part_number));
					part_number++;
				}
				else if (intermediate_code_list[i][j].operation_symbol == "!=")
				{
					temp_code = ("\tJE FUN" + to_string(part_number));
					temp_position.push("FUN" + to_string(part_number));
					part_number++;
				}
				else if (intermediate_code_list[i][j].operation_symbol == "<=")
				{
					temp_code = ("\tJA FUN" + to_string(part_number));
					temp_position.push("FUN" + to_string(part_number));
					part_number++;
				}
				else if (intermediate_code_list[i][j].operation_symbol == ">=")
				{
					temp_code = ("\tJB FUN" + to_string(part_number));
					temp_position.push("FUN" + to_string(part_number));
					part_number++;
				}
				else if (intermediate_code_list[i][j].operation_symbol == "<")
				{
					temp_code = ("\tJAE FUN" + to_string(part_number));
					temp_position.push("FUN" + to_string(part_number));
					part_number++;
				}
				else if (intermediate_code_list[i][j].operation_symbol == ">")
				{
					temp_code = ("\tJBE FUN" + to_string(part_number));
					temp_position.push("FUN" + to_string(part_number));
					part_number++;
				}
				if (symbol_to_reg.count(intermediate_code_list[i][j].first_target.operation_target_value))
				{//当前有寄存器存了第一个值
					object_code_list[i].push_back("\tCMP " + symbol_to_reg[intermediate_code_list[i][j].first_target.operation_target_value] + "," + intermediate_code_list[i][j].second_target.operation_target_value);
				}
				else if (symbol_to_reg.count(intermediate_code_list[i][j].second_target.operation_target_value) && (intermediate_code_list[i][j].operation_symbol == "=="))
				{//当前有寄存器存了第二个数的值，且运算符号为==
					object_code_list[i].push_back("\tCMP " + symbol_to_reg[intermediate_code_list[i][j].second_target.operation_target_value] + "," + intermediate_code_list[i][j].first_target.operation_target_value);
				}
				else
				{//需要一个新寄存器
					if (get_symbol_size[function_number][intermediate_code_list[i][j].first_target.operation_target_value] == 8)
					{//double型需要16位寄存器
						int k, flag = 0;
						for (k = 0; k < reg_16.size(); k++)
						{
							if (!reg_to_symbol.count(reg_16[k]))
							{
								reg_to_symbol.insert(pair<string, string>(reg_16[k], intermediate_code_list[i][j].first_target.operation_target_value));
								symbol_to_reg.insert(pair<string, string>(intermediate_code_list[i][j].first_target.operation_target_value, reg_16[k]));
								object_code_list[i].push_back("\tMOV " + symbol_to_reg[intermediate_code_list[i][j].first_target.operation_target_value] + "," + intermediate_code_list[i][j].first_target.operation_target_value);
								object_code_list[i].push_back("\tCMP " + symbol_to_reg[intermediate_code_list[i][j].first_target.operation_target_value] + "," + intermediate_code_list[i][j].second_target.operation_target_value);
								//CMP reg_A,B
								flag = 1;
								break;
							}
						}
						if (flag)
							continue;
					}
					else
					{//其他类型只需要8位寄存器
						int k, flag = 0;
						for (k = 0; k < reg_8.size(); k++)
						{
							if (!reg_to_symbol.count(reg_8[k]))
							{
								reg_to_symbol.insert(pair<string, string>(reg_8[k], intermediate_code_list[i][j].first_target.operation_target_value));
								symbol_to_reg.insert(pair<string, string>(intermediate_code_list[i][j].first_target.operation_target_value, reg_8[k]));
								object_code_list[i].push_back("\tMOV " + symbol_to_reg[intermediate_code_list[i][j].first_target.operation_target_value] + "," + intermediate_code_list[i][j].first_target.operation_target_value);
								object_code_list[i].push_back("\tCMP " + symbol_to_reg[intermediate_code_list[i][j].first_target.operation_target_value] + "," + intermediate_code_list[i][j].second_target.operation_target_value);
								//CMP reg_A,B
								flag = 1;
								break;
							}
						}
						if (flag)
							continue;
					}
					if (get_symbol_size[function_number][intermediate_code_list[i][j].first_target.operation_target_value] == 8)
					{//没有空闲寄存器，则需保存寄存器的值
						if ((get_symbol_number(reg_to_symbol["AX"], i)) != 0)
						{//当前AX寄存器变量需要保存
							object_code_list[i].push_back("\tMOV DS:[" + to_string(symbol_table_list[i].symbol_property_table[get_symbol_number(reg_to_symbol["AX"], i)].offset) + "]," + reg_to_symbol["AX"]);
							symbol_to_reg.erase(reg_to_symbol["AX"]);
							reg_to_symbol["AX"] = intermediate_code_list[i][j].first_target.operation_target_value;
						}
						//reg_to_symbol.insert(pair<string, string>("AX", intermediate_code_list[i][j].first_target.operation_target_value));
						symbol_to_reg.insert(pair<string, string>(intermediate_code_list[i][j].first_target.operation_target_value, "AX"));
						object_code_list[i].push_back("\tPUSH AX");
						object_code_list[i].push_back("\tMOV " + symbol_to_reg[intermediate_code_list[i][j].first_target.operation_target_value] + "," + intermediate_code_list[i][j].first_target.operation_target_value);
						object_code_list[i].push_back("\tCMP " + symbol_to_reg[intermediate_code_list[i][j].first_target.operation_target_value] + "," + intermediate_code_list[i][j].second_target.operation_target_value);
						object_code_list[i].push_back("\tPOP AX");
						continue;
					}
					else
					{
						if ((get_symbol_number(reg_to_symbol["AL"], i)) != 0)
						{//当前AX寄存器变量需要保存
							object_code_list[i].push_back("\tMOV DS:[" + to_string(symbol_table_list[i].symbol_property_table[get_symbol_number(reg_to_symbol["AL"], i)].offset) + "]," + reg_to_symbol["AL"]);
							symbol_to_reg.erase(reg_to_symbol["AL"]);
							reg_to_symbol["AL"] = intermediate_code_list[i][j].first_target.operation_target_value;
						}
						//reg_to_symbol.insert(pair<string, string>("AL", intermediate_code_list[i][j].first_target.operation_target_value));
						symbol_to_reg.insert(pair<string, string>(intermediate_code_list[i][j].first_target.operation_target_value, "AL"));
						object_code_list[i].push_back("\tPUSH AL");
						object_code_list[i].push_back("\tMOV " + symbol_to_reg[intermediate_code_list[i][j].first_target.operation_target_value] + "," + intermediate_code_list[i][j].first_target.operation_target_value);
						object_code_list[i].push_back("\tCMP " + symbol_to_reg[intermediate_code_list[i][j].first_target.operation_target_value] + "," + intermediate_code_list[i][j].second_target.operation_target_value);
						object_code_list[i].push_back("\tPOP AL");
						continue;
					}
				}
			}

			/*算术运算*/
			else if ((intermediate_code_list[i][j].operation_symbol == "+") || (intermediate_code_list[i][j].operation_symbol == "-") || (intermediate_code_list[i][j].operation_symbol == "*") || (intermediate_code_list[i][j].operation_symbol == "/"))
			{//(w B C A)
				if (symbol_to_reg.count(intermediate_code_list[i][j].first_target.operation_target_value))
				{//当前有寄存器存了第一个值
					if (intermediate_code_list[i][j].operation_symbol == "+")
						object_code_list[i].push_back("\tADD " + symbol_to_reg[intermediate_code_list[i][j].first_target.operation_target_value] + "," + intermediate_code_list[i][j].second_target.operation_target_value);
					else if (intermediate_code_list[i][j].operation_symbol == "-")
						object_code_list[i].push_back("\tSUB " + symbol_to_reg[intermediate_code_list[i][j].first_target.operation_target_value] + "," + intermediate_code_list[i][j].second_target.operation_target_value);
					else if (intermediate_code_list[i][j].operation_symbol == "*")
						object_code_list[i].push_back("\tMUL " + symbol_to_reg[intermediate_code_list[i][j].first_target.operation_target_value] + "," + intermediate_code_list[i][j].second_target.operation_target_value);
					else
						object_code_list[i].push_back("\tDIV " + symbol_to_reg[intermediate_code_list[i][j].first_target.operation_target_value] + "," + intermediate_code_list[i][j].second_target.operation_target_value);
				}
				else if (symbol_to_reg.count(intermediate_code_list[i][j].second_target.operation_target_value) && ((intermediate_code_list[i][j].operation_symbol == "+") || (intermediate_code_list[i][j].operation_symbol == "*")))
				{//当前有寄存器存了第二个数的值，且运算符号为+或者*
					if (intermediate_code_list[i][j].operation_symbol == "+")
						object_code_list[i].push_back("\tADD " + symbol_to_reg[intermediate_code_list[i][j].second_target.operation_target_value] + "," + intermediate_code_list[i][j].first_target.operation_target_value);
					else if (intermediate_code_list[i][j].operation_symbol == "*")
						object_code_list[i].push_back("\tMUL " + symbol_to_reg[intermediate_code_list[i][j].second_target.operation_target_value] + "," + intermediate_code_list[i][j].first_target.operation_target_value);
				}
				else
				{//需要一个新寄存器
					if (get_symbol_size[function_number][intermediate_code_list[i][j].first_target.operation_target_value] == 8)
					{//double型需要16位寄存器
						int k, flag = 0;
						for (k = 0; k < reg_16.size(); k++)
						{
							if (!reg_to_symbol.count(reg_16[k]))
							{
								reg_to_symbol.insert(pair<string, string>(reg_16[k], intermediate_code_list[i][j].first_target.operation_target_value));
								symbol_to_reg.insert(pair<string, string>(intermediate_code_list[i][j].first_target.operation_target_value, reg_16[k]));
								object_code_list[i].push_back("\tMOV " + symbol_to_reg[intermediate_code_list[i][j].first_target.operation_target_value] + "," + intermediate_code_list[i][j].first_target.operation_target_value);
								if (intermediate_code_list[i][j].operation_symbol == "+")
									object_code_list[i].push_back("\tADD " + symbol_to_reg[intermediate_code_list[i][j].first_target.operation_target_value] + "," + intermediate_code_list[i][j].second_target.operation_target_value);
								else if (intermediate_code_list[i][j].operation_symbol == "-")
									object_code_list[i].push_back("\tSUB " + symbol_to_reg[intermediate_code_list[i][j].first_target.operation_target_value] + "," + intermediate_code_list[i][j].second_target.operation_target_value);
								else if (intermediate_code_list[i][j].operation_symbol == "*")
									object_code_list[i].push_back("\tMUL " + symbol_to_reg[intermediate_code_list[i][j].first_target.operation_target_value] + "," + intermediate_code_list[i][j].second_target.operation_target_value);
								else
									object_code_list[i].push_back("\tDIV " + symbol_to_reg[intermediate_code_list[i][j].first_target.operation_target_value] + "," + intermediate_code_list[i][j].second_target.operation_target_value);
								flag = 1;
								break;
							}
						}
						if (flag)
							continue;
					}
					else
					{//其他类型只需要8位寄存器
						int k, flag = 0;
						for (k = 0; k < reg_8.size(); k++)
						{
							if (!reg_to_symbol.count(reg_8[k]))
							{
								reg_to_symbol.insert(pair<string, string>(reg_8[k], intermediate_code_list[i][j].first_target.operation_target_value));
								symbol_to_reg.insert(pair<string, string>(intermediate_code_list[i][j].first_target.operation_target_value, reg_8[k]));
								object_code_list[i].push_back("\tMOV " + symbol_to_reg[intermediate_code_list[i][j].first_target.operation_target_value] + "," + intermediate_code_list[i][j].first_target.operation_target_value);
								if (intermediate_code_list[i][j].operation_symbol == "+")
									object_code_list[i].push_back("\tADD " + symbol_to_reg[intermediate_code_list[i][j].first_target.operation_target_value] + "," + intermediate_code_list[i][j].second_target.operation_target_value);
								else if (intermediate_code_list[i][j].operation_symbol == "-")
									object_code_list[i].push_back("\tSUB " + symbol_to_reg[intermediate_code_list[i][j].first_target.operation_target_value] + "," + intermediate_code_list[i][j].second_target.operation_target_value);
								else if (intermediate_code_list[i][j].operation_symbol == "*")
									object_code_list[i].push_back("\tMUL " + symbol_to_reg[intermediate_code_list[i][j].first_target.operation_target_value] + "," + intermediate_code_list[i][j].second_target.operation_target_value);
								else
									object_code_list[i].push_back("\tDIV " + symbol_to_reg[intermediate_code_list[i][j].first_target.operation_target_value] + "," + intermediate_code_list[i][j].second_target.operation_target_value);
								flag = 1;
								break;
							}
						}
						if (flag)
							continue;
					}
					if (get_symbol_size[function_number][intermediate_code_list[i][j].first_target.operation_target_value] == 8)
					{//没有空闲寄存器，则需保存寄存器的值
						if ((get_symbol_number(reg_to_symbol["AX"], i)) != 0)
						{//当前AX寄存器变量需要保存
							object_code_list[i].push_back("\tMOV DS:[" + to_string(symbol_table_list[i].symbol_property_table[get_symbol_number(reg_to_symbol["AX"], i)].offset) + "]," + reg_to_symbol["AX"]);
							symbol_to_reg.erase(reg_to_symbol["AX"]);
							reg_to_symbol["AX"] = intermediate_code_list[i][j].first_target.operation_target_value;
						}
						//reg_to_symbol.insert(pair<string, string>("AX", intermediate_code_list[i][j].first_target.operation_target_value));
						symbol_to_reg.insert(pair<string, string>(intermediate_code_list[i][j].first_target.operation_target_value, "AX"));
						object_code_list[i].push_back("\tPUSH AX");
						object_code_list[i].push_back("\tMOV " + symbol_to_reg[intermediate_code_list[i][j].first_target.operation_target_value] + "," + intermediate_code_list[i][j].first_target.operation_target_value);
						if (intermediate_code_list[i][j].operation_symbol == "+")
							object_code_list[i].push_back("\tADD " + symbol_to_reg[intermediate_code_list[i][j].first_target.operation_target_value] + "," + intermediate_code_list[i][j].second_target.operation_target_value);
						else if (intermediate_code_list[i][j].operation_symbol == "-")
							object_code_list[i].push_back("\tSUB " + symbol_to_reg[intermediate_code_list[i][j].first_target.operation_target_value] + "," + intermediate_code_list[i][j].second_target.operation_target_value);
						else if (intermediate_code_list[i][j].operation_symbol == "*")
							object_code_list[i].push_back("\tMUL " + symbol_to_reg[intermediate_code_list[i][j].first_target.operation_target_value] + "," + intermediate_code_list[i][j].second_target.operation_target_value);
						else
							object_code_list[i].push_back("\tDIV " + symbol_to_reg[intermediate_code_list[i][j].first_target.operation_target_value] + "," + intermediate_code_list[i][j].second_target.operation_target_value);
						object_code_list[i].push_back("\tPOP AX");
						continue;
					}
					else
					{
						if ((get_symbol_number(reg_to_symbol["AL"], i)) != 0)
						{//当前AX寄存器变量需要保存
							object_code_list[i].push_back("\tMOV DS:[" + to_string(symbol_table_list[i].symbol_property_table[get_symbol_number(reg_to_symbol["AL"], i)].offset) + "]," + reg_to_symbol["AL"]);
							symbol_to_reg.erase(reg_to_symbol["AL"]);
							reg_to_symbol["AL"] = intermediate_code_list[i][j].first_target.operation_target_value;
						}
						//reg_to_symbol.insert(pair<string, string>("AL", intermediate_code_list[i][j].first_target.operation_target_value));
						symbol_to_reg.insert(pair<string, string>(intermediate_code_list[i][j].first_target.operation_target_value, "AL"));
						object_code_list[i].push_back("\tPUSH AL");
						if (intermediate_code_list[i][j].operation_symbol == "+")
							object_code_list[i].push_back("\tADD " + symbol_to_reg[intermediate_code_list[i][j].first_target.operation_target_value] + "," + intermediate_code_list[i][j].second_target.operation_target_value);
						else if (intermediate_code_list[i][j].operation_symbol == "-")
							object_code_list[i].push_back("\tSUB " + symbol_to_reg[intermediate_code_list[i][j].first_target.operation_target_value] + "," + intermediate_code_list[i][j].second_target.operation_target_value);
						else if (intermediate_code_list[i][j].operation_symbol == "*")
							object_code_list[i].push_back("\tMUL " + symbol_to_reg[intermediate_code_list[i][j].first_target.operation_target_value] + "," + intermediate_code_list[i][j].second_target.operation_target_value);
						else
							object_code_list[i].push_back("\tDIV " + symbol_to_reg[intermediate_code_list[i][j].first_target.operation_target_value] + "," + intermediate_code_list[i][j].second_target.operation_target_value);
						object_code_list[i].push_back("\tPOP AL");
						continue;
					}
				}
			}

			/*自整自减*/
			else if ((intermediate_code_list[i][j].operation_symbol == "behind++") || (intermediate_code_list[i][j].operation_symbol == "front++"))
			{//behind++ _ _ _
				object_code_list[i].push_back("\tPUSH AX");
				object_code_list[i].push_back("\tMOV AX,DS:[" + to_string(symbol_table_list[i].symbol_property_table[get_symbol_number(intermediate_code_list[i][j].result_target.result_target_value, i)].offset) + "D]");
				object_code_list[i].push_back("\tINC AX");
				object_code_list[i].push_back("\tMOV DS:[" + to_string(symbol_table_list[i].symbol_property_table[get_symbol_number(intermediate_code_list[i][j].result_target.result_target_value, i)].offset) + "D],AX");
				object_code_list[i].push_back("\tPOP AX");
}
			else if ((intermediate_code_list[i][j].operation_symbol == "behind--") || (intermediate_code_list[i][j].operation_symbol == "front--"))
			{
			object_code_list[i].push_back("\tPUSH AX");
			object_code_list[i].push_back("\tMOV AX,DS:[" + to_string(symbol_table_list[i].symbol_property_table[get_symbol_number(intermediate_code_list[i][j].result_target.result_target_value, i)].offset) + "D]");
			object_code_list[i].push_back("\tDEC AX");
			object_code_list[i].push_back("\tMOV DS:[" + to_string(symbol_table_list[i].symbol_property_table[get_symbol_number(intermediate_code_list[i][j].result_target.result_target_value, i)].offset) + "D],AX");
			object_code_list[i].push_back("\tPOP AX");
			}

			/*数组取值*/
			else if (intermediate_code_list[i][j].operation_symbol == "array")
			{//array arr 9 c
				if (get_symbol_size[function_number][intermediate_code_list[i][j].first_target.operation_target_value] == 8)
				{//没有空闲寄存器，则需保存寄存器的值
					if ((get_symbol_number(reg_to_symbol["AX"], i)) != 0)
					{//当前AX寄存器变量需要保存
						object_code_list[i].push_back("\tMOV DS:[" + to_string(symbol_table_list[i].symbol_property_table[get_symbol_number(reg_to_symbol["AX"], i)].offset) + "]," + reg_to_symbol["AX"]);
						symbol_to_reg.erase(reg_to_symbol["AX"]);
						reg_to_symbol["AX"] = intermediate_code_list[i][j].first_target.operation_target_value;
					}
					//reg_to_symbol.insert(pair<string, string>("AX", intermediate_code_list[i][j].first_target.operation_target_value));
					symbol_to_reg.insert(pair<string, string>(intermediate_code_list[i][j].first_target.operation_target_value, "AX"));
					object_code_list[i].push_back("\tPUSH AX");
					object_code_list[i].push_back("\tMOV " + symbol_to_reg[intermediate_code_list[i][j].first_target.operation_target_value] + "," + intermediate_code_list[i][j].first_target.operation_target_value);
					if (intermediate_code_list[i][j].operation_symbol == "+")
						object_code_list[i].push_back("\tADD " + symbol_to_reg[intermediate_code_list[i][j].first_target.operation_target_value] + "," + intermediate_code_list[i][j].second_target.operation_target_value);
					else if (intermediate_code_list[i][j].operation_symbol == "-")
						object_code_list[i].push_back("\tSUB " + symbol_to_reg[intermediate_code_list[i][j].first_target.operation_target_value] + "," + intermediate_code_list[i][j].second_target.operation_target_value);
					else if (intermediate_code_list[i][j].operation_symbol == "*")
						object_code_list[i].push_back("\tMUL " + symbol_to_reg[intermediate_code_list[i][j].first_target.operation_target_value] + "," + intermediate_code_list[i][j].second_target.operation_target_value);
					else
						object_code_list[i].push_back("\tDIV " + symbol_to_reg[intermediate_code_list[i][j].first_target.operation_target_value] + "," + intermediate_code_list[i][j].second_target.operation_target_value);
					object_code_list[i].push_back("\tPOP AX");
					continue;
				}
				else
				{
					if ((get_symbol_number(reg_to_symbol["AL"], i)) != 0)
					{//当前AX寄存器变量需要保存
						object_code_list[i].push_back("\tMOV DS:[" + to_string(symbol_table_list[i].symbol_property_table[get_symbol_number(reg_to_symbol["AL"], i)].offset) + "]," + reg_to_symbol["AL"]);
						symbol_to_reg.erase(reg_to_symbol["AL"]);
						reg_to_symbol["AL"] = intermediate_code_list[i][j].first_target.operation_target_value;
					}
					//reg_to_symbol.insert(pair<string, string>("AL", intermediate_code_list[i][j].first_target.operation_target_value));
					symbol_to_reg.insert(pair<string, string>(intermediate_code_list[i][j].first_target.operation_target_value, "AL"));
					object_code_list[i].push_back("\tPUSH AL");
					if (intermediate_code_list[i][j].operation_symbol == "+")
						object_code_list[i].push_back("\tADD " + symbol_to_reg[intermediate_code_list[i][j].first_target.operation_target_value] + "," + intermediate_code_list[i][j].second_target.operation_target_value);
					else if (intermediate_code_list[i][j].operation_symbol == "-")
						object_code_list[i].push_back("\tSUB " + symbol_to_reg[intermediate_code_list[i][j].first_target.operation_target_value] + "," + intermediate_code_list[i][j].second_target.operation_target_value);
					else if (intermediate_code_list[i][j].operation_symbol == "*")
						object_code_list[i].push_back("\tMUL " + symbol_to_reg[intermediate_code_list[i][j].first_target.operation_target_value] + "," + intermediate_code_list[i][j].second_target.operation_target_value);
					else
						object_code_list[i].push_back("\tDIV " + symbol_to_reg[intermediate_code_list[i][j].first_target.operation_target_value] + "," + intermediate_code_list[i][j].second_target.operation_target_value);
					object_code_list[i].push_back("\tPOP AL");
					continue;
				}
			}
		}
		object_code_list[i].push_back("\tMOV AX,4C00H");
		object_code_list[i].push_back("\tINT 21H");//中断
		object_code_list[i].push_back("CODE\tENDS");//代码段结束
		object_code_list[i].push_back("END\tSTART");//程序结束
	}
	for (int i = 0; i < object_code_list.size(); i++)
	{
		for (int j = 0; j < object_code_list[i].size(); j++)
		{
			cout << object_code_list[i][j] << endl;
		}
	}
}

void Code_Generation::write_in_file()
{
	fstream file;
	file.open("OutputFile.txt");
	for (int i = 0; i < object_code_list.size(); i++)
	{
		for (int j = 0; j < object_code_list[i].size(); j++)
		{
			file << object_code_list[i][j];
			file << endl;
		}
	}
}