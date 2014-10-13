# Adding problem
To add problem to checker you only have to write class which inherit Problem class and add it to problems_available array in problems_available.cpp. Let's see an example:
Reaplace part:
```C++
Problem* problems_available[] = {
	};
```
in problems_available.cpp with:
```C++
class EasyProblem : public Problem {
public:
	string name() const { return "EASY"; }

	string tag() const { return "EAS"; }

	string help() const { return "EASY:\n  Using is obvious\n"; }

	int genout(const string& input, const string& output) {
		FILE *in = fopen(input.c_str(), "r"), *out = fopen(output.c_str(), "w");
		if (in == NULL || out == NULL)
			return 1;
		int n;
		fscanf(in, "%i", &n);
		fprintf(out, "%i\n", n);
		fclose(in);
		fclose(out);
		return 0;
	}

protected:
	int genin(const string& file, int seed) {
		srand(seed);
		FILE *f = fopen(file.c_str(), "w");
		int n = 1 + rand() % 1000000;
		if (f == NULL)
			return 1;
		fprintf(f, "%i\n", n);
		fclose(f);
		return 0;
	}
};

Problem* problems_available[] = {
	new EasyProblem // Easy problem
	};
```
In Easy problem you have to read a number and write it. Now you can build checker. (See [How to build](#build))
Let's see more complex example:
```C++

#include <fstream>

// For given number n (0 < n < 10^9) you have to find two numbers 0 < a, b < 10^18 for which GCD(a, b) = n
class ComplexProblem : public Problem {
public:
	string name() const { return "Complex"; }

	string tag() const { return "COM"; }

	string help() const { return "Complex:\n  gen com [N] [nA]   generate N tests where n < A (default 10^9); e.g. gen com 10 n28 - generates 10 tests in which n < 28\n"; }

	int checker(const string& input, const string& output, const string& answer, size_t* l = NULL, string* errors = NULL) {
		(void) output; // Disable Unused parameter warning
		std::fstream in(input.c_str(), std::ios::in), ans(answer.c_str(), std::ios::in);
		if (!in.good() || !ans.good())
			return 2; // Checker error
		string a, b;
		long long n;
		if (!(in >> n))
			return 2; // Checker error - failed to read n
		if (!(ans >> a >> b)) {
			if (l)
				*l = 1;
			if (errors)
				*errors = "Fail to read a and b";
			return 1;
		}
		if (a.size() > 18)
			swap(a, b);
		if (b.size() > 18) {
			if (l)
				*l = 1;
			if (errors)
				*errors = "'" + b + "' is to large";
			return 1;
		}
		long long a1, b1, gcd;
		if (!isPositiveNum(a) || atoull(a) == 0)
			swap(a, b);
		if (!isPositiveNum(b) || atoull(b) == 0) {
			if (l)
				*l = 1;
			if (errors)
				*errors = "'" + b + "' is not a positive number";
			return 1;
		}
		a1 = atoull(a);
		b1 = atoull(b);
		gcd = GCD(a1, b1);
		if (n != gcd) {
			if (l)
				*l = 1;
			if (errors)
				*errors = "GCD(" + a + ", " + b +  ") = " + myto_string(gcd) + " != " + myto_string(n);
			return 1;
		}
		return 0;
	}

	int gen(const std::string& path, const std::string& args, bool in_only = false) {
		string N, n = "999999999", x;
		ArgParser ap(args);
		// Parsing arguments
		while (x = ap.getNextArg(), x.size()) {
			if (x[0] == 'n') {
				x.erase(0, 1);
				if (isPositiveNum(x) && x.size() < 10 && atoull(x) != 0) {
					n = x;
				}
			} else if (isPositiveNum(x))
				N = x;
		}
		max_n_ = atoull(n);
		printf ("Set: n < %i\n", max_n_);
		Problem::gen(path, N, in_only); // We can use default gen
		return 0;
	}

	int genout(const string& input, const string& output) {
		FILE *in = fopen(input.c_str(), "r"), *out = fopen(output.c_str(), "w");
		if (in == NULL || out == NULL)
			return 1;
		long long n;
		fscanf(in, "%lli", &n);
		fprintf(out, "%lli %lli\n", n, n * 2);
		fclose(in);
		fclose(out);
		return 0;
	}

protected:
	int genin(const string& file, int seed) {
		srand(seed);
		int n = 1 + rand() % (max_n_ - 1);
		FILE *f = fopen(file.c_str(), "w");
		if (f == NULL)
			return 1;
		fprintf(f, "%i\n", n);
		fclose(f);
		return 0;
	}

private:
	int max_n_;

	static long long GCD(long long a, long long b) {
		long long c;
		while (b > 0) {
			c = a % b;
			a = b;
			b = c;
		}
		return a;
	}

	static bool isPositiveNum(const string& s) {
		for (size_t i = 0; i < s.size(); ++i)
			if (!isdigit(s[i]))
				return false;
		return true;
	}
	// Convert string to unsigned long long
	static unsigned long long atoull(const string& s) {
		unsigned long long res = 0;
		for (size_t i = 0; i < s.size(); ++i) {
			res *= 10;
			res += s[i] - '0';
		}
		return res;
	}
};

Problem* problems_available[] = {
	new ComplexProblem
	};
```

We think all you want to know is in code above, but if you don't think so you can ask a question e.g. by Issue.
