#include "classifier.h"

// Author: Edwin Tay
// 20529864

int main(int argc, char *argv[])
{
	std::vector<point> inputPoints;
	bool okay = getPoints(inputPoints);
	if (!okay)
	{
		std::cerr << "Error: could not read input file of points" << std::endl;
		return 1;
	}
	//okay = normalise(inputPoints);
	if (!okay)
	{
		std::cerr << "Error: could not normalise points" << std::endl;
		return 2;
	}
	double a, b, c; //We will find equation such that ax + by + c = 0
	int nBadClassOne = 0, nBadClassTwo = 0;
	point c1, c2;
	okay = generateLinearSeparation(inputPoints, a, b, c, nBadClassOne, nBadClassTwo,c1,c2);
	if (!okay)
	{
		std::cerr << "Error: could not classify points" << std::endl;
		return 3;
	}
	//okay = renormalise(a, b, c);
	if (!okay)
	{
		std::cerr << "Error: could not renormalise parameters" << std::endl;
		return 4;
	}
	else
	{
		std::cout << "Line parameters as follows:" << std::endl;
		std::cout << "a = " << a << ", b = " << b << ", c = " << c << std::endl;
		std::cout << "Class One Falseties: " << nBadClassOne << std::endl;
		std::cout << "Class Two Falseties: " << nBadClassTwo << std::endl;
	}
	okay = classifyFuturePoints(a,b,c,c1,c2);
	if (!okay)
	{
		std::cerr << "Error: exit whilst classifying points" << std::endl;
		return 5;
	}
	return 0;
}
