#include "judge.h"
#include "functions.h"
#include <dirent.h>
#include <sys/time.h>
#include <cmath>
#include <string>
#include <cstdio>
#include <vector>
#include <algorithm>

#ifndef _WIN32
# include <unistd.h>
# include <sys/wait.h>
#endif

#define eprintf(args...) fprintf(stderr, args)

#ifdef DEBUG
# define D(...) __VA_ARGS__
#else
# define D(...)
#endif

using std::string;

struct CompareTestName {
	static std::pair<string, string> _getId(const string& str) {
		std::pair<string, string> res;
		string::const_reverse_iterator i = str.rbegin();
		while (i != str.rend() && isalpha(*i))
			res.second += *i++;
		while (i != str.rend() && isdigit(*i))
			res.first += *i++;
		std::reverse(res.first.begin(), res.first.end());
		std::reverse(res.second.begin(), res.second.end());
		return res;
	}

	bool operator()(const string& a, const string& b) {
		std::pair<string, string> x = _getId(a), y = _getId(b);
		return (x.first.size() == y.first.size() ? (x.first == y.first ? x.second < y.second : x.first < y.first) : x.first.size() < y.first.size());
	}
};

int Problem::JudgeClass::checkOnTest(Problem* pr, bool display_errors) {
	// Runtime
	timeval ts, te;
	gettimeofday(&ts, nullptr);
#ifdef _WIN32
	int ret = system((exec_ + " < " + inFile_ + " > " + ansFile_).c_str()) >> 8;
#else
	pid_t cpid;
	if ((cpid = fork()) == 0)
	{
		// Set up enviroment
		freopen((inFile_).c_str(), "r", stdin);
		freopen(ansFile_.c_str(), "w", stdout);

		char *arg[] = {nullptr};
		execve(exec_.c_str(), arg, arg);
		exit(-1);
	}
	int ret;
	waitpid(cpid, &ret, 0);
	if (WIFEXITED(ret))
		ret = WEXITSTATUS(ret);
	else if (WIFSIGNALED(ret))
		ret = WTERMSIG(ret) + 128;
	else
		// Shouldn't happen. Unknown status...
		ret = EXIT_FAILURE;
#endif
	gettimeofday(&te, nullptr);
	double cl = (te.tv_sec + static_cast<double>(te.tv_usec)/1000000) - (ts.tv_sec + static_cast<double>(ts.tv_usec)/1000000);
	// End of runtime && time calculating
	cl *= 1000;
	cl = round(cl) / 1000;
	runtime_ = cl;

	if (ret != 0) {
		printf("Runtime error (returned value: %i) %.3lfs\n", ret, cl);
		remove(ansFile_.c_str());
		return 2;
	}

	size_t line = 0;
	string errors;
	switch (pr->checker(inFile_, outFile_, ansFile_, &line, &errors)) {
		case 0:
			printf("[ OK ]  %.3lfs\n", cl);
			break;
		case 1:
			printf("WA  %.3lfs -> line: %lu\n", cl, (unsigned long)line);
			if (display_errors)
				printf("%s\n", errors.c_str());
			remove(ansFile_.c_str());
			return 1;
		default:
			printf("???  %.3lfs -> Checker error\n", cl);
			if (display_errors)
				printf("%s\n", errors.c_str());
			remove(ansFile_.c_str());
			return 3;
	}
	remove(ansFile_.c_str());
	return 0;
}

void Problem::JudgeClass::operator()(Problem* pr, const string& exec, const string& args) {
	double total_time = 0, max_time = 0;
	exec_ = exec;

	// Check if exec exists
	if (exec.empty()) {
		eprintf("Missing executable argument\n");
		return;
	}

	FILE *f = fopen(exec.c_str(), "r");
	if (f)
		fclose(f);
	else {
		eprintf("No such file: '%s'\n", exec.c_str());
		return;
	}
	// Check if exec is executeable
#ifndef _WIN32
	if (access(exec.c_str(), X_OK) == -1) {
		eprintf("Execution permission denied: '%s'\n", exec.c_str());
		return;
	}
#endif

	std::vector<string> WA, RTE; // Wrong answer, runtime error
	std::vector<string> tests;
	ArgParser ap(args);
	string max_time_test, test_dir = ap.getNextArg();
	bool is_default = test_dir.empty();
	if (test_dir.empty())
		test_dir = "tests/" + tolower(pr->tag());
	if (*test_dir.rbegin() != '/')
		test_dir += '/';
	D (eprintf("test_dir: %s\n", test_dir.c_str());)

	DIR* dir;
	dirent* test;
	string file_name;
try_open_dir:
	dir = opendir(test_dir.c_str());
	if (!dir) {
		eprintf("Cannot open directory '%s'\n", test_dir.c_str());
		if (!is_default) {
			is_default = true;
			test_dir = "tests/" + tolower(pr->tag()) + "/";
			eprintf("Try default directory '%s'\n", test_dir.c_str());
			ap.reset();
			closedir(dir);
			goto try_open_dir;
		} else {
			closedir(dir);
			return;
		}
	}

	// Check if tests are specified
	file_name = ap.getNextArg();
	if (file_name.size()) {
		do {
			inFile_ = test_dir + file_name + ".in";
			if (!file_exists(inFile_))
				eprintf("No such file: '%s'\n", inFile_.c_str());
			else {
				// We have .in and may .out file
				outFile_ = test_dir + file_name + ".out";
				printf("%s: ", file_name.c_str());
				fflush(stdout);

				int rc = checkOnTest(pr, true);
				if (rc == 1)
					WA.push_back(file_name);
				else if (rc != 0)
					RTE.push_back(file_name);

				total_time += runtime_;
				if (runtime_ > max_time) {
					max_time = runtime_;
					max_time_test = file_name;
				}
			}
			file_name = ap.getNextArg();
		} while (file_name.size());
	} else {
		// Run on all test in directory
		while ((test = readdir(dir)))
			if (file_exists(test_dir + test->d_name)) {
				file_name = test->d_name;
				size_t len = file_name.size();
				if (len > 3 && file_name.compare(len-3, 3, ".in") == 0)
					tests.push_back(file_name.substr(0, len-3));
			}

		sort(tests.begin(), tests.end(), CompareTestName());

		for (size_t i = 0; i < tests.size(); ++i) {
			// We have .in and may .out file
			inFile_ = test_dir + tests[i] + ".in";
			outFile_ = test_dir + tests[i] + ".out";

			printf("%s: ", tests[i].c_str());
			fflush(stdout);

			int rc = checkOnTest(pr);
			if (rc == 1)
				WA.push_back(tests[i]);
			else if (rc != 0)
				RTE.push_back(tests[i]);

			total_time += runtime_;
			if (runtime_ > max_time) {
				max_time = runtime_;
				max_time_test = tests[i];
			}
		}
	}

	if (WA.size() || RTE.size())
		printf("\n");
	if (WA.size()) {
		printf("WA:\n%s", WA.front().c_str());
		for (size_t i = 1; i < WA.size(); ++i)
			printf(" %s", WA[i].c_str());
		printf("\n");
	}
	if (RTE.size()) {
		printf("RTE:\n%s", RTE.front().c_str());
		for (size_t i = 1; i < RTE.size(); ++i)
			printf(" %s", RTE[i].c_str());
		printf("\n");
	}

	printf("\nTotal time - %.3lfs\nMax time - %.3lfs : %s\n", total_time, max_time, max_time_test.c_str());
}

Problem::JudgeClass Problem::judge("answer.checker");
