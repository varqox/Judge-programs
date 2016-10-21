#pragma once

#include "functions.h"

#include <string>

class Problem {
private:
	class JudgeClass;

public:
	static JudgeClass judge;

	virtual std::string name() const = 0;
	virtual std::string tag() const = 0;
	virtual std::string help() const = 0;

	// input - .in, output - .out, answer - answer to verify, l - line, error - specific errors
	virtual int checker(const std::string& input, const std::string& output, const std::string& answer, size_t* l = nullptr, std::string* errors = nullptr);

	// path - directory in which tests have to be created, args arguments to gen
	virtual int gen(const std::string& path, const std::string& args, bool in_only = false);

	// path - directory in which tests have to be created, args arguments to refute
	virtual int refute(const std::string& path, const std::string& exec, const std::string& args);

	virtual int genout(const std::string& input, const std::string& output) = 0;

	virtual ~Problem() {}

protected:
	virtual int genin(const std::string& file, int seed) = 0;
};

class StandardProblem : public Problem {
protected:
	std::map<std::string, std::pair<int64_t, int64_t>> limits;

public:
	// Returns args after parsing out limits
	virtual std::string parseOutAndSetLimits(const std::string& args) = 0;

	virtual int gen(const std::string& path, const std::string& args,
		bool in_only = false)
	{
		ArgParser ap(parseOutAndSetLimits(args)); // Parse args for new limits, set them and create ap with other arguments (these which don't set limits)
        // Parsing arguments
		std::string N, x;
        while (x = ap.getNextArg(), x.size())
            if (isPositiveNum(x)) // If x is positive number
                N = x;
        printLimits(stdout, limits);
        return Problem::gen(path, N, in_only); // We can use default gen
	}

	// path - directory in which tests have to be created, args arguments to refute
	virtual int refute(const std::string& path, const std::string& exec,
		const std::string& args)
	{
		// Parse args for new limits and set them
		std::string new_args = parseOutAndSetLimits(args);
        printLimits(stdout, limits);
		return Problem::refute(path, exec, new_args);
	}
};
