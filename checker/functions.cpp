#include <string>
#include <algorithm>
#include <cstdio>

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
