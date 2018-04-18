#ifndef _UTILS_INCLUDE
#define _UTILS_INCLUDE

#include <iostream>
#include <cmath>
#include <vector>
#include <string>
#include <algorithm>

#define NUM_POWERS 9
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