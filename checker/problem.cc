#include "functions.h"
#include "judge.h"
#include "problem.h"

#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <deque>
#include <fstream>

#ifdef _WIN32
# include <direct.h>

std::string convertToWinPath(std::string s) {
	for (size_t i = 0; i < s.size(); ++i)
		if (s[i] == '/')
			s[i] = '\\';
	return s;
}

inline bool path_exists(const std::string& path) {
	return !system(("IF EXIST \"" + path + "\" (EXIT 0) ELSE (EXIT 1)").c_str());
}

#endif

#define eprintf(args...) fprintf(stderr, args)

#ifdef DEBUG
# define D(...) __VA_ARGS__
#else
# define D(...)
#endif

using std::string;
using std::vector;

int Problem::checker(const string& input, const string& output, const string& answer, size_t* l, string* errors) {
	(void)(input); // Disable -Wunused-parameter
	std::fstream out(output.c_str(), std::ios::in), ans(answer.c_str(), std::ios::in);
	if (!ans.good() && !out.good())
		return 2; // Evaluation failure
	vector<string> ans_in, out_in;
	string ans_tmp, out_tmp;
	while (ans.good() && out.good()) {
		getline(ans, ans_tmp);
		getline(out, out_tmp);
		remove_trailing_spaces(ans_tmp);
		remove_trailing_spaces(out_tmp);
		ans_in.push_back(ans_tmp);
		out_in.push_back(out_tmp);
	}


	while (!ans_in.empty() && ans_in.back().empty())
		ans_in.pop_back();
	while (!out_in.empty() && out_in.back().empty())
		out_in.pop_back();

	auto shortenLine = [](string line) {
		constexpr uint max_length = 512;
		if (line.size() > max_length) {
			line.erase(line.begin() + max_length, line.end());
			line += "...";
		}

		return line;
	};

	size_t line = -1;
	while (++line < ans_in.size() && line < out_in.size())
		if (out_in[line] != ans_in[line]) {
			if (l)
				*l = line + 1;
			if (errors)
				*errors = string("Get:\n'") + shortenLine(ans_in[line]) + "'\nExpected:\n'" + shortenLine(out_in[line]) + '\'';
			return 1; // Wrong answer
		}
	if (out_in.size() > ans_in.size()) {
		if (l)
			*l = line + 1;
		if (errors)
			*errors = string("Get:\n'EOF'\nExpected:\n'") + shortenLine(out_in[line]) + '\'';
		return 1; // Wrong answer
	}
	return 0; // OK
}

static int cleanDirectory(const string& path) {
	printf("Cleaning directory...");
	fflush(stdout);
#ifdef _WIN32
	remove_r(path.c_str());
	system(("mkdir \"" + convertToWinPath(path) + "\" > NUL 2> NUL").c_str());
	if (!path_exists(path)) {
		printf(" Failed.\n");
		return 1;
	}
#else
	if (0 != system(("rm -rf '" + path + "'; mkdir -p '" + path +"'").c_str())) {
		printf(" Failed.\n");
		return 1;
	}
#endif
	printf(" Done.\n");
	return 0;
}

int Problem::gen(const string& path, const std::string& args, bool in_only) {
	int n = 0;
	ArgParser ap(args);
	string sn = ap.getNextArg();
	if (isNum(sn))
		n = atoi(sn.c_str());
	else
		eprintf("'%s' is not a number\n", sn.c_str());
	D (printf("'%s'\n", args.c_str());)

	// Clean directory
	{
		int rc = cleanDirectory(path);
		if (rc)
			return rc;
	}

	int seed = getRandom(1, 1e9), val = getRandom(1, 1024);
	string test;
	for (int i = 1; i <= n; ++i) {
		test = path;
		test += myto_string(i);
		printf("%i: ", i);
		fflush(stdout);
		if (0 != genin(test + ".in", seed += val)) {
			printf("failed -> in\n");
			return 2;
		}
		if (!in_only && 0 != genout(test + ".in", test + ".out")) {
			printf("failed -> out\n");
			return 3;
		}
		printf("generated\n");
	}
	return 0;
}

int Problem::refute(const string& path, const std::string& exec, const std::string&) {
	// Check if exec exists
	if (exec.empty()) {
		eprintf("Missing executable argument\n");
		return 4;
	}

	FILE *f = fopen(exec.c_str(), "r");
	if (f)
		fclose(f);
	else {
		eprintf("No such file: '%s'\n", exec.c_str());
		return 4;
	}
	// Check if exec is executeable
#ifndef _WIN32
	if (access(exec.c_str(), X_OK) == -1) {
		eprintf("Execution permission denied: '%s'\n", exec.c_str());
		return 4;
	}
#endif

	// Clean directory
	{
		int rc = cleanDirectory(path);
		if (rc)
			return rc;
	}

	int seed = getRandom(1, 1e9), val = getRandom(1, 1024);
	judge.inFile_ = path + "refute.in";
	judge.outFile_ = path + "refute.out";
	judge.ansFile_ = path + "refute.ans";
	judge.exec_ = exec;
	for (uint i = 0; ; ++i) {
		printf("%u: ", i);
		fflush(stdout);
		if (genin(judge.inFile_, seed += val)) {
			printf("failed -> in\n");
			return 2;
		}
		if (genout(judge.inFile_, judge.outFile_)) {
			printf("failed -> out\n");
			return 3;
		}

		if (judge.checkOnTest(this, true)) {
			printf("\nFound refuting test - see:\nin -> '%s'\nout -> '%s'", judge.inFile_.c_str(), judge.outFile_.c_str());
			break;
		}
	}
	return 0;
}
