#include "Grammar_Analysis.h"

Grammar_Analysis::Grammar_Analysis()
{
	production_number = 0;
	first_list.resize(not_end_symbol.size());
	follow_list.resize(not_end_symbol.size());
	select_list.resize(production_number + 1);
	LL1_list.resize(not_end_symbol.size() + 1);
	for (vector<char>::size_type i = 0; i < LL1_list.size(); i++)
	{
		LL1_list[i].resize(end_symbol.size() + 1);
	}
	for (vector<int>::size_type i = 1; i <= not_end_symbol.size(); i++)
	{
		for (vector<int>::size_type j = 1; j <= end_symbol.size(); j++)
		{
			LL1_list[i][j] = "empty";
		}
	}
	for (vector<int>::size_type i = 1; i < LL1_list.size() && i != not_end_symbol.size(); i++)
	{
		LL1_list[i][0] = not_end_symbol[i];
	}
	for (vector<int>::size_type i = 1; i < LL1_list[0].size() && i != end_symbol.size(); i++)
	{
		LL1_list[0][i] = end_symbol[i];
		if (i == end_symbol.size() - 1)
		{
			LL1_list[0][i + 1] = "#";
		}
	}
}

Grammar_Analysis::~Grammar_Analysis() {}

void Grammar_Analysis::Generate_Token() //词法分析器
{
	code_file.open("Code_Information.txt");
	if (code_file.is_open())
		cout << "源程序文件:Code_Information.txt" << endl;
	char code_char, token[token_max_size]; //每次读入的字符/存放中间token串
	memset(token, '\0', sizeof(token));
	int i = 0, state = 0, code_end = 1;
	code_file >> noskipws; //强制读入空格和回车
	while (code_file.peek() != EOF || code_end)
	{
		if (code_file.peek() == EOF && state < 100)
		{
			code_end = 0;
			state = change_state(state, '\n');
			check_code(state, token);
			if (!LL1_sentence_analysis(token))
			{
				system("pause");
				exit(0);
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
			check_code(state, token); //判断单词
			if (!LL1_sentence_analysis(token))
			{
				system("pause");
				exit(0);
			}
			i = 0;
			state = 0;
			if ((code_char <= 'z' && code_char >= 'a') || (code_char <= 'Z' && code_char >= 'A')) //小写字母
				state = change_state(state, 'a');
			else if (code_char <= '9' && code_char >= '0') //数字
				state = change_state(state, '0');
			else
				state = change_state(state, code_char);
			memset(token, '\0', sizeof(token));
			if (code_char != ' ' && code_char != '\n')
				token[i++] = code_char; //将读入字符存入重置后的token串首字符
		}
		else
		{
			if (code_char != ' ' && code_char != '\n') //非终结态，
				token[i++] = code_char;				   //将读入字符存入token串首字符,
													   //cout << token << endl;
		}
	}
	token_list.push_back("#");
	token_type.push_back("结束符");
	token_number.push_back(1);
	LL1_sentence_analysis("#");
}

void Grammar_Analysis::get_first_list() //求first集
{
	int flag = 1; //设置循环标志
	while (flag == 1)
	{
		flag = 0;
		for (int i = 0; i < production_number; i++) //遍历所有产生式
		{
			if (production_right[i].size() == 0)
			{ //空串的select集为空
				continue;
			}
			else if (get_end_symbol_number(production_right[i][0]))
			{ //非字态的终结符
				if (new_symbol_flag(&first_list, production_right[i][0], get_not_end_symbol_number(production_left[i])))
				{
					flag = 1;
					first_list[get_not_end_symbol_number(production_left[i])].push_back(production_right[i][0]);
				}
			}
			else if (get_not_end_symbol_number(production_right[i][0]))
			{ //不带角标的非终结符
				int j = 0;
				while (empty_symbol_flag(production_right[i][j]))
				{
					for (vector<char>::size_type k = 0; k < first_list[get_not_end_symbol_number(production_right[i][j])].size(); k++)
					{
						if (new_symbol_flag(&first_list, first_list[get_not_end_symbol_number(production_right[i][j])][k], get_not_end_symbol_number(production_left[i])))
						{
							flag = 1;
							first_list[get_not_end_symbol_number(production_left[i])].push_back(first_list[get_not_end_symbol_number(production_right[i][j])][k]);
						}
					}
					if (j == production_right[i].size() - 1)
						break;
					else
						j++;
				}
				if (get_end_symbol_number(production_right[i][j]))
				{
					if (new_symbol_flag(&first_list, production_right[i][j], get_not_end_symbol_number(production_left[i])))
					{
						first_list[get_not_end_symbol_number(production_left[i])].push_back(production_right[i][j]);
						flag = 1;
					}
				}
				else
				{
					for (vector<char>::size_type k = 0; k < first_list[get_not_end_symbol_number(production_right[i][j])].size(); k++)
					{
						if (new_symbol_flag(&first_list, first_list[get_not_end_symbol_number(production_right[i][j])][k], get_not_end_symbol_number(production_left[i])))
						{
							flag = 1;
							first_list[get_not_end_symbol_number(production_left[i])].push_back(first_list[get_not_end_symbol_number(production_right[i][j])][k]);
						}
					}
				}
			}
		}
	}
}

void Grammar_Analysis::get_follow_list() //求follow集
{
	follow_list[get_not_end_symbol_number(head_symbol)].push_back("#");
	int flag = 1;
	while (flag == 1)
	{
		flag = 0;
		for (int i = 0; i < production_number; i++)
		{ //遍历所有产生式
			for (vector<char>::size_type j = 0; j < production_right[i].size(); j++)
			{ //依次查看产生式的右部
				if (get_not_end_symbol_number(production_right[i][j]))
				{ //遇到非终结符
					if (j == production_right[i].size() - 1)
					{ //当前为最后一项
						for (vector<char>::size_type k = 0; k < follow_list[get_not_end_symbol_number(production_left[i])].size(); k++)
						{ //最后一项，将产生式左部的follow集加入产生式右部最后一项的follow集
							if (new_symbol_flag(&follow_list, follow_list[get_not_end_symbol_number(production_left[i])][k], get_not_end_symbol_number(production_right[i][j])))
							{
								flag = 1;
								follow_list[get_not_end_symbol_number(production_right[i][j])].push_back(follow_list[get_not_end_symbol_number(production_left[i])][k]);
							}
						}
					}
					else if (get_end_symbol_number(production_right[i][j + 1]))
					{ //下一项为终结符
						if (new_symbol_flag(&follow_list, production_right[i][j + 1], get_not_end_symbol_number(production_right[i][j])))
						{ //下一项为终结符，加入前一项非终结符的follow集
							flag = 1;
							follow_list[get_not_end_symbol_number(production_right[i][j])].push_back(production_right[i][j + 1]);
						}
					}
					else if (get_not_end_symbol_number(production_right[i][j + 1]))
					{ //下一项为非终结符
						int k = j;
						while (k != production_right[i].size() - 1)
						{
							k++;
							if (get_end_symbol_number(production_right[i][k]))
							{ //下一项为终结符，加入前一项非终结符的follow集
								if (new_symbol_flag(&follow_list, production_right[i][j + 1], get_not_end_symbol_number(production_right[i][j])))
								{
									flag = 1;
									follow_list[get_not_end_symbol_number(production_right[i][j])].push_back(production_right[i][j + 1]);
								}
								break;
							}
							else if (get_not_end_symbol_number(production_right[i][k]))
							{ //下一项为非终结符
								if (empty_symbol_flag(production_right[i][k]))
								{ //下一项终结符可空
									for (vector<char>::size_type l = 0; l < first_list[get_not_end_symbol_number(production_right[i][k])].size(); l++)
									{ //下一项的first集加入前一项的follow集
										if (new_symbol_flag(&follow_list, first_list[get_not_end_symbol_number(production_right[i][k])][l], get_not_end_symbol_number(production_right[i][j])))
										{
											flag = 1;
											follow_list[get_not_end_symbol_number(production_right[i][j])].push_back(first_list[get_not_end_symbol_number(production_right[i][k])][l]);
										}
									}
									if (k == production_right[i].size() - 1)
									{ //最后一项，将产生式左部的follow集加入产生式右部最后一项的follow集
										for (vector<char>::size_type l = 0; l < follow_list[get_not_end_symbol_number(production_left[i])].size(); l++)
										{
											if (new_symbol_flag(&follow_list, follow_list[get_not_end_symbol_number(production_left[i])][l], get_not_end_symbol_number(production_right[i][j])))
											{
												flag = 1;
												follow_list[get_not_end_symbol_number(production_right[i][j])].push_back(follow_list[get_not_end_symbol_number(production_left[i])][l]);
											}
										}
									}
								}
								else if (!empty_symbol_flag(production_right[i][k]) && k == j + 1)
								{ //下一项非终结符非空
									for (vector<char>::size_type l = 0; l < first_list[get_not_end_symbol_number(production_right[i][k])].size(); l++)
									{ //下一项非终结符first加入前一项的follow集
										if (new_symbol_flag(&follow_list, first_list[get_not_end_symbol_number(production_right[i][k])][l], get_not_end_symbol_number(production_right[i][j])))
										{
											flag = 1;
											follow_list[get_not_end_symbol_number(production_right[i][j])].push_back(first_list[get_not_end_symbol_number(production_right[i][k])][l]);
										}
									}
									break;
								}
							}
						}
					}
				}
			}
		}
	}
}

void Grammar_Analysis::get_select_list() //求select集
{
	vector<char>::size_type production_right_empty_symbol = 0;
	for (int i = 0; i < production_number; i++)
	{ //遍历所有产生式
		production_right_empty_symbol = 0;
		if (production_right[i].size() == 0)
		{ //右部产生式为空串,x号产生式的select集为其左部的follow集
			for (vector<char>::size_type j = 0; j < follow_list[get_not_end_symbol_number(production_left[i])].size(); j++)
			{
				if (new_symbol_flag(&select_list, follow_list[get_not_end_symbol_number(production_left[i])][j], i + 1))
				{
					select_list[i + 1].push_back(follow_list[get_not_end_symbol_number(production_left[i])][j]);
				}
			}
			continue;
		}
		else
		{ //右部产生式至少有一个字符
			for (vector<char>::size_type j = 0; j < production_right[i].size(); j++)
			{ //遍历产生式的每个符号
				if (get_end_symbol_number(production_right[i][j]))
				{ //存在终结符则右部产生式非空，且首符号为终结符
					if (new_symbol_flag(&select_list, production_right[i][j], get_not_end_symbol_number(production_left[i])) + 1)
					{ //x号产生式的select集为该终结符
						select_list[i + 1].push_back(production_right[i][j]);
					}
					break;
				}
				else if (get_not_end_symbol_number(production_right[i][j]))
				{ //右部产生式遇到非终结符
					if (get_end_symbol_number(production_right[i][j]))
					{
						if (new_symbol_flag(&select_list, production_right[i][j], get_not_end_symbol_number(production_left[i])) + 1)
						{ //x号产生式的select集为该终结符
							select_list[i + 1].push_back(production_right[i][j]);
						}
						break;
					}
					else if (!empty_symbol_flag(production_right[i][j]))
					{ //非空的非终结符
						if (j == production_right_empty_symbol)
						{ //第一个为非空非终结符，其first集加入follow集
							for (vector<char>::size_type k = 0; k < first_list[get_not_end_symbol_number(production_right[i][j])].size(); k++)
							{
								if (new_symbol_flag(&select_list, first_list[get_not_end_symbol_number(production_right[i][j])][k], i + 1))
								{ //x号产生式的select集为其左部的first集
									select_list[i + 1].push_back(first_list[get_not_end_symbol_number(production_right[i][j])][k]);
								}
							}
						}
						break;
					}
					else
					{
						for (vector<char>::size_type k = 0; k < first_list[get_not_end_symbol_number(production_right[i][j])].size(); k++)
						{
							if (new_symbol_flag(&select_list, first_list[get_not_end_symbol_number(production_right[i][j])][k], i + 1))
							{ //x号产生式的select集为其左部的first集
								select_list[i + 1].push_back(first_list[get_not_end_symbol_number(production_right[i][j])][k]);
							}
						}
						production_right_empty_symbol++;
						if (production_right_empty_symbol == production_right[i].size())
						{
							for (vector<char>::size_type j = 0; j < follow_list[get_not_end_symbol_number(production_left[i])].size(); j++)
							{
								if (new_symbol_flag(&select_list, follow_list[get_not_end_symbol_number(production_left[i])][j], i + 1))
								{ //x号产生式的select集为其左部的first集
									select_list[i + 1].push_back(follow_list[get_not_end_symbol_number(production_left[i])][j]);
								}
							}
							break;
						}
					}
				}
			}
		}
	}
}

void Grammar_Analysis::get_LL1_list() //求LL1分析表
{
	for (int i = 1; i <= production_number; i++)
	{
		for (vector<char>::size_type j = 0; j < select_list[i].size(); j++)
		{
			LL1_list[get_not_end_symbol_number(production_left[i - 1])][get_end_symbol_number(select_list[i][j])] = to_string(i);
		}
	}
}

int Grammar_Analysis::LL1_sentence_analysis(string token) //LL1语法分析
{
	static int first_flag = 1, step_number = 1;
	/*cout << "LL1分析法，待识别串:";
	for (vector<int>::size_type i = 0; i < token_list.size(); i++)
	{
		cout << token_list[i];
	}*/
	if (first_flag == 1)
	{
		first_flag = 0;
		cout << "\n步骤\t栈顶\t读入\t操作" << endl;
		grammar_stack.push("#");
		//cout << step_number++ << "\t空\t无\t结束符'#'入栈" << endl;
		grammar_stack.push(head_symbol);
		//cout << step_number++ << "\t'#'\t无\t起始符'" << head_symbol << "'入栈" << endl;
	}
	while (!grammar_stack.empty()) //栈非空
	{
		if (get_end_symbol_number(grammar_stack.top()))
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
					//cout << step_number++ << "\t" << grammar_stack.top() << "\t" << token << "\t'" << grammar_stack.top() << "'出栈" << endl;
					grammar_stack.pop();
					for (int j = production_right[i - 1].size() - 1; j >= 0; j--)
					{
						//cout << "\t" << grammar_stack.top() << "\t" << token << "\t'" << production_right[i - 1][j] << "'入栈" << endl;
						grammar_stack.push(production_right[i - 1][j]);
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

void Grammar_Analysis::scanf_information() //输入
{
	fstream fp;
	string str;
	string head;
	vector<string> right;
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
}

void Grammar_Analysis::print_information() //输出
{
	// 输出所有产生式
	cout << "消除左递归后的文法共计" << production_number << "个产生式:" << endl;
	for (int i = 0; i < production_number; i++)
	{
		cout << i + 1 << "号产生式:" << production_left[i] << "→";
		for (vector<char>::size_type j = 0; j < production_right[i].size(); j++)
		{
			cout << production_right[i][j] << " ";
		}
		cout << endl;
	}
	cout << endl;
	// 输出能所有能推出空产生式的符号
	cout << "所有能推出空产生式的符号:";
	for (vector<char>::size_type i = 1; i < not_end_symbol.size(); i++)
	{
		if (empty_symbol_flag(not_end_symbol[i]))
			cout << not_end_symbol[i] << " ";
	}
	cout << "\n\n";
	// 输出first集
	cout << "first集合:" << endl;
	for (vector<char>::size_type i = 1; i < first_list.size(); i++)
	{
		cout << "first(" << not_end_symbol[i] << "):";
		for (vector<char>::size_type j = 0; j < first_list[i].size(); j++)
		{
			cout << first_list[i][j] << " ";
		}
		cout << endl;
	}
	cout << endl;
	cout << "follow集合:" << endl;
	for (vector<char>::size_type i = 1; i < first_list.size(); i++)
	{
		cout << "follow(" << not_end_symbol[i] << "):";
		for (vector<char>::size_type j = 0; j < follow_list[i].size(); j++)
		{
			cout << follow_list[i][j] << " ";
		}
		cout << endl;
	}
	cout << endl;
	cout << "select集合:" << endl;
	for (int i = 1; i <= production_number; i++)
	{
		cout << "select(" << i << "):";
		for (vector<char>::size_type j = 0; j < select_list[i].size(); j++)
		{
			cout << select_list[i][j] << " ";
		}
		cout << endl;
	}
	cout << endl;
	//// 输出LL1分析表
	//cout << "LL1分析表:" << endl;
	//for (vector<int>::size_type i = 0; i < LL1_list.size(); i++)
	//{
	//	for (vector<int>::size_type j = 0; j < LL1_list[i].size(); j++)
	//	{
	//		cout << LL1_list[i][j] << "\t";
	//	}
	//	cout << endl;
	//}
	//cout << endl;
}

int Grammar_Analysis::get_not_end_symbol_number(string s) //如果匹配成功，返回非终结符的序号
{
	for (vector<int>::size_type i = 0; i < not_end_symbol.size(); i++)
	{
		if (not_end_symbol[i] == s)
		{
			return i;
		}
	}
	return 0;
}

int Grammar_Analysis::get_end_symbol_number(string s) //如果匹配成功，返回终结符的序号
{
	for (vector<int>::size_type i = 0; i < end_symbol.size(); i++)
	{
		if (end_symbol[i] == s)
		{
			return i;
		}
	}
	return 0;
}

string Grammar_Analysis::get_next_w_type(string s) //返回输入符号对应的非终结符
{
	for (vector<int>::size_type i = 0; i < const_symbol.size(); i++)
	{
		if (const_symbol[i] == s)
		{
			return "标识符";
		}
	}
	for (vector<int>::size_type i = 0; i < const_number.size(); i++)
	{
		if (const_number[i] == s)
		{
			return "数值";
		}
	}
	for (vector<int>::size_type i = 0; i < const_char.size(); i++)
	{
		if (const_char[i] == s)
		{
			return "字符";
		}
	}
	for (vector<int>::size_type i = 0; i < const_string.size(); i++)
	{
		if (const_string[i] == s)
		{
			return "字符串";
		}
	}
	return s;
}

int Grammar_Analysis::new_symbol_flag(vector<vector<string>>* symbol, string s, vector<char>::size_type i) //查找i项的(*symbol)集中s是否是新符号
{
	for (vector<char>::size_type j = 0; j < (*symbol)[i].size(); j++)
	{
		if ((*symbol)[i][j] == s)
			return 0;
	}
	return 1;
}

int Grammar_Analysis::empty_symbol_flag(string s) //判断一个非终结符是否可为空
{
	for (int i = 0; i < production_number; i++) //遍历所有产生式
	{
		if (production_left[i] == s) //在产生式左部查找判断字符
		{
			if (production_right[i].size() == 0) //为空直接返回
				return 1;
			else
			{
				for (vector<char>::size_type j = 0; j < production_right[i].size(); j++)
				{
					if (get_not_end_symbol_number(production_right[i][j]))
					{
						if (!empty_symbol_flag(production_right[i][j]))
						{
							break;
						}
						else if (j == production_right[i].size() - 1) //最后一个依然为空
							return 1;
					}
					else //终结符必不为空，继续下一次查找判断
						break;
				}
			}
		}
	}
	return 0;
}
