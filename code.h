/* 
*  ����˵�������� vector<string> code(const vector<string>)
*  ���룺�ַ�����vector
*  �����ÿ���ַ�����Ӧ�Ĺ���������
*  ע������ͬʱ����ļ�decode.txt�����д洢�˽��뷽ʽ
*/

#ifndef CODE_H
#define CODE_H
#define NUM 0
#include <vector>
#include <string>
#include <climits>
#include <fstream> 

std::vector<std::string> code(const std::vector<std::string>&);//���������뺯��

std::vector<std::string> decode(const std::vector<std::string>&);


namespace code_space
{
	typedef unsigned long long int ulli;
	struct HTree//��������
	{
		char data;
		HTree* lchild, * rchild;
	};
	struct Node//�������������������Ҫ�õ��Ľṹ��
	{
		HTree* tree;
		ulli weight;//Ȩ��
	};
	void generate_tree(std::vector<Node*>&);//������������ĺ���
	void generate_code(HTree*, std::vector<std::string>&, std::string);//�������������
	void gothrough_tree(HTree* treenode, int add, std::string a, std::string& s);//���ݶ�������������������
}

#endif