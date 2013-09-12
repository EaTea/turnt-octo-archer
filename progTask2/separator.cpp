#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <map>
using namespace std;

struct pt
{
	double x, y;
	pt(double xx = 0, double yy = 0) : x(xx), y(yy) {}
};

#define SGN(p) ((p) < 0 ? -1 : (p) > 0 ? 1 : 0)

int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		cerr << "Need to provide file to read for initial classification." << endl;
		exit(1);
	}
	map<int,vector<pt> > pts;
	ifstream ifs(argv[1], ifstream::in);
	while(ifs.good())
	{
		pt p;
		ifs >> p.x >> p.y;
		int k;
		ifs >> k;
		pts[k].push_back(p);
	}
	if (ifs.bad())
	{
		cerr << "File " << argv[1] << " is formatted wrongly." << endl;
		exit(1);
	}
	// normalising
	double mx = 0, my = 0;
	for (map<int,vector<pt> >::iterator vctr = pts.begin(); vctr != pts.end(); vctr++)
		for (vector<pt>::iterator pt = vctr->second.begin(); pt != vctr->second.end(); pt++)
		{
			mx = max(mx,pt->x);
			my = max(my,pt->y);
		}
	for (map<int,vector<pt> >::iterator vctr = pts.begin(); vctr != pts.end(); vctr++)
		for (vector<pt>::iterator pt = vctr->second.begin(); pt != vctr->second.end(); pt++)
		{
			pt->x *= my;
			pt->y *= mx;
		}
	// finding centroids in new coordinate space
	map<int,pt> ctrds;
	for (map<int,vector<pt> >::iterator vctr = pts.begin(); vctr != pts.end(); vctr++)
	{
		int mapTo = vctr->first;
		for (vector<pt>::iterator pt = vctr->second.begin(); pt != vctr->second.end(); pt++)
		{
			ctrds[mapTo].x += pt->x;
			ctrds[mapTo].y += pt->y;
		}
		ctrds[mapTo].x /= vctr->second.size();
		ctrds[mapTo].y /= vctr->second.size();
	}
	// getting midpoint --- rescale
	pt midpt(ctrds[1].x+ctrds[2].x,ctrds[1].y+ctrds[2].y);
	midpt.x /= 2;
	midpt.y /= 2;
	// getting bisector, rescale
	pt bisector(-ctrds[2].y+ctrds[1].y, ctrds[2].x-ctrds[1].x);
	bisector.x /= mx;
	bisector.y /= my;
	double mag = sqrt(bisector.x*bisector.x + bisector.y*bisector.y);
	bisector.x /= mag;
	bisector.y /= mag;
	// to get the line
	double b = bisector.x, a = -bisector.y, c = -(b*midpt.y/mx + a*midpt.x/my);
	int bad1 = 0, bad2 = 0;
	// checking which points are bad
	for (map<int,vector<pt> >::iterator vctr = pts.begin(); vctr != pts.end(); vctr++)
	{
		int toMap = vctr->first;
		ctrds[toMap].x /= my;
		ctrds[toMap].y /= mx;
		for (vector<pt>::iterator pt = vctr->second.begin(); pt != vctr->second.end(); pt++)
		{
			pt->x /= my;
			pt->y /= mx;
			if (SGN(a*pt->x+b*pt->y+c) != SGN(a*ctrds[toMap].x+b*ctrds[toMap].y+c))
				toMap == 1 ? bad1++ : bad2++;
		}
	}
	cout << "Equation form is ax + by + c = 0.\nWe have:\n\ta = " << a <<"\n\tb = " << b << "\n\tc = " << c << endl;
	cout << "Had " << bad1 << " bad classifies for class 1 and " << bad2 << " bad classifies for class 2" << endl;
	cout << "Time to classify!" << endl;
	// further classifying
	while (!cin.eof() && cin.good())
	{
		pt p;
		cin >> p.x >> p.y;
		if (SGN(a*p.x+b*p.y+c) == SGN(b*ctrds[1].y+a*ctrds[1].x+c))
			cout << "Point (" << p.x << ", " << p.y << ") is a class 1 point" << endl;
		else
			cout << "Point (" << p.x << ", " << p.y << ") is a class 2 point" << endl;
	}
	if (!cin.good())
	{
		cerr << "Problem reading from standard input" << endl;
		exit(1);
	}
	exit(0);
}
