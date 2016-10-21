#include "problem.h"
#include "judge.h"
#include "functions.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>

#ifdef _WIN32
# include <iostream>
#endif

#define eprintf(args...) fprintf(stderr, args)

using std::string;

void parse_line(const char* line);

#ifdef USE_READLINE
# include <sys/types.h>
# include <sys/select.h>
# include <readline/readline.h>
# include <readline/history.h>

int running;

void checker_linehandler(char *line) {
	if (!line || strcmp(line, "exit") == 0) {
		if (!line)
			printf ("\n");
		rl_callback_handler_remove ();
		running = 0;
	}
	else {
		if (*line)
			add_history(line);
		parse_line(line);
		free(line);
	}
}
#endif

const char* prompt = "checker$ ";

extern std::vector<Problem*> problems_available;

void problems() {
	printf("Available problems:\n");
	for (auto&& problem : problems_available)
		printf("%s [%s]\n", problem->name().c_str(), problem->tag().c_str());
}

void help() {
	printf("Available commands:\n"
		"  help                      display this help\n"
		"  problems                  display available problems\n"
		"  judge TAG EXEC [TEST_DIR] [TEST]...  judge EXEC in TAG problem, in TEST_DIR (default tests/TAG/), on tests TEST... (default all)\n"
		"  gen TAG [ARGS]...         generate tests for problem TAG\n"
		"  genin TAG [ARGS]...       generate only in tests for problem TAG\n"
		"  genout TAG FILE...        generate output file for input FILE in problem TAG\n"
		"  refute TAG EXEC [ARGS]... generate tests for problem TAG and test EXEC on them immediately, until refuting test is found\n"
		"\n"
		"Default:\n"
		"  gen TAG [N]               generate N (default 0) tests for problem TAG\n"
		"\n"
		"Using comparisons:\n"
		"  You can use: 'n < A' or 'A < n' or 'A = n' or 'A < n <= B' or 'n > A'... (A, B are numbers) you don't have to use spaces between expressions e.g. 'n<1 m>2 x=4', but expression syntax is 'VAR OPER N' or 'N OPER VAR' (VAR - variable name, OPER - operator ('<', '>', '<=', '>=' or '='), N - number)\n"
		"\n");
	for (auto&& problem : problems_available)
		printf("%s\n", problem->help().c_str());
}

void parse_line(const char* line) {
	// Extracting command
	string cmd;
	size_t s = strlen(line), i = 0;
	while (line[i] == ' ' || line[i] == '\t' || line[i] == '\n')
		++i;
	for (; i < s; ++i) {
		if (line[i] == ' ' || line[i] == '\t' || line[i] == '\n')
			break;
		cmd += line[i];
	}

	// Help
	if (strcmp(cmd.c_str(), "help") == 0)
		help();

	// Problems
	else if (strcmp(cmd.c_str(), "problems") == 0)
		problems();

	// Generate tests
	else if (strcmp(cmd.c_str(), "gen") == 0) {
		string tag = tolower(getNextArg(line, i, s));
		bool tag_exists = false;
		for (auto&& problem : problems_available)
			if (tolower(problem->tag()) == tag) {
				problem->gen("tests/" + tag + "/", string(line + i, line + s));
				tag_exists = true;
				break;
			}
		if (!tag_exists)
			eprintf("Unknown problem tag: '%s'\n", tag.c_str());

	// Generate tests' inputs
	} else if (strcmp(cmd.c_str(), "genin") == 0) {
		string tag = tolower(getNextArg(line, i, s));
		bool tag_exists = false;
		for (auto&& problem : problems_available)
			if (tolower(problem->tag()) == tag) {
				problem->gen("tests/" + tag + "/", string(line + i, line + s), true);
				tag_exists = true;
				break;
			}
		if (!tag_exists)
			eprintf("Unknown problem tag: '%s'\n", tag.c_str());

	// Generate tests' outputs
	} else if (strcmp(cmd.c_str(), "genout") == 0) {
		string tag = tolower(getNextArg(line, i, s));
		bool tag_exists = false;
		for (auto&& problem : problems_available)
			if (tolower(problem->tag()) == tag) {
				string test_in, test_out;
				while (test_in = getNextArg(line, i, s), test_in.size()) {
					if (file_exists(test_in)) {
						if (test_in.size() >= 3 && test_in.compare(test_in.size() - 3, 3, ".in") == 0)
							test_out = test_in.substr(0, test_in.size()-3) + ".out";
						else
							test_out = test_in + ".out";
						problem->genout(test_in, test_out);
					} else
						eprintf("No such file: '%s'\n", test_in.c_str());
				}
				tag_exists = true;
				break;
			}
		if (!tag_exists)
			eprintf("Unknown problem tag: '%s'\n", tag.c_str());

	// Judge solution
	} else if (strcmp(cmd.c_str(), "judge") == 0) {
		string tag = tolower(getNextArg(line, i, s));
		bool tag_exists = false;
		for (auto&& problem : problems_available)
			if (tolower(problem->tag()) == tag) {
				string exec = getNextArg(line, i, s);
				Problem::judge(problem, exec, string(line + i, line + s));
				tag_exists = true;
				break;
			}
		if (!tag_exists)
			eprintf("Unknown problem tag: '%s'\n", tag.c_str());

	// Refute solution
	} else if (strcmp(cmd.c_str(), "refute") == 0) {
		string tag = tolower(getNextArg(line, i, s));
		bool tag_exists = false;
		for (auto&& problem : problems_available)
			if (tolower(problem->tag()) == tag) {
				string exec = getNextArg(line, i, s);
				problem->refute("tests/" + tag + "/", exec, string(line + i, line + s));
				tag_exists = true;
				break;
			}
		if (!tag_exists)
			eprintf("Unknown problem tag: '%s'\n", tag.c_str());

	// Non-internal command - run it via shell
	} else
		system(line);
}

int main(int argc, char **argv) {
	// Run command passed via main parameters and exit
	if (argc > 1) {
		string line;
		for (int i = 1; i < argc; ++i) {
			if (i > 0)
				line += ' ';

			// Escape white spaces
			/*for (char *c = argv[i]; *c != '\0'; ++c) {
				if (isspace(*c) || *c == '\\')
					line += '\\';
				line += *c;
			}*/

			// Escaping makes problem with passing limits via parameters
			line += argv[i];
		}

		// Run command
		parse_line(line.c_str());
		return 0;
	}

	printf("Type help for help\n");
#ifdef USE_READLINE
	fd_set fds;
	int r;
	rl_callback_handler_install(prompt, checker_linehandler);
	running = 1;
	while (running) {
		FD_ZERO(&fds);
		FD_SET(fileno(rl_instream), &fds);
		r = select(FD_SETSIZE, &fds, nullptr, nullptr, nullptr);
		if (r < 0) {
			perror("select()");
			rl_callback_handler_remove();
			break;
		}
		if (FD_ISSET(fileno(rl_instream), &fds))
			rl_callback_read_char();
	}
#elif defined(_WIN32)
	string line;
	printf("%s", prompt);
	fflush(stdout);
	while (getline(std::cin, line) && line != "exit") {
		parse_line(line.c_str());
		printf("%s", prompt);
		fflush(stdout);
	}
	printf("\n");
#else
	char *line = nullptr;
	size_t n = 0;
	ssize_t read;
	printf("%s", prompt);
	while ((read = getline(&line, &n, stdin)) != -1) {
		while (read-1 >= 0 && (line[read-1] == '\n' || line[read-1] == '\r'))
			line[--read] = '\0';
		if (strcmp(line, "exit") == 0)
			break;
		parse_line(line);
		printf("%s", prompt);
	}
	printf("\n");
#endif
	return 0;
}