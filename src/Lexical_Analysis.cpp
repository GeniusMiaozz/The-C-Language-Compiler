#include "Lexical_Analysis.h"

Lexical_Analysis::Lexical_Analysis()
{
	token_list.resize(0);
	token_type.resize(0);
	const_symbol.resize(0);
	const_number.resize(0);
	const_char.resize(0);
	const_string.resize(0);
}

Lexical_Analysis::~Lexical_Analysis() 
{
	code_file.close();
};

void Lexical_Analysis::Generate_Token() //词法分析器
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
			break;
		}
		code_file >> code_char;//读入下一个字符
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
				token[i++] = code_char;									   //将读入字符存入token串首字符,
			//cout << token << endl;
		}
	}
	token_list.push_back("#");
	token_type.push_back("结束符");
	token_number.push_back(1);
}

int Lexical_Analysis::change_state(int state, char ch) //状态转换
{
	switch (state)
	{
	case 0: //初始态
	{
		switch (ch)
		{
		case ' ':
			return 0;
		case '\n':
			return 0;
		case '\t':
			return 0;
		case '\r':
			return 0;
		case 'a':
			return 1;
		case '0':
			return 2;
		case '(':
			return 3;
		case ')':
			return 3;
		case '[':
			return 3;
		case ']':
			return 3;
		case '{':
			return 3;
		case '}':
			return 3;
		case '\'':
			return 9;
		case '\"':
			return 10;
		case ',':
			return 11;
		case ';':
			return 12;
		case '<':
			return 13;
		case '>':
			return 15;
		case '=':
			return 17;
		case '+':
			return 19;
		case '-':
			return 21;
		case '*':
			return 23;
		case '/':
			return 24;
		case '|':
			return 31;
		case '&':
			return 33;
		case '!':
			return 37;
		case ':':
			return 39;
		case '^':
			return 40;
		case '#':
			return 40;
		case '%':
			return 40;
		case '~':
			return 40;
		case '$':
			return 40;
		case '@':
			return 40;
		case '.':
			return 40;
		case '?':
			return 40;
		case '_':
			return 1;
		case '\\':
			return 47;
		default:
			break;
		}
	}
	case 1: //读字母
	{
		switch (ch)
		{
		default:
			return 101;
			break;
		case 'a':
			return 1;
		case '0':
			return 1;
		case '_':
			return 1;
			break;
		}
	}
	case 2: //读数字
		switch (ch)
		{
		default:
			return 104;
		case '0':
			return 2;
		case '.':
			return 27;
		case 'e':
			return 28;
		}
	case 3: //各种括号
		return 102;
	case 9: //识别字符
		switch (ch)
		{
		case '0':
			return 9;
		case 'a':
			return 9;
		case '\'':
			return 25;
		default:
			break;
		}
	case 25:
		return 105;
	default:
		break;
	case 10: //识别字符串
		switch (ch)
		{
		case '0':
			return 10;
		case 'a':
			return 10;
		case '\"':
			return 26;
		default:
			break;
		}
	case 26:
		return 106;
	case 11: //逗号
		return 102;
	case 12: //分号
		return 102;
	case 13: //第一个小于号
		switch (ch)
		{
		default: //<
			return 102;
		case '=':
			return 14;
		case '<':
			return 35;
		}
	case 14: //<=
		return 102;
	case 15: //第一个大于号
		switch (ch)
		{
		default: //>
			return 102;
		case '=':
			return 16;
		case '>':
			return 36;
		}
	case 16: //>=
		return 102;
	case 17: //第一个等于号
		switch (ch)
		{
		default: //=
			return 102;
		case '=':
			return 18;
		}
	case 18: //==
		return 102;
	case 19:
		switch (ch)
		{
		case '+':
			return 20;
		default: //+
			return 102;
		}
	case 20: //++
		return 102;
	case 21:
		switch (ch)
		{
		case '-':
			return 22;
		case '>':
			return 48;
		default: //-
			return 102;
		}
	case 22: //--
		return 102;
	case 23: //乘号
		return 102;
	case 24: //除号
		switch (ch)
		{
		case '/'://"//"
			return 41;
		case '*':
			return 42;
		default:
			return 102;
		}
	case 27:
		switch (ch)
		{
		case '0':
			return 27;
		default:
			return 104;
		}
	case 28:
		switch (ch)
		{
		case '0':
			return 30;
		case '-':
			return 29;
		default:
			return 104;
		}
	case 29:
		switch (ch)
		{
		case '0':
			return 29;
		default:
			return 104;
		}
	case 30: //指数的幂为正
		switch (ch)
		{
		case '0':
			return 30;
		default:
			return 104;
		}
	case 31://"|"
		switch (ch)
		{
		case '|':
			return 32;
		default:
			return 102;
		}
	case 32://"||"
		return 102;
	case 33://"&"
		switch (ch)
		{
		case '&':
			return 34;
		default:
			return 102;
		}
	case 34://"&&"
		return 102;
	case 35://"<<"
		return 102;
	case 36://">>"
		return 102;
	case 37://"!"
		switch (ch)
		{
		case '=':
			return 38;
		default:
			return 102;
		}
	case 38://"!="
		return 102;
	case 39://":"
		switch (ch)
		{
		case ':':
			return 46;
		default://":"
			return 102;
		}
	case 40://"^","#","%","~","@","$","?","."
		return 102;
	case 41://"//"
		return 102;
	case 42://"/*"
		switch (ch)
		{
		case '*':
			return 43;
		default:
			return 42;
		}
	case 43://注释结束读到第一个‘*’
		switch (ch)
		{
		case '/':
			return 44;
		default:
			return 42;
		}
	case 44://"*/"
		return 102;
	case 45://"."
		return 102;
	case 46://"::"
		return 102;
	case 47://"\\"
		switch (ch)
		{
		case '\'':
			return 47;
		case '\"':
			return 47;
		default:
			return 102;
		}
	case 48://"->"
		return 102;
	}

	return 0;
}


void Lexical_Analysis::check_code(int state, char* token) //生成单词
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

int Lexical_Analysis::search_key(char* token, vector<string>* key, string type) //查找并加入token序列
{
	for (vector<int>::size_type i = 0; i < (*key).size(); i++) //遍历查找表
	{
		if ((*key)[i] == token) //匹配
		{
			token_list.push_back(token); //存入token序列
			token_type.push_back(type);
			token_number.push_back(i);
			return 1; //返回
		}
	}
	if (type != "关键字" && type != "关键符号") //未匹配，必定是用户定义符号
	{
		(*key).push_back(token);	 //加入查找表
		token_list.push_back(token); //存入token序列
		token_type.push_back(type);
		token_number.push_back((*key).size());
	}
	return 0;
}

void Lexical_Analysis::print_token_list()
{
	{
		cout << "类型\t序号\t单词" << endl;
		for (vector<int>::size_type i = 0; i < token_list.size(); i++)
		{
			cout << token_type[i] << "\t" << token_number[i] << "\t" << token_list[i] << endl;
		}
		cout << endl;
	}
}