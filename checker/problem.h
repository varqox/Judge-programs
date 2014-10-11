#include <string>

#pragma once

class Problem {
private:
	class JudgeClass;

public:
	static JudgeClass judge;

	virtual std::string name() const=0;
	virtual std::string tag() const=0;
	virtual std::string help() const=0;
	// input - .in, output - .out, answer - answer to verify, l - line, error - specific errors
	virtual int checker(const std::string& input, const std::string& output, const std::string& answer, size_t* l = NULL, std::string* errors = NULL);
	// path - directory in which tests have to be created, args arguments to gen
	virtual int gen(const std::string& path, const std::string& args, bool in_only = false);
	virtual ~Problem() {}
	virtual int genout(const std::string& input, const std::string& output)=0;
protected:
	virtual int genin(const std::string& file, int seed)=0;
};
