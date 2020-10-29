#pragma once
#include <bits/stdc++.h>
#define token_max_size 1000            //����token������󳤶�

using namespace std;

/*�м����Ķ��壬ǰ�����˼�Ľӿ�*/

/*���ű�Ķ���*/
typedef struct
{
	string type_code;				  //������룬�硰���Ρ��������͡�
	void* information_pointer = NULL; //��Ϣ��ָ�룬������������ָ��NULL������ָ�������ȵ�
} type_property;					  //�������������

typedef struct
{
	int bottom_number;//�����½�
	int top_number;//�����Ͻ�
	type_property* type_information;//������ÿ��Ԫ�صĵ�������Ϣ����int a[10]ָ��int��int a[5][10]ָ����泤��Ϊ10�����飬
	int array_size;//���鳤��
} array_table;//�����

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
} constant_table; //������

typedef struct
{
	string ID;			//�ṹ���б����ı�ʶ��	
	int OFF = 0;			//ƫ����
	type_property* tp = NULL;//����ָ��
	constant_table* vp = NULL;//��ֵָ�룬ָ������
} structure_property;//�ṹ���������

typedef struct
{
	int num;			//�ṹ���ڱ�������
	int total_OFF;       //��ƫ����
	vector<structure_property> structure_property_list; //�ṹ�������
} structure_table;//�ṹ��

typedef struct
{

} function_table;//������

typedef struct
{
	string name;		  //��ʶ��������
	type_property* type = NULL;   //��ʶ��������ָ��
	string kind;		  //��ʶ����������Ϣ���硰��������������
	int offset = 0;//��ַƫ����
	void* address = NULL; //��ʶ���Ľ��ͱ��糣��ָ����������ָ������
} symbol_property;		  //���ű��ܱ���ÿ����ʶ��������

typedef struct symbol_table
{
	symbol_table* header = NULL;                   //ÿ�ź������ű��ͷָ�룬ָ����øú����ĺ����ķ��ű�
	int total_size = 0;                            //һ�ź������ű���ܳ���
	vector<symbol_property> symbol_property_table; //���ű��ܱ���ÿ����ʶ�������Ա�
} symbol_table;                                    //���ű�ÿ������һ�ŷ��ű�

/*��Ԫʽ�Ķ���*/
typedef struct
{
	string operation_target_value;  //��һ����������ֵ
	bool active_information = false;//��һ���������Ļ�Ծ��Ϣ
} first_operation_target;		    //��һ��������������

typedef struct
{
	string operation_target_value;  //�ڶ�����������ֵ
	bool active_information = false;//�ڶ����������Ļ�Ծ��Ϣ
} second_operation_target;		    //�ڶ���������������

typedef struct
{
	string result_target_value;     //�����������ֵ
	bool active_information = false;//����������Ļ�Ծ��Ϣ
} operation_result;				    //���������������

typedef struct
{
	string basic_block_flag = "˳���";    //��ǻ��������ڻ����
	string operation_symbol;			   //�������
	first_operation_target first_target;   //��һ��������������
	second_operation_target second_target; //�ڶ���������������
	operation_result result_target;		   //������������
} four_item_production;		               //��Ԫʽ���е�����