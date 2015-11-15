#include "problem.h"
#include "functions.h"

#include <string>
#include <cstdlib>
#include <cstdio>

#define eprintf(args...) fprintf(stderr, args)

using std::string;
using std::pair;
using std::make_pair;
using std::map;



Problem* problems_available[] = {
};

size_t problems_available_size = sizeof(problems_available) / sizeof(Problem*); // Number of elements
