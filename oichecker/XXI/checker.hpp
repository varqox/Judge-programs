#include <sys/time.h>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <queue>
#include <cmath>

using namespace std;

#ifdef WIN32
	inline bool file_exist(const string& name)
	{return !system(("IF EXIST \""+name+"\" (EXIT 0) ELSE (EXIT 1)").c_str());}
#else
	inline bool file_exist(const string& name)
	{return !system(("stat "+name+" 2> /dev/null > /dev/null").c_str());}
#endif

namespace checker
{
	int main(int argc, const char **argv, char task=0);
	int sprKLO(int argc, const char **argv);
}