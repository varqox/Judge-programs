#include "problem.h"

#include <string>
#include <cstdlib>
#include <cstdio>

using std::string;

class UnknownProblem : public Problem {
public:
	string name() const { return "Unknown"; }

	string tag() const { return "UNK"; }

	string help() const { return "Unknown:\n    ...\n"; }

	int genout(const string& input, const string& output) {
		FILE *in = fopen(input.c_str(), "r"), *out = fopen(output.c_str(), "w");
		if (in == NULL || out == NULL)
			return 1;
		int n, a;
		fscanf(in, "%i", &n);
		long long res = 0;
		for (int i = 0; i < n; ++i) {
			fscanf(in, "%i", &a);
			res += a;
		}
		fprintf(out, "%lli\n", res);
		fclose(in);
		fclose(out);
		return 0;
	}

protected:
	int genin(const string& file, int seed) {
		srand(seed);
		int n = 1 + rand() % 100000;
		FILE *f = fopen(file.c_str(), "w");
		if (f == NULL)
			return 1;
		fprintf(f, "%i\n", n);
		for (int i = 0; i < n; ++i)
			fprintf(f, "%i ", rand() % 100000000);
		fprintf(f, "\n");
		fclose(f);
		return 0;
	}
};

Problem* problems_available[] = {
	new UnknownProblem // Unknown problem
	};

size_t problems_available_size = sizeof(problems_available) / sizeof(Problem*); // Number of elements
