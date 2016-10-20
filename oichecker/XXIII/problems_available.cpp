#include "problem.h"
#include "functions.h"

// Krzysztof Małysa
#include <bits/stdc++.h>
#include <unistd.h>

using namespace std;

#define FOR(i,a,n) for (int i = (a), __n ## i = n; i < __n ## i; ++i)
#define REP(i,n) FOR(i,0,n)
#define FORD(i,a,n) for (int i = (a), __n ## i = n; i >= __n ## i; --i)
#define LET(x,a) __typeof(a) x = (a)
#define FOREACH(i,x) for (LET(i, x.begin()), __n##i = x.end(); i != __n##i; ++i)
#define ALL(x) (x).begin(), (x).end()
#define SZ(x) (int(x.size()))
#define eprintf(...) fprintf(stderr, __VA_ARGS__)
#define ST first
#define ND second
#define MP make_pair
#define PB push_back
#define O(...) ostream& operator <<(ostream& os, const __VA_ARGS__& x)
#define OO(...) O(__VA_ARGS__) { return __out(os, ALL(x)); }
#define T template
#define CL class

typedef unsigned uint;
typedef long long LL;
typedef unsigned long long ULL;
typedef vector<int> VI;
typedef vector<VI> VVI;
typedef vector<LL> VLL;
typedef pair<int, int> PII;
typedef vector<PII> VPII;
typedef vector<VPII> VVPII;
typedef pair<LL, LL> PLLLL;
typedef vector<PLLLL> VPLLLL;
typedef vector<bool> VB;
typedef vector<char> VC;
typedef vector<string> VS;

T<CL A>
inline A abs(const A& a) { return a < A() ? -a : a; }

T<CL A, CL B>
inline void mini(A& a, const B& b) {
	if (b < a)
		a = b;
}

T<CL A, CL B>
inline void maxi(A& a, const B& b) {
	if (b > a)
		a = b;
}

T<CL Iter>
ostream& __out(ostream& os, Iter a, Iter b, const string& s = ", ");

T<CL A, CL B>
O(pair<A, B>);

T<CL A>
OO(vector<A>)

T<CL A>
OO(deque<A>)

T<CL A>
OO(list<A>)

T<CL A, CL B>
OO(set<A, B>)

T<CL A, CL B, CL C>
OO(map<A, B, C>)

T<CL A, CL B>
OO(multiset<A, B>)

T<CL A, CL B, CL C>
OO(multimap<A, B, C>)

T<CL Iter>
ostream& __out(ostream& os, Iter a, Iter b, const string& s) {
	os << "{";
	if (a != b) {
		os << *a;
		while (++a != b)
			os << s << *a;
	}
	return os << "}";
}

T<CL Iter>
ostream& __dump(ostream& os, Iter a, Iter b) {
	os << "{\n";
	Iter beg = a;
	while (a != b) {
		os << " " << a - beg << ": " << *a << "\n";
		++a;
	}
	return os << "}";
}

T<CL A, CL B>
O(pair<A, B>) {
	return os << "(" << x.ST << ", " << x.ND << ")";
}

CL Input {
	static const int BUFF_SIZE = 1 << 16;
	unsigned char buff[BUFF_SIZE], *pos, *end;

	void grabBuffer() {
		pos = buff;
		end = buff + read(0, buff, BUFF_SIZE);
	}

public:
	Input() : pos(buff), end(buff) {}

	int peek() {
		if (pos == end)
			grabBuffer();
		return pos != end ? *pos : -1;
	}

	int getChar() {
		if (pos == end)
			grabBuffer();
		return pos != end ? *pos++ : -1;
	}

	void skipWhiteSpaces() {
		while (isspace(peek()))
			++pos;
	}

	T<CL A>
	A get();

	void operator()(char* s) {
		skipWhiteSpaces();
		while (!isspace(peek()))
			*s++ = *pos++;
		*s = '\0';
	}

	T<CL A>
	void operator()(A& x) { x = get<A>(); }

	T<CL A, CL B>
	void operator()(A& a, B& b) { operator()(a), operator()(b); }

	T<CL A, CL B, CL C>
	void operator()(A& a, B& b, C& c) { operator()(a, b), operator()(c); }

	T<CL A, CL B, CL C, CL D>
	void operator()(A& a, B& b, C& c, D& d) {
		operator()(a, b, c);
		operator()(d);
	}

	T<CL A, CL B, CL C, CL D, CL E>
	void operator()(A& a, B& b, C& c, D& d, E& e) {
		operator()(a, b, c, d);
		operator()(e);
	}

	T<CL A, CL B, CL C, CL D, CL E, CL F>
	void operator()(A& a, B& b, C& c, D& d, E& e, F& f) {
		operator()(a, b, c, d, e);
		operator()(f);
	}

	Input& operator>>(char *s) {
		operator()(s);
		return *this;
	}

	T<CL A>
	Input& operator>>(A& a) {
		operator()(a);
		return *this;
	}
} fin;

T<> char Input::get<char>() {
	skipWhiteSpaces();
	return *pos++;
}

T<> uint Input::get<uint>() {
	skipWhiteSpaces();
	uint x = 0;
	while (isdigit(peek()))
		x = x * 10 + *pos++ - '0';
	return x;
}

T<> int Input::get<int>() {
	skipWhiteSpaces();
	return peek() == '-' ? (++pos, -get<uint>()) : get<uint>();
}

T<> ULL Input::get<ULL>() {
	skipWhiteSpaces();
	ULL x = 0;
	while (isdigit(peek()))
		x = x * 10 + *pos++ - '0';
	return x;
}

T<> LL Input::get<LL>() {
	skipWhiteSpaces();
	return peek() == '-' ? (++pos, -get<ULL>()) : get<ULL>();
}

T<> string Input::get<string>() {
	skipWhiteSpaces();
	string x;
	while (!isspace(peek()))
		x += *pos++;
	return x;
}

int ceil2(int x) { return 1 << (sizeof(x) * 8 -__builtin_clz(x - 1)); }

#undef T
#undef CL
#ifdef DEBUG
# define D(...) __VA_ARGS__
# define E(...) eprintf(__VA_ARGS__)
# define OUT(a,b) cerr << #a ":", __out(cerr, a, b), E("\n")
# define DUMP(x) cerr << #x ":", __dump(cerr, ALL(x)), E("\n")
# define LOG(x) cerr << #x ": " << (x)
# define LOG2(x, y) cerr << x << ": " << (y)
# define LOGN(x) LOG(x) << endl
# define LOGN2(x, y) LOG2(x, y) << endl
#else
# define D(...)
# define E(...)
# define OUT(...)
# define DUMP(...)
# define LOG(...)
# define LOG2(...)
# define LOGN(...)
# define LOGN2(...)
#endif
/// End of templates

inline int strangeRandom(int a, int b) {
	b -= a;
	int x = pow(b, getRandom(0, 1 << 30) / double(1 << 30));
	if (x > 0)
		--x;
	mini(x, b);
	return x + a;
}

inline int strangeRandom(const PII& p) { return strangeRandom(p.ST, p.ND); }

class NimProblem : public Problem {
public:
	NimProblem(): limits() {}

	string name() const { return "Nim z utrudnieniem"; }

	string tag() const { return "NIM"; }

	string help() const {
		return "Nim:\n"
			"  gen nim [N] [ARGS]... - generates N tests, in ARGS you can use comparisons to set variables: n, d, ai\n"
			"    To see default ranges type 'gen nim'\n"
			"    Example: gen nim 10 n <= 28 d = 5 ai < 10 - generates 10 tests in which n <= 28, d = 5 and ai < 10\n";
	}

	int gen(const std::string& path, const std::string& args, bool in_only = false) {
		limits.clear(); // Remove old limits (optional)
		limits["n"] = make_pair(1, 500000); // Set default limit for n
		limits["d"] = make_pair(1, 10); // Set default limit for n
		limits["ai"] = make_pair(1, 1000000); // Set default limit for ai
		string N, x;
		ArgParser ap(parseArgLimits(args, limits)); // Parse args for new limits, set them and create ap with other arguments (these which don't set limits)
		// Parsing arguments
		while (x = ap.getNextArg(), x.size())
			if (isPositiveNum(x)) // If x is positive number
				N = x;

		mini(limits["n"].ST, 10000000 / limits["ai"].ST);
		mini(limits["n"].ND, 10000000 / limits["ai"].ST);

		printLimits(stdout, limits);
		Problem::gen(path, N, in_only); // We can use default gen
		return 0;
	}

private:
	static void wzorcowka(FILE *__in, FILE *__out) {
		constexpr int mod = 1000000007;
		int n, d;
		// fin >> n >> d;
		fscanf(__in, "%i%i", &n, &d);
		VI in(n);
		REP (i, n)
			// fin >> in[i];
			fscanf(__in, "%i", &in[i]);

		sort(ALL(in));

		VVI t(d, VI(1));
		t[0][0] = 1; // We can achieve empty set (with xor 0) in only one way
		for (int x : in) {
			int c = ceil2(x + 1);
			for (auto& i : t)
				i.resize(c);

			VI last = t[d - 1];
			FORD (i, d - 1, 1)
				FORD (j, c - 1, 0)
					t[i][j] = (t[i][j] + t[i - 1][j ^ x]) % mod;

			FORD (j, c - 1, 0)
				t[0][j] = (t[0][j] + last[j ^ x]) % mod;

			// D(
				// E("======================================\n");
				// LOGN(x);
				// DUMP(t);
			// )
		}

		int xor_all = 0;
		for (int i : in)
			xor_all ^= i;

		// LOGN(xor_all);

		int res = t[0][xor_all];
		if (n % d == 0 && --res < 0)
			res += mod;

		fprintf(__out, "%i\n", res);
	}
public:

	int genout(const string& input, const string& output) {
		FILE *in = fopen(input.c_str(), "r"), *out = fopen(output.c_str(), "w");
		if (in == NULL || out == NULL) // If something went wrong
			return 1;

		// All is OK
		wzorcowka(in, out);

		fclose(in);
		fclose(out);
		return 0;
	}

protected:
	int genin(const string& file, int seed) {
		srand(seed);
		FILE *f = fopen(file.c_str(), "w");
		if (f == NULL) // Check if cannot open .in file
			return 1;

		int max_sum = 10000000;
		int n = getRandom(limits["n"]);
		int d = getRandom(limits["d"]);
		int min_ai = limits["ai"].ST;
		int max_ai = (n * limits["ai"].ND > max_sum ? strangeRandom(limits["ai"]) : (getRandom(0, 10) < 2 ? limits["ai"].ND : getRandom(limits["ai"])));
		double probability = (max_ai * n > max_sum ? 1.0 / n : 1.0);

		fprintf(f, "%i %i\n", n, d);
		REP (i, n) {
			int down = min_ai;
			int up = min(max_ai, max_sum - (n - i - 1) * min_ai);
			int a = (getRandom(0, n) < n * probability ? getRandom(down, up) : strangeRandom(down, up));
			max_sum -= a;
			fprintf(f, i ? " %i" : "%i", a);
		}
		fprintf(f, "\n");

		fclose(f);
		return 0;
	}

private:
	map<string, pair<int, int> > limits;
};

Problem* problems_available[] = {
	new NimProblem
};

size_t problems_available_size = sizeof(problems_available) / sizeof(Problem*); // Number of elements
