#include <algorithm>
#include <iostream>
#include <fstream>
#include <vector>
#include <locale>
#include <queue>
#include "gen.hpp"

using namespace std;

bool is_number(const string& str)
{
	for(unsigned i=0; i<str.size(); ++i)
		if(!(str[i]>='0' && str[i]<='9')) return false;
return true;
}

int to_int(const string& str)
{
	int out=0;
	for(unsigned i=0; i<str.size(); ++i)
	{
		out*=10;
		out+=str[i]-'0';
	}
return out;
}

string to_string(unsigned a)
{
	char st[10];
	int idx=0;
	while(a>0)
	{
		st[idx++]=a%10;
		a/=10;
	}
	string w;
	while(idx>0)
	{
		w+=st[--idx]+'0';
	}
	if(w.empty()) w="0";
return w;
}

namespace gen
{
	inline unsigned Ceil_to_power2(unsigned x)
	{return (x&(x-1) ? 1u<<(sizeof(unsigned)*8-__builtin_clz(x)):x);}

	namespace KUR
	{
		// Zadanie: Kurierzy XXI OI - etap 1
		// Autor: Krzysztof Małysa
		// Złożoność: czasowa - O(n lg n), pamięciowa - O(n)

		inline void merge(pair<int, int>& th, const pair<int, int>& _n)
		{
			if(th.first==_n.first) th.second+=_n.second;
			else
				if(th.second>=_n.second) th.second-=_n.second;
				else
				{
					th.first=_n.first;
					th.second=_n.second-th.second;
				}
		}

		class interval_tree
		{
			interval_tree(const interval_tree&): size(), t(){}
			interval_tree& operator=(const interval_tree&){return *this;}
		public:
			unsigned size;
			pair<int, int> *t;
			interval_tree(unsigned n): size(Ceil_to_power2(n)), t(new pair<int, int>[size<<1]){}
			~interval_tree(){delete[] t;}
			void init();
			int getlider(unsigned, unsigned);
		};

		void interval_tree::init()
		{
			for(int i=size-1; i>0; --i)
			{
				t[i]=t[i<<1];
				merge(t[i], t[(i<<1)+1]);
			}
		}

		int interval_tree::getlider(unsigned begin, unsigned end)
		{
			begin+=size;
			end+=size;
			pair<int, int> out;
			merge(out, t[begin]);
			if(begin!=end) merge(out, t[end]);
			while(begin>>1!=end>>1)
			{
				if(!(begin&1)) merge(out, t[begin+1]);
				if(end&1) merge(out, t[end-1]);
				begin>>=1;
				end>>=1;
			}
		return out.first;
		}

		vector<vector<int> > outer;

		inline bool checklider(unsigned a, unsigned b, int lider)
		{return upper_bound(outer[lider].begin(), outer[lider].end()-1, b)-lower_bound(outer[lider].begin(), outer[lider].end()-1, a)>(b-a+1)/2;}

		int main(fstream& gout, int n1, int m1, const vector<int>& arr, const vector<int>& aarr, const vector<int> barr)
		{
			ios_base::sync_with_stdio(false);
			int n, m, a, b, lider;
			n=n1;
			m=m1;
			// cin >> n >> m;
			interval_tree tree(n);
			vector<vector<int> >(n+1).swap(outer);
			for(int i=0; i<n; ++i)
			{
				a=arr[i];
				// cin >> a;
				tree.t[tree.size+i]=make_pair(a, 1);
				outer[a].push_back(i);
			}
			for(int i=0; i<=n; ++i)
				outer[i].push_back(n);
			tree.init();
			string out;
			for(int y=0; y<m; ++y)
			{
				a=aarr[y];
				b=barr[y];
				// cin >> a >> b;
				--a;--b;
				out+=to_string(checklider(a, b, lider=tree.getlider(a, b)) ? lider:0);
				out+='\n';
			}
			gout << out;
		return 0;
		}
	};

	void genKUR(fstream& in, fstream& out, int argc, const char** argv)
	{
		bool n_set=false, m_set=false, *const_set;
		int n, m, *constant;
		string option, number;
		for(int i=3; i<argc; ++i)
		{
			option=argv[i];
			if(option.size()<3)
			{
				cout << "Wrong option: " << option << endl;
				throw 1;
			}
			if(option[0]=='n'){constant=&n;const_set=&n_set;}
			else if(option[0]=='m'){constant=&m;const_set=&m_set;}
			else{cout << "Undefined constant: " << option[0] << endl;throw 1;}
			if(option[1]=='=')
			{
				number=string(option.begin()+2, option.end());
				if(is_number(number))
				{
					*constant=to_int(number);
					if(0<*constant && *constant<=500000) *const_set=true;
					else{cout << *constant << " doesn't belong to [1, 5000]" << endl;throw 1;}
				}
				else{cout << number << " isn't a number" << endl;throw 1;}
			}
			else if(option[1]=='<')
			{
				if(option[2]=='=')
				{
					number=string(option.begin()+3, option.end());
					if(is_number(number))
					{
						*constant=to_int(number);
						if(0<*constant && *constant<=500000) *const_set=true;
						else{cout << n << " doesn't belong to [1, 5000]" << endl;throw 1;}
						*constant=1+(rd()%*constant);
					}
					else{cout << number << " isn't a number" << endl;throw 1;}
				}
				else
				{
					number=string(option.begin()+2, option.end());
					if(is_number(number))
					{
						*constant=to_int(number);
						if(1<*constant && *constant<=500000) *const_set=true;
						else{cout << *constant << " doesn't belong to (1, 5000]" << endl;throw 1;}
						*constant=1+(rd()%(*constant-1));
					}
					else{cout << number << " isn't a number" << endl;throw 1;}
				}
			}
			else{cout << "Undefined symbol: " << option[1] << endl;throw 1;}
		}
		if(!n_set) n=1+(rd()%500000);
		if(!m_set) m=1+(rd()%500000);
		///////////////////////////////
		in << n << " " << m << endl;
		int maxrd=rd()%n+1;
		vector<int> arr(n), aarr(m), barr(m);
		for(int i=0; i<n;)
		{
			for(int k=min(rd()%maxrd+1, n-i), j=i+k, value=rd()%n+1; i<j; ++i)
				arr[i]=value;
		}
		permute(&arr.front(), &arr.back(), arr.size());
		in << arr[0];
		for(int i=1; i<n; ++i)
			in << " " << arr[i];
		in << endl;
		for(int i=0; i<m; ++i)
		{
			aarr[i]=1+(rd()%n);
			barr[i]=1+(rd()%n);
			if(aarr[i]>barr[i]) swap(aarr[i], barr[i]);
			in << aarr[i] << " " << barr[i] << endl;
		}
		KUR::main(out, n, m, arr, aarr, barr);
	}

	class _BAR
	{
		struct par
		{
			long long int a, b;
			par(int a1=0, int b1=0): a(a1), b(b1){}
		};
	public:
		int main(fstream& gout, int n1, string str)
		{
			ios_base::sync_with_stdio(false);
			int n=n1;
			// cin >> n;
			vector<char> arr(n);
			vector<par> out;
			for(int i=0; i<n; ++i)
			{
				// cin >> arr[i];
				arr[i]=str[i];
				arr[i]=(arr[i]=='p' ? 1:-1);
			}
			int output=0;
			for(int i=0; i<n; ++i)
			{
				out.push_back(par());
				for(int j=0; j<=i; ++j)
				{
					out[j].a+=arr[i];
					if(out[j].a<0) out[j].a-=1<<20;
					if(arr[i]==1)
					{
						if(++out[j].b>=0 && out[j].a>=0) output=max(output, i-j+1);
					}
					else out[j].b=(out[j].b>0 ? -1:out[j].b-1);
				}
			}
			gout << output << endl;
		return 0;
		}
	};

	void genBAR(fstream& in, fstream& out, int argc, const char** argv)
	{
		bool n_set=false;
		int n;
		string option, number;
		for(int i=3; i<argc; ++i)
		{
			option=argv[i];
			if(option.size()<3)
			{
				cout << "Wrong option: " << option << endl;
				throw 1;
			}
			if(option[0]=='n')
			{
				if(option[1]=='=')
				{
					number=string(option.begin()+2, option.end());
					if(is_number(number))
					{
						n=to_int(number);
						if(0<n && n<=1000000) n_set=true;
						else{cout << n << " doesn't belong to [1, 5000]" << endl;throw 1;}
					}
					else{cout << number << " isn't a number" << endl;throw 1;}
				}
				else if(option[1]=='<')
				{
					if(option[2]=='=')
					{
						number=string(option.begin()+3, option.end());
						if(is_number(number))
						{
							n=to_int(number);
							if(0<n && n<=1000000) n_set=true;
							else{cout << n << " doesn't belong to [1, 5000]" << endl;throw 1;}
							n=1+(rd()%n);
						}
						else{cout << number << " isn't a number" << endl;throw 1;}
					}
					else
					{
						number=string(option.begin()+2, option.end());
						if(is_number(number))
						{
							n=to_int(number);
							if(1<n && n<=1000000) n_set=true;
							else{cout << n << " doesn't belong to (1, 5000]" << endl;throw 1;}
							n=1+(rd()%(n-1));
						}
						else{cout << number << " isn't a number" << endl;throw 1;}
					}
				}
				else{cout << "Undefined symbol: " << option[1] << endl;throw 1;}
			}
			else{cout << "Undefined constant: " << option[0] << endl;throw 1;}
		}
		_BAR BAR;
		if(!n_set) n=1+(rd()%10000);
		in << n << endl;
		string ingen;
		for(int i=0; i<n; ++i)
			ingen+=(rd()&1 ? 'p':'j');
		in << ingen << endl;
		BAR.main(out, n, ingen);
	}

	class _HOT
	{
		int n;
		vector<int> *G;

		_HOT(const _HOT&): n(), G(), _a(), _b(), _n(){}
		_HOT& operator=(const _HOT&){return *this;}
		// _HOT(){}

		long long get(unsigned centr)
		{
			vector<char> col(n+1, -1);
			unsigned deep=1, qsize, st_centr=G[centr].size();
			long long K1, K2, K3, ret=0;
			if(st_centr<3) return 0;
			vector<vector<int> > out(2, vector<int>(st_centr));
			queue<int> q;
			col[centr]=0;
			for(unsigned vvv=0; vvv<st_centr; ++vvv)
			{
				deep=1;
				qsize=1;
				out[0][vvv]++;
				col[G[centr][vvv]]=0;
				q.push(G[centr][vvv]);
				while(!q.empty())
				{
					if(qsize==0)
					{
						++deep;
						if(deep==out.size()) out.push_back(vector<int>(st_centr));
						qsize=q.size();
					}
					for(unsigned i=0; i<G[q.front()].size(); ++i)
					{
						if(col[G[q.front()][i]]==-1)
						{
							++out[deep][vvv];
							col[G[q.front()][i]]=deep;
							q.push(G[q.front()][i]);
						}
					}
					q.pop();
					--qsize;
				}
			}
			for(unsigned i=0; i<out.size(); ++i)
			{
				K1=K2=K3=0;
				register long long tmp;
				for(unsigned j=0; j<st_centr; ++j)
				{
					tmp=out[i][j];
					K1+=tmp;
					K2+=tmp*tmp;
					K3+=tmp*tmp*tmp;
				}
				ret+=(K1*(K1*K1-3*K2)+2*K3)/6;
			}
		return ret;
		}
	public:
		_HOT(): n(), G(), _a(), _b(), _n(){}
		queue<int> _a, _b;
		int _n;

		int main(fstream& gout)
		{
			int a, b;
			// cin >> n;
			n=_n;
			G=new vector<int>[n+1];
			for(int i=1; i<n; ++i)
			{
				// cin >> a >> b;
				a=_a.front();_a.pop();
				b=_b.front();_b.pop();
				G[a].push_back(b);
				G[b].push_back(a);
			}
			long long out=0;
			for(int i=1; i<=n; ++i)
				out+=get(i);
			gout << out << endl;
			delete[] G;
		return 0;
		}
	};

	void genHOT(fstream& in, fstream& out, int argc, const char** argv)
	{
		bool n_set=false;
		int n=0;
		string option, number;
		for(int i=3; i<argc; ++i)
		{
			option=argv[i];
			if(option.size()<3)
			{
				cout << "Wrong option: " << option << endl;
				throw 1;
			}
			if(option[0]=='n')
			{
				if(option[1]=='=')
				{
					number=string(option.begin()+2, option.end());
					if(is_number(number))
					{
						n=to_int(number);
						if(0<n && n<=5000) n_set=true;
						else{cout << n << " doesn't belong to [1, 5000]" << endl;throw 1;}
					}
					else{cout << number << " isn't a number" << endl;throw 1;}
				}
				else if(option[1]=='<')
				{
					if(option[2]=='=')
					{
						number=string(option.begin()+3, option.end());
						if(is_number(number))
						{
							n=to_int(number);
							if(0<n && n<=5000) n_set=true;
							else{cout << n << " doesn't belong to [1, 5000]" << endl;throw 1;}
							n=1+(rd()%n);
						}
						else{cout << number << " isn't a number" << endl;throw 1;}
					}
					else
					{
						number=string(option.begin()+2, option.end());
						if(is_number(number))
						{
							n=to_int(number);
							if(1<n && n<=5000) n_set=true;
							else{cout << n << " doesn't belong to (1, 5000]" << endl;throw 1;}
							n=1+(rd()%(n-1));
						}
						else{cout << number << " isn't a number" << endl;throw 1;}
					}
				}
				else{cout << "Undefined symbol: " << option[1] << endl;throw 1;}
			}
			else{cout << "Undefined constant: " << option[0] << endl;throw 1;}
		}
		_HOT HOT;
		if(!n_set) n=1+(rd()%5000);
		int end=1, new_end, tab[n], maxrd=rd()%n+1;
		HOT._n=n;
		in << n << endl;
		for(int i=0; i<n; ++i)
			tab[i]=i;
		permute(tab, tab+n);
		for(int i=0; end<n; ++i)
		{
			new_end=min(end+1+(rd()%maxrd), n);
			for(int j=end; j<new_end; ++j)
			{
				in << tab[i]+1 << " " << tab[j]+1 << endl;
				HOT._a.push(tab[i]+1);
				HOT._b.push(tab[j]+1);
			}
			end=new_end;
		}
		HOT.main(out);
	}

	string to_lower(const string& str)
	{
		string out;
		for(unsigned i=0; i<str.size(); ++i)
			out+=tolower(str[i]);
	return out;
	}

	int main(int argc, const char** argv)
	{
		if(argc<3)
		{
			cout << "Usage: gen <task name> <number of tests> [additional terms...]" << endl;
			return 1;
		}
		if(!is_number(argv[2]))
		{
			cout << argv[2] << " isn't a number" << endl;
			return 1;
		}
		void (*gentest)(fstream&, fstream&, int, const char**);
		string task_name=to_lower(argv[1]);
		if(task_name=="hot") gentest=genHOT;
		else if(task_name=="bar") gentest=genBAR;
		else if(task_name=="kur") gentest=genKUR;
		else
		{
			cout << task_name << ": undefined task" << endl;
			return 1;
		}
		srand(time(NULL));
		int n=to_int(argv[2]);
		fstream test_in, test_out;
	#ifdef _WIN32
		{
			system(("(mkdir \"tests\\"+task_name+"\\\") > NUL 2> NUL").c_str());
			system(("(rd /S /Q \"tests\\"+task_name+"\\\") > NUL 2> NUL").c_str());
			system(("(mkdir \"tests\\"+task_name+"\\\") > NUL 2> NUL").c_str());
		}
	#else
		system(("(mkdir -p tests/"+task_name+"/ && rm -f tests/"+task_name+"/*) > /dev/null 2> /dev/null").c_str());
	#endif
		try
		{
			for(int q=1; q<=n; ++q)
			{
				string test_name;
				test_name="tests/"+task_name+"/"+to_string(q);
				test_in.open((test_name+".in").c_str(), ios_base::out);
				test_out.open((test_name+".out").c_str(), ios_base::out);
				gentest(test_in, test_out, argc, argv);
				test_in.close();
				test_out.close();
				// system(("./hotn3 < "+test_name+".in > "+test_name+".out");
				cout << q << ": generated!" << endl;
			}
		}
		catch(...)
		{return 1;}
	return 0;
	}
}