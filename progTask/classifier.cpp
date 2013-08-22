#include "classifier.h"

double distance(const point& a, const point& b)
{
	return sqrt((a.x-b.x) * (a.x-b.x) + (a.y-b.y) * (a.y-b.y));
}

#define SGN(x) ((x) < 0 ? -1 : ((x) > 0 ? 1 : 0))

bool generateLinearSeparation(std::vector<point>& inputPoints,
															double& a,
															double& b,
															double& c,
															int& nBad1,
															int& nBad2,
															point& centroidOne,
															point& centroidTwo)
{
	int numOne=0, numTwo=0;
	for (std::vector<point>::iterator pt = inputPoints.begin(); pt != inputPoints.end(); pt++)
	{
		if (pt->isClassOne)
		{
			numOne++;
			centroidOne.x += pt->x;
			centroidOne.y += pt->y;
		}
		else
		{
			numTwo++;
			centroidTwo.x += pt->x;
			centroidTwo.y += pt->y;
		}
	}
	centroidOne.x /= numOne;
	centroidOne.y /= numOne;

	centroidTwo.x /= numTwo;
	centroidTwo.y /= numTwo;

	outputPoint(centroidOne);
	outputPoint(centroidTwo);

	point midpt(
			(centroidTwo.x+centroidOne.x)/2,
			(centroidTwo.y+centroidOne.y)/2);
	outputPoint(midpt);

	//direction vector
	point oneToTwo(
			centroidTwo.x-centroidOne.x,
			centroidTwo.y-centroidOne.y);

	double mag = distance(oneToTwo,point(0,0,false));
	std::cerr << mag << std::endl;

	oneToTwo.x /= mag;
	oneToTwo.y /= mag;
	//actual direction
	
	point bisector(oneToTwo.x,oneToTwo.y);
	outputPoint(bisector);
	
	b = bisector.y;
	a = bisector.x;
	c = -(a*midpt.x+b*midpt.y);

	for (std::vector<point>::iterator pt = inputPoints.begin(); pt != inputPoints.end(); pt++)
	{
		if(pt->isClassOne)
		{
			if(SGN(a*pt->x+b*pt->y+c) != SGN(a*centroidOne.x+b*centroidOne.y+c))
				nBad1++;
		}
		else
		{
			if(SGN(a*pt->x+b*pt->y+c) != SGN(a*centroidTwo.x+b*centroidTwo.y+c))
				nBad2++;
		}
	}
	return true;
}

bool classifyFuturePoints(double& a,
												  double& b,
												  double& c,
													point& c1,
													point& c2)
{
	point p;
	while(readPoint(p,std::cin,false) && !std::cin.eof())
	{
		double val = a*p.x+b*p.y+c;
		if (SGN(val) == SGN(a*c1.x+b*c1.y+c)) std::cout << "1" << std::endl;
		else std::cout << "2" << std::endl;
	}
	return true;
}
