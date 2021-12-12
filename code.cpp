#include"code.h"
#include<iostream>

code_space::HTree* root;

std::vector<std::string> code(const std::vector<std::string>& arr)
{
	using namespace std;
	using namespace code_space;
	vector<ulli> times(128 - NUM - 1, 0);//times[i]��ʾASCII��Ϊ(i+NUM)���ַ����ֵĴ�����(128-NUM-1)ΪASCII���д�ӡ�ַ�������
	ulli num = 0;//��¼���ַ����������ַ����ﵽunsigned long long int����޶�ʱ���ټ���(���ڽ���������Ҫ��Ȩ����ӣ��������ַ������ܳ�������޶�)
	for (int i = 0; i < arr.size(); i++)
	{
		for (int j = 0; j < arr[i].length(); j++)
		{
			times[arr[i][j] - NUM]++;
			num++;
			if (ULLONG_MAX == num)
				break;
		}
		if (ULLONG_MAX == num)
			break;
	}
	vector<Node*> node(128 - NUM - 1);
	for (int i = 0; i < node.size(); i++)
	{
		HTree* temp = new HTree{ char(NUM + i),NULL,NULL };
		node[i] = new Node{ temp,times[i] };
	}
	generate_tree(node);
	HTree* htree = node[0]->tree;//�����Ĺ�������
	vector<string> coding(128 - NUM - 1);//coding[i]����ASCII��Ϊ(i+NUM)���ַ��ı���
	generate_code(htree, coding, "");//�������������
	vector<string> result(arr.size(), "");
	ofstream outfile;
	outfile.open("decode.txt");
	for (int i = 0; i < coding.size(); i++)
		outfile << (i + NUM) << ' ' << coding[i] << endl;
	outfile.close();
	for (int i = 0; i < arr.size(); i++)
		for (int j = 0; j < arr[i].size(); j++)
			result[i] += coding[arr[i][j] - NUM];
	return result;
}
std::vector<std::string> decode(const std::vector<std::string>& arr) {  //���ݶ����������н���
	std::vector<std::string> result(arr.size(), "");
	for (int i = 0; i < arr.size(); i++) {
		std::string temp = "";
		code_space::gothrough_tree(root, 0, arr[i], temp);
		result[i] = temp;
	}
	return result;
}
void code_space::gothrough_tree(HTree* treenode, int add, std::string a, std::string& s) {//������������
	if (add > a.length()) {
		return;
	}
	if (treenode->data != (char)200) {
		s += treenode->data;
		gothrough_tree(root, add, a, s);
	}
	else {
		if (a[add] == '0') {
			if (treenode->lchild) {
				gothrough_tree(treenode->lchild, add + 1, a, s);
			}
		}
		else {
			if (treenode->rchild) {
				gothrough_tree(treenode->rchild, add + 1, a, s);
			}
		}
	}
}
void code_space::generate_tree(std::vector<Node*>& node)
{
	if (node.size() == 1)
		return;
	int min1 = node[0]->weight, min2 = node[1]->weight;//Ȩ����С������Ԫ�ص�Ȩ
	int min1_num = 0, min2_num = 1;//��С������Ԫ�صı��
	if (min1 > min2)
	{
		min1 = node[1]->weight;
		min2 = node[0]->weight;
		min1_num = 1;
		min2_num = 0;
	}
	for (int i = 2; i < node.size(); i++)
		if (node[i]->weight <= min1)
		{
			min2 = min1;
			min1 = node[i]->weight;
			min2_num = min1_num;
			min1_num = i;
		}
		else if (node[i]->weight < min2)
		{
			min2 = node[i]->weight;
			min2_num = i;
		}
	HTree* root = new HTree{ (char)200,node[min1_num]->tree,node[min2_num]->tree };//ÿ�κϲ�����Ȩֵ��С�Ľ��
	ulli root_weight = min1 + min2;
	Node* new_node = new Node{ root,root_weight };
	int a = min1_num, b = min2_num;
	if (a > b)
	{
		a = min2_num;
		b = min1_num;
	}
	delete node[a];
	node[a] = new_node;
	node.erase(node.begin() + b);
	generate_tree(node);
}
void code_space::generate_code(HTree* htree, std::vector<std::string>& coding, std::string temp)
{
	if (htree->data != (char)200)
		coding[htree->data - NUM] = temp;
	if (htree->lchild != NULL)
		generate_code(htree->lchild, coding, temp + "0");
	if (htree->rchild != NULL)
		generate_code(htree->rchild, coding, temp + "1");
}