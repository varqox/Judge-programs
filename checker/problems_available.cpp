#include "problem.h"
#include "functions.h"

#include <string>
#include <cstdlib>
#include <cstdio>

#define eprintf(args...) fprintf(stderr, args)

using std::string;
using std::pair;
using std::make_pair;
using std::map;

#include <fstream>

// For given number n (1 <= n <= 10^9) you have to find two numbers 0 < a, b < 10^18 for which GCD(a, b) = n
class ComplexProblem : public Problem {
public:
    ComplexProblem(): limits() {}

    string name() const { return "Complex"; }

    string tag() const { return "COM"; }

    string help() const { return "Complex:\n  gen com [N] [ARGS]... - generates N tests, in ARGS you can use comparisons to set variables: n\n    To see default ranges type 'gen com'\n    Example: gen com 10 n <= 28 - generates 10 tests in which n <= 28\n"; }

    int checker(const string& input, const string& output, const string& answer, size_t* l = nullptr, string* errors = nullptr) {
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
        limits["n"] = {1, 1000000000}; // Set default limit for n
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
        if (!in || !out) // If something went wrong
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
        if (!f) // Check if cannot open .in file
            return 1;
        fprintf(f, "%i\n", n);
        fclose(f);
        return 0;
    }

private:
    map<string, pair<int64_t, int64_t>> limits;

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

size_t problems_available_size = sizeof(problems_available) / sizeof(Problem*); // Number of elements
