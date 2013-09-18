#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <cfloat>
using namespace std;

struct pt
{
  double x, y;
  bool isOne;
  pt(double xx = 0, double yy = 0, bool isO = false) : x(xx), y(yy), isOne(isO) {}
};

int main(int argc, char* argv[])
{
  if (argc < 2)
  {
    cerr << "Need filename to use" << endl;
    exit(1);
  }
  ifstream iff(argv[1],ifstream::in);
  vector<pt> points;
  double mx = -1 * DBL_MAX, my = -1 * DBL_MAX;
  int n1 = 0, n2 = 0;
  while (!iff.eof())
  {
    double x, y;
    int c;
    iff >> x >> y >> c >> ws;
    points.push_back(pt(x,y,c == 1));
    n1 += c == 1 ? 1 : 0;
    n2 += c != 1 ? 1 : 0;
    mx = max(x,mx);
    my = max(y,my);
  }
  pt cntrd1, cntrd2;
  for (vector<pt>::iterator pp = points.begin(); pp != points.end(); pp++)
  {
    if (pp->isOne)
    {
      cntrd1.x += (pp->x *= my);
      cntrd1.y += (pp->y *= mx);
    }
    else
    {
      cntrd2.x += (pp->x *= my);
      cntrd2.y += (pp->y *= mx);
    }
  }
  cntrd1.x /= n1;
  cntrd1.y /= n1;
  cntrd2.x /= n2;
  cntrd2.y /= n2;
  pt midpt((cntrd1.x + cntrd2.x)/2,(cntrd1.y+cntrd2.y)/2);
  midpt.x /= my, midpt.y /= mx;
  pt c1Toc2(-cntrd2.y+cntrd1.y,cntrd2.x-cntrd1.x);
  double mag = sqrt(c1Toc2.x*c1Toc2.x + c1Toc2.y*c1Toc2.y);
  c1Toc2.x /= mag;
  c1Toc2.y /= mag;
  // ax + by + c = 0
  double a, b, c;
  b = c1Toc2.x;
  a = -c1Toc2.y;
  c = -(a*midpt.x + b*midpt.y);
  cout << a << "x + " << b << "y + " << c << " = 0" << endl;
  int miss1 = 0, miss2 = 0;
#define SGN(x) ((x) < 0 ? -1 : (x) > 0 ? 1 : 0)
  cntrd1.x /= my, cntrd1.y /= mx;
  cntrd2.x /= my, cntrd2.y /= mx;
  for (vector<pt>::iterator pp = points.begin(); pp != points.end(); pp++)
  {
    pp->x /= my;
    pp->y /= mx;
    if (SGN(a*pp->x + b*pp->y + c) == SGN(a*cntrd1.x + b*cntrd1.y + c))
    {
      if (!pp->isOne) miss2++;
    }
    else if(SGN(a*pp->x + b*pp->y + c) == SGN(a*cntrd2.x + b*cntrd2.y + c))
    {
      if (pp->isOne) miss1++;
    }
  }
  cout << "Class One misclassified " << miss1 << " Class Two misclassified " << miss2 << endl;
  cout << "Now classifying" << endl;
  while (!cin.eof())
  {
    pt pp;
    cin >> pp.x >> pp.y;
    if (SGN(a*pp.x + b*pp.y + c) == SGN(a*cntrd1.x + b*cntrd1.y + c))
      cout << "Class One" << endl;
    else if (SGN(a*pp.x + b*pp.y + c) == SGN(a*cntrd2.x + b*cntrd2.y + c))
      cout << "Class Two" << endl;
  }
  exit(0);
}
