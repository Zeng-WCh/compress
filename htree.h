#ifndef HTREE_H
#define HTREE_H

#include <string>
#include <vector>

struct node {
	char val;
	unsigned long long cost;
	node* lchild, * rchild;
	node(char val = '\0', unsigned long long cost = -1, node* lchild = nullptr, node* rchild = nullptr) :val(val), cost(cost), lchild(lchild), rchild(rchild) {};
};

struct comp {
	bool operator() (node* t1, node* t2) {
		return t1->cost > t2->cost;
	}
};

struct Pair {
	char val;
	unsigned long long cost;
	Pair(char val = 0, unsigned long long cost = 0) :val(val), cost(cost) {};
};

struct code {
	char val;
	std::string codes;
	code(char val = 0, const std::string& codes = "") :val(val), codes(codes) {};
};

void calc(const std::string&, std::vector<Pair> &);

class HTree {
private:
	node* root;
	std::vector<struct code> cache;
protected:
	void travel(std::vector<node*>&,node*)const;
	void generate_code(node* root, std::vector<struct code>&, std::string)const;
	void update_cache();
public:
	HTree();
	~HTree();
	void code(const std::vector<std::string>& res);
	int find(const std::string& bit)const;
	std::string find(const char)const;
	void get_code(std::vector<struct code>&)const;
	void save(const std::string& filename)const;
};

void transtohaff(const std::vector<std::string>& res, std::vector<std::string>& ans, const HTree&);

#endif