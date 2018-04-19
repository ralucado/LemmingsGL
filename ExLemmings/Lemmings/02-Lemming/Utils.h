#ifndef _UTILS_INCLUDE
#define _UTILS_INCLUDE

#include <algorithm>
#include <cmath>
#include <iostream>
#include <set>
#include <string>
#include <vector>

#define NUM_POWERS 8
using namespace std;

class Utils
{
public:
	Utils();
	~Utils();
	static Utils &instance()
	{
		static Utils U;

		return U;
	}
	double pit_distance(int x1, int y1, int x2, int y2) const;
};

#endif // _UTILS_INCLUDE