#include "functions.h"
#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include <cstdio>
#include <cstring>
#include <dirent.h>
#include <sys/stat.h>

#define eprintf(args...) fprintf(stderr, args)

using std::string;
using std::map;
using std::vector;

std::mt19937_64 gen__(std::chrono::system_clock::now().time_since_epoch().count());

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

long long atoll(const string& s) { return atoll(s.c_str()); }

bool isNum(const string& s, long long down, long long up) {
	if (isPositiveNum(s)) {
		long long x = atoull(s);
		return down <= x && x <= up;
	}
	return false;
}

template<class T>
static void try_set(std::pair<T, T>& p, const string& oper, const string& val) {
	if (oper == "<") {
		if (isNum(val, p.first + 1, p.second + 1) && p.first <= atoll(val) - 1)
			p.second = atoull(val) - 1;
		else
			eprintf("Wrong value: '%s'\n", val.c_str());
	}
	else if (oper == "<=") {
		if (isNum(val, p.first, p.second) && p.first <= atoll(val))
			p.second = atoull(val);
		else
			eprintf("Wrong value: '%s'\n", val.c_str());
	} else if (oper == ">") {
		if (isNum(val, p.first - 1, p.second - 1) && p.second >= atoll(val) + 1)
			p.first = atoull(val) + 1;
		else
			eprintf("Wrong value: '%s'\n", val.c_str());
	}  else if (oper == ">=") {
		if (isNum(val, p.first, p.second) && p.second >= atoll(val))
			p.first = atoull(val);
		else
			eprintf("Wrong value: '%s'\n", val.c_str());
	} else if (oper == "=") {
		if (isNum(val, p.first, p.second))
			p.first = p.second = atoull(val);
		else
			eprintf("Wrong value: '%s'\n", val.c_str());
	} else
		eprintf("Unknown operator: '%s'\n", oper.c_str());
}

bool isOperator(const string& s) {
	return s == "<" || s == ">" || s == "<=" || s == ">=" || s == "="; // We don't use !=
}

template<class T>
static string __parseArgLimits(const string& args, map<string, std::pair<T, T>>& limits) {
	string other_args, new_args;
	new_args.reserve(args.size()); // For more efficiency
	for (size_t i = 0; i < args.size(); ++i) {
		if (args[i] == '<' || args[i] == '>') {
				new_args += string(" ") + args[i];
			if (i < args.size() && args[i+1] == '=') {
				new_args += "=";
				++i;
			}
			new_args += " ";
		} else if (args[i] == '=')
			new_args += string(" ") + args[i] + " ";
		else
			new_args += args[i];
	}
	ArgParser ap(new_args);
	// Parsing arguments
	string x;
	vector<string> arg_arr;
	while (x = ap.getNextArg(), x.size())
		arg_arr.push_back(x);
	for (size_t i = 0, len = arg_arr.size(); i < len; ++i) {
		// If next arg is an operator
		if (i + 1 < len && isOperator(arg_arr[i+1]))
			continue;
		// If current arg is an operator and it makes an expression
		if (i > 0 && i + 1 < len && isOperator(arg_arr[i])) {
			string *var, *value;
			if (isPositiveNum(arg_arr[i-1])) {
				// We need to reverse operators
				if (arg_arr[i][0] == '<')
					arg_arr[i][0] = '>';
				else if (arg_arr[i][0] == '>')
					arg_arr[i][0] = '<';
				value = &arg_arr[i-1];
				var = &arg_arr[i+1];
			} else {
				var = &arg_arr[i-1];
				value = &arg_arr[i+1];
			}
			// If variable exists
			if (limits.find(*var) != limits.end())
				try_set(limits[*var], arg_arr[i], *value);
			else
				eprintf("Unknown variable: '%s'\n", var->c_str());
			++i;
		} else {
			other_args += arg_arr[i];
			other_args += ' ';
		}
	}
	return other_args;
}

// Alias
string parseArgLimits(const string& args, map<string, std::pair<int, int>>& limits) {
	return __parseArgLimits(args, limits);
}

// Alias
string parseArgLimits(const string& args,
	map<string, std::pair<int64_t, int64_t>>& limits)
{
	return __parseArgLimits(args, limits);
}

template<class T>
static void __printLimits(FILE* stream, const map<string, std::pair<T, T>>& limits) {
	fprintf(stream, "Ranges:\n");
	for (auto&& p : limits)
		fprintf(stream, " %s -> [%s, %s]\n", p.first.c_str(),
			myto_string(p.second.first).c_str(),
			myto_string(p.second.second).c_str());
}

// Alias
void printLimits(FILE* stream, const map<string, std::pair<int, int>>& limits) {
	return __printLimits(stream, limits);
}

// Alias
void printLimits(FILE* stream, const map<string, std::pair<int64_t, int64_t>>& limits) {
	return __printLimits(stream, limits);
}

#ifdef _WIN32

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

#else
/**
 * @brief Removes recursively directory @p pathname relative to a directory file
 * descriptor @p dirfd
 *
 * @param dirfd directory file descriptor
 * @param pathname directory pathname (relative to @p dirfd)
 *
 * @return 0 on success, -1 on error
 *
 * @errors The same that occur for fstatat64(2), openat(2), unlinkat(2),
 * fdopendir(3)
 */
int __remove_rat(int dirfd, const char* path) {
	int fd = openat(dirfd, path, O_RDONLY | O_NOCTTY | O_NONBLOCK |
			O_LARGEFILE | O_DIRECTORY | O_NOFOLLOW);
	if (fd == -1)
		return unlinkat(dirfd, path, AT_REMOVEDIR);

	DIR *dir = fdopendir(fd);
	if (!dir) {
		sclose(fd);
		return unlinkat(dirfd, path, AT_REMOVEDIR);
	}

	dirent *file;
	while ((file = readdir(dir)))
		if (0 != strcmp(file->d_name, ".") && 0 != strcmp(file->d_name, "..")) {
			if (file->d_type == DT_DIR)
				__remove_rat(fd, file->d_name);
			else
				unlinkat(fd, file->d_name, 0);
		}

	closedir(dir);
	return unlinkat(dirfd, path, AT_REMOVEDIR);
}

int remove_rat(int dirfd, const char* path) {
	struct stat64 sb;
	if (fstatat64(dirfd, path, &sb, AT_SYMLINK_NOFOLLOW) == -1)
		return -1;

	if (S_ISDIR(sb.st_mode))
		return __remove_rat(dirfd, path);

	return unlinkat(dirfd, path, 0);
}
#endif
