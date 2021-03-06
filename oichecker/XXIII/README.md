<div align="center">
  <img src="http://krzyk240.github.io/img/checker.png"/>
</div>

# What is checker

Checker is a library/tool used to simplify writing test generators.

# How it works
# How to use it
## Adding problem

To add problem to checker you only have to write class which inherit Problem class and add it to problems_available array in problems_available.cpp. Let's see an example:

Replace part:
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
		if (f == NULL)
			return 1;
		int n = 1 + rand() % 1000000;
		fprintf(f, "%i\n", n);
		fclose(f);
		return 0;
	}
};

Problem* problems_available[] = {
	new EasyProblem // Easy problem
};
```

In Easy problem you have to read a number and write it. Now you can build checker. (See [How to build](#building))

Let's see more complex example:

```C++
#include <fstream>

// For given number n (1 <= n <= 10^9) you have to find two numbers 0 < a, b < 10^18 for which GCD(a, b) = n
class ComplexProblem : public Problem {
public:
    ComplexProblem(): limits() {}

    string name() const { return "Complex"; }

    string tag() const { return "COM"; }

    string help() const { return "Complex:\n  gen com [N] [ARGS]... - generates N tests, in ARGS you can use comparisons to set variables: n\n    To see default ranges type 'gen com'\n    Example: gen com 10 n <= 28 - generates 10 tests in which n <= 28\n"; }

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
        limits.clear(); // Remove old limits (optional)
        limits["n"] = make_pair(1, 1000000000); // Set default limit for n
        string N, x;
        ArgParser ap(parseArgLimits(args, limits)); // Parse args for new limits, set them and create ap with other arguments (these which don't set limits)
        // Parsing arguments
        while (x = ap.getNextArg(), x.size())
            if (isPositiveNum(x)) // If x is positive number
                N = x;
        printLimits(stdout, limits);
        Problem::gen(path, N, in_only); // We can use default gen
        return 0;
    }

    int genout(const string& input, const string& output) {
        FILE *in = fopen(input.c_str(), "r"), *out = fopen(output.c_str(), "w");
        if (in == NULL || out == NULL) // If something went wrong
            return 1;
        // All is OK
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
        int n = getRandom(limits["n"]);
        FILE *f = fopen(file.c_str(), "w");
        if (f == NULL) // Check if cannot open .in file
            return 1;
        fprintf(f, "%i\n", n);
        fclose(f);
        return 0;
    }

private:
    map<string, pair<int, int> > limits;

    static long long GCD(long long a, long long b) {
        long long c;
        while (b > 0) {
            c = a % b;
            a = b;
            b = c;
        }
        return a;
    }
};

Problem* problems_available[] = {
    new ComplexProblem
};
```

We think all you want to know is in code above, but unless you think it is you can ask a question e.g. by Issue.

# Building

## Linux
If you want better command line install GNU Readline library (on Ubuntu `sudo apt-get install libreadline-dev`).

1. Clone Judge-programs repository:

  ```sh
  git clone https://github.com/krzyk240/Judge-programs
  ```

2. Build checker

  ```sh
  cd Judge-programs/checker
  make
  ```
Now you can run checker:
  ```sh
  ./chck
  ```

## Windows

1. Clone this repository where you want checker to live (you can [download it as zip](https://github.com/krzyk240/Judge-programs/archive/master.zip)).

2. Download [compile.bat](https://github.com/krzyk240/Judge-programs/releases/download/vWIN/compile.bat) and place it to `checker/` directory (this in which are files such as main.cpp problem.cpp)

3. Get the compiler - there are two ways:
 - Download and install [TDM-GCC-32](http://sourceforge.net/projects/tdm-gcc/files/TDM-GCC%20Installer/tdm-gcc-4.8.1-3.exe/download)
 - Download [TDM-GCC-32.zip](https://github.com/krzyk240/Judge-programs/releases/download/vWIN/TDM-GCC-32.zip) or [TDM-GCC-32.7z](https://github.com/krzyk240/Judge-programs/releases/download/vWIN/TDM-GCC-32.7z) and extract to the same directory where you have placed `compile.bat`

4. Run script `compile.bat`

Now you can run checker by executing file `chck.exe`
