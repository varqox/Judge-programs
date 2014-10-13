#include "judge.h"
#include "functions.h"
#include <dirent.h>
#include <sys/time.h>
#include <cmath>
#include <string>
#include <cstdio>
#include <vector>
#include <algorithm>

#ifndef WIN32
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
	gettimeofday(&ts, NULL);
#ifdef WIN32
	int ret = system(("\"" << exec << "\" < \"" + inFile_ + "\" > \""+ ansFile_ +"\"").c_str()) >> 8;
#else
	pid_t cpid;
	if ((cpid = fork()) == 0)
	{
		// Set up enviroment
		freopen((inFile_).c_str(), "r", stdin);
		freopen(ansFile_.c_str(), "w", stdout);

		char *arg[] = {NULL};
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
	gettimeofday(&te, NULL);
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
		case 1:
			printf("WA  %.3lfs -> line: %u\n", cl, line);
			if (display_errors)
				printf("%s\n", errors.c_str());
			remove(ansFile_.c_str());
			return 1;
			break;
		case 2:
			printf("???  %.3lfs -> Checker error\n", cl);
			if (display_errors)
				printf("%s\n", errors.c_str());
			remove(ansFile_.c_str());
			return 3;
			break;
		default:
			printf("[ OK ]  %.3lfs\n", cl);
	}
	remove(ansFile_.c_str());
	return 0;
}

void Problem::JudgeClass::operator()(Problem* pr, const string& exec, const string& args) {
	double total_time = 0, max_time = 0;
	exec_ = exec;

	// Check if exec exists
	FILE *f = fopen(exec.c_str(), "r");
	if (f)
		fclose(f);
	else {
		eprintf("No such file: '%s'\n", exec.c_str());
		return;
	}
	// Check if exec is executeable
#ifndef WIN32
	if (access(exec.c_str(), X_OK) == -1) {
		eprintf("Execution permission denied: '%s'\n", exec.c_str());
		return;
	}
#endif

	std::vector<string> wrong_tests;
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
	if (dir == NULL) {
		eprintf("Cannot open directory '%s'\n", test_dir.c_str());
		if (!is_default) {
			is_default = true;
			test_dir = "tests/" + tolower(pr->tag()) + "/";
			eprintf("Try default directory '%s'\n", test_dir.c_str());
			ap.reset();
			goto try_open_dir;
		} else
			return;
	}

	// Check if tests are specified
	file_name = ap.getNextArg();
	if (file_name.size()) {
		do {
			if (!file_exists(inFile_ = test_dir + file_name + ".in"))
				eprintf("No sych file: '%s'\n", inFile_.c_str());
			else if (!file_exists(outFile_ = test_dir + file_name + ".out"))
				eprintf("No sych file: '%s'\n", outFile_.c_str());
			else {
				printf("%s: ", file_name.c_str());
				fflush(stdout);
				if (checkOnTest(pr, true) != 0)
					wrong_tests.push_back(file_name);

				total_time += runtime_;
				if (runtime_ > max_time) {
					max_time = runtime_;
					max_time_test = file_name;
				}
			}
			file_name = ap.getNextArg();
		} while (file_name.size());
	} else {
		while ((test = readdir(dir)))
			if (test->d_type == DT_REG) {
				file_name = test->d_name;
				size_t len = file_name.size();
				if (len > 3 && file_name.compare(len-3, 3, ".in") == 0)
					tests.push_back(file_name.substr(0, len-3));
				else if (len > 4 && file_name.compare(len-4, 4, ".out") == 0)
					tests.push_back(file_name.substr(0, len-4));
			}

		sort(tests.begin(), tests.end(), CompareTestName());

		for (size_t i = 0; i < tests.size(); ++i)
			if (i + 1 < tests.size() && tests[i] == tests[i+1]) {
				// We have .in and .out file
				inFile_ = test_dir + tests[i] + ".in";
				outFile_ = test_dir + tests[i] + ".out";

				printf("%s: ", tests[i].c_str());
				fflush(stdout);
				if (checkOnTest(pr) != 0)
					wrong_tests.push_back(tests[i]);

				total_time += runtime_;
				if (runtime_ > max_time) {
					max_time = runtime_;
					max_time_test = tests[i];
				}

				++i;
			}
	}

	if (wrong_tests.size()) {
		printf("Wrong test:\n%s", wrong_tests.front().c_str());
		for (size_t i = 1; i < wrong_tests.size(); ++i)
			printf(" %s", wrong_tests[i].c_str());
		printf("\n");
	}
	printf("Total time - %.3lfs\nMax time - %.3lfs : %s\n", total_time, max_time, max_time_test.c_str());
}

Problem::JudgeClass Problem::judge("answer.checker");
