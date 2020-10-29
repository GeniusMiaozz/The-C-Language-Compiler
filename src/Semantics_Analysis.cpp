#include "Semantics_Analysis.h"

Semantics_Analysis::Semantics_Analysis()
{
	structure_table_list.insert(structure_table_list.begin(), 20, { -1,0, });
	token_list.resize(0);
	symbol_table_list.resize(1);
	type_property_list.insert(type_property_list.begin(), 50, { "未声明类型",NULL });
	type_property_list.at(0).type_code = "int";
	type_property_list.at(1).type_code = "float";
	type_property_list.at(2).type_code = "char";
	type_property_list.at(3).type_code = "bool";
	type_property_list.at(4).type_code = "double";
	array_table_list.insert(array_table_list.begin(), 50, { -1,-1,NULL,-1 });
	constant_table_list.resize(1);
	/*初始化常量表*/
	constant_table_list.at(0).value.char_value = '#';
	for (int i = 0; i < 1000; i++) {
		constant_table_list.push_back(constant_table_list.at(0));
	}
	//semantics_property.resize(max_production_list_length);
	/*生成四元式的函数*/
	get_semantic_action.insert({ "push_token", &Semantics_Analysis::push_token });
	get_semantic_action.insert({ "handle_token", &Semantics_Analysis::handle_token });
	get_semantic_action.insert({ "handle_equal_symbol", &Semantics_Analysis::handle_equal_symbol });
	get_semantic_action.insert({ "handle_array", &Semantics_Analysis::handle_array });
	get_semantic_action.insert({ "handle_begin_if", &Semantics_Analysis::handle_begin_if });
	get_semantic_action.insert({ "handle_else", &Semantics_Analysis::handle_else });
	get_semantic_action.insert({ "handle_else_if", &Semantics_Analysis::handle_else_if });
	get_semantic_action.insert({ "handle_end_if", &Semantics_Analysis::handle_end_if });
	get_semantic_action.insert({ "handle_judge_end", &Semantics_Analysis::handle_judge_end });
	get_semantic_action.insert({ "handle_begin_while", &Semantics_Analysis::handle_begin_while });
	get_semantic_action.insert({ "handle_do_while", &Semantics_Analysis::handle_do_while });
	get_semantic_action.insert({ "handle_end_while", &Semantics_Analysis::handle_end_while });
	get_semantic_action.insert({ "handle_compare_symbol", &Semantics_Analysis::handle_compare_symbol });
	get_semantic_action.insert({ "handle_begin_for", &Semantics_Analysis::handle_begin_for });
	get_semantic_action.insert({ "handle_judge_for", &Semantics_Analysis::handle_judge_for });
	get_semantic_action.insert({ "handle_do_for", &Semantics_Analysis::handle_do_for });
	get_semantic_action.insert({ "handle_after_do", &Semantics_Analysis::handle_after_do });
	get_semantic_action.insert({ "handle_end_for", &Semantics_Analysis::handle_end_for });
	get_semantic_action.insert({ "handle_auto_change", &Semantics_Analysis::handle_auto_change });
	get_semantic_action.insert({ "handle_break", &Semantics_Analysis::handle_break });
	get_semantic_action.insert({ "handle_continue", &Semantics_Analysis::handle_continue });
	get_semantic_action.insert({ "handle_switch", &Semantics_Analysis::handle_switch });
	get_semantic_action.insert({ "handle_case", &Semantics_Analysis::handle_case });
	get_semantic_action.insert({ "handle_default", &Semantics_Analysis::handle_default });
	get_semantic_action.insert({ "handle_switch_end", &Semantics_Analysis::handle_switch_end });
	get_semantic_action.insert({ "handle_case_end",&Semantics_Analysis::handle_case_end });
	get_semantic_action.insert({ "handle_function_begin", &Semantics_Analysis::handle_function_begin });
	get_semantic_action.insert({ "handle_function_end", &Semantics_Analysis::handle_function_end });
	get_semantic_action.insert({ "handle_program_end", &Semantics_Analysis::handle_program_end });
	get_semantic_action.insert({ "handle_jump_function", &Semantics_Analysis::handle_jump_function });
	get_semantic_action.insert({ "handle_jump_void_function", &Semantics_Analysis::handle_jump_void_function });
	get_semantic_action.insert({ "handle_return", &Semantics_Analysis::handle_return });
	get_semantic_action.insert({ "handle_dian_symbol", &Semantics_Analysis::handle_dian_symbol });
	get_semantic_action.insert({ "handle_before_identifier", &Semantics_Analysis::handle_before_identifier });

	/*填写符号表的函数*/
	get_semantic_action.insert({ "write_symbol_table", &Semantics_Analysis::write_symbol_table });
	get_semantic_action.insert({ "write_struct_table", &Semantics_Analysis::write_struct_table });
	get_semantic_action.insert({ "save_assignment", &Semantics_Analysis::save_assignment });
	get_semantic_action.insert({ "save_array_assignment", &Semantics_Analysis::save_array_assignment });
	get_semantic_action.insert({ "save_array", &Semantics_Analysis::save_array });
	get_semantic_action.insert({ "save_struct", &Semantics_Analysis::save_struct });
	get_semantic_action.insert({ "save_struct_name", &Semantics_Analysis::save_struct_name });
	get_semantic_action.insert({ "save_struct_member", &Semantics_Analysis::save_struct_member });
	get_semantic_action.insert({ "write_function_table", &Semantics_Analysis::write_function_table });
}

Semantics_Analysis::~Semantics_Analysis() {}

void Semantics_Analysis::scanf_information()
{
	fstream fp;
	string str;
	string head;
	vector<string> right;
	vector<string> actionlist;
	fp.open("Grammar_Information.txt");
	if (!fp.is_open())
	{
		cout << "Can't find the file.\n";
		system("pause");
	}
	fp >> str;
	head = str;
	head_symbol = str;
	while (!fp.eof())
	{
		fp >> str;
		production_left.push_back(head);
		if (str == "->")
		{
			fp >> str;
			while (str != "#")
			{
				right.push_back(str);
				fp >> str;
			}
			production_right.push_back(right);
			right.clear();
		}
		fp >> str;
		head = str;
	}
	production_number = production_left.size();
	select_list.resize(production_number + 1);
	fp.close();
	fp.open("Semantics_Information.txt");
	if (!fp.is_open())
	{
		cout << "Can't find the file.\n";
		system("pause");
	}
	while (!fp.eof())
	{
		fp >> str;
		while (str != "#")
		{
			actionlist.push_back(str);
			fp >> str;
		}
		semantics_property.push_back(actionlist);
		actionlist.clear();
	}
	fp.close();
}

void Semantics_Analysis::Generate_Token() //词法分析器
{
	code_file.open("Code_Information.txt");
	if (code_file.is_open())
		cout << "源程序文件:Code_Information.txt" << endl;
	char code_char;
	string temp_token; //每次读入的字符/存放中间token串
	temp_token = "\0";
	int i = 0, state = 0, code_end = 1;
	code_file >> noskipws; //强制读入空格和回车
	while (code_file.peek() != EOF || code_end)
	{
		if (code_file.peek() == EOF && state < 100)
		{
			code_end = 0;
			state = change_state(state, '\n');
			check_code(state, temp_token);
			/*if (!LL1_sentence_analysis(temp_token))
			{
				cout << "error:源代码语法编写错误！";
				system("pause");
				exit(0);
			}*/
			if ((temp_token.substr(0, 2) != "/*") && (temp_token.substr(0, 2) != "//"))
			{
				if (!LL1_sentence_analysis_with_print_step(temp_token))
				{
					system("pause");
					exit(0);
				}
			}
			break;
		}
		code_file >> code_char;																					//读入下一个字符
		if (((code_char <= 'z' && code_char >= 'a') || (code_char <= 'Z' && code_char >= 'A')) && (state != 2)) //字母
			state = change_state(state, 'a');
		else if (code_char <= '9' && code_char >= '0') //数字
			state = change_state(state, '0');
		else
			state = change_state(state, code_char); //根据下一个字符进行状态转换
		if (state > 100)							//终结态
		{
			//cout << state << ' ' << token << endl;
			check_code(state, temp_token); //判断单词
			/*if (!LL1_sentence_analysis(temp_token))
			{
				cout << "error:源代码语法编写错误！";
				system("pause");
				exit(0);
			}*/
			if ((temp_token.substr(0, 2) != "/*") && (temp_token.substr(0, 2) != "//"))
			{
				if (!LL1_sentence_analysis_with_print_step(temp_token))
				{
					system("pause");
					exit(0);
				}
			}
			i = 0;
			state = 0;
			if ((code_char <= 'z' && code_char >= 'a') || (code_char <= 'Z' && code_char >= 'A')) //小写字母
				state = change_state(state, 'a');
			else if (code_char <= '9' && code_char >= '0') //数字
				state = change_state(state, '0');
			else
				state = change_state(state, code_char);
			temp_token = "\0";
			if (code_char != ' ' && code_char != '\n')
				temp_token.push_back(code_char); //将读入字符存入重置后的token串首字符
		}
		else
		{
			if (code_char != ' ' && code_char != '\n') //非终结态，
				temp_token.push_back(code_char);		   //将读入字符存入token串首字符,
				//cout << token << endl;
		}
	}
	token_property last_token;
	last_token.token_word = "#";
	last_token.token_type = "结束符";
	last_token.token_number = 1;
	last_token.token_pointer = NULL;
	token_list.push_back(last_token);
	LL1_sentence_analysis_with_print_step("#");
}

void Semantics_Analysis::check_code(int state, string token) //生成单词
{
	switch (state) //根据已知状态按不同类别查找
	{
	case (101): //关键字或标识符
		if (!search_key(token, &key_word, "关键字"))
			search_key(token, &const_symbol, "标识符");
		break;
	case (102): //字符
		search_key(token, &key_symbol, "关键符号");
		break;
	case (103): //标识符
		search_key(token, &const_symbol, "标识符");
		break;
	case (104): //数字
		search_key(token, &const_number, "数字");
		break;
	case (105): //字符
		search_key(token, &const_char, "字符");
		break;
	case (106): //字符串
		search_key(token, &const_string, "字符串");
		break;
	default:
		break;
	}
}

int Semantics_Analysis::search_key(string one_token, vector<string>* key, string type) //查找并加入token序列
{
	token_property temp_token;
	for (vector<int>::size_type i = 0; i < (*key).size(); i++) //遍历查找表
	{
		if ((*key)[i] == one_token) //匹配
		{
			temp_token.token_word = one_token; //存入token序列
			temp_token.token_type = type;
			temp_token.token_number = i;
			token_list.push_back(temp_token);
			return 1; //返回
		}
	}
	if (type != "关键字" && type != "关键符号") //未匹配，必定是用户定义符号
	{
		(*key).push_back(one_token);	   //加入查找表
		temp_token.token_word = one_token; //存入token序列
		temp_token.token_type = type;
		temp_token.token_number = ((*key).size());
		token_list.push_back(temp_token);
	}
	return 0;
}

void Semantics_Analysis::print_token_list()
{
	{
		cout << "类型\t序号\t单词" << endl;
		for (vector<int>::size_type i = 0; i < token_list.size(); i++)
			cout << token_list[i].token_type << " " << token_list[i].token_number << " " << token_list[i].token_word << endl;
		cout << endl;
	}
}

int Semantics_Analysis::LL1_sentence_analysis(string token) //LL1语法分析
{
	static int first_flag = 1, step_number = 1;
	if (first_flag == 1)
	{
		first_flag = 0;
		grammar_stack.push("#");
		grammar_stack.push(head_symbol);
	}
	while (!grammar_stack.empty()) //栈非空
	{
		if (grammar_stack.top().substr(0, 1) == "$")
		{
			function_pointer semantic_action = (Semantics_Analysis::function_pointer)get_semantic_action.find(grammar_stack.top().substr(1, grammar_stack.top().size() - 1))->second;
			(this->*semantic_action)(temp_semantic_data_stack.top());
			temp_semantic_data_stack.pop();
			grammar_stack.pop();
		}
		else if (get_end_symbol_number(grammar_stack.top()))
		{ //栈顶符号为终结符
			if (grammar_stack.top() == get_next_w_type(token))
			{ //等于当前读入字符
				if (get_next_w_type(token) == "#")
				{
					return 2;
				}
				grammar_stack.pop(); //弹栈
				return 1;
			}
			else //识别不成功
			{
				return 0;
			}
		}
		else if (get_not_end_symbol_number(grammar_stack.top()))
		{ //栈顶符号为非终结符
			if (get_end_symbol_number(get_next_w_type(token)))
			{
				if (LL1_list[get_not_end_symbol_number(grammar_stack.top())][get_end_symbol_number(get_next_w_type(token))] != "empty")
				{
					int i = atoi(LL1_list[get_not_end_symbol_number(grammar_stack.top())][get_end_symbol_number(get_next_w_type(token))].c_str());
					grammar_stack.pop();
					for (int j = production_right[(__int64)i - 1].size() - 1; j >= 0; j--)
					{
						if (semantics_property[(__int64)i - 1].size() != 0 && semantics_property[(__int64)i - 1][j] != "empty")
						{
							grammar_stack.push(semantics_property[(__int64)i - 1][j]);
							temp_semantic_data_stack.push(token);
						}
						grammar_stack.push(production_right[(__int64)i - 1][j]);
					}
				}
				else
				{
					return 0;
				}
			}
			else
			{
				return 0;
			}
		}
	}
	return 0;
}

int Semantics_Analysis::LL1_sentence_analysis_with_print_step(string token) //LL1语法分析
{
	static int first_flag = 1, step_number = 1;
	if (first_flag == 1)
	{
		first_flag = 0;
		cout << "\n步骤\t栈顶\t读入\t操作" << endl;
		grammar_stack.push("#");
		cout << step_number++ << "\t空\t无\t结束符'#'入栈" << endl;
		grammar_stack.push(head_symbol);
		cout << step_number++ << "\t'#'\t无\t起始符'" << head_symbol << "'入栈" << endl;
	}
	while (!grammar_stack.empty()) //栈非空
	{
		if (grammar_stack.top().substr(0, 1) == "$")
		{//语义动作符号
			function_pointer semantic_action = (Semantics_Analysis::function_pointer)get_semantic_action.find(grammar_stack.top().substr(1, grammar_stack.top().size() - 1))->second;
			(this->*semantic_action)(temp_semantic_data_stack.top());
			cout << step_number++ << "\t" << grammar_stack.top() << "\t" << token << "\t'" << grammar_stack.top() << "'出栈" << endl;
			temp_semantic_data_stack.pop();
			grammar_stack.pop();
		}
		else if (get_end_symbol_number(grammar_stack.top()))
		{ //栈顶符号为终结符
			if (grammar_stack.top() == get_next_w_type(token))
			{ //等于当前读入字符
				cout << step_number++ << "\t" << grammar_stack.top() << "\t" << token << "\t'" << token << "'匹配成功√!" << endl;
				if (get_next_w_type(token) == "#")
				{
					cout << "该句子识别成功！\n" << endl;
					return 2;
				}
				grammar_stack.pop(); //弹栈
				return 1;
			}
			else //识别不成功
			{
				cout << step_number++ << "\t" << grammar_stack.top() << "\t" << token << "\t'" << token << "'匹配失败×!" << endl;
				cout << "该句子不能被识别！\n" << endl;
				return 0;
			}
		}
		else if (get_not_end_symbol_number(grammar_stack.top()))
		{ //栈顶符号为非终结符
			if (get_end_symbol_number(get_next_w_type(token)))
			{
				if (LL1_list[get_not_end_symbol_number(grammar_stack.top())][get_end_symbol_number(get_next_w_type(token))] != "empty")
				{
					int i = atoi(LL1_list[get_not_end_symbol_number(grammar_stack.top())][get_end_symbol_number(get_next_w_type(token))].c_str());
					cout << step_number++ << "\t" << grammar_stack.top() << "\t" << token << "\t'" << grammar_stack.top() << "'出栈" << endl;
					grammar_stack.pop();
					for (int j = production_right[(__int64)i - 1].size() - 1; j >= 0; j--)
					{
						if (semantics_property[(__int64)i - 1].size() != 0 && semantics_property[(__int64)i - 1][j] != "empty")
						{
							cout << "\t" << grammar_stack.top() << "\t" << token << "\t'" << semantics_property[(__int64)i - 1][j] + " " + token << "'入栈" << endl;
							grammar_stack.push(semantics_property[(__int64)i - 1][j]);
							temp_semantic_data_stack.push(token);
						}
						cout << "\t" << grammar_stack.top() << "\t" << token << "\t'" << production_right[(__int64)i - 1][j] << "'入栈" << endl;
						grammar_stack.push(production_right[(__int64)i - 1][j]);
					}
				}
				else
				{
					cout << step_number++ << "\t" << grammar_stack.top() << "\t" << token << "\t'" << token << "'匹配失败×!" << endl;
					cout << "该句子不能被识别！\n" << endl;
					return 0;
				}
			}
			else
			{
				cout << step_number++ << "\t" << grammar_stack.top() << "\t" << token << "\t'" << token << "'匹配失败×!" << endl;
				cout << "该句子不能被识别！\n" << endl;
				return 0;
			}
		}
	}
	return 0;
}

void Semantics_Analysis::add_production(string operate)
{
	four_item_production temp_item;
	temp_item.operation_symbol = operate;
	temp_item.first_target.operation_target_value = "_";
	temp_item.second_target.operation_target_value = "_";
	temp_item.result_target.result_target_value = "_";
	temp_item.first_target.active_information = false;
	temp_item.second_target.active_information = false;
	temp_item.result_target.active_information = false;
	four_item_production_list.push_back(temp_item);
}

void Semantics_Analysis::add_production(string operate, string first)//函数重载
{
	four_item_production temp_item;
	temp_item.operation_symbol = operate;
	temp_item.first_target.operation_target_value = first;
	temp_item.second_target.operation_target_value = "_";
	temp_item.result_target.result_target_value = "_";
	temp_item.first_target.active_information = false;
	temp_item.second_target.active_information = false;
	temp_item.result_target.active_information = false;
	four_item_production_list.push_back(temp_item);
}

void Semantics_Analysis::push_token(string s)
{
	semantic_data_stack.push(s);
}

void Semantics_Analysis::handle_token(string s)
{
	string operation_symbol = s;
	string second_symbol = semantic_data_stack.top();
	semantic_data_stack.pop();
	string first_symbol = semantic_data_stack.top();
	semantic_data_stack.pop();
	four_item_production temp;
	temp.operation_symbol = operation_symbol;
	temp.first_target.operation_target_value = first_symbol;
	temp.second_target.operation_target_value = second_symbol;
	temp.result_target.result_target_value = to_string(temp_t_number) + "t";
	semantic_data_stack.push(to_string(temp_t_number) + "t");
	four_item_production_list.push_back(temp);
	temp_t_number++;
}

void Semantics_Analysis::handle_equal_symbol(string s)
{
	string operation_symbol = "=";
	string old_symbol = semantic_data_stack.top();
	semantic_data_stack.pop();
	string result_symbol = semantic_data_stack.top();
	semantic_data_stack.pop();
	four_item_production temp;
	temp.operation_symbol = operation_symbol;
	temp.first_target.operation_target_value = old_symbol;
	temp.second_target.operation_target_value = "_";
	temp.result_target.result_target_value = result_symbol;
	four_item_production_list.push_back(temp);
}

void  Semantics_Analysis::handle_begin_if(string s)
{
	string result = semantic_data_stack.top();
	semantic_data_stack.pop();
	add_production("ifJudge", result);
	add_production("ifBeginDo");
}

void Semantics_Analysis::handle_else_if(string s)
{
	string result = semantic_data_stack.top();
	semantic_data_stack.pop();
	add_production("ifEndJudge", result);
	add_production("ifBeginDo");
}

void Semantics_Analysis::handle_else(string s)
{
	add_production("ifToEnd");
	add_production("else");
}

void Semantics_Analysis::handle_end_if(string s)
{
	add_production("endIf");
	add_production("beginDo");
}

void Semantics_Analysis::handle_judge_end(string s)
{
	four_item_production_list.pop_back();
	four_item_production_list.pop_back();
	add_production("ifToEnd");
	add_production("ifBeginJudge");
}

void Semantics_Analysis::handle_begin_while(string s)
{
	add_production("endDo");
	add_production("beginWhile");
}

void Semantics_Analysis::handle_do_while(string s)
{
	string result = semantic_data_stack.top();
	semantic_data_stack.pop();
	add_production("whileDoIf", result);
	add_production("whileBeginDo");
}

void Semantics_Analysis::handle_end_while(string s)
{
	add_production("endWhile");
	add_production("beginDo");
}

void Semantics_Analysis::handle_compare_symbol(string s)
{
	string second_symbol = semantic_data_stack.top();
	semantic_data_stack.pop();
	string operation_symbol = semantic_data_stack.top();
	semantic_data_stack.pop();
	string first_symbol = semantic_data_stack.top();
	semantic_data_stack.pop();
	four_item_production temp;
	temp.operation_symbol = operation_symbol;
	temp.first_target.operation_target_value = first_symbol;
	temp.second_target.operation_target_value = second_symbol;
	temp.result_target.result_target_value = to_string(temp_t_number) + "t";
	semantic_data_stack.push(to_string(temp_t_number) + "t");
	four_item_production_list.push_back(temp);
	temp_t_number++;
}

void Semantics_Analysis::handle_array(string s)
{
	string operation_symbol = "array";
	string number = semantic_data_stack.top();
	semantic_data_stack.pop();
	string array_symbol = semantic_data_stack.top();
	semantic_data_stack.pop();
	four_item_production temp;
	temp.operation_symbol = operation_symbol;
	temp.first_target.operation_target_value = array_symbol;
	temp.second_target.operation_target_value = number;
	temp.result_target.result_target_value = to_string(temp_t_number) + "t";
	semantic_data_stack.push(to_string(temp_t_number) + "t");
	four_item_production_list.push_back(temp);
	temp_t_number++;
}

void Semantics_Analysis::handle_begin_for(string s)  //for操作开始
{
	add_production("endDo");
	add_production("forBegin");
}

void Semantics_Analysis::handle_judge_for(string s)   //for判断部分
{
	add_production("forBeginEnd");
	add_production("forBeginJudge");
}

void Semantics_Analysis::handle_do_for(string s)   //for循环体部分
{
	add_production("afterForDoEnd");
	add_production("forDo");
}

void Semantics_Analysis::handle_after_do(string s)   //循环体结束后操作部分
{
	string result = semantic_data_stack.top();
	semantic_data_stack.pop();
	add_production("forDoIf", result);   //判断结果是否正确跳转的四元式
	add_production("afterForDo");      //循环体结束后执行的操作的四元式
}

void Semantics_Analysis::handle_end_for(string s)   //for循环结束
{
	add_production("endFor");
	add_production("beginDo");
}

void Semantics_Analysis::handle_auto_change(string s)   //自增或自减操作
{
	four_item_production temp;
	string second_symbol = semantic_data_stack.top();
	semantic_data_stack.pop();
	string first_symbol = semantic_data_stack.top();
	semantic_data_stack.pop();
	if (second_symbol == "++" || second_symbol == "--")    //++(--)后置四元式
	{
		temp.operation_symbol = "behind" + second_symbol;
		temp.first_target.operation_target_value = first_symbol;
		temp.second_target.operation_target_value = "_";
		temp.result_target.result_target_value = first_symbol;
	}
	else                                                   //++(--)前置四元式
	{
		temp.operation_symbol = "front" + first_symbol;
		temp.first_target.operation_target_value = second_symbol;
		temp.second_target.operation_target_value = "_";
		temp.result_target.result_target_value = second_symbol;
	}
	four_item_production_list.push_back(temp);
}

void Semantics_Analysis::handle_break(string s) // 四元式
{
	add_production("break");
}

void Semantics_Analysis::handle_continue(string s)    //continue四元式
{
	add_production("continue");
}

void Semantics_Analysis::handle_switch(string s)     //switch四元式
{
	string handle_symbol = semantic_data_stack.top();
	add_production("switch", handle_symbol);
}
void Semantics_Analysis::handle_case(string s)        //case四元式
{
	add_production("caseBeginJudge");
	string compare_symbol = semantic_data_stack.top();   //case后比较的字符
	semantic_data_stack.pop();
	string handle_symbol = semantic_data_stack.top();    //switch后比较的字符
	four_item_production temp;
	temp.operation_symbol = "==";
	temp.first_target.operation_target_value = handle_symbol;
	temp.second_target.operation_target_value = compare_symbol;
	temp.result_target.result_target_value = to_string(four_item_production_list.size()) + "t";
	four_item_production_list.push_back(temp);
	add_production("case", temp.result_target.result_target_value);
	add_production("caseBegin");
}

void Semantics_Analysis::handle_case_end(string s)
{
	add_production("caseEnd");
}

void Semantics_Analysis::handle_default(string s)    //default四元式
{
	add_production("default");
	semantic_data_stack.pop();
}

void Semantics_Analysis::handle_switch_end(string s)    //switch结束四元式
{
	add_production("endSwitch");
	add_production("beginDo");
}

void Semantics_Analysis::write_symbol_table(string s) {
	symbol_table ls_symbol_table;//临时符号表
	queue<string> fuzhu;
	while (semantic_data_stack.top() != "int" && semantic_data_stack.top() != "char" && semantic_data_stack.top() != "float" && semantic_data_stack.top() != "bool" && semantic_data_stack.top() != "double" && semantic_data_stack.top() != "string") {
		fuzhu.push(semantic_data_stack.top());
		semantic_data_stack.pop();
	}
	string leixing = semantic_data_stack.top();
	semantic_data_stack.pop();
	fuzhu.push("#");
	while (fuzhu.front() != "#") {
		if (fuzhu.front() == "数组") {
			symbol_property ls_symbol_property;//临时标识符属性表
			fuzhu.pop();//“数组”出栈
			string ls_array_size = fuzhu.front();
			fuzhu.pop();//数组长度出栈赋值给ls_array_size
			ls_symbol_property.name = fuzhu.front();
			fuzhu.pop();//数组标识符出栈赋值给临时标识符
			if (duplicate_check(ls_symbol_property.name) != -1) {//查重
				cout << "error:标识符已声明" << endl;
				system("pause");
				exit(0);
			}
			int ls_an;//记录此条数组表位置
			for (ls_an = 0; ls_an < 50; ls_an++) {
				if (array_table_list.at(ls_an).bottom_number == -1)
					break;
			}
			array_table_list.at(ls_an).top_number = atoi(ls_array_size.c_str()) - 1;//数组上界填入临时表
			array_table_list.at(ls_an).bottom_number = 0;//数组下界填入临时表
			int ls_i;//记录数组表指针指向类型表位置
			for (ls_i = 0; ls_i < type_property_list.size(); ls_i++) {
				if (type_property_list.at(ls_i).type_code == leixing)
				{
					type_property* p = &type_property_list.at(ls_i);
					array_table_list.at(ls_an).type_information = p;//将数组类型填入临时表
					break;
				}
			}
			switch (ls_i) {//数组size填入临时表
			case 0://int型
				array_table_list.at(ls_an).array_size = 4;
				break;
			case 1://float
				array_table_list.at(ls_an).array_size = 4;
				break;
			case 2://char
				array_table_list.at(ls_an).array_size = 1;
				break;
			case 3://bool
				array_table_list.at(ls_an).array_size = 1;
				break;
			case 4://double
				array_table_list.at(ls_an).array_size = 8;
				break;
			default://array
				type_property* p1 = (type_property*)type_property_list.at(ls_i).information_pointer;
				array_table* p2 = (array_table*)p1->information_pointer;
				array_table_list.at(ls_an).array_size = p2->array_size * (p2->top_number + 1);
				break;
			}
			int ls_n;//记录类型表新数组位置
			for (ls_n = 0; ls_n < 50; ls_n++) {
				if (type_property_list.at(ls_n).type_code == "未声明类型") {
					break;
				}
			}
			string ss = "array" + to_string(ls_an + 1);
			type_property_list.at(ls_n).type_code = ss;
			type_property_list.at(ls_n).information_pointer = &array_table_list.at(ls_an);//类型表指针指向数组表
			ls_symbol_property.kind = "变量";
			int ls_con_num;
			for (ls_con_num = 0; ls_con_num < 1000; ls_con_num++) {
				if (constant_table_list.at(ls_con_num).value.char_value == '#')
					break;
			}
			constant_table_list.at(ls_con_num).value.int_value = array_table_list.at(ls_an).array_size * (stoi(ls_array_size.c_str()));//数组长度存入常量表
			ls_symbol_property.offset = symbol_table_list.at(jilu_symbol_num).total_size;
			symbol_table_list.at(jilu_symbol_num).total_size = constant_table_list.at(ls_con_num).value.int_value + ls_symbol_property.offset;
			ls_symbol_property.address = &constant_table_list.at(ls_con_num);
			ls_symbol_property.type = &type_property_list.at(ls_n);
			symbol_table_list.at(jilu_symbol_num).symbol_property_table.push_back(ls_symbol_property);//将符号属性表加入符号表
		}
		else if (fuzhu.front() == "赋值") {
			symbol_property ls_symbol_property;//临时标识符属性表
			fuzhu.pop();
			if (fuzhu.front() == "数组赋值") {
				cout << "error:无法进行数组声明赋值" << endl;
				system("pause");
				exit(0);
			}
			else {
				string ls_s = fuzhu.front();//暂存数值
				fuzhu.pop();
				ls_symbol_property.name = fuzhu.front();//标识符存入临时属性表 
				fuzhu.pop();
				if (duplicate_check(ls_symbol_property.name) != -1) {//查重
					cout << "erroe:标识符已声明" << endl;
					system("pause");
					exit(0);
				}
				if (duplicate_check(ls_s) == -1) {//赋值常数
					int ls_con_num;
					for (ls_con_num = 0; ls_con_num < 1000; ls_con_num++) {
						if (constant_table_list.at(ls_con_num).value.char_value == '#')
							break;
					}
					if (leixing == "int") {
						constant_table_list.at(ls_con_num).value.int_value = stoi(ls_s.c_str());
						ls_symbol_property.offset = symbol_table_list.at(jilu_symbol_num).total_size;
						symbol_table_list.at(jilu_symbol_num).total_size = 4 + ls_symbol_property.offset;
						ls_symbol_property.address = &constant_table_list.at(ls_con_num);//address指针指向对应位置
					}
					else if (leixing == "float") {
						constant_table_list.at(ls_con_num).value.float_value = stof(ls_s.c_str());
						ls_symbol_property.offset = symbol_table_list.at(jilu_symbol_num).total_size;
						symbol_table_list.at(jilu_symbol_num).total_size = 4 + ls_symbol_property.offset;
						ls_symbol_property.address = &constant_table_list.at(ls_con_num);//address指针指向对应位置
					}
					else if (leixing == "char") {
						constant_table_list.at(ls_con_num).value.char_value = ls_s.at(1);
						ls_symbol_property.offset = symbol_table_list.at(jilu_symbol_num).total_size;
						symbol_table_list.at(jilu_symbol_num).total_size = 1 + ls_symbol_property.offset;
						ls_symbol_property.address = &constant_table_list.at(ls_con_num);//address指针指向对应位置
					}
					else if (leixing == "bool") {
						if (ls_s == "true")
							constant_table_list.at(ls_con_num).value.bool_value = true;
						else if (ls_s == "false")
							constant_table_list.at(ls_con_num).value.bool_value = false;
						ls_symbol_property.offset = symbol_table_list.at(jilu_symbol_num).total_size;
						symbol_table_list.at(jilu_symbol_num).total_size = 1 + ls_symbol_property.offset;
						ls_symbol_property.address = &constant_table_list.at(ls_con_num);//address指针指向对应位置
					}
					else if (leixing == "double") {
						constant_table_list.at(ls_con_num).value.double_value = stof(ls_s.c_str());
						ls_symbol_property.offset = symbol_table_list.at(jilu_symbol_num).total_size;
						symbol_table_list.at(jilu_symbol_num).total_size = 8 + ls_symbol_property.offset;
						ls_symbol_property.address = &constant_table_list.at(ls_con_num);//address指针指向对应位置
					}
					/*else if (leixing == "string") {
					constant_table_list.at(ls_con_num).value.string_value = (char*)ls_s.data();
					ls_symbol_property.offset = symbol_table_list.at(0).total_size;
					symbol_table_list.at(0).total_size = ls_s.size() + ls_symbol_property.offset;
					ls_symbol_property.address = &constant_table_list.at(ls_con_num);//address指针指向对应位置
					}*/
				}
				else if (symbol_table_list.at(jilu_symbol_num).symbol_property_table.at(duplicate_check(ls_s)).type->type_code == leixing) {//用标识符赋值
					int ls_con_num;
					for (ls_con_num = 0; ls_con_num < 1000; ls_con_num++) {
						if (constant_table_list.at(ls_con_num).value.char_value == '#')
							break;
					}
					constant_table* pointer = (constant_table*)symbol_table_list.at(jilu_symbol_num).symbol_property_table.at(duplicate_check(ls_s)).address;
					if (pointer == nullptr) {
						cout << "error:变量未赋值" << endl;
						system("pause");
						exit(0);
					}
					if (leixing == "int") {
						constant_table_list.at(ls_con_num).value.int_value = pointer->value.int_value;
						ls_symbol_property.offset = symbol_table_list.at(jilu_symbol_num).total_size;
						symbol_table_list.at(jilu_symbol_num).total_size = 4 + ls_symbol_property.offset;
						ls_symbol_property.address = &constant_table_list.at(ls_con_num);//address指针指向对应位置
					}
					else if (leixing == "float") {
						constant_table_list.at(ls_con_num).value.float_value = pointer->value.float_value;
						ls_symbol_property.offset = symbol_table_list.at(jilu_symbol_num).total_size;
						symbol_table_list.at(jilu_symbol_num).total_size = 4 + ls_symbol_property.offset;
						ls_symbol_property.address = &constant_table_list.at(ls_con_num);//address指针指向对应位置
					}
					else if (leixing == "char") {
						constant_table_list.at(ls_con_num).value.char_value = pointer->value.char_value;
						ls_symbol_property.offset = symbol_table_list.at(jilu_symbol_num).total_size;
						symbol_table_list.at(jilu_symbol_num).total_size = 1 + ls_symbol_property.offset;
						ls_symbol_property.address = &constant_table_list.at(ls_con_num);//address指针指向对应位置
					}
					else if (leixing == "bool") {
						constant_table_list.at(ls_con_num).value.bool_value = pointer->value.bool_value;
						ls_symbol_property.offset = symbol_table_list.at(jilu_symbol_num).total_size;
						symbol_table_list.at(jilu_symbol_num).total_size = 1 + ls_symbol_property.offset;
						ls_symbol_property.address = &constant_table_list.at(ls_con_num);//address指针指向对应位置
					}
					else if (leixing == "double") {
						constant_table_list.at(ls_con_num).value.double_value = pointer->value.double_value;
						ls_symbol_property.offset = symbol_table_list.at(jilu_symbol_num).total_size;
						symbol_table_list.at(jilu_symbol_num).total_size = 8 + ls_symbol_property.offset;
						ls_symbol_property.address = &constant_table_list.at(ls_con_num);//address指针指向对应位置
					}
					/*else if (leixing == "string") {
					constant_table_list.at(ls_con_num).value.string_value = pointer->value.string_value;
					ls_symbol_property.offset = symbol_table_list.at(0).total_size;
					symbol_table_list.at(0).total_size = pointer->value.string_value.size() + ls_symbol_property.offset;
					ls_symbol_property.address = &constant_table_list.at(ls_con_num);//address指针指向对应位置
					}*/
				}
				else {
					cout << "error:赋值类型出错" << endl;
					cout << symbol_table_list.at(jilu_symbol_num).symbol_property_table.at(duplicate_check(ls_s)).type->type_code << endl;
					cout << leixing << endl;
					system("pause");
					exit(0);

				}
				for (int i = 0; i < 50; i++) {
					if (type_property_list.at(i).type_code == leixing)
					{
						ls_symbol_property.type = &type_property_list.at(i);//临时表类型指针指向类型表

						break;
					}
				}
				ls_symbol_property.kind = "变量";
				symbol_table_list.at(jilu_symbol_num).symbol_property_table.push_back(ls_symbol_property);//将符号属性表加入符号表
			}
		}
		else {
			symbol_property ls_symbol_property;//临时标识符属性表
			ls_symbol_property.name = fuzhu.front();//将标识符存入临时符号表
			fuzhu.pop();
			if (duplicate_check(ls_symbol_property.name) != -1)//查重
			{
				cout << "error:标识符已声明" << endl;
				system("pause");
				exit(0);
			}
			for (int i = 0; i < type_property_list.size(); i++) {
				if (type_property_list[i].type_code == leixing)
				{
					ls_symbol_property.type = &type_property_list[i];//临时表类型指针指向类型表
					switch (i)
					{
					case 0:
						ls_symbol_property.offset = symbol_table_list.at(jilu_symbol_num).total_size;
						symbol_table_list.at(jilu_symbol_num).total_size = 4 + ls_symbol_property.offset;
						break;
					case 1:
						ls_symbol_property.offset = symbol_table_list.at(jilu_symbol_num).total_size;
						symbol_table_list.at(jilu_symbol_num).total_size = 4 + ls_symbol_property.offset;
						break;
					case 2:
						ls_symbol_property.offset = symbol_table_list.at(jilu_symbol_num).total_size;
						symbol_table_list.at(jilu_symbol_num).total_size = 1 + ls_symbol_property.offset;
						break;
					case 3:
						ls_symbol_property.offset = symbol_table_list.at(jilu_symbol_num).total_size;
						symbol_table_list.at(jilu_symbol_num).total_size = 1 + ls_symbol_property.offset;
						break;
					case 4:
						ls_symbol_property.offset = symbol_table_list.at(jilu_symbol_num).total_size;
						symbol_table_list.at(jilu_symbol_num).total_size = 8 + ls_symbol_property.offset;
						break;
					default:
						break;
					}
					break;
				}
			}
			ls_symbol_property.kind = "变量";
			ls_symbol_property.address = NULL;
			symbol_table_list.at(jilu_symbol_num).symbol_property_table.push_back(ls_symbol_property);//将符号属性表加入符号表
		}
	}
	cout << "符号表总长度" << symbol_table_list.at(jilu_symbol_num).total_size << endl;
	cout << "标识符" << "\t" << "类型" << "\t" << "种类" << "\t" << "偏移量" << "\t" << "常数表地址" << endl;
	for (int i = 0; i < symbol_table_list.at(jilu_symbol_num).symbol_property_table.size(); i++) {
		cout << symbol_table_list.at(jilu_symbol_num).symbol_property_table.at(i).name << "\t" << symbol_table_list.at(jilu_symbol_num).symbol_property_table.at(i).type->type_code << "\t";
		cout << symbol_table_list.at(jilu_symbol_num).symbol_property_table.at(i).kind << "\t" << symbol_table_list.at(jilu_symbol_num).symbol_property_table.at(i).offset << "\t";
		cout << symbol_table_list.at(jilu_symbol_num).symbol_property_table.at(i).address << endl;
		cout << endl;
	}
	cout << "类型表" << endl;
	for (int i = 0; type_property_list.at(i).type_code != "未声明类型"; i++) {
		cout << type_property_list.at(i).type_code << endl;
	}
	cout << endl;
	cout << "数组表" << endl;
	cout << "下界" << "\t" << "上界" << "\t" << "类型" << "\t" << "元素长度" << endl;
	for (int i = 0; array_table_list.at(i).bottom_number != -1; i++) {
		cout << array_table_list.at(i).bottom_number << "\t" << array_table_list.at(i).top_number << "\t" << array_table_list.at(i).type_information->type_code << "\t" << array_table_list.at(i).array_size << endl;
	}
	cout << endl;
}

void Semantics_Analysis::write_struct_table(string s) {
	symbol_property ls_symbol_table;//临时符号表
	vector<structure_property> ls_struct_list;
	stack<string> fuzhu;
	fuzhu.push("#");
	while (semantic_data_stack.top() != "struct") {
		fuzhu.push(semantic_data_stack.top());
		semantic_data_stack.pop();
	}
	string leixing = fuzhu.top();
	semantic_data_stack.pop();
	fuzhu.pop();
	int ls_struct_list_num;//记录结构表位置
	int ls_struct_num = 0;//记录变量数量
	while (fuzhu.top() != "#") {
		if (fuzhu.top() == "结构名") {//有结构体类型的结构体声明
			fuzhu.pop();//结构名出栈
			int ls_type_num;
			for (ls_type_num = 0; ls_type_num < 50; ls_type_num++) {
				if (type_property_list.at(ls_type_num).type_code == "未声明类型") {
					type_property_list.at(ls_type_num).type_code = leixing;
					break;
				}
			}
			for (ls_struct_list_num = 0; ls_struct_list_num < 20; ls_struct_list_num++) {
				if (structure_table_list.at(ls_struct_list_num).num == -1) {
					break;
				}
			}
			while (fuzhu.top() != "结构体成员") {
				string leixing1 = fuzhu.top();
				fuzhu.pop();
				structure_property ls_structure_property;
				for (int i = 0; i < structure_table_list.at(ls_struct_list_num).structure_property_list.size(); i++)//查重
				{
					if (structure_table_list.at(ls_struct_list_num).structure_property_list.at(i).ID == fuzhu.top()) {
						cout << "error:结构变量已声明" << endl;
						system("pause");
						exit(0);
					}
				}
				ls_structure_property.ID = fuzhu.top();//存入ID
				fuzhu.pop();
				string ls_s = fuzhu.top();//暂存
				fuzhu.pop();
				int ls_con_num;
				for (ls_con_num = 0; ls_con_num < 1000; ls_con_num++) {
					if (constant_table_list.at(ls_con_num).value.char_value == '#')
						break;
				}
				if (fuzhu.top() == "赋值") {
					fuzhu.pop();
					if (leixing1 == "int") {
						constant_table_list.at(ls_con_num).value.int_value = stoi(ls_s.c_str());
						ls_structure_property.OFF = structure_table_list.at(ls_struct_list_num).total_OFF;
						structure_table_list.at(ls_struct_list_num).total_OFF = 4 + ls_structure_property.OFF;
						ls_structure_property.vp = &constant_table_list.at(ls_con_num);//vp指针指向对应位置
					}
					else if (leixing1 == "float") {
						constant_table_list.at(ls_con_num).value.float_value = stof(ls_s.c_str());
						ls_structure_property.OFF = structure_table_list.at(ls_struct_list_num).total_OFF;
						structure_table_list.at(ls_struct_list_num).total_OFF = 4 + ls_structure_property.OFF;
						ls_structure_property.vp = &constant_table_list.at(ls_con_num);//vp指针指向对应位置
					}
					else if (leixing1 == "char") {
						constant_table_list.at(ls_con_num).value.char_value = ls_s.at(1);
						ls_structure_property.OFF = structure_table_list.at(ls_struct_list_num).total_OFF;
						structure_table_list.at(ls_struct_list_num).total_OFF = 1 + ls_structure_property.OFF;
						ls_structure_property.vp = &constant_table_list.at(ls_con_num);//vp指针指向对应位置
					}
					else if (leixing1 == "bool") {
						if (ls_s == "true")
							constant_table_list.at(ls_con_num).value.bool_value = true;
						else if (ls_s == "false")
							constant_table_list.at(ls_con_num).value.bool_value = false;
						ls_structure_property.OFF = structure_table_list.at(ls_struct_list_num).total_OFF;
						structure_table_list.at(ls_struct_list_num).total_OFF = 1 + ls_structure_property.OFF;
						ls_structure_property.vp = &constant_table_list.at(ls_con_num);//vp指针指向对应位置
					}
					else if (leixing1 == "double") {
						constant_table_list.at(ls_con_num).value.double_value = stof(ls_s.c_str());
						ls_structure_property.OFF = structure_table_list.at(ls_struct_list_num).total_OFF;
						structure_table_list.at(ls_struct_list_num).total_OFF = 8 + ls_structure_property.OFF;
						ls_structure_property.vp = &constant_table_list.at(ls_con_num);//vp指针指向对应位置
					}
					/*else if (leixing1 == "string") {
					constant_table_list.at(ls_con_num).value.string_value = ls_s;
					ls_structure_property.OFF = structure_table_list.at(ls_struct_list_num).total_OFF;
					structure_table_list.at(ls_struct_list_num).total_OFF = 8 + ls_structure_property.OFF;
					ls_structure_property.vp = &constant_table_list.at(ls_con_num);//vp指针指向对应位置
					}*/
					for (int i = 0; i < type_property_list.size(); i++) {
						if (type_property_list[i].type_code == leixing1) {
							ls_structure_property.tp = &type_property_list.at(i);//指向类型表
						}
					}
					ls_struct_list.push_back(ls_structure_property);//结构变量属性表存入临时结构表
					ls_struct_num++;
					structure_table_list.at(ls_struct_list_num).num = ls_struct_num;
					ls_s = fuzhu.top();//暂存
					fuzhu.pop();
				}
				else if (fuzhu.top() == "数组") {
					fuzhu.pop();
					int ls_an;//记录此条数组表位置
					for (ls_an = 0; ls_an < 50; ls_an++) {
						if (array_table_list.at(ls_an).bottom_number == -1)
							break;
					}
					array_table_list.at(ls_an).top_number = atoi(ls_s.c_str()) - 1;//数组上界填入临时表
					array_table_list.at(ls_an).bottom_number = 0;//数组下界填入临时表
					int ls_i;//记录数组表指针指向类型表位置
					for (ls_i = 0; ls_i < type_property_list.size(); ls_i++) {
						if (type_property_list.at(ls_i).type_code == leixing1)
						{
							array_table_list.at(ls_an).type_information = &type_property_list.at(ls_i);//将数组类型填入临时表
							cout << "数组指针指向类型表1" << array_table_list.at(ls_an).type_information->type_code << endl;
							break;
						}
					}
					switch (ls_i) {//数组size填入临时表
					case 0://int型
						array_table_list.at(ls_an).array_size = 4;
						break;
					case 1://float
						array_table_list.at(ls_an).array_size = 4;
						break;
					case 2://char
						array_table_list.at(ls_an).array_size = 1;
						break;
					case 3://bool
						array_table_list.at(ls_an).array_size = 1;
						break;
					case 4://double
						array_table_list.at(ls_an).array_size = 8;
						break;
					default://array
						type_property* p1 = (type_property*)type_property_list.at(ls_i).information_pointer;
						array_table* p2 = (array_table*)p1->information_pointer;
						array_table_list.at(ls_an).array_size = p2->array_size * (p2->top_number + 1);
						break;
					}
					int ls_n;//记录类型表新数组位置
					for (ls_n = 0; ls_n < 50; ls_n++) {
						if (type_property_list.at(ls_n).type_code == "未声明类型") {
							break;
						}
					}
					string ss = "array" + to_string(ls_an + 1);
					type_property_list.at(ls_n).type_code = ss;
					type_property_list.at(ls_n).information_pointer = &array_table_list.at(ls_an);//类型表指针指向数组表
					ls_structure_property.tp = &type_property_list.at(ls_i);//结构体数组变量指向类型表
					ls_structure_property.vp = NULL;
					ls_structure_property.OFF = structure_table_list.at(ls_struct_list_num).total_OFF;
					structure_table_list.at(ls_struct_list_num).total_OFF = array_table_list.at(ls_an).array_size * stoi(ls_s.c_str()) + ls_structure_property.OFF;
					ls_struct_list.push_back(ls_structure_property);//结构变量属性表存入临时结构表
					cout << "szie" << ls_struct_list.size() << endl;
					ls_struct_num++;
					structure_table_list.at(ls_struct_list_num).num = ls_struct_num;
					ls_s = fuzhu.top();//暂存
					fuzhu.pop();
				}
				else {
					if (leixing1 == "int") {
						ls_structure_property.OFF = structure_table_list.at(ls_struct_list_num).total_OFF;
						structure_table_list.at(ls_struct_list_num).total_OFF = 4 + ls_structure_property.OFF;
					}
					else if (leixing1 == "float") {
						ls_structure_property.OFF = structure_table_list.at(ls_struct_list_num).total_OFF;
						structure_table_list.at(ls_struct_list_num).total_OFF = 4 + ls_structure_property.OFF;
					}
					else if (leixing1 == "char") {
						ls_structure_property.OFF = structure_table_list.at(ls_struct_list_num).total_OFF;
						structure_table_list.at(ls_struct_list_num).total_OFF = 1 + ls_structure_property.OFF;
					}
					else if (leixing1 == "bool") {
						ls_structure_property.OFF = structure_table_list.at(ls_struct_list_num).total_OFF;
						structure_table_list.at(ls_struct_list_num).total_OFF = 1 + ls_structure_property.OFF;
					}
					else if (leixing1 == "double") {
						ls_structure_property.OFF = structure_table_list.at(ls_struct_list_num).total_OFF;
						structure_table_list.at(ls_struct_list_num).total_OFF = 8 + ls_structure_property.OFF;
					}
					/*else if (leixing1 == "string") {
					ls_structure_property.OFF = structure_table_list.at(ls_struct_list_num).total_OFF;
					structure_table_list.at(ls_struct_list_num).total_OFF = 8 + ls_structure_property.OFF;
					}*/
					for (int i = 0; i < type_property_list.size(); i++) {
						if (type_property_list[i].type_code == leixing1) {
							ls_structure_property.tp = &type_property_list.at(i);//指向类型表
						}
					}
					ls_structure_property.vp = NULL;
					ls_struct_list.push_back(ls_structure_property);//结构变量属性表存入临时结构表
					ls_struct_num++;
					structure_table_list.at(ls_struct_list_num).num = ls_struct_num;
				}
				fuzhu.push(ls_s);//退回暂存数据
			}
			structure_table_list.at(ls_struct_list_num).structure_property_list.resize(ls_struct_list.size());
			structure_table_list.at(ls_struct_list_num).structure_property_list = ls_struct_list;
			type_property_list.at(ls_type_num).information_pointer = &structure_table_list.at(ls_struct_list_num);
			fuzhu.pop();//“结构体成员”出栈
			while (fuzhu.top() != "结构体赋值") {
				if (duplicate_check(ls_symbol_table.name) != -1) {//查重
					cout << "error:标识符已声明" << endl;
					system("pause");
					exit(0);
				}
				ls_symbol_table.name = fuzhu.top();//结构体标识符进栈
				fuzhu.pop();
				string ls_s = fuzhu.top();//暂存
				fuzhu.pop();
				if (fuzhu.top() == "数组") {
					fuzhu.pop();
					if (duplicate_check(ls_symbol_table.name) != -1) {//查重
						cout << "error:标识符已声明" << endl;
						system("pause");
						exit(0);
					}
					int ls_an;//记录此条数组表位置
					for (ls_an = 0; ls_an < 50; ls_an++) {
						if (array_table_list.at(ls_an).bottom_number == -1)
							break;
					}
					array_table_list.at(ls_an).top_number = atoi(ls_s.c_str()) - 1;//数组上界填入临时表
					array_table_list.at(ls_an).bottom_number = 0;//数组下界填入临时表
					int ls_i;//记录数组表指针指向类型表位置
					for (ls_i = 0; ls_i < type_property_list.size(); ls_i++) {
						if (type_property_list.at(ls_i).type_code == leixing)
						{
							type_property* p = &type_property_list.at(ls_i);
							array_table_list.at(ls_an).type_information = p;//将数组类型填入临时表
							cout << "数组指针指向类型表" << array_table_list.at(ls_an).type_information->type_code << endl;
							break;
						}
					}
					array_table_list.at(ls_an).array_size = structure_table_list.at(ls_struct_list_num).total_OFF;//数组类型长度
					int ls_n;//记录类型表新数组位置
					for (ls_n = 0; ls_n < 50; ls_n++) {
						if (type_property_list.at(ls_n).type_code == "未声明类型") {
							break;
						}
					}
					string ss = "array" + to_string(ls_an + 1);
					type_property_list.at(ls_n).type_code = ss;
					type_property_list.at(ls_n).information_pointer = &array_table_list.at(ls_an);//类型表指针指向数组表
					ls_symbol_table.type = &type_property_list.at(ls_n);//结构体数组变量指向数组类型
					ls_symbol_table.kind = "结构体数组变量";
					ls_symbol_table.offset = array_table_list.at(ls_an).array_size * stoi(ls_s.c_str());
					ls_symbol_table.address = NULL;
					symbol_table_list.at(jilu_symbol_num).symbol_property_table.push_back(ls_symbol_table);//将符号属性表加入符号表
				}
				else {
					for (int i = 0; i < type_property_list.size(); i++) {
						if (type_property_list.at(i).type_code == leixing) {
							ls_symbol_table.type = &type_property_list.at(i);//结构题标识符指向结构体类型
							break;
						}
					}
					ls_symbol_table.kind = "结构体变量";
					ls_symbol_table.offset = structure_table_list.at(ls_struct_list_num).total_OFF;
					ls_symbol_table.address = NULL;
					symbol_table_list.at(jilu_symbol_num).symbol_property_table.push_back(ls_symbol_table);//标识符属性表进入标识符表
					fuzhu.push(ls_s);//退回暂存数据
				}
			}
			fuzhu.pop();
		}
	}
	/*cout << symbol_table_list.at(jilu_symbol_num).symbol_property_table.at(1).name << endl;
	array_table* p = (array_table*)symbol_table_list.at(jilu_symbol_num).symbol_property_table.at(1).type->information_pointer;
	cout << p->type_information->type_code << endl;
	structure_table* pp = (structure_table*)p->type_information->information_pointer;
	cout << pp->num << " " << pp->total_OFF << " " << endl;
	cout << "ID\tOFF\ttp\tvp" << endl;
	for (int i = 0; i < pp->structure_property_list.size(); i++) {
		cout << pp->structure_property_list[i].ID << "\t" << pp->structure_property_list[i].OFF << "\t" << pp->structure_property_list[i].tp->type_code << "\t" << pp->structure_property_list[i].vp << "\t" << endl;
	}*/
}

int Semantics_Analysis::duplicate_check(string s) {
	for (int i = 0; i < symbol_table_list.at(jilu_symbol_num).symbol_property_table.size(); i++) {//查重
		if (s == symbol_table_list.at(jilu_symbol_num).symbol_property_table.at(i).name) {
			return i;
		}
	}
	return -1;
}

void Semantics_Analysis::write_function_table(string s)    //写函数表操作
{
	stack<string> fuzhu_stack;
	fuzhu_stack.push("#");
	while (!semantic_data_stack.empty())
	{
		fuzhu_stack.push(semantic_data_stack.top());
		semantic_data_stack.pop();
	}
	string leixing = "函数";
	fuzhu_stack.push("function");
	cout << "";
}

void Semantics_Analysis::handle_function_begin(string s)   //函数开始
{
	four_item_production temp;
	temp.operation_symbol = "beginFunction";
	temp.first_target.operation_target_value = semantic_data_stack.top();
	temp.second_target.operation_target_value = "_";
	temp.result_target.result_target_value = "_";
	four_item_production_list.push_back(temp);
}

void Semantics_Analysis::handle_function_end(string s)   //函数结束
{
	add_production("endFunction");
	function_four_item_production_list.push_back(four_item_production_list);
	four_item_production_list.clear();
	if (!semantic_data_stack.empty())
		semantic_data_stack.pop();
	jilu_symbol_num++;
	symbol_table_list.resize(symbol_table_list.size() + 1);
}

void Semantics_Analysis::handle_program_end(string s)   //程序结束
{
	add_production("endProgram");
}

void Semantics_Analysis::handle_jump_function(string s)  //跳转到执行函数操作四元式（有返回值）
{
	string function_name = semantic_data_stack.top();
	semantic_data_stack.pop();
	four_item_production temp;
	temp.operation_symbol = "function";
	temp.first_target.operation_target_value = function_name;
	temp.second_target.operation_target_value = "_";
	temp.result_target.result_target_value = to_string(four_item_production_list.size()) + "t";
	semantic_data_stack.push(to_string(four_item_production_list.size()) + "t");
	four_item_production_list.push_back(temp);
}

void Semantics_Analysis::handle_jump_void_function(string s) //跳转到执行函数操作四元式（无返回值）
{
	string function_name = semantic_data_stack.top();
	semantic_data_stack.pop();
	add_production("function", function_name);
}

void Semantics_Analysis::handle_return(string s)   //return四元式
{
	string return_symbol = semantic_data_stack.top();
	semantic_data_stack.pop();
	add_production("return", return_symbol);
}

void Semantics_Analysis::handle_dian_symbol(string s)
{

}

void Semantics_Analysis::handle_before_identifier(string s)  //标识符前缀四元式（指针*和取地址&）
{
	four_item_production temp;
	string symbol = semantic_data_stack.top();
	string identifier;
	semantic_data_stack.pop();
	if (!semantic_data_stack.empty())
		identifier = semantic_data_stack.top();
	if (identifier == "*" || identifier == "&")
	{
		semantic_data_stack.pop();
		temp.operation_symbol = identifier;
		temp.first_target.operation_target_value = symbol;
		temp.second_target.operation_target_value = "_";
		temp.result_target.result_target_value = to_string(four_item_production_list.size()) + "t";
		semantic_data_stack.push(to_string(four_item_production_list.size()) + "t");
		four_item_production_list.push_back(temp);
	}
	else
		semantic_data_stack.push(symbol);
}

void Semantics_Analysis::save_assignment(string s)
{
	semantic_data_stack.push("赋值");
}

void Semantics_Analysis::save_array(string s)
{
	semantic_data_stack.push("数组");
}

void Semantics_Analysis::save_struct_name(string s)
{
	semantic_data_stack.push("结构名");
}

void Semantics_Analysis::save_struct_member(string s)
{
	semantic_data_stack.push("结构体成员");
}

void Semantics_Analysis::save_struct(string s)
{
	semantic_data_stack.push("结构体赋值");
}

void Semantics_Analysis::save_array_assignment(string s)
{
	semantic_data_stack.push("数组赋值");
}
