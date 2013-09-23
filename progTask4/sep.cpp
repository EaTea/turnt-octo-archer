#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <cmath>
using namespace std;

struct pt
{
  double x, y;
  bool isOne;
  pt (double xx=0, double yy=0, bool i=false) : x(xx), y(yy), isOne(i) {}
};

typedef pt vec;

int main(int argc, char *argv[])
{
  if (argc < 2)
  {
    cerr << "Usage: needs at least one more argument" << endl;
    exit(1);
  }
  ifstream iff(argv[1], ifstream::in);
  vector<pt> points;
  double maxX = 0, maxY = 0;
  while (!iff.eof())
  {
    double x, y;
    int k;
    iff >> x >> y >> k;
    maxX = max(maxX, x);
    maxY = max(maxY, y);
    points.push_back(pt(x,y,(k == 1)));
  }
  pt cntr1(0,0,true), cntr0;
  int n1 = 0, n0 = 0;
  for (vector<pt>::iterator p = points.begin(); p != points.end(); p++)
  {
    if (p->isOne)
    {
      cntr1.x += (p->x *= maxY);
      cntr1.y += (p->y *= maxX);
      n1++;
    }
    else
    {
      cntr0.x += (p->x *= maxY);
      cntr0.y += (p->y *= maxX);
      n0++;
    }
  }
  cntr1.x /= n1;
  cntr1.y /= n1;
  cntr0.x /= n0;
  cntr0.y /= n0;

  pt mid((cntr1.x+cntr0.x)/2, (cntr1.y+cntr0.y)/2);
  mid.x /= maxY;
  mid.y /= maxX;

  vec grad(cntr0.y - cntr1.y, cntr1.x - cntr0.x);
  double mg = grad.x*grad.x + grad.y*grad.y;
  mg = sqrt(mg);
  grad.x /= mg;
  grad.y /= mg;

  double a = -grad.y, b = grad.x;
  double c = -(a * mid.x + b * mid.y);
  // y = (dy/dx) x + c => ax + by + k = dx y - dy x + dx c = 0
  cout << a << "x + " << b << "y + " << c << " = 0" << endl;
  
  cntr1.x /= maxY;
  cntr1.y /= maxX;
  cntr0.x /= maxY;
  cntr0.y /= maxX;

  #define SGN(x) ((x) < 0 ? -1 : ((x) > 0 ? 1 : 0))
  int s1 = SGN(a*cntr1.x + b*cntr1.y + c), s0 = SGN(a*cntr0.x + b*cntr0.y + c);
  int bad1 = 0, bad0 = 0;
  for (vector<pt>::iterator p = points.begin(); p != points.end(); p++)
  {
    p->x /= maxY;
    p->y /= maxX;
    if (p->isOne && SGN(a*p->x+b*p->y+c) != s1) bad1++;
    else if (!p->isOne && SGN(a*p->x+b*p->y+c) != s0) bad0++;
  }
  cout << "Bad Ones = " << bad1 << " Bad Others = " << bad0 << endl;
  while (!cin.eof())
  {
    double x, y;
    cin >> x >> y;
    if (SGN(a*x+b*y+c) == s1) cout << "(" << x << "," << y << ") is Class One" << endl;
    else cout << "(" << x << "," << y << ") is Class Two" << endl;
  }
  exit(0);
}
