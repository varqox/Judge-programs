#include "functions.h"
#include <string>
#include <algorithm>
#include <cstdio>
#include <cstring>
#include <dirent.h>

#define eprintf(args...) fprintf(stderr, args)

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
	bool minus = false;
	if (a < 0) {
		minus = true;
		a = -a;
	}
	while (a > 0) {
		w += static_cast<char>(a % 10 + '0');
		a /= 10;
	}
	if (minus)
		w += "-";
	if (w.empty())
		w = "0";
	else
		std::reverse(w.begin(), w.end());
return w;
}

string dmyto_string(double a) {
	char buffer[256];
	sprintf(buffer, "%.1lf", a);
return string(buffer);
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

bool isPositiveNum(const string& s) {
	for (size_t i = 0; i < s.size(); ++i)
		if (!isdigit(s[i]))
			return false;
	return true;
}

// Convert string to unsigned long long
unsigned long long atoull(const string& s) {
	unsigned long long res = 0;
	for (size_t i = 0; i < s.size(); ++i) {
		res *= 10;
		res += s[i] - '0';
	}
	return res;
}

bool isNum(const string& s, long long down, long long up) {
	if (isPositiveNum(s)) {
		long long x = atoull(s);
		return down <= x && x <= up;
	}
	return false;
}

void try_set(std::pair<int, int>& p, const string& oper, const string& val, int mn, int mx) {
	if (oper == "<") {
		if (isNum(val, mn+1, mx+1) && p.first <= static_cast<int>(atoull(val)-1))
			p.second = atoull(val)-1;
		else
			eprintf("Wrong value: '%s'\n", val.c_str());
	} else if (oper == ">") {
		if (isNum(val, mn-1, mx-1) && p.second >= static_cast<int>(atoull(val)+1))
			p.first = atoull(val)+1;
		else
			eprintf("Wrong value: '%s'\n", val.c_str());
	} else if (oper == "=") {
		if (isNum(val, mn, mx))
			p.first = p.second = atoull(val);
		else
			eprintf("Wrong value: '%s'\n", val.c_str());
	} else
		eprintf("Unknown operator: '%s'\n", oper.c_str());
}
