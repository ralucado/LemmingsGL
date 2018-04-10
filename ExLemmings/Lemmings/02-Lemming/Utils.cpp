#include "Utils.h"


Utils::Utils()
{
}


Utils::~Utils()
{
}



double Utils::pit_distance(int x1, int y1, int x2, int y2) const
{
	return sqrt(double(pow(x1 - x2, 2) + pow(y1 - y2, 2)));
}
