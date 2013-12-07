#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <vector>
#include <algorithm>
#include <dirent.h>

using namespace std;

class task
{
	string _name;
	vector<string> _test_names, WA; // WA - wrong tests
	double _total_time, _max_time;

public:
	task(string str): _name(str), _total_time(0), _max_time(0){}

	const string& name() const
	{return this->_name;}

	const vector<string>& tests() const
	{return this->_test_names;}

	void swap(task& _t)
	{
		this->_name.swap(_t._name);
		this->_test_names.swap(_t._test_names);
		this->WA.swap(_t.WA);
		std::swap(this->_total_time, _t._total_time);
		std::swap(this->_max_time, _t._max_time);
	}

	void make_list_of_tests();

	int run(int argc, char** argv);

	const vector<string>& worng_tests() const
	{return this->WA;}

	const double total_time() const
	{return this->_total_time;}

	const double max_time() const
	{return this->_max_time;}
};

struct compare
{
	bool operator()(const string& s1, const string& s2) const
	{return (s1.size()==s2.size() ? s1<s2:s1.size()<s2.size());}
};

void task::make_list_of_tests()
{
	DIR* directory;
	dirent* current_file;
	int	name_lenght;
	if((directory=opendir(this->_name.c_str())))
		while((current_file=readdir(directory)))
		{
			name_lenght=strlen(current_file->d_name);
			if(name_lenght>3 && 0==memcmp(current_file->d_name+name_lenght-3, ".in", 3)) this->_test_names.push_back(string(current_file->d_name, name_lenght-3));
		}
	sort(this->_test_names.begin(), this->_test_names.end(), compare());
	for(vector<string>::iterator i=this->_test_names.begin(); i!=this->_test_names.end(); ++i)
		cerr << *i << endl;
}

int main()
{
	string k;
	cin >> k;
	task lol(k);
	lol.make_list_of_tests();
return 0;
}