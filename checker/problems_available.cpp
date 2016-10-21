#include "problem.h"
#include "functions.h"

#include <string>
#include <cstdlib>
#include <cstdio>

#define eprintf(args...) fprintf(stderr, args)

using namespace std;

std::vector<Problem*> problems_available {
};
