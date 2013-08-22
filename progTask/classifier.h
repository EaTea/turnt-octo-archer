#include <vector>
#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>

struct point
{
	double x, y;
	bool isClassOne;
	point(double X = 0, double Y = 0, bool classOne = false)
		: x(X), y(Y), isClassOne(classOne) {}
};

bool getPoints(std::vector<point>& inputPoints);

bool readPoint(point& p, std::istream& iii,bool readType = true);

void outputPoint(const point& p);

double squaredDistance(const point& a, const point& b);

bool generateLinearSeparation(std::vector<point>& inputPoints,
															double& a,
															double& b,
															double& c,
															int& nBad1,
															int& nBad2,
															point& cent1,
															point& cent2);

bool classifyFuturePoints(double& a,
												  double& b,
												  double& c,
													point& cent1,
													point& cent2);
