#ifndef FILE_H
#define FILE_H

#include <string>
#include <vector>
#include <bitset>
#include <fstream>
#include <cstring>

namespace file {
	const unsigned int width = 64;

	const char dira[] = "cmd /r dir ";
	const char dirb[] = "\\*.txt /s/b >> dir.tmp";
	const char dirname[] = "dir.tmp";

	void savefiledir(const std::vector<std::string>&, const std::string&);

	template<size_t _size>
	inline void savebittofile(const std::bitset<_size>& s, const std::string& filename) {
		std::fstream out;
		out.open(filename, std::ios::app);
		if (out.is_open()) {
			out << s.to_ullong() << ' ';
		}
		out.close();
		return;
	}

	void save(const std::vector<std::string>& des, const std::string& filename);//�ӿ�

	inline bool switchtype(const std::string& s) { //true��ʾ����·����false��ʾ���·��
		for (int i = 0; i < (int)s.length(); ++i) {
			if (s[i] == ':')
				return true;
		}
		return false;
	}

	std::string getlocaldir();//��ȡ����ǰ����·��

	inline void getalltxt(const std::string& dir) {//��ȡ·�������е�txt�ļ�
		std::string local;
		if (!switchtype(dir)) {
			local = getlocaldir();
			local += "\\";
		}
		const char* buf = dir.c_str();
		const char* buf1 = local.c_str();
		char* buffer = new char[(int)dir.length() + 2];
		std::strcpy(buffer, buf);
		char command[1024] = { 0 };
		std::strcat(command, dira);
		std::strcat(command, buf1);
		std::strcat(command, buffer);
		std::strcat(command, dirb);
		system(command);
	}

	void readres(std::vector<std::string>& des);

	inline std::string getfilename(const std::string& filedir) { //��ø��ļ�·����Ҫѹ�����ļ�����
		int i = 0;
		for (i = (int)filedir.size() - 1; i >= 0; --i) {
			if (filedir[i] == '\\') {
				break;
			}
		}
		auto name = filedir.substr(i);
		return name;
	}

	inline void readfromfile(const std::string& filedir, std::string& ans) {//filedirΪһ�ļ�·�������ж�ȡ�ַ����洢��ans��
		std::ifstream in;
		in.open(filedir);
		if (in.is_open()) {
			char t;
			while (in.get(t)) {
				ans.push_back(t);
			}
		}
	}

	inline std::string cat(const std::vector<std::string>& res) {//��res�е��ַ�ƴ������
		std::string ans;
		auto it = res.begin();
		while (it != res.end()) {
			ans += *it;
			++it;
		}
		return ans;
	}
};

#endif