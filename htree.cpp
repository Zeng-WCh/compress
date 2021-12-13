#include "htree.h"
#include <string>
#include <vector>
#include <queue>
#include <fstream>
#include <climits>
#include <cstring>

void calc(const std::string& res, std::vector<Pair>& ans) {
	for (const auto &x:res) {
		auto it = ans.begin();
		while (it != ans.end()) {
			if ((*it).val == x) {
				if((*it).cost<ULLONG_MAX)
					++(*it).cost;
				break;
			}
			++it;
		}
		if (it == ans.end()) {
			ans.push_back({ x,1 });
		}
	}
}

void transtohaff(const std::vector<std::string>& res, std::vector<std::string>& ans, const HTree& htree){
	if (!ans.empty()) {
		ans.clear();
	}
	ans.resize(res.size());
	int count = 0;
	for (const auto& x : res) {
		for (const auto& c : x) {
			ans[count] += htree.find(c);
		}
		++count;
	}
}

HTree::HTree() {
	root = nullptr;
	cache.clear();
}

void HTree::travel(std::vector<node*>& list,node *it)const {
	if (it->lchild) {
		travel(list, it->lchild);
	}
	list.push_back(it);
	if (it->rchild) {
		travel(list, it->rchild);
	}
}

void HTree::generate_code(node* root, std::vector<struct code>& list, std::string tmp) const{
	if (root->val != 0) {
		list.push_back({ root->val,tmp });
	}
	if (root->lchild) {
		generate_code(root->lchild, list, tmp + "0");
	}
	if (root->rchild) {
		generate_code(root->rchild, list, tmp + "1");
	}
}

void HTree::update_cache(){
	if (!cache.empty()) {
		cache.clear();
	}
	get_code(cache);
}

HTree::~HTree() {
	cache.clear();
	std::vector<node*> list;
	travel(list, root);
	for (size_t i = 0; i < list.size(); ++i) {
		delete list[i];
		list[i] = nullptr;
	}
	root = nullptr;
}

void HTree::code(const std::vector<std::string>& res) {
	std::priority_queue<node*, std::vector<node*>, comp> q;
	std::vector<Pair> s;
	for (const auto& x : res) {
		calc(x, s);
	}
	for (int i = 0; i < (int)s.size(); ++i) {
		q.push(new node(s[i].val,s[i].cost));
	}
	while (q.size() != 1) {
		auto t1 = q.top();
		q.pop();
		auto t2 = q.top();
		q.pop();
		q.push(new node(0,t1->cost+t2->cost,t1,t2));
	}
	if (root != nullptr) {
		std::vector<node*> list;
		travel(list, root);
		for (int i = 0; i < (int)list.size(); ++i) {
			delete list[i];
			list[i] = nullptr;
		}
		root = nullptr;
	}
	root = q.top();
	update_cache();
	return;
}

int HTree::find(const std::string& bit) const {
	auto it = root;
	int i = 0;
	int n = bit.length();
	while (it&&i<n) {
		switch (bit[i]) {
		case '0':
			it = it->lchild;
			break;
		case '1':
			it = it->rchild;
			break;
		default:
			break;
		}
		++i;
	}
	if (it && i == n) {
		return it->val;
	}
	return -1;
}

std::string HTree::find(const char c) const{
	for (const auto& x : cache) {
		if (x.val == c) {
			return x.codes;
		}
	}
}

void HTree::get_code(std::vector<struct code>& list) const{
	generate_code(root, list, "");
}

void HTree::save(const std::string& filename) const{
	std::vector<struct code> res;
	get_code(res);
	std::ofstream out;
	out.open(filename, std::ios::app | std::ios::binary);
	if (out.is_open()) {
		out << res.size() << '\n';
		auto it = res.begin();
		while (it != res.end()) {
			out << (int)(*it).val << ' ' << (*it).codes << '\n';
			++it;
		}
	}
	out.close();
	return;
}