/* 
*  程序说明：函数 vector<string> code(const vector<string>)
*  输入：字符串的vector
*  输出：每个字符串对应的哈夫曼编码
*  注：程序同时输出文件decode.txt，其中存储了解码方式
*/

#ifndef CODE_H
#define CODE_H
#define NUM 0
#include <vector>
#include <string>
#include <climits>
#include <fstream> 

std::vector<std::string> code(const std::vector<std::string>&);//哈夫曼编码函数

std::vector<std::string> decode(const std::vector<std::string>&);


namespace code_space
{
	typedef unsigned long long int ulli;
	struct HTree//哈夫曼树
	{
		char data;
		HTree* lchild, * rchild;
	};
	struct Node//构造哈夫曼树过程中需要用到的结构体
	{
		HTree* tree;
		ulli weight;//权重
	};
	void generate_tree(std::vector<Node*>&);//构造哈夫曼树的函数
	void generate_code(HTree*, std::vector<std::string>&, std::string);//构造哈夫曼编码
	void gothrough_tree(HTree* treenode, int add, std::string a, std::string& s);//根据二进制流遍历哈夫曼树
}

#endif