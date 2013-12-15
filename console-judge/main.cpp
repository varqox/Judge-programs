#include <sys/time.h>
#include <algorithm>
#include <dirent.h>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <vector>
#include <cctype>
#include <cmath>
#include <deque>

using namespace std;

#ifdef WIN32
#include <windows.h>
	inline bool file_exist(const string& name)
	{return !system(("IF EXIST \""+name+"\" (EXIT 0) ELSE (EXIT 1)").c_str());}
#else
	inline bool file_exist(const string& name)
	{return !system(("stat "+name+" 2> /dev/null > /dev/null").c_str());}
#endif

class task
{
	string _name, _longest_test, outf_name;
	vector<string> _test_names, WA; // WA - wrong tests
	double _total_time, _max_time;

	int check_on_test(const string& test, const string& exec, bool wrongs_info=false);

public:
	task(const string& str): _name(str), _total_time(0), _max_time(0)
	{
		if(*this->_name.rbegin()!='/') this->_name+='/';
		// get name of temporary file
	#ifdef WIN32
		char tmp[MAX_PATH], path[MAX_PATH];
		GetTempPath(MAX_PATH, path);
		GetTempFileName(path, TEXT("judge"), 0, tmp);
	#else
		char tmp[L_tmpnam]="/tmp/judge.XXXXXX";
		mkstemp(tmp);
	#endif
		this->outf_name=tmp;
	}

	~task()
	{remove(this->outf_name.c_str());}

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

	void judge(int argc, char** argv);

	const vector<string>& worng_tests() const
	{return this->WA;}

	const double total_time() const
	{return this->_total_time;}

	const double max_time() const
	{return this->_max_time;}

	const string& longest_test() const
	{return this->_longest_test;}
};

void remove_trailing_spaces(string& str)
{
	string::iterator erase_begin=str.end();
	while(erase_begin!=str.begin() && isspace(*(erase_begin-1))) --erase_begin;
	str.erase(erase_begin, str.end());
}

int task::check_on_test(const string& test, const string& exec, bool wrongs_info)
{
	// runtime
	timeval ts, te;
	gettimeofday(&ts, NULL);
#ifdef WIN32
	int ret=system((exec+" < \""+this->_name+test+".in\" > \""+outf_name+"\"").c_str());
#else
	int ret=system(("./"+exec+" < "+this->_name+test+".in > "+outf_name).c_str());
#endif
	gettimeofday(&te, NULL);
	double cl=(te.tv_sec+static_cast<double>(te.tv_usec)/1000000)-(ts.tv_sec+static_cast<double>(ts.tv_usec)/1000000);
	// end of runtime && time calculating
	cl*=1000;
	cl=round(cl)/1000;
	this->_total_time+=cl;
	if(cl>this->_max_time)
	{
		this->_max_time=cl;
		this->_longest_test=test;
	}
	// checking answer
	fstream out(outf_name.c_str(), ios_base::in), ans((this->_name+test+".out").c_str(), ios_base::in);
	if(!out.good() && !ans.good())
	{
		cerr << "Error with checking test\n";
		remove(this->outf_name.c_str());
		return 0;
	}
	if(ret!=0)
	{
		cout << test << ": Runtime error (returned value: " << (ret>>8) << ") time - " << fixed;
		cout.precision(3);
		cout << cl << 's' << endl;
		remove(this->outf_name.c_str());
		return 1;
	}
	deque<string> out_in, ans_in;
	string out_tmp, ans_tmp;
	while(out.good() && ans.good())
	{
		getline(out, out_tmp);
		getline(ans, ans_tmp);
		remove_trailing_spaces(out_tmp);
		remove_trailing_spaces(ans_tmp);
		out_in.push_back(out_tmp);
		ans_in.push_back(ans_tmp);
	}
	while(!out_in.empty() && out_in.back().empty()) out_in.pop_back();
	while(!ans_in.empty() && ans_in.back().empty()) ans_in.pop_back();
	int line=-1;
	while(++line<out_in.size() && line<ans_in.size())
		if(ans_in[line]!=out_in[line])
		{
			cout << test << ": Wrong! time - " << fixed;
			cout.precision(3);
			cout << cl << "s >> line: " << line+1 << endl;
			if(wrongs_info)
			{
				cout << "Get:\n'" << out_in[line] << "'\nExpected:\n'" << ans_in[line] << '\'' << endl;;
			}
			remove(this->outf_name.c_str());
			return 1;
		}
	if(ans_in.size()>out_in.size())
	{
		cout << test << ": Wrong! time - " << fixed;
		cout.precision(3);
		cout << cl << "s >> line: " << line+1 << endl;
		if(wrongs_info)
		{
			cout << "Get:\n'EOF'\nExpected:\n'" << ans_in[line] << '\'' << endl;
		}
		remove(this->outf_name.c_str());
		return 1;
	}
	// end of checking answer
	cout << test << ": [ OK ] time - " << fixed;
	cout.precision(3);
	cout << cl << 's' << endl;
	remove(this->outf_name.c_str());
return 0;
}

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
}

void task::judge(int argc, char** argv)
{
	this->_total_time=this->_max_time=0;
	this->_longest_test="";
	vector<string>().swap(this->_test_names);
	vector<string>().swap(this->WA);
	bool show_wrongs_info=argc!=1;
	if(argc==1) this->make_list_of_tests();
	else
		for(int i=1; i<argc; ++i)
		{
			if(file_exist(this->_name+argv[i]+".in"))
				this->_test_names.push_back(argv[i]);
			else cerr << argv[i] << ": test doesn't exist" << endl;
		}
	for(vector<string>::iterator current_test=this->_test_names.begin(); current_test!=this->_test_names.end(); ++current_test)
	{
		if(this->check_on_test(*current_test, argv[0], show_wrongs_info))
			this->WA.push_back(*current_test);
	}
	if(!this->WA.empty())
	{
		cout << "Wrong tests: " << this->WA.front();
		for(vector<string>::iterator i=this->WA.begin()+1; i!=this->WA.end(); ++i)
			cout << ", " << *i;
		cout << endl;
	}
	cout << "Total time - " << fixed;
	cout.precision(3);
	cout << this->_total_time << "s\nMax time - " << fixed;
	cout.precision(3);
	cout << this->_max_time << "s : " << this->_longest_test << endl;
}

int main(int argc, char** argv)
{
	if(argc<3)
	{
		cerr << "Usage: spr <task name> <exec name> [test names...]\n";
		return 1;
	}
	if(!file_exist(argv[1])) cerr << argv[1] << ": doesn't exist" << endl;
	else if(!file_exist(argv[2])) cerr << argv[2] << ": doesn't exist" << endl;
	else
	{
		task judge(argv[1]);
		judge.judge(argc-2, argv+2);
	}
return 0;
}
