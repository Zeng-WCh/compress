#include <string>
#include <iostream>
#include <fstream>
#include <bitset>
#include <cstring>
#include <map>
#include "file.h"
#include "htree.h"
#include "decode.h"

const std::string help[] = {
	" -> Type exit to quit",
	" -> Type zip {foldername} {outputname} to compress a folder",
	" -> Type unzip {compressed file} {outputfolder} to decompress",
	" -> Only Support text file coded by ASCII and filename extension is \"txt\""
};

inline void printhelp() {
	for (const auto& x : help) {
		std::cout << x << std::endl;
	}
}

void readcommand(std::string& s) {
	char* buf = new char[1024];
	memset(buf, 0, sizeof(char) * 1024);
	size_t count = 0;
	int tmp = getchar();
	while (tmp != ' ' && tmp != '\n') {
		buf[count] = tmp;
		count++;
		tmp = getchar();
	}
	buf[count] = '\0';
	if (!s.empty()) {
		s.clear();
	}
	s.resize(count);
	for (int i = 0; i <= count; ++i) {
		s[i] = buf[i];
	}
	delete[]buf;
	return;
}

inline void printcli() {
	std::cout << ">>> ";
}

int main() {
	std::string command;
	printhelp();
	printcli();
	readcommand(command);
	while (command != "exit") {
		if (command == "zip") {
			std::string dir, des;
			std::cin >> dir >> des;
			getchar();
			file::getalltxt(dir);
			std::vector<std::string> res;
			file::readres(res);
			std::string tmp;
			bool isrelative = !(file::switchtype(dir));
			auto local = file::getlocaldir();
			std::vector<std::string> oris;
			if (isrelative) {
				local += '\\';
				local += dir;
				//std::cout << local << std::endl;
			}
			for (int i = 0; i < (int)res.size(); ++i) {
				file::readfromfile(res[i], tmp);
				if (isrelative) {
					int n = local.length();
					res[i] = res[i].substr(n + 1);
				}
				else {
					int n = dir.length();
					res[i] = res[i].substr(n + 1);
				}
				//res[i] += " ";
				//res[i] += tmp;
				oris.push_back(tmp);
				tmp.clear();
			}
			HTree htree;
			htree.code(oris);
			std::vector<std::string> ans;
			transtohaff(oris, ans, htree);
			file::save(ans, des);
			std::ofstream outp;
			outp.open(des, std::ios::app | std::ios::binary);
			auto it = oris.begin();
			while (it != oris.end()) {
				outp << (unsigned long long)(*it).length() << ' ';
				++it;
			}
			outp.close();
			htree.save(des);
			file::savefiledir(res, des);
			std::cout << "Zip Complete" << std::endl;
		}
		else if (command == "unzip") {
			FILE* p = NULL;
			std::string ans;
			std::string dir, des;
			std::cin >> dir >> des;
			getchar();
			bool isrelative = !(file::switchtype(dir));//是否是相对路径
			std::string local = file::getlocaldir();
			if (isrelative) {
				local += "\\";
				local += dir;
				p = fopen(local.c_str(), "rb");
			}
			else {
				p = fopen(dir.c_str(), "rb");
			}
			if (p!=NULL) {
				unsigned long long val[2]; 
				fread(val, sizeof(unsigned long long), 2, p);
				auto s = val[0];
				auto add = val[1];
				auto tmpbul = new unsigned long long[s];
				fread(tmpbul, sizeof(unsigned long long), s, p);
				fclose(p);
				p = NULL;
				if (isrelative) {
					p = fopen(local.c_str(), "r");
				}
				else {
					p = fopen(dir.c_str(), "r");
				}
				fseek(p, sizeof(unsigned long long) * (s + 2), SEEK_CUR);
				unsigned long long filelists;
				fscanf(p, "%lld", &filelists);
				std::vector<unsigned long long> filesize(filelists);
				unsigned long long tmp;
				for (unsigned long long i = 0; i < filelists; ++i) {
					fscanf(p, "%lld", &tmp);
					filesize[i] = tmp;
				}
				std::map<std::string,int>mp;
				readhaff(mp, p);
				char filebuf[1024];
				std::vector<std::string> filesdir;
				for (unsigned long long int i = 0; i < filelists; ++i) {
					fscanf(p, "%1024s", filebuf);
					std::string ftmp(filebuf);
					filesdir.push_back(ftmp);
				}
				std::string bitstr;
				for (size_t i = 0; i < s; ++i) {
					std::bitset<file::width> tmp(tmpbul[i]);
					bitstr += (tmp.to_string());
				}
				bitstr = bitstr.substr(0, bitstr.length() - add);
				std::string out;
				decode(mp, bitstr, out);
				//std::cout << out << std::endl;
				/*
				for (const auto& x : filesdir) {
					std::cout << x << std::endl;
				}
				for (const auto& x : filesize) {
					std::cout << x << std::endl;
				}
				*/
				auto it1 = filesdir.begin();
				auto it2 = filesize.begin();
				bool desrelative = !(file::switchtype(des));
				local = file::getlocaldir();
				std::ofstream output;
				std::string writedir;
				if (desrelative) {
					local += "\\";
					local += des;
					writedir = local;
				}
				else {
					writedir = des;
				}
				while (it1 != filesdir.end()) {
					auto wr1 = writedir + "\\" + (*it1);
					std::cout << wr1 << std::endl;
					output.open(wr1);
					if (!output.is_open()) {
						auto name = file::getfilename(wr1);
						auto newfolder = wr1.substr(0, wr1.length() - name.length());
						char mkdircom[1024]="cmd /r mkdir ";
						strcat(mkdircom, newfolder.c_str());
						system(mkdircom);
						output.open(wr1);
					}
					output << out.substr(0,(*it2));
					out = out.substr((*it2));
					output.close();
					++it1;
					++it2;
				}
				std::cout << "Unzip Complete" << std::endl;
				fclose(p);
			}
			else if (command == "cls" || command == "clean" || command == "clear") {
				system("cls");
			}
			else if (command == "help") {
				printhelp();
			}
		}
		command.clear();
		printcli();
		readcommand(command);
	}
	std::cout << "Goodbye" << std::endl;
	return 0;
}