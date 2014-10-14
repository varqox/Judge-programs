#include "functions.h"
#include <string>
#include <algorithm>
#include <cstdio>
#include <cstring>
#include <dirent.h>

using std::string;

string getNextArg(const char* line, size_t& i, size_t s) {
	string res;
	while (line[i] == ' ' || line[i] == '\t' || line[i] == '\n')
		++i;
	for (; i < s; ++i) {
		if (line[i] == '\\') {
			if (i + 1 < s)
				res += line[++i];
			continue;
		}
		if (line[i] == ' ' || line[i] == '\t' || line[i] == '\n')
			break;
		res += line[i];
	}
	return res;
}

string tolower(string str) {
	for (size_t i = 0, s = str.size(); i < s; ++i)
		str[i] = tolower(str[i]);
	return str;
}

string myto_string(long long a) {
	string w;
	while (a > 0) {
		w += static_cast<char>(a % 10 + '0');
		a /= 10;
	}
	if (w.empty())
		w = "0";
	else
		std::reverse(w.begin(), w.end());
return w;
}

void remove_trailing_spaces(string& s) {
	string::iterator begin = s.end();
	while (begin != s.begin() && isspace(*(begin-1)))
		--begin;
	s.erase(begin, s.end());
}

bool file_exists(const std::string& file) {
	FILE *f = fopen(file.c_str(), "r");
	if (f) {
		fclose(f);
		return true;
	}
	return false;
}

int remove_r(const char* path) {
	DIR* directory;
	dirent* current_file;
	string tmp_dir_path = path;
	if (*tmp_dir_path.rbegin() != '/')
		tmp_dir_path += '/';
	if ((directory = opendir(path)))
	while ((current_file = readdir(directory)))
		if (strcmp(current_file->d_name, ".") && strcmp(current_file->d_name, ".."))
			remove_r((tmp_dir_path + current_file->d_name).c_str());
	return remove(path);
}

bool isNum(const std::string& s) {
	if (s.empty())
		return false;
	for (size_t i = 0, len = s.size(); i < len; ++i)
		if (!isdigit(s[i]))
			return false;
	return true;
}

ArgParser::ArgParser(const std::string& s) : pos_(0), size_(s.size()), arg_(new char[size_ + 1]) {
	strcpy(arg_, s.c_str());
}
ArgParser::ArgParser(const ArgParser& ap) : pos_(0), size_(ap.size_), arg_(new char[size_ + 1]) {
	strcpy(arg_, ap.arg_);
}

ArgParser& ArgParser::operator=(const ArgParser& ap) {
	if (arg_)
		delete[] arg_;
	pos_ = 0;
	size_ = ap.size_;
	arg_ = new char[size_ + 1];
	strcpy(arg_, ap.arg_);
	return *this;
}

string ArgParser::getNextArg() { return ::getNextArg(arg_, pos_, size_); }
