#include"code.h"
#include<iostream>

code_space::HTree* root;

std::vector<std::string> code(const std::vector<std::string>& arr)
{
	using namespace std;
	using namespace code_space;
	vector<ulli> times(128 - NUM - 1, 0);//times[i]表示ASCII码为(i+NUM)的字符出现的次数，(128-NUM-1)为ASCII码中打印字符的数量
	ulli num = 0;//记录总字符数，当总字符数达到unsigned long long int最大限度时不再计数(由于建树过程中要对权重相加，所以总字符数不能超过最大限度)
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
	HTree* htree = node[0]->tree;//构造后的哈夫曼树
	vector<string> coding(128 - NUM - 1);//coding[i]代表ASCII码为(i+NUM)的字符的编码
	generate_code(htree, coding, "");//构造哈夫曼编码
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
std::vector<std::string> decode(const std::vector<std::string>& arr) {  //根据二进制流进行解码
	std::vector<std::string> result(arr.size(), "");
	for (int i = 0; i < arr.size(); i++) {
		std::string temp = "";
		code_space::gothrough_tree(root, 0, arr[i], temp);
		result[i] = temp;
	}
	return result;
}
void code_space::gothrough_tree(HTree* treenode, int add, std::string a, std::string& s) {//遍历哈夫曼树
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
	int min1 = node[0]->weight, min2 = node[1]->weight;//权重最小的两个元素的权
	int min1_num = 0, min2_num = 1;//最小的两个元素的标号
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
	HTree* root = new HTree{ (char)200,node[min1_num]->tree,node[min2_num]->tree };//每次合并两个权值最小的结点
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