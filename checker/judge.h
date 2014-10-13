#include "problem.h"
#include <string>
#include <cstdio>

#pragma once

class Problem::JudgeClass {
private:
	std::string inFile_, outFile_, exec_, ansFile_; // in, out - test files, ansFile_ - output file of exec
	double runtime_;
	int checkOnTest(Problem* pr, bool display_errors = false);

public:
	explicit JudgeClass(const std::string& s) : inFile_(), outFile_(), exec_(), ansFile_(s), runtime_(0) {
		FILE *f = fopen(s.c_str(), "w");
		if (f)
			fclose(f);
		else {
			struct : std::exception {
			  const char* what() const throw() {return "Cannot create answer file\n";}
			} exc;
			throw exc;
		}
	}
	void operator()(Problem* pr, const std::string& exec, const std::string& args);
	~JudgeClass() {
		if (ansFile_.size())
			remove(ansFile_.c_str());
	}
};
