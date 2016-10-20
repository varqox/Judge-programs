#pragma once

#include <chrono>
#include <cstdlib>
#include <fcntl.h>
#include <map>
#include <random>
#include <stdint.h>
#include <string>
#include <unistd.h>

std::string getNextArg(const char* line, size_t& i, size_t s);
std::string tolower(std::string str);
std::string myto_string(long long a);
std::string dmyto_string(double a);
void remove_trailing_spaces(std::string& s);
bool file_exists(const std::string& file);
bool isNum(const std::string& s);

class ArgParser {
private:
	size_t pos_, size_;
	char* arg_;
public:
	explicit ArgParser(const std::string& s);
	ArgParser(const ArgParser& ap);
	ArgParser& operator=(const ArgParser& ap);

	std::string getNextArg();
	void reset() { pos_ = 0; }

	~ArgParser() {
		if (arg_)
			delete[] arg_;
	}
};

extern std::mt19937_64 gen__;

// Returns random number from [a, b]
inline int64_t getRandom(int64_t a, int64_t b) {
	return std::uniform_int_distribution<int64_t>(a, b)(gen__);
}

// Returns random number from [p.a, p.b]
template<class A, class B>
inline int getRandom(const std::pair<A, B>& p) {
	return getRandom(p.first, p.second);
}

// Returns random number from [a, b]
inline int64_t rd(int64_t a, int64_t b) {
	return getRandom(a, b);
}

bool isPositiveNum(const std::string& s);
unsigned long long atoull(const std::string& s);
bool isNum(const std::string& s, long long down, long long up);
void try_set(std::pair<int, int>& p, const std::string& oper, const std::string& val);
bool isOperator(const std::string& s);
std::string parseArgLimits(const std::string& args, std::map<std::string, std::pair<int,int> >& limits);
void printLimits(FILE* stream, const std::map<std::string, std::pair<int,int> >& limits);

#ifdef _WIN32

int remove_r(const char* path);

inline int remove_r(const std::string& path) { return remove_r(path.c_str()); }

#else

/**
 * @brief Behaves like close(2) but cannot be interrupted by signal
 *
 * @param fd file descriptor to close
 * @return 0 on success, -1 on error
 *
 * @errors The same that occur for close(2) expect EINTR
 */
inline int sclose(int fd) noexcept {
	while (close(fd) == -1)
		if (errno != EINTR)
			return -1;

	return 0;
}

/**
 * @brief Removes recursively file/directory @p pathname relative to the
 * directory file descriptor @p dirfd
 *
 * @param dirfd directory file descriptor
 * @param pathname file/directory pathname (relative to @p dirfd)
 *
 * @return 0 on success, -1 on error
 *
 * @errors The same that occur for fstatat64(2), openat(2), unlinkat(2),
 * fdopendir(3)
 */
int remove_rat(int dirfd, const char* pathname);

/**
 * @brief Removes recursively file/directory @p pathname
 * @details Uses remove_rat()
 *
 * @param pathname file/directory to remove
 * @return 0 on success, -1 on error
 *
 * @errors The same that occur for remove_rat()
 */
inline int remove_r(const char* pathname) {
	return remove_rat(AT_FDCWD, pathname);
}

// The same as remove_r(const char*)
inline int remove_r(const std::string& pathname) {
	return remove_rat(AT_FDCWD, pathname.c_str());
}
#endif
