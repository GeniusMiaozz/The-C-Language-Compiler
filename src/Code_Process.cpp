
#include "Code_Process.h"

Code_Process::Code_Process()
{

}

Code_Process::~Code_Process()
{

}

void Code_Process::get_symbol_table_list(vector<symbol_table> s)
{
	this->symbol_table_list = s;
}

void Code_Process::get_four_item_production(vector<vector<four_item_production>> f)
{
	this->four_item_production_list = f;
}

void Code_Process::get_const_symbol(vector<string> c)
{
	this->const_symbol = c;
}

void Code_Process::print_old_four_item_production()//输出四元式
{
	cout << "未优化的四元式组:" << endl;
	for (unsigned int i = 0; i < four_item_production_list.size(); i++)
	{
		cout << "函数" << i << ":" << endl;
		for (unsigned int j = 0; j < four_item_production_list[i].size(); j++)
		{
			cout << j << "号式:" << four_item_production_list[i][j].basic_block_flag << "  " << four_item_production_list[i][j].operation_symbol << "  ";
			if (four_item_production_list[i][j].first_target.operation_target_value != "_")
				cout << four_item_production_list[i][j].first_target.operation_target_value << "(" << four_item_production_list[i][j].first_target.active_information << ")\t";
			else
				cout << four_item_production_list[i][j].first_target.operation_target_value << "\t";
			if (four_item_production_list[i][j].second_target.operation_target_value != "_")
				cout << four_item_production_list[i][j].second_target.operation_target_value << "(" << four_item_production_list[i][j].second_target.active_information << ")\t";
			else
				cout << four_item_production_list[i][j].second_target.operation_target_value << "\t";
			if (four_item_production_list[i][j].result_target.result_target_value != "_")
				cout << four_item_production_list[i][j].result_target.result_target_value << "(" << four_item_production_list[i][j].result_target.active_information << ")" << endl;
			else
				cout << four_item_production_list[i][j].result_target.result_target_value << endl;
		}
		cout << endl;
	}
}

void Code_Process::print_new_four_item_production()//输出四元式
{
	cout << "优化后的四元式组:" << endl;
	for (unsigned int i = 0; i < new_four_item_production_list.size(); i++)
	{
		cout << "函数" << i << ":" << endl;
		for (unsigned int j = 0; j < new_four_item_production_list[i].size(); j++)
		{
			cout << j << "号式:" << new_four_item_production_list[i][j].basic_block_flag << "  " << new_four_item_production_list[i][j].operation_symbol << "  ";
			if (new_four_item_production_list[i][j].first_target.operation_target_value != "_")
				cout << new_four_item_production_list[i][j].first_target.operation_target_value << "(" << new_four_item_production_list[i][j].first_target.active_information << ")\t";
			else
				cout << new_four_item_production_list[i][j].first_target.operation_target_value << "\t";
			if (four_item_production_list[i][j].second_target.operation_target_value != "_")
				cout << new_four_item_production_list[i][j].second_target.operation_target_value << "(" << new_four_item_production_list[i][j].second_target.active_information << ")\t";
			else
				cout << new_four_item_production_list[i][j].second_target.operation_target_value << "\t";
			if (four_item_production_list[i][j].result_target.result_target_value != "_")
				cout << new_four_item_production_list[i][j].result_target.result_target_value << "(" << new_four_item_production_list[i][j].result_target.active_information << ")" << endl;
			else
				cout << new_four_item_production_list[i][j].result_target.result_target_value << endl;
		}
		cout << endl;
	}
}
void Code_Process::print_DAG_information()
{
	for (int k = 0; k < out_DAG.size(); k++) {
		cout << k << "号图：" << endl;
		for (int i = 0; i < out_DAG[k].size(); i++)
		{
			cout << out_DAG[k][i].node_number << "号点:主标记：" << out_DAG[k][i].main_mark << "，附加标记：";
			for (int j = 0; j < out_DAG[k][i].other_mark.size(); j++)
			{
				cout << out_DAG[k][i].other_mark[j] << " ";
			}
			cout << "\n运算符：" << out_DAG[k][i].operate_symbol;
			cout << "\n前驱：";
			for (int j = 0; j < out_DAG[k][i].front_node.size(); j++)
			{
				cout << out_DAG[k][i].front_node[j] << " ";
			}
			cout << "\n后继：";
			for (int j = 0; j < out_DAG[k][i].rear_node.size(); j++)
			{
				cout << out_DAG[k][i].rear_node[j] << " ";
			}
			cout << endl;
		}
		cout << endl;
	}
}

void Code_Process::divide_basic_block(vector<vector<four_item_production>>* four_item_production_list)//划分基本块
{
	for (int j = 0; j < (*four_item_production_list).size(); j++)
	{
		(*four_item_production_list)[j][0].basic_block_flag = "入口块";
		(*four_item_production_list)[j][(*four_item_production_list)[j].size() - 1].basic_block_flag = "出口块";
		for (__int64 i = 0; i < (*four_item_production_list)[j].size(); i++)
		{
			if (((*four_item_production_list)[j][i].operation_symbol == "ifEndJudge") || ((*four_item_production_list)[j][i].operation_symbol == "ifJudge") || ((*four_item_production_list)[j][i].operation_symbol == "endIf") || ((*four_item_production_list)[j][i].operation_symbol == "whileDoIf") || ((*four_item_production_list)[j][i].operation_symbol == "forDoIf") || ((*four_item_production_list)[j][i].operation_symbol == "case") || ((*four_item_production_list)[j][i].operation_symbol == "switch"))
			{//跳转语句的下一句是入口
				(*four_item_production_list)[j][i + 1].basic_block_flag = "入口块";
			}
			if (((*four_item_production_list)[j][i].operation_symbol == "ifToEnd") || ((*four_item_production_list)[j][i].operation_symbol == "endWhile") || ((*four_item_production_list)[j][i].operation_symbol == "endFor") || ((*four_item_production_list)[j][i].operation_symbol == "endSwitch") || ((*four_item_production_list)[j][i].operation_symbol == "caseEnd"))
			{//跳转语句跳转到的位置的是入口
				(*four_item_production_list)[j][i + 1].basic_block_flag = "入口块";
			}
			if (((*four_item_production_list)[j][i].operation_symbol == "else") || ((*four_item_production_list)[j][i].operation_symbol == "forBeginJudge") || ((*four_item_production_list)[j][i].operation_symbol == "afterForDo") || ((*four_item_production_list)[j][i].operation_symbol == "forDo") || ((*four_item_production_list)[j][i].operation_symbol == "beginWhile") || ((*four_item_production_list)[j][i].operation_symbol == "default") || ((*four_item_production_list)[j][i].operation_symbol == "forBegin"))
			{//跳转语句跳转到的位置是入口
				(*four_item_production_list)[j][i].basic_block_flag = "入口块";
			}
		}
		for (__int64 i = (*four_item_production_list)[j].size() - 1; i > 0; i--)
		{
			if ((*four_item_production_list)[j][i].basic_block_flag == "入口块")
			{//入口块的前一个位置是出口块，第一个入口块除外
				(*four_item_production_list)[j][i - 1].basic_block_flag = "出口块";
			}
		}
	}
}

void Code_Process::initialize_active_information(vector<vector<four_item_production>>* four_item_production_list)//活跃信息初始化前初始化辅助表
{
	for (unsigned int k = 0, j = -1; k < (*four_item_production_list).size(); k++)
	{
		for (__int64 i = 0; i < (*four_item_production_list)[k].size(); i++)
		{
			if ((*four_item_production_list)[k][i].basic_block_flag == "入口块")
			{
				j++;
				symbol_active_list.resize(symbol_active_list.size() + 1);
			}
			if (!symbol_active_list[j].count((*four_item_production_list)[k][i].first_target.operation_target_value))
			{//不存在
				if (block_part_variable_flag((*four_item_production_list)[k][i].first_target.operation_target_value))
				{//块内局部变量
					symbol_active_list[j].insert(pair<string, bool>((*four_item_production_list)[k][i].first_target.operation_target_value, false));
				}
				else
				{
					symbol_active_list[j].insert(pair<string, bool>((*four_item_production_list)[k][i].first_target.operation_target_value, true));
				}

			}
			if (!symbol_active_list[j].count((*four_item_production_list)[k][i].second_target.operation_target_value))
			{//不存在
				if (block_part_variable_flag((*four_item_production_list)[k][i].second_target.operation_target_value))
				{//块内局部变量
					symbol_active_list[j].insert(pair<string, bool>((*four_item_production_list)[k][i].second_target.operation_target_value, false));
				}
				else
				{
					symbol_active_list[j].insert(pair<string, bool>((*four_item_production_list)[k][i].second_target.operation_target_value, true));
				}

			}
			if (!symbol_active_list[j].count((*four_item_production_list)[k][i].result_target.result_target_value))
			{//不存在
				if (block_part_variable_flag((*four_item_production_list)[k][i].result_target.result_target_value))
				{//块内局部变量
					symbol_active_list[j].insert(pair<string, bool>((*four_item_production_list)[k][i].result_target.result_target_value, false));
				}
				else
				{
					symbol_active_list[j].insert(pair<string, bool>((*four_item_production_list)[k][i].result_target.result_target_value, true));
				}
			}
		}
	}
}

void Code_Process::change_active_information(vector<vector<four_item_production>>* four_item_production_list)//为变量添加初始化活跃信息
{
	for (int j = (*four_item_production_list).size() - 1, k = symbol_active_list.size(); j > 0; j--) {
		for (int i = (*four_item_production_list)[j].size() - 1; i > 0; i--)
		{//从最后一个基本块开始遍历
			if ((*four_item_production_list)[j][i].basic_block_flag == "出口块")
			{//遇到出口块标志
				k--;
			}
			if ((*four_item_production_list)[j][i].result_target.result_target_value != "_")
			{
				(*four_item_production_list)[j][i].result_target.active_information = symbol_active_list[k].find((*four_item_production_list)[j][i].result_target.result_target_value)->second;
				symbol_active_list[k][(*four_item_production_list)[j][i].result_target.result_target_value] = false;
			}
			if ((*four_item_production_list)[j][i].first_target.operation_target_value != "_")
			{
				(*four_item_production_list)[j][i].first_target.active_information = symbol_active_list[k].find((*four_item_production_list)[j][i].first_target.operation_target_value)->second;
				symbol_active_list[k][(*four_item_production_list)[j][i].first_target.operation_target_value] = true;
			}
			if ((*four_item_production_list)[j][i].second_target.operation_target_value != "_")
			{
				(*four_item_production_list)[j][i].second_target.active_information = symbol_active_list[k].find((*four_item_production_list)[j][i].second_target.operation_target_value)->second;
				symbol_active_list[k][(*four_item_production_list)[j][i].second_target.operation_target_value] = true;
			}
		}
	}
}

void Code_Process::code_optomization()//代码优化
{//结点主标记的顺序：常量>临时变量>非临时变量
	this->divide_basic_block(&four_item_production_list);//划分基本块
	this->initialize_active_information(&four_item_production_list);//基本块内变量活跃信息的初始化预备工作
	this->change_active_information(&four_item_production_list);//基本块内变量活跃信息的添加
	int jump_flag = 0;//处理break或者continue的跳出
	four_item_production jump_action;//break或者continue的四元式
	for (int j = 0; j < four_item_production_list.size(); j++)
	{//遍历每一个函数的四元式表
		function_number++;
		get_symbol_size.resize(get_symbol_size.size() + 1);
		for (int i = 0; i < four_item_production_list[j].size(); i++)
		{//遍历一个函数四元式表中的每一条
			if (four_item_production_list[j][i].basic_block_flag == "入口块")
			{
				temp_f.push_back(four_item_production_list[j][i]);//直接加入临时四元式表，查看下一条
				continue;
			}
			else if ((four_item_production_list[j][i].operation_symbol == "break") || (four_item_production_list[j][i].operation_symbol == "continue"))
			{
				jump_flag = 1;
				jump_action = four_item_production_list[j][i];
			}
			else if (four_item_production_list[j][i].basic_block_flag == "出口块")
			{
				handle_DAG();//处理DAG图，生成优化后的四元式，加入temp_f中
				out_DAG.push_back(DAG_map);
				DAG_map.clear();//清空DAG
				DAG_symbol_to_node.clear();//清空常量、标识符与结点对应图
				if (jump_flag == 1) {
					temp_f.push_back(jump_action);
					jump_flag = 0;
				}
				temp_f.push_back(four_item_production_list[j][i]);
				continue;
			}
			else if ((four_item_production_list[j][i].operation_symbol == "break") || (four_item_production_list[j][i].operation_symbol == "continue"))
			{
				temp_f.push_back(four_item_production_list[j][i]);
				continue;
			}
			else if (four_item_production_list[j][i].operation_symbol == "=")
			{//赋值表达式:A=B，B为常量或变量
				int B_node = get_A_B_node_number(four_item_production_list[j][i].first_target.operation_target_value);//查找B的结点是否存在
				if (DAG_symbol_to_node.count(four_item_production_list[j][i].result_target.result_target_value))
				{//当前已有结点的附加标记中有A
					vector<string>* s = &DAG_map[DAG_symbol_to_node[four_item_production_list[j][i].result_target.result_target_value]].other_mark;
					s->erase(remove(s->begin(), s->end(), four_item_production_list[j][i].result_target.result_target_value), s->end());
					//移除其附加标记中的A
					DAG_symbol_to_node[four_item_production_list[j][i].result_target.result_target_value] = B_node;
					//A结点的序号是B结点的序号
				}
				else
				{//当前没有包含A的结点
					DAG_symbol_to_node.insert(pair<string, int>(four_item_production_list[j][i].result_target.result_target_value, B_node));
					//插入A结点的序号，为B结点的序号
				}
				DAG_map[B_node].other_mark.push_back(four_item_production_list[j][i].result_target.result_target_value);
				//A加入B结点的附加标记
				handle_main_other_order(B_node);
				//处理一个结点上主标记和附加标记的关系
				continue;
			}
			else if (const_number_flag(four_item_production_list[j][i].first_target.operation_target_value) && const_number_flag(four_item_production_list[j][i].second_target.operation_target_value))
			{//常量表达式：A=C1wC2，C1wC2为常量P
				int P_node = get_A_C1wC2_node_number(four_item_production_list[j][i].first_target.operation_target_value, four_item_production_list[j][i].second_target.operation_target_value, four_item_production_list[j][i].operation_symbol);
				if (DAG_symbol_to_node.count(four_item_production_list[j][i].result_target.result_target_value))
				{//当前已有结点的附加标记中有A
					vector<string>* s = &DAG_map[DAG_symbol_to_node[four_item_production_list[j][i].result_target.result_target_value]].other_mark;
					s->erase(remove(s->begin(), s->end(), four_item_production_list[j][i].result_target.result_target_value), s->end());
					//移除其附加标记中的A
					DAG_symbol_to_node[four_item_production_list[j][i].result_target.result_target_value] = P_node;
					//A结点的序号是P结点的序号
				}
				else
				{//当前没有包含A的结点
					DAG_symbol_to_node.insert(pair<string, int>(four_item_production_list[j][i].result_target.result_target_value, P_node));
					//插入A结点的序号，为P结点的序号
				}
				DAG_map[P_node].other_mark.push_back(four_item_production_list[j][i].result_target.result_target_value);
				//A加入P结点的附加标记
				handle_main_other_order(P_node);
				//处理一个结点上主标记和附加标记的关系
				continue;
			}
			else if (four_item_production_list[j][i].second_target.operation_target_value == "_")
			{//运算表达式：A=wB，B不为常量
				int wB_node = get_A_wB_node_number(four_item_production_list[j][i].first_target.operation_target_value, four_item_production_list[j][i].operation_symbol);
				if (DAG_symbol_to_node.count(four_item_production_list[j][i].result_target.result_target_value))
				{//当前已有结点的附加标记中有A
					vector<string>* s = &DAG_map[DAG_symbol_to_node[four_item_production_list[j][i].result_target.result_target_value]].other_mark;
					s->erase(remove(s->begin(), s->end(), four_item_production_list[j][i].result_target.result_target_value), s->end());
					//移除其附加标记中的A
					DAG_symbol_to_node[four_item_production_list[j][i].result_target.result_target_value] = wB_node;
					//A结点的序号是wB结点的序号
				}
				else
				{//当前没有包含A的结点
					DAG_symbol_to_node.insert(pair<string, int>(four_item_production_list[j][i].result_target.result_target_value, wB_node));
					//A结点的序号是wB结点的序号
				}
				DAG_map[wB_node].other_mark.push_back(four_item_production_list[j][i].result_target.result_target_value);
				//A加入wB结点的附加标记
				handle_main_other_order(wB_node);
				//处理一个结点上主标记和附加标记的关系
				continue;
			}
			else
			{//运算表达式：A=BwC，B、C不为常量
				int BwC_node = get_A_BwC_node_number(four_item_production_list[j][i].first_target.operation_target_value, four_item_production_list[j][i].second_target.operation_target_value, four_item_production_list[j][i].operation_symbol);
				if (DAG_symbol_to_node.count(four_item_production_list[j][i].result_target.result_target_value))
				{//当前已有结点的附加标记中有A
					vector<string>* s = &DAG_map[DAG_symbol_to_node[four_item_production_list[j][i].result_target.result_target_value]].other_mark;
					s->erase(remove(s->begin(), s->end(), four_item_production_list[j][i].result_target.result_target_value), s->end());
					//移除其附加标记中的A
					DAG_symbol_to_node[four_item_production_list[j][i].result_target.result_target_value] = BwC_node;
					//A结点的序号是BwC结点的序号
				}
				else
				{//当前没有包含A的结点
					DAG_symbol_to_node.insert(pair<string, int>(four_item_production_list[j][i].result_target.result_target_value, BwC_node));
					//A结点的序号是BwC结点的序号
				}
				DAG_map[BwC_node].other_mark.push_back(four_item_production_list[j][i].result_target.result_target_value);
				//A加入BwC结点的附加标记
				handle_main_other_order(BwC_node);
				//处理一个结点上主标记和附加标记的关系
				continue;
			}
		}
		new_four_item_production_list.push_back(temp_f);
		temp_f.clear();
	}
	this->divide_basic_block(&new_four_item_production_list);//优化后四元式划分基本块
	this->initialize_active_information(&new_four_item_production_list);//优化后四元式基本块内变量活跃信息的初始化预备工作
	this->change_active_information(&new_four_item_production_list);//优化后四元式基本块内变量活跃信息的添加
}

int Code_Process::block_part_variable_flag(string s)//判断一个变量是否是块内局部变量
{
	if ((isdigit(s[0])) && (s[s.size() - 1] == 't'))
	{
		return 1;
	}
	else
		return 0;
}

int Code_Process::const_symbol_flag(string s)//判断一个字符是否是标识符
{
	for (int i = 1; i <= const_symbol.size(); i++)
	{
		if (const_symbol[i] == s)
		{
			return 1;
		}
	}
	return 0;
}

int Code_Process::const_number_flag(string s)//判断一个字符是否是常数
{
	if (isdigit(s[0]) && isdigit(s[s.size() - 1]))
		return 1;
	else
		return 0;
}

int Code_Process::get_A_B_node_number(string B)//返回B结点的序号，如果不存在则生成
{
	if (DAG_symbol_to_node.count(B))
	{//如果B结点所在的结点存在，直接返回该结点的值
		return DAG_symbol_to_node[B];
	}
	else
	{
		DAG_property temp_node;//生成DAG图时的临时结点
		temp_node.node_number = DAG_map.size();//新结点序号为当前DAG的长度
		temp_node.main_mark = B;
		temp_node.other_mark.clear();
		if ((B[0] == '\'') || (B[0] == '\"'))
			temp_node.symbol_size = 4;
		else if (B.size() == 1 && isdigit(B[0]))
			temp_node.symbol_size = 4;
		else if (B[1] == '.')
			temp_node.symbol_size = 8;
		else if (isdigit(B[0]))
			temp_node.symbol_size = 4;
		else if (symbol_table_list[function_number].symbol_property_table[get_symbol_number(B, function_number)].type->type_code == "double")
			temp_node.symbol_size = 8;
		else
			temp_node.symbol_size = 4;
		DAG_map.push_back(temp_node);//DAG图加入结点
		DAG_symbol_to_node.insert(pair<string, int>(B, temp_node.node_number));
		//B结点的序号是新建结点的序号
		return DAG_symbol_to_node[B];
	}
}

int Code_Process::get_A_C1wC2_node_number(string c1, string c2, string w)//返回P=C1wC2结点的序号，如果不存在则生成
{
	double result = 0;
	if (!strcmp(w.c_str(), "+"))
		result = atof(c1.c_str()) + atof(c2.c_str());
	else if (!strcmp(w.c_str(), "-"))
		result = atof(c1.c_str()) - atof(c2.c_str());
	else if (!strcmp(w.c_str(), "*"))
		result = atof(c1.c_str()) * atof(c2.c_str());
	else if (!strcmp(w.c_str(), "/"))
		result = atof(c1.c_str()) / atof(c2.c_str());
	if (DAG_symbol_to_node.count(to_string(result)))
	{//如果常值表达式的结果P存在某一个结点，返回结点号
		return DAG_symbol_to_node[to_string(result)];
	}
	else
	{//否则新建P结点
		DAG_property temp_node;//生成DAG图时的临时结点
		temp_node.node_number = DAG_map.size();//新结点序号为当前DAG的长度
		temp_node.main_mark = to_string(result);//主结点为P
		temp_node.other_mark.clear();
		if ((to_string(result)[0] == '\'') || (to_string(result)[0] == '\"'))
			temp_node.symbol_size = 4;
		else if (to_string(result).size() == 1 && isdigit(to_string(result)[0]))
			temp_node.symbol_size = 4;
		else if (to_string(result)[1] == '.')
			temp_node.symbol_size = 8;
		else if (isdigit(to_string(result)[0]))
			temp_node.symbol_size = 4;
		else if ((symbol_table_list[function_number].symbol_property_table[get_symbol_number(c1, function_number)].type->type_code != "double") && (symbol_table_list[function_number].symbol_property_table[get_symbol_number(c2, function_number)].type->type_code != "double"))
			temp_node.symbol_size = 4;
		else
			temp_node.symbol_size = 8;
		DAG_map.push_back(temp_node);//DAG图接入结点
		DAG_symbol_to_node.insert(pair<string, int>(to_string(result), temp_node.node_number));
		//P结点的序号是新建结点的序号
		return DAG_symbol_to_node[to_string(result)];
	}
}

int Code_Process::get_A_wB_node_number(string B, string w)//返回wB结点中的序号，如果不存在则生成
{
	if (DAG_symbol_to_node.count(B))
	{//有包含B的结点
		for (int i = 0; i < DAG_map[DAG_symbol_to_node[B]].front_node.size(); i++)
		{//查看B的所有前驱结点
			if (DAG_map[DAG_map[DAG_symbol_to_node[B]].front_node[i]].operate_symbol == w)
			{//存在一B结点的前驱结点，其操作符w等于wB中的w
				return DAG_map[DAG_map[DAG_symbol_to_node[B]].front_node[i]].node_number;
			}
		}
		//B的前驱结点中没有任一结点的运算符为w，则新建wB结点
		DAG_property temp_node;
		temp_node.node_number = DAG_map.size();//新结点序号为当前DAG的长度
		temp_node.operate_symbol = w;//新结点操作符为w
		temp_node.rear_node.push_back(DAG_symbol_to_node[B]);//B结点加入新结点的后继结点
		temp_node.symbol_size = DAG_map[DAG_symbol_to_node[B]].symbol_size;
		DAG_map.push_back(temp_node);
		return temp_node.node_number;
	}
	else
	{//没有包含B的结点，也必然没有wB的结点
		DAG_property temp_node;
		temp_node.node_number = DAG_map.size();//新结点序号为当前DAG的长度
		temp_node.main_mark = B;//新结点主标记为B
		temp_node.front_node.push_back(DAG_map.size() + 1);//前驱为下一个设置的操作符为w的结点
		if ((B[0] == '\'') || (B[0] == '\"'))
			temp_node.symbol_size = 4;
		else if (B.size() == 1 && isdigit(B[0]))
			temp_node.symbol_size = 4;
		else if (B[1] == '.')
			temp_node.symbol_size = 8;
		else if (isdigit(B[0]))
			temp_node.symbol_size = 4;
		else if (symbol_table_list[function_number].symbol_property_table[get_symbol_number(B, function_number)].type->type_code == "double")
			temp_node.symbol_size = 8;
		else
			temp_node.symbol_size = 4;
		DAG_symbol_to_node.insert(pair<string, int>(B, temp_node.node_number));//B与结点的对应关系加入查找图
		DAG_map.push_back(temp_node);
		temp_node.node_number = DAG_map.size();//新结点序号为当前DAG的长度
		temp_node.operate_symbol = w;//新结点操作符为w
		temp_node.main_mark.clear();//主标记清空
		temp_node.front_node.clear();//前驱清空
		temp_node.rear_node.push_back(DAG_symbol_to_node[B]);//B结点加入新结点的后继结点
		temp_node.symbol_size = DAG_map[DAG_symbol_to_node[B]].symbol_size;
		DAG_map.push_back(temp_node);
		return temp_node.node_number;
	}
	return 0;
}

int Code_Process::get_A_BwC_node_number(string B, string C, string w)//返回BwC结点中的序号，如果不存在则生成
{
	if (DAG_symbol_to_node.count(B) && DAG_symbol_to_node.count(C))
	{//B、C均在查找图中
		for (int i = 0; i < DAG_map[DAG_symbol_to_node[B]].front_node.size(); i++)
		{//查看B的所有前驱结点
			if (DAG_map[DAG_map[DAG_symbol_to_node[B]].front_node[i]].operate_symbol == w)
			{//存在一B结点的前驱结点，其操作符等于BwC中的w
				int temp_result = DAG_map[DAG_symbol_to_node[B]].front_node[i];//记下临时结果结点
				for (int j = 0; j < DAG_map[DAG_symbol_to_node[C]].front_node.size(); j++)
				{//查看C的所有前驱结点
					if (DAG_map[DAG_map[DAG_symbol_to_node[C]].front_node[j]].operate_symbol == w)
					{//存在一C结点的前驱结点，其操作符w等于BwC中的w
						if (DAG_map[DAG_symbol_to_node[C]].front_node[j] == temp_result)
						{//B和C的某一个运算符为w的前驱是同一个结点，则BwC结点存在
							return temp_result;
						}
					}
				}
				//如果C没有前驱结点的运算符为w，或者运算符为w的C前驱结点与B不是同一结点，则需新建BwC点
				DAG_property temp_node;
				temp_node.node_number = DAG_map.size();
				//B C加入新结点后继
				temp_node.rear_node.push_back(DAG_symbol_to_node[B]);
				temp_node.rear_node.push_back(DAG_symbol_to_node[C]);
				temp_node.operate_symbol = w;//新结点的运算符为w
				if ((DAG_map[DAG_symbol_to_node[B]].symbol_size != 8) && (DAG_map[DAG_symbol_to_node[C]].symbol_size != 8))
					temp_node.symbol_size = 4;
				else
					temp_node.symbol_size = 8;
				DAG_map[DAG_symbol_to_node[C]].front_node.push_back(temp_node.node_number);//w加入C结点的前驱
				DAG_map.push_back(temp_node);
				return temp_node.node_number;
			}
		}
		//如果B没有前驱结点的运算符为w，则需新建BwC点
		DAG_property temp_node;
		temp_node.node_number = DAG_map.size();
		//B C加入新结点后继
		temp_node.rear_node.push_back(DAG_symbol_to_node[B]);
		temp_node.rear_node.push_back(DAG_symbol_to_node[C]);
		temp_node.operate_symbol = w;//新结点的运算符为w
		if ((DAG_map[DAG_symbol_to_node[B]].symbol_size != 8) && (DAG_map[DAG_symbol_to_node[C]].symbol_size != 8))
			temp_node.symbol_size = 4;
		else
			temp_node.symbol_size = 8;
		DAG_map[DAG_symbol_to_node[B]].front_node.push_back(temp_node.node_number);//w加入B结点的前驱
		DAG_map.push_back(temp_node);
		for (int j = 0; j < DAG_map[DAG_symbol_to_node[C]].front_node.size(); j++)
		{//查看C的所有前驱结点
			if (DAG_map[DAG_map[DAG_symbol_to_node[C]].front_node[j]].operate_symbol == w)
			{//存在一C结点的前驱结点，其操作符w等于BwC中的w
				return temp_node.node_number;//B C的前驱结点均存在w，直接返回
			}
		}
		DAG_map[DAG_symbol_to_node[C]].front_node.push_back(temp_node.node_number);//w加入B结点的前驱
		return temp_node.node_number;
	}
	else
	{//只要B C中有一点不在查找图中，那么一定没有BwC的点
		int flag_B = 0, flag_C = 0;//是否新建了B C结点的标志
		int insert_node_number = DAG_map.size();//用来标记插入结点的序号
		DAG_property temp_node_B, temp_node_C;
		if (!DAG_symbol_to_node.count(B))
		{//B结点不存在
			temp_node_B.node_number = insert_node_number;
			temp_node_B.main_mark = B;
			if ((B[0] == '\'') || (B[0] == '\"'))
				temp_node_B.symbol_size = 4;
			else if (B.size() == 1 && isdigit(B[0]))
				temp_node_B.symbol_size = 4;
			else if (B[1] == '.')
				temp_node_B.symbol_size = 8;
			else if (isdigit(B[0]))
				temp_node_B.symbol_size = 4;
			else if (symbol_table_list[function_number].symbol_property_table[get_symbol_number(B, function_number)].type->type_code == "double")
				temp_node_B.symbol_size = 8;
			else
				temp_node_B.symbol_size = 4;
			DAG_symbol_to_node.insert(pair<string, int>(B, temp_node_B.node_number));
			insert_node_number++;//调整为下一个插入点的序号
			flag_B = 1;
		}
		if (!DAG_symbol_to_node.count(C))
		{//C结点不存在
			temp_node_C.node_number = insert_node_number;
			temp_node_C.main_mark = C;
			if (C.size() == 1 && isdigit(C[0]))
				temp_node_C.symbol_size = 4;
			else if ((C[0] == '\'') || (C[0] == '\"'))
				temp_node_C.symbol_size = 4;
			else if (C[1] == '.')
				temp_node_C.symbol_size = 8;
			else if (isdigit(C[0]))
				temp_node_C.symbol_size = 4;
			else if (symbol_table_list[function_number].symbol_property_table[get_symbol_number(C, function_number)].type->type_code == "double")
				temp_node_C.symbol_size = 8;
			else
				temp_node_C.symbol_size = 4;
			DAG_symbol_to_node.insert(pair<string, int>(C, temp_node_C.node_number));
			insert_node_number++;//调整为下一个插入点的序号
			flag_C = 1;
		}
		DAG_property temp_node_w;
		temp_node_w.node_number = insert_node_number;
		temp_node_w.operate_symbol = w;//新结点的运算符为w
		//B C加入新结点后继
		if (flag_B)
		{
			temp_node_B.front_node.push_back(insert_node_number);
			DAG_map.push_back(temp_node_B);
		}
		if (flag_C)
		{
			temp_node_C.front_node.push_back(insert_node_number);
			DAG_map.push_back(temp_node_C);
		}
		temp_node_w.rear_node.push_back(DAG_symbol_to_node[B]);
		temp_node_w.rear_node.push_back(DAG_symbol_to_node[C]);
		if ((DAG_map[DAG_symbol_to_node[B]].symbol_size != 8) && (DAG_map[DAG_symbol_to_node[C]].symbol_size != 8))
			temp_node_w.symbol_size = 4;
		else
			temp_node_w.symbol_size = 8;
		DAG_map.push_back(temp_node_w);
		return temp_node_w.node_number;
	}
}

void Code_Process::handle_main_other_order(int node)//主标记和最新添加的从标记位置处理
{
	if (DAG_map[node].main_mark == "\0")
	{//当前主标记未赋值，只有一种情况：附加标记只有一个新加的点
		DAG_map[node].main_mark = DAG_map[node].other_mark[0];
		DAG_map[node].other_mark.pop_back();
		return;
	}
	else if (const_number_flag(DAG_map[node].main_mark))
	{//主标记为常数，无需交换
		return;
	}
	else if (const_number_flag(DAG_map[node].other_mark[DAG_map[node].other_mark.size() - 1]))
	{//新插入的从标记为常量，直接交换
		string temp_s = DAG_map[node].main_mark;
		DAG_map[node].main_mark = DAG_map[node].other_mark[DAG_map[node].other_mark.size() - 1];
		DAG_map[node].other_mark.pop_back();
		DAG_map[node].other_mark.push_back(temp_s);
		return;
	}
	else if (block_part_variable_flag(DAG_map[node].main_mark))
	{//主标记为块内局部变量，直接交换
		string temp_s = DAG_map[node].main_mark;
		DAG_map[node].main_mark = DAG_map[node].other_mark[DAG_map[node].other_mark.size() - 1];
		DAG_map[node].other_mark.pop_back();
		DAG_map[node].other_mark.push_back(temp_s);
		return;
	}
}

void Code_Process::handle_DAG()
{
	for (int i = 0; i < DAG_map.size(); i++)
	{
		if (!get_symbol_size[function_number].count(DAG_map[i].main_mark))
			get_symbol_size[function_number].insert(pair<string, int>(DAG_map[i].main_mark, DAG_map[i].symbol_size));
		if (DAG_map[i].rear_node.size() == 2)
		{
			temp_item.operation_symbol = DAG_map[i].operate_symbol;
			temp_item.first_target.operation_target_value = DAG_map[DAG_map[i].rear_node[0]].main_mark;
			temp_item.second_target.operation_target_value = DAG_map[DAG_map[i].rear_node[1]].main_mark;
			temp_item.result_target.result_target_value = DAG_map[i].main_mark;
			temp_f.push_back(temp_item);
		}
		else if (DAG_map[i].rear_node.size() == 1)
		{
			temp_item.operation_symbol = DAG_map[i].operate_symbol;
			temp_item.first_target.operation_target_value = DAG_map[DAG_map[i].rear_node[0]].main_mark;
			temp_item.second_target.operation_target_value = "_";
			temp_item.result_target.result_target_value = DAG_map[i].main_mark;
			temp_f.push_back(temp_item);
		}
		if (!DAG_map[i].other_mark.empty())
		{
			for (int j = 0; j < DAG_map[i].other_mark.size(); j++)
			{
				if (block_part_variable_flag(DAG_map[i].other_mark[j]) == 0)
				{
					temp_item.operation_symbol = "=";
					temp_item.first_target.operation_target_value = DAG_map[i].main_mark;
					temp_item.second_target.operation_target_value = "_";
					temp_item.result_target.result_target_value = DAG_map[i].other_mark[j];
					temp_f.push_back(temp_item);
				}
			}
		}
	}
}

int Code_Process::get_symbol_number(string s, int i)//查找一个字符串s在符号表i中的位置
{
	for (int j = 0; j < symbol_table_list[i].symbol_property_table.size(); j++)
	{
		if (symbol_table_list[i].symbol_property_table[j].name == s)
		{
			return j;
		}
	}
	return 0;
}