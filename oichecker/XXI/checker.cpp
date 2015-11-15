#include "checker.hpp"

namespace checker
{
	double total_time, max_time;
	string max_test;

	namespace task
	{
		string t_name, time, command, path="tests/";
		queue<string> wa;

		inline void clean();
		bool run(bool=false);
		bool runKLO(bool=false);
		void spr(const string& name, const string& exec, char testk=0);
	}

	inline void task::clean()
	{
	#ifdef _WIN32
		system("(del tests\\out) > NUL 2> NUL");
	#else
		system("rm -f tests/out");
	#endif
	}

	bool task::run(bool out_wrongs)
	{
		// runtime
		timeval ts, te;
		gettimeofday(&ts, NULL);
	#ifdef _WIN32
		system((command+path+t_name+".in > tests\\out").c_str());
	#else
		system((command+path+t_name+".in > tests/out").c_str());
	#endif
		gettimeofday(&te, NULL);
		double cl=(te.tv_sec+static_cast<double>(te.tv_usec)/1000000)-(ts.tv_sec+static_cast<double>(ts.tv_usec)/1000000);
		// round with precision 3
		cl*=1000;
		cl=round(cl)/1000;
		// end of runtime
		fstream out, ans;
		out.open("tests/out", ios_base::in);
		ans.open((path+t_name+".out").c_str(), ios_base::in);
		string s_out, s_ans;
		cout << fixed;
		cout.precision(3);
		int line=0;
		while(out.good() && ans.good())
		{
			++line;
			getline(ans,s_ans);
			getline(out,s_out);
			if(s_ans!=s_out || (!out.good() && (ans.good() && s_ans.empty())))
			{
				cout << t_name << ": Wrong! time - " << cl << "s >> line: " << line;
				if(out_wrongs)
				{
					cout << "\nGet: ";
					if(!out.good()) cout << "EOF";
					else cout << '\'' << s_out << '\'';
					cout << "\nExpected: '" << s_ans << '\'';
				}
				cout << endl;
				total_time+=cl;
				if(cl>max_time)
				{
					max_time=cl;
					max_test=t_name;
				}
				clean();
				return false;
			}
		}
		out.close();
		ans.close();
		cout << t_name << ": [ OK ] time - " << cl << "s" << endl;
		total_time+=cl;
		if(cl>max_time)
		{
			max_time=cl;
			max_test=t_name;
		}
		clean();
	return true;
	}

	bool task::runKLO(bool out_wrongs)
	{
		// runtime
		timeval ts, te;
		gettimeofday(&ts, NULL);
	#ifdef _WIN32
		system((command+path+t_name+".in > tests\\out").c_str());
	#else
		system((command+path+t_name+".in > tests/out").c_str());
	#endif
		gettimeofday(&te, NULL);
		double cl=(te.tv_sec+static_cast<double>(te.tv_usec)/1000000)-(ts.tv_sec+static_cast<double>(ts.tv_usec)/1000000);
		// round with precision 3
		cl*=1000;
		cl=round(cl)/1000;
		// end of runtime
		fstream out, ans, in;
		out.open("tests/out", ios_base::in);
		ans.open((path+t_name+".out").c_str(), ios_base::in);
		in.open((path+t_name+".in").c_str(), ios_base::in);
		string s_out, s_ans;
		cout << fixed;
		cout.precision(3);
		int line=0;
		if(out.good() && ans.good())
		{
			getline(ans,s_ans);
			if(s_ans=="0")
			{
				getline(out,s_out);
				if(s_out!=s_ans)
				{
					cout << t_name << ": Wrong! time - " << cl << "s >> line: " << (line=1);
					if(out_wrongs)
					{
						cout << "\nGet: ";
						if(!out.good()) cout << "EOF";
						else cout << '\'' << s_out << '\'';
						cout << "\nExpected: '" << s_ans << '\'';
					}
					cout << endl;
					total_time+=cl;
					if(cl>max_time)
					{
						max_time=cl;
						max_test=t_name;
					}
					clean();
					return false;
				}
			}
			else
			{
				int t, a, b, n=0, k;
				bool wrong=false;
				if(in.good())
				{
					in >> t >> a >> b;
					vector<int> arr(t);
					for(int i=0; i<t && in.good(); ++i)
					{
						in >> arr[i];
						n+=arr[i];
					}
					/*--arr[a-1];
					--arr[b-1];
					if(out.eof()) wrong=true;
					else
					{
						out >> k;
						if(k!=a) wrong=true;
					}*/
					vector<int> check(n);
					for(int i=0; i<n; ++i)
					{
						if(out.eof())
						{
							wrong=true;
							break;
						}
						else out >> k;
						check[i]=k;
						if(k<=0 || k>t)
						{
							wrong=true;
							break;
						}
						--arr[k-1];
						if(arr[k-1]<0)
						{
							wrong=true;
							break;
						}
					}
					if(check.front()!=a || check.back()!=b) wrong=true;
					else
					{
						for(int i=1; i<n; ++i)
							if(check[i]==check[i-1])
							{
								wrong=true;
								break;
							}
					}
					if(wrong)
					{
						cout << t_name << ": Wrong! time - " << cl << "s >> line: " << (line=1);
						out.close();
						out.open("tests/out", ios_base::in);
						getline(out, s_out);
						if(out_wrongs)
							cout << "\nGet: '" << s_out << "'\nExpected: '" << s_ans << '\'';
						cout << endl;
						total_time+=cl;
						if(cl>max_time)
						{
							max_time=cl;
							max_test=t_name;
						}
						clean();
						return false;
					}
				}
			}
		}
		out.close();
		ans.close();
		in.close();
		cout << t_name << ": [ OK ] time - " << cl << "s" << endl;
		total_time+=cl;
		if(cl>max_time)
		{
			max_time=cl;
			max_test=t_name;
		}
		clean();
	return true;
	}

	void task::spr(const string& name, const string& exec, char task)
	{
		bool (*run_test)(bool)=NULL;
		if(task==0) run_test=run;
		else if(task==1) run_test=runKLO;
		total_time=max_time=0;
	#ifdef _WIN32
		system(("dir /b \"tests\\"+name+"\" > tests\\list.txt 2> NUL").c_str());
	#else
		command="./";
		system(("(ls -1 tests/"+name+" > tests/list.txt) 2> /dev/null").c_str());
	#endif
		fstream list;
		path="tests/"+name+"/";
		list.open("tests/list.txt", ios_base::in);
		if(!list.good())
		{
			cout << "Cannot list tests directory" << endl;
			return;
		}
		command+=exec+" < ";
		queue<string> t_lst;
		while(!list.eof())
		{
			getline(list,t_name);
			if(t_name[t_name.size()-3]=='.' && t_name[t_name.size()-2]=='i' && t_name[t_name.size()-1]=='n')
			{
				t_name.erase(t_name.size()-3, 3);
				t_lst.push(t_name);
			}
		}
		while(!t_lst.empty())
		{
			t_name.swap(t_lst.front());
			t_lst.pop();
			if(!run_test(false)) wa.push(t_name);
		}
		list.close();
	#ifdef _WIN32
		system("(del tests\\list.txt) > NUL 2> NUL");
	#else
		system("rm -f tests/list.txt");
	#endif
		if(!wa.empty())
		{
			cout << "\nWrong tests: " << wa.front();
			wa.pop();
			while(!wa.empty())
			{
				cout << ", " << wa.front();
				wa.pop();
			}
			cout << endl;
		}
		cout << "Total time - " << total_time << "s \nMax time - " << max_time << "s : " << max_test << endl;
	}

	int main(int argc, const char **argv, char task)
	{
		if(argc<3)
		{
		#ifdef _WIN32
			cout << "Usage: spr <task name> <exec name> [test names...]" << endl;
		#else
			cout << "Usage: spr <task name> <exec name> [test names...]" << endl;
		#endif
			return 1;
		}
		task::command="";
		if(!file_exist(string("tests/")+argv[1])) cout << "tests/" << argv[1] << ": directory doesn't exist" << endl;
		else if(!file_exist(argv[2])) cout << argv[2] << ": file doesn't exist" << endl;
		else if(argc==3) task::spr(string(argv[1]), string(argv[2]), task);
		else
		{
			bool (*run_test)(bool)=NULL;
			if(task==0) run_test=task::run;
			else if(task==1) run_test=task::runKLO;
			total_time=max_time=0;
		#ifndef _WIN32
			task::command="./";
		#endif
			task::path="tests/"+string(argv[1])+'/';
			task::command+=string(argv[2])+" < ";
			for(int i=3; i<argc; ++i)
			{
				if(file_exist(task::path+argv[i]+".in"))
				{
					task::t_name=argv[i];
					run_test(true);
				}
				else cout << argv[i] << ": test doesn't exist" << endl;
			}
			cout << fixed;
			cout.precision(3);
			cout << "Total time - " << total_time << "s \nMax time - " << max_time << "s : " << max_test << endl;
		}
	return 0;
	}

	int sprKLO(int argc, const char **argv)
	{return main(argc, argv, 1);}
}