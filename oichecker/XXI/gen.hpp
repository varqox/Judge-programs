#include <cstdlib>
#include <string>
#include <stack>

using namespace std;

bool is_number(const string&);
int to_int(const string&);

namespace gen
{
	template<typename type>
	inline type abs(type a)

	{return (a<type() ? -a:a);}
	inline int rd()
	{return abs(rand());}

	/*template<typename type>
	inline void swap(type& a, type& b)
	{type tmp(a); a=b; b=tmp;}*/

	template<typename type>
	void permute(type* begin, type* end, unsigned n=0)
	{
		unsigned size=end-begin;
		if(size==0) return;
		if(n==0) n=size*__builtin_clz(size)*2;
		for(unsigned i=0; i<n; ++i)
			swap(*(begin+rd()%size), *(begin+rd()%size));
	}

	int main(int argc, const char **argv);
}