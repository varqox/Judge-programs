#include <string>

#pragma once

template<class C>
inline std::string& operator<<(const std::string& s, const C& x)
{return const_cast<std::string&>(s) += x;}

std::string getNextArg(const char* line, size_t& i, size_t s);
std::string tolower(std::string str);
std::string myto_string(long long a);
void remove_trailing_spaces(std::string& s);
bool file_exists(const std::string& file);
