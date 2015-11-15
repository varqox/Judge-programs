#include "problem.h"
#include "functions.h"

#include <string>
#include <cstdlib>
#include <cstdio>

#define eprintf(args...) fprintf(stderr, args)

using std::string;
using std::pair;

int getRandom(const pair<int, int>& p) {
	return p.first + rand() % (p.second - p.first + 1);
}

int getRandom(int a, int b) {
	return a + rand() % (b - a + 1);
}

#include <bits/stdc++.h>

using namespace std;

#define FOR(x,b,e) for(int x = b; x < (e); ++x)
#define FOR2(x,b,e) for(int x = b; x <= (e); ++x)
#define FORD(x,b,e) for(int x = b; x >= (e); --x)
#define REP(x,n) for(int x = 0; x < (n); ++x)
#define REP2(x,n) for(int x = 0; x <= (n); ++x)
#define VAR(v,n) typeof(n) v = (n)
#define ALL(x) x.begin(), x.end()
#define ALL2(x) x.rbegin(), x.rend()
#define FOREACH(i,c) for(VAR(i, (c).begin()); i != (c).end(); ++i)
#define FOREACH2(i,c) for(VAR(i, (c).rbegin()); i != (c).rend(); ++i)
#define PB push_back
#define ST first
#define ND second
#define PF push_front
#define MP make_pair
#define I(x) static_cast<int>(x)
#define U(x) static_cast<unsigned>(x)
#define SZ(x) I((x).size())
#define eprint(...) fprintf(stderr, __VA_ARGS__)
#define ARR(t,x,n) t *x = new t[n]

typedef unsigned char uchar;
typedef long long LL;
typedef unsigned long long ULL;
typedef vector<int> VI;
typedef vector<VI> VVI;
typedef vector<LL> VLL;
typedef vector<string> VS;
typedef pair<int, int> PII;
typedef pair<LL, LL> PLLLL;
typedef vector<PII> VPII;
typedef vector<PLLLL> VPLLLL;
typedef vector<VPII> VVPII;

template<class T>
inline T abs(T x)
{return x<T() ? -x : x;}

template<class T, class T1>
void mini(T& a, const T1& x)
{if(x < a)a = x;}

template<class T, class T1>
void maxi(T& a, const T1& x)
{if(x > a)a = x;}

#define MOD(x, mod_val) {if(x >= mod_val) x %= mod_val; else if(x < 0){x %= mod_val;if(x < 0) x += mod_val;}}

#ifdef DEBUG

#define D(...) __VA_ARGS__
#define E(...) eprint(__VA_ARGS__)
#define LOG(x) cerr << #x << ": " << (x) << flush
#define LOGN(x) cerr << #x << ": " << (x) << endl

template<class C1, class C2>
ostream& operator<<(ostream& os, const pair<C1, C2>& _)
{return os << "(" << _.ST << "," << _.ND << ")";}

template<class T>
ostream& operator<<(ostream& os, const vector<T>& _)
{
	os << "{";
	if(_.size())
	{
		os << *_.begin();
		for(typename vector<T>::const_iterator i=++_.begin(); i!=_.end(); ++i)
			os << ", " << *i;
	}
	return os << "}";
}

template<class T>
ostream& operator<<(ostream& os, const deque<T>& _)
{
	os << "{";
	if(_.size())
	{
		os << *_.begin();
		for(typename deque<T>::const_iterator i=++_.begin(); i!=_.end(); ++i)
			os << ", " << *i;
	}
	return os << "}";
}

template<class T, class K>
ostream& operator<<(ostream& os, const set<T, K>& _)
{
	os << "{";
	if(_.size())
	{
		os << *_.begin();
		for(typename set<T>::const_iterator i=++_.begin(); i!=_.end(); ++i)
			os << ", " << *i;
	}
	return os << "}";
}

template<class T, class K>
ostream& operator<<(ostream& os, const map<T, K>& _)
{
	os << "{";
	if(_.size())
	{
		os << _.begin()->ST << " => " << _.begin()->ND;
		for(typename map<T, K>::const_iterator i=++_.begin(); i!=_.end(); ++i)
			os << ", " << i->ST << " => " << i->ND;
	}
	return os << "}";
}

template<class Iter>
void out(ostream& os, Iter begin, Iter end)
{
	os << "{";
	if(begin != end)
	{
		os << *begin;
		for(Iter i = ++begin; i != end; ++i)
			os << ", " << *i;
	}
	os << "}";
}

#define OUT(a, b) cerr << #a << ": ", out(cerr, a, b), eprint("\n")

#else
#define D(...)
#define E(...)
#define LOG(x)
#define LOGN(x)
#define OUT(...)
#endif

class PieProblem : public Problem {
public:
	string name() const { return "Pieczatki"; }

	string tag() const { return "PIE"; }

	string help() const { return "PIE:\n  gen pie [N] [ARGS]... - generates N tests, in ARGS you can use comparisions to set variables: q, n, m, a, b\n    To see default limits type 'gen pie'\n    Example: gen pie 10 n<10 m=30 3<a<7 - generates 10 tests where n < 10, m = 30, a belongs to [4, 6]\n"; }

	int gen(const string& path, const string& args, bool in_only = false) {
		q_ = make_pair(1, 10);
		n_ = make_pair(1, 1000);
		m_ = make_pair(1, 1000);
		a_ = make_pair(1, 1000);
		b_ = make_pair(1, 1000);
		string N, x, new_args;
		for (size_t i = 0; i < args.size(); ++i) {
			if (args[i] == '<' || args[i] == '>' || args[i] == '=')
				new_args += string(" ") + args[i] + " ";
			else
				new_args += args[i];
		}
		ArgParser ap(new_args);
		// Parsing arguments
		vector<string> arg_arr;
		while (x = ap.getNextArg(), x.size())
			arg_arr.push_back(x);
		for (size_t i = 0, len = arg_arr.size(); i < len; ++i) {
			// If next arg is operator
			if (i + 1 < len && (arg_arr[i+1] == "<" || arg_arr[i+1] == ">" || arg_arr[i+1] == "="))
				continue;
			if (0 < i && i+1 < len && (arg_arr[i] == "<" || arg_arr[i] == ">" || arg_arr[i] == "=")) {
				string *var, *value;
				if (isPositiveNum(arg_arr[i-1])) {
					if (arg_arr[i] == "<")
						arg_arr[i] = ">";
					else if (arg_arr[i] == ">")
						arg_arr[i] = "<";
					value = &arg_arr[i-1];
					var = &arg_arr[i+1];
				} else {
					var = &arg_arr[i-1];
					value = &arg_arr[i+1];
				}
				if (*var == "q")
					try_set(q_, arg_arr[i], *value, 1, 10);
				else if(*var == "n")
					try_set(n_, arg_arr[i], *value, 1, 1000);
				else if(*var == "m")
					try_set(m_, arg_arr[i], *value, 1, 1000);
				else if(*var == "a")
					try_set(a_, arg_arr[i], *value, 1, 1000);
				else if(*var == "b")
					try_set(b_, arg_arr[i], *value, 1, 1000);
				else
					eprintf("Unknown variable: '%s'\n", var->c_str());
				++i;
			} else if (isPositiveNum(arg_arr[i]))
				N = arg_arr[i];
		}
		printf ("Set:\n q belongs to [%i, %i]\n n belongs to [%i, %i]\n m belongs to [%i, %i]\n a belongs to [%i, %i]\n b belongs to [%i, %i]\n", q_.first, q_.second, n_.first, n_.second, m_.first, m_.second, a_.first, a_.second, b_.first, b_.second);
		Problem::gen(path, N, in_only); // We can use default gen
		return 0;
	}

	int genout(const string& input, const string& output) {
		FILE *in = fopen(input.c_str(), "r"), *out = fopen(output.c_str(), "w");
		if (in == NULL || out == NULL)
			return 1;
		int _;
		if (0 == fscanf(in, "%i", &_))
			return 1;
		REP (__, _) {
			int n, m, a, b, x, y;
			if (0 == fscanf(in, "%i%i%i%i", &n, &m, &a, &b))
				return 1;
			char t[n+1][m+1], s[a+1][b+1];
			vector<pair<short, short> > pp;
			pp.reserve(a*b);
			REP (i, n)
				if (0 == fscanf(in, "%s", t[i]))
					return 1;
			REP (i, a)
				if (0 == fscanf(in, "%s", s[i]))
					return 1;
			REP (i, a)
				REP (j, b) {
					if (s[i][j] == 'x')
						pp.PB(MP(i, j));
				}
			REP (i, n)
				REP (j, m) {
					if (t[i][j] == 'x') {
						FOREACH(k, pp) {
							x = i + k->ST - pp[0].ST;
							y = j + k->ND - pp[0].ND;
							if (!good(n, m, x, y) || t[x][y] == '.')
								goto no;
							else
								t[x][y] = '.';
						}
					}
				}
			fprintf(out, "TAK\n");
			continue;
		no:
			fprintf(out, "NIE\n");
		}
		fclose(in);
		fclose(out);
		return 0;
	}

protected:
	int genin(const string& file, int seed) {
		srand(seed);
		FILE *f = fopen(file.c_str(), "w");
		if (f == NULL)
			return 1;
		int q = getRandom(q_);
		fprintf(f, "%i\n", q);
		for (int _ = 0; _ < q; ++_) {
			int n = getRandom(n_), m = getRandom(m_), a, b;
			if ((rand() & 31) < 25 && n >= a_.first && m >= b_.first) { // a <= n, b <= m
				a = getRandom(make_pair(a_.first, min(a_.second, n)));
				b = getRandom(make_pair(b_.first, min(b_.second, m)));
			} else {
				a = getRandom(a_);
				b = getRandom(b_);
			}
			int others = a * b, covered = 1 + rand() % others;
			char t[n][m+1], s[a][b+1];
			for (int i = 0; i < n; ++i) {
				t[i][m] = '\0';
				for (int j = 0; j < m; ++j)
					t[i][j] = '.';
			}
			vector<pair<short, short> > cov;
			cov.reserve(covered);
			PII cov_x(1e9, 0), cov_y(1e9, 0); // mins & maxs x, y
			for (int i = 0; i < a; ++i) {
				s[i][b] = '\0';
				for (int j = 0; j < b; ++j) {
					if (rand() % others < covered) {
						--covered;
						cov.push_back(make_pair(i, j));
						mini(cov_x.first, i);
						maxi(cov_x.second, i);
						mini(cov_y.first, j);
						maxi(cov_y.second, j);
						s[i][j] = 'x';
					} else
						s[i][j] = '.';
					--others;
				}
			}
			bool is_any_covered = false;
			int x, y, prob_m = 1 + rand() % 1123641227, prob = rand() % prob_m;
			for (int i = 0; i < n; ++i)
				for (int j = 0; j < m; ++j) {
					bool can_be = true;
					if (good(n, m, i + cov_x.first - cov[0].first, j + cov_y.first - cov[0].second) && good(n, m, i + cov_x.second - cov[0].first, j + cov_y.second - cov[0].second)) {
						for (typeof(cov.begin()) k = cov.begin(); k != cov.end(); ++k) {
								x = i + k->first - cov[0].first;
								y = j + k->second - cov[0].second;
								if (!good(n, m, x, y) || t[x][y] == 'x') {
									can_be = false;
									break;
								}
						}
					} else
						can_be = false;
					if (can_be && rand() % prob_m < prob) {
						is_any_covered = true;
						for (typeof(cov.begin()) k = cov.begin(); k != cov.end(); ++k) {
								x = i + k->first - cov[0].first;
								y = j + k->second - cov[0].second;
								t[x][y] = 'x';
						}
					}
				}
			if (!is_any_covered || rand() & 1) {
				x = rand() % n;
				y = rand() % m;
				t[x][y] = (t[x][y] == 'x' ? '.' : 'x');
			}
			fprintf(f, "%i %i %i %i\n", n, m, a, b);
			for (int i = 0; i < n; ++i)
				fprintf(f, "%s\n", t[i]);
			for (int i = 0; i < a; ++i)
				fprintf(f, "%s\n", s[i]);
		}
		fclose(f);
		return 0;
	}
private:

	pair<int, int> q_, n_, m_, a_, b_;

	static inline bool good(int n, int m, int a, int b) { return (a >= 0 && a < n && b >= 0 && b < m); }
};

class KinProblem : public Problem {
public:
	string name() const { return "Kinoman"; }

	string tag() const { return "KIN"; }

	string help() const { return "KIN:\n  gen kin [N] [ARGS]... - generates N tests, in ARGS you can use comparisions to set variables: n, m, wi\n    To see default limits type 'gen kin'\n    Example: gen kin 10 n<10 m=5 wi<30 - generates 10 tests where n < 10, m = 5, wi < 30\n"; }

	int gen(const string& path, const string& args, bool in_only = false) {
		n_ = make_pair(1, 1000000);
		m_ = make_pair(1, 1000000);
		wi_ = make_pair(1, 1000000);
		string N, x, new_args;
		for (size_t i = 0; i < args.size(); ++i) {
			if (args[i] == '<' || args[i] == '>' || args[i] == '=')
				new_args += string(" ") + args[i] + " ";
			else
				new_args += args[i];
		}
		ArgParser ap(new_args);
		// Parsing arguments
		vector<string> arg_arr;
		while (x = ap.getNextArg(), x.size())
			arg_arr.push_back(x);
		for (size_t i = 0, len = arg_arr.size(); i < len; ++i) {
			// If next arg is operator
			if (i + 1 < len && (arg_arr[i+1] == "<" || arg_arr[i+1] == ">" || arg_arr[i+1] == "="))
				continue;
			if (0 < i && i+1 < len && (arg_arr[i] == "<" || arg_arr[i] == ">" || arg_arr[i] == "=")) {
				string *var, *value;
				if (isPositiveNum(arg_arr[i-1])) {
					if (arg_arr[i] == "<")
						arg_arr[i] = ">";
					else if (arg_arr[i] == ">")
						arg_arr[i] = "<";
					value = &arg_arr[i-1];
					var = &arg_arr[i+1];
				} else {
					var = &arg_arr[i-1];
					value = &arg_arr[i+1];
				}
				if(*var == "n")
					try_set(n_, arg_arr[i], *value, 1, 1000000);
				else if(*var == "m")
					try_set(m_, arg_arr[i], *value, 1, 1000000);
				else if(*var == "wi")
					try_set(wi_, arg_arr[i], *value, 1, 1000000);
				else
					eprintf("Unknown variable: '%s'\n", var->c_str());
				++i;
			} else if (isPositiveNum(arg_arr[i]))
				N = arg_arr[i];
		}
		printf ("Set:\n n belongs to [%i, %i]\n m belongs to [%i, %i]\n wi belongs to [%i, %i]\n", n_.first, n_.second, m_.first, m_.second, wi_.first, wi_.second);
		Problem::gen(path, N, in_only); // We can use default gen
		return 0;
	}

	int genout(const string& input, const string& output) {
		FILE *in = fopen(input.c_str(), "r"), *out = fopen(output.c_str(), "w");
		if (in == NULL || out == NULL)
			return 1;
		int n, m;
		if (0 == fscanf(in, "%i%i", &n, &m))
			return 1;
		int f[n], w[m];
		ARR(PII, last, m);
		REP (i, n) {
			if (0 == fscanf(in, "%i", f+i))
				return 1;
			--f[i];
		}
		REP (i, m) {
			if (0 == fscanf(in, "%i", w+i))
				return 1;
			last[i] = MP(-1, -1);
		}
		itree tree(n);
		long long res = 0;
		REP (i, n) {
			E("========================\n");
			LOGN(i);
			LOGN(f[i]);
			LOGN(last[f[i]]);
			LOGN(w[f[i]]);
			if (last[f[i]].ST == -1) {
				tree.add(0, i, w[f[i]]);
				last[f[i]] = MP(0, i);
			} else {
				tree.add(last[f[i]].ST, last[f[i]].ND, -w[f[i]]);
				tree.add(last[f[i]].ND+1, i, w[f[i]]);
				last[f[i]] = MP(last[f[i]].ND+1, i);
			}
			maxi(res, tree.getMax());
			D(tree.print();)
		}
		fprintf(out, "%lli\n", res);
		fclose(in);
		fclose(out);
		return 0;
	}

protected:
	int genin(const string& file, int seed) {
		srand(seed);
		FILE *f = fopen(file.c_str(), "w");
		if (f == NULL)
			return 1;
		int n = getRandom(n_), m = getRandom((m_.first > n ? n : m_.first), min(n, m_.second));
		fprintf(f, "%i %i\n%i", n, m, getRandom(1, m));
		for (int i = 1; i < n; ++i)
			fprintf(f, " %i", getRandom(1, m));
		fprintf(f, "\n%i", getRandom(wi_));
		for (int i = 1; i < m; ++i)
			fprintf(f, " %i", getRandom(wi_));
		fprintf(f, "\n");
		fclose(f);
		return 0;
	}
private:

	pair<int, int> n_, m_, wi_;

	static inline unsigned ceil2(unsigned x) {
		return x & (x-1) ? 1 << (sizeof(x)*8 - __builtin_clz(x)) : x;
	}

	class itree {
	public:
		const size_t size_;
		PLLLL *t_; // ST - add, ND - max

		itree(size_t n) : size_(ceil2(n)), t_(new PLLLL[size_ << 1]) {}
		~itree() { delete[] t_; }

		long long getMax() { return t_[1].ND; }

		void add(size_t a, size_t b, long long x) {
			a += size_;
			b += size_;
			t_[a].ST += x;
			t_[a].ND += x;
			if (a != b) {
				t_[b].ST += x;
				t_[b].ND += x;
			}
			while (a >> 1 != b >> 1) {
				if (!(a & 1)) {
					t_[a+1].ST += x;
					t_[a+1].ND += x;
				}
				if (b & 1) {
					t_[b-1].ST += x;
					t_[b-1].ND += x;
				}
				a >>= 1;
				b >>= 1;
				t_[a].ND = t_[a].ST + max(t_[a << 1].ND, t_[(a << 1) + 1].ND);
				t_[b].ND = t_[b].ST + max(t_[b << 1].ND, t_[(b << 1) + 1].ND);
			}
			while (a >>= 1)
				t_[a].ND = t_[a].ST + max(t_[a << 1].ND, t_[(a << 1) + 1].ND);
		}

		D (
			void inorder_walk(size_t x, std::string sp, std::string sn)
			{
			#ifdef _WIN32
				static std::string cr="  ", cl="  ", cp="  ";
				static bool _$_=true;
				if(_$_)
				{
					_$_=false;
					cr[0]=218;cr[1]=196;
					cl[0]=192;cl[1]=196;
					cp[0]=179;
				}
			#else
				static std::string cr="/", cl="\\", cp="| ";
			#endif
				std::string t;
				if(x < size_ << 1)
				{
					t=sp;
					if(sn==cr) t[t.size()-2] = ' ';
					inorder_walk((x << 1) + 1, t+cp, cr);
					t=t.substr(0,sp.size()-2);
					std::cerr << t << sn << "\e[1;34m" << x << ": \e[1;32m" << t_[x] << "\e[0m" << std::endl;
					t=sp;
					if(sn==cl) t[t.size()-2] = ' ';
					inorder_walk(x << 1, t+cp, cl);
				}
			}

			void print()
			{
				std::cerr << "{\n";
				inorder_walk(1, "", "");
				std::cerr << "}" << std::endl;
			}
		)
	};
};

class LasProblem : public Problem {
public:
	string name() const { return "Lasuchy"; }

	string tag() const { return "LAS"; }

	string help() const { return "LAS:\n  gen las [N] [ARGS]... - generates N tests, in ARGS you can use comparisions to set variables: n, ci\n    To see default limits type 'gen las'\n    Example: gen las 10 n<10 ci<30 - generates 10 tests where n < 10, ci < 30\n"; }

	int checker(const string& input, const string& output, const string& answer, size_t* l = NULL, string* errors = NULL) {
		(void) output; // Disable Unused parameter warning
		FILE *in = fopen(input.c_str(), "r"), *ans = fopen(answer.c_str(), "r");
		if (in == NULL || ans == NULL)
			return 2; // Checker error
		int n;
		if (0 == fscanf(in, "%i", &n))
			return 2;
		int arr[n+1];
		for (int i = 0; i < n; ++i)
			if (0 == fscanf(in, "%i", arr+i))
				return 2;
		arr[n] = arr[0];
		int out[n];
		for (int i = 0; i < n; ++i)
			if (0 == fscanf(ans, "%i", out+i)) {
				if (l)
					*l = 1;
				if (errors)
					*errors = "Not enough numbers (there is no answer 'NIE')";
				return 1;
			}
		for (int i = 0; i < n; ++i) {
			if (out[i] != i+1 && out[i] != 1 + (i+1) % n) {
				if (l)
					*l = 1;
				if (errors)
					*errors = "Wrong choice by lasuch " + myto_string(i+1) + "\nGet: " + myto_string(out[i]) + "\nExpected: " + myto_string(i+1) + " or " + myto_string(1 + (i+1) % n);
				return 1;
			}
			--out[i];
		}
		int a, b, c, d;
		for (int i = 0; i < n; ++i) {
			a = i;
			b = mod(i+1, n);
			c = arr[a] << 1;
			if (out[mod(i-1, n)] == a)
				c >>= 1;
			d = arr[b] << 1;
			if (out[mod(i+1, n)] == b)
				d >>= 1;
			if (out[i] == a && d > c) {
				if (l)
					*l = 1;
				if (errors)
					*errors = "Not optimal choise by lasuch " + myto_string(i+1) + " -> " + myto_string(a+1) + " which gives " + dmyto_string(double(c)/2) + " but " + myto_string(b+1) + ". gives " + dmyto_string(double(d)/2);
				return 1;
			} else if (out[i] == b && c > d) {
				if (l)
					*l = 1;
				if (errors)
					*errors = "Not optimal choise by lasuch " + myto_string(i+1) + " -> " + myto_string(b+1) + " which gives " + dmyto_string(double(d)/2) + " but " + myto_string(a+1) + ". gives " + dmyto_string(double(c)/2);
				return 1;
			}
		}
		fclose(in);
		fclose(ans);
		return 0;
	}

	int gen(const string& path, const string& args, bool in_only = false) {
		n_ = make_pair(2, 1000000);
		ci_ = make_pair(1, 1000000000);
		string N, x, new_args;
		for (size_t i = 0; i < args.size(); ++i) {
			if (args[i] == '<' || args[i] == '>' || args[i] == '=')
				new_args += string(" ") + args[i] + " ";
			else
				new_args += args[i];
		}
		ArgParser ap(new_args);
		// Parsing arguments
		vector<string> arg_arr;
		while (x = ap.getNextArg(), x.size())
			arg_arr.push_back(x);
		for (size_t i = 0, len = arg_arr.size(); i < len; ++i) {
			// If next arg is operator
			if (i + 1 < len && (arg_arr[i+1] == "<" || arg_arr[i+1] == ">" || arg_arr[i+1] == "="))
				continue;
			if (0 < i && i+1 < len && (arg_arr[i] == "<" || arg_arr[i] == ">" || arg_arr[i] == "=")) {
				string *var, *value;
				if (isPositiveNum(arg_arr[i-1])) {
					if (arg_arr[i] == "<")
						arg_arr[i] = ">";
					else if (arg_arr[i] == ">")
						arg_arr[i] = "<";
					value = &arg_arr[i-1];
					var = &arg_arr[i+1];
				} else {
					var = &arg_arr[i-1];
					value = &arg_arr[i+1];
				}
				if(*var == "n")
					try_set(n_, arg_arr[i], *value, 2, 1000000);
				else if(*var == "ci")
					try_set(ci_, arg_arr[i], *value, 1, 1000000000);
				else
					eprintf("Unknown variable: '%s'\n", var->c_str());
				++i;
			} else if (isPositiveNum(arg_arr[i]))
				N = arg_arr[i];
		}
		printf ("Set:\n n belongs to [%i, %i]\n ci belongs to [%i, %i]\n", n_.first, n_.second, ci_.first, ci_.second);
		Problem::gen(path, N, in_only); // We can use default gen
		return 0;
	}

	int genout(const string& input, const string& output) {
		FILE *in = fopen(input.c_str(), "r"), *out = fopen(output.c_str(), "w");
		if (in == NULL || out == NULL)
			return 1;
		/*int n, m;
		if (0 == fscanf(in, "%i%i", &n, &m))
			return 1;
		int f[n], w[m];
		ARR(PII, last, m);
		REP (i, n) {
			if (0 == fscanf(in, "%i", f+i))
				return 1;
			--f[i];
		}
		REP (i, m) {
			if (0 == fscanf(in, "%i", w+i))
				return 1;
			last[i] = MP(-1, -1);
		}
		itree tree(n);
		long long res = 0;
		REP (i, n) {
			E("========================\n");
			LOGN(i);
			LOGN(f[i]);
			LOGN(last[f[i]]);
			LOGN(w[f[i]]);
			if (last[f[i]].ST == -1) {
				tree.add(0, i, w[f[i]]);
				last[f[i]] = MP(0, i);
			} else {
				tree.add(last[f[i]].ST, last[f[i]].ND, -w[f[i]]);
				tree.add(last[f[i]].ND+1, i, w[f[i]]);
				last[f[i]] = MP(last[f[i]].ND+1, i);
			}
			maxi(res, tree.getMax());
			D(tree.print();)
		}
		fprintf(out, "%lli\n", res);*/
		fclose(in);
		fclose(out);
		return 0;
	}

protected:
	int genin(const string& file, int seed) {
		srand(seed);
		FILE *f = fopen(file.c_str(), "w");
		if (f == NULL)
			return 1;
		int n = getRandom(n_);
		fprintf(f, "%i\n%i", n, getRandom(ci_));
		for (int i = 1; i < n; ++i)
			fprintf(f, " %i", getRandom(ci_));
		fprintf(f, "\n");
		fclose(f);
		return 0;
	}
private:

	pair<int, int> n_, ci_;

	inline int mod(int a, int n) {
		a %= n;
		return a < 0 ? a + n : a;
	}
};

Problem* problems_available[] = {
	new PieProblem,
	new KinProblem,
	new LasProblem
	};

size_t problems_available_size = sizeof(problems_available) / sizeof(Problem*); // Number of elements
