#include "sim.h"

Strategy::Strategy(int nswe, int nsweTest, int nsweFix) : nSWE(nswe), nSWETesting(nsweTest), nSWEFixing(nsweFix) { }

Strategy::Strategy(const Strategy& s) : nSWE(s.nSWE), nSWETesting(s.nSWETesting), nSWEFixing(s.nSWEFixing) { }

StrategyFixMajorDefectsFirst::StrategyFixMajorDefectsFirst(int a, int b, int c) : Strategy(a,b,c), keepGoing(true)
{}

void StrategyFixMajorDefectsFirst::update(const Simulation& s)
{
	keepGoing = s.defects.empty() || s.weekNumber <= 10;
	return;
}
// note that we will always want to fix defects that are found earlier fast
bool StrategyFixMajorDefectsFirst::comparison(const Defect& a, const Defect& b)
{
	if (a.isMajor() && !b.isMajor())
		return false;
	else if (!a.isMajor() && b.isMajor())
		return true;
	else return a.weekFound > b.weekFound;
}
void StrategyFixMajorDefectsFirst::update_defects(std::vector<Defect>& defects) const
{
	std::sort(defects.begin(), defects.end(), StrategyFixMajorDefectsFirst::comparison);
}
bool StrategyFixMajorDefectsFirst::simContinue() const {
	return keepGoing;
}
