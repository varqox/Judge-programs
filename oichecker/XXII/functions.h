#include <string>

#pragma once

template<class C>
inline std::string& operator<<(const std::string& s, const C& x)
{return const_cast<std::string&>(s) += x;}

std::string getNextArg(const char* line, size_t& i, size_t s);
std::string tolower(std::string str);
std::string myto_string(long long a);
std::string dmyto_string(double a);
void remove_trailing_spaces(std::string& s);
bool file_exists(const std::string& file);
int remove_r(const char* path);
bool isNum(const std::string& s);

class ArgParser {
private:
	size_t pos_, size_;
	char* arg_;
public:
	explicit ArgParser(const std::string& s);
	ArgParser(const ArgParser& ap);
	ArgParser& operator=(const ArgParser& ap);

	std::string getNextArg();
	void reset() { pos_ = 0; }

	~ArgParser() {
		if (arg_)
			delete[] arg_;
	}
};

bool isPositiveNum(const std::string& s);
unsigned long long atoull(const std::string& s);
bool isNum(const std::string& s, long long down, long long up);
void try_set(std::pair<int, int>& p, const std::string& oper, const std::string& val, int mn, int mx);
