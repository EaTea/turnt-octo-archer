#include "classifier.h"

bool getPoints(std::vector<point>& inputPoints)
{
	std::string fileName;
	std::cout << "Please enter the file containing the points. ";
	std::cin >> fileName;
	std::ifstream iff(fileName.c_str(), std::ifstream::in);
	if (!iff.is_open())
	{
		return false;
	}
	point tmp;
	while (iff.good() && !iff.eof())
	{
		if (!readPoint(tmp, iff)) return iff.eof();
		inputPoints.push_back(tmp);
	}
	iff.close();
	return iff.eof();
}

bool readPoint(point& p, std::istream& iii, bool readType)
{
	iii >> p.x >> p.y;
	if (readType) { int k; iii >> k; p.isClassOne = (k == 1) ; }
	return true;
}

#define DBG 1

void outputPoint(const point& p)
{
	if (DBG)
	{
		std::cerr << p.x << ' ' << p.y << ' ' << p.isClassOne << std::endl;
	}
	else
	{
		std::cout << p.x << ' ' << p.y << ' ' << (p.isClassOne ? 1 : 2) << std::endl;
	}
}
