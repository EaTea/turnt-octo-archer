#include "sim.h"

Strategy::Strategy(int nswe, int nsweTest, int nsweFix) : nSWE(nswe), nSWETesting(nsweTest), nSWEFixing(nsweFix) { }

Strategy::Strategy(const Strategy& s) : nSWE(s.nSWE), nSWETesting(s.nSWETesting), nSWEFixing(s.nSWEFixing) { }

//FIX MAJOR DEFECTS FIRST
StrategyFixMajorDefectsFirst::StrategyFixMajorDefectsFirst(int a, int b, int c) : Strategy(a,b,c), keepGoing(true)
{}
void StrategyFixMajorDefectsFirst::update(Simulation& s)
{
	edrm.update(s);
	keepGoing = s.weekNumber < AT_LEAST_WEEKS || edrm.values.back() >= 1;
	if (keepGoing && edrm.values.size() > 1) // stability?
		keepGoing &= fabs(edrm.values.back() - edrm.values[edrm.values.size()-2]) > 0.5;
}
// note that we will always want to fix defects that are found earlier fast
bool StrategyFixMajorDefectsFirst::comparison(const Defect& a, const Defect& b)
{
	if (a.isMajor() && !b.isMajor())
		return false;
	else if (!a.isMajor() && b.isMajor())
		return true;
	else if (a.isEasy() && !b.isEasy())
		return false;
	else if (!a.isEasy() && b.isEasy())
		return true;
	return a.weekFound > b.weekFound;
}
void StrategyFixMajorDefectsFirst::update_defects(std::vector<Defect>& defects) const
{
	std::sort(defects.begin(), defects.end(), StrategyFixMajorDefectsFirst::comparison);
}
bool StrategyFixMajorDefectsFirst::simContinue() const { return keepGoing; }
string StrategyFixMajorDefectsFirst::getName() const { return "fixMajorFirst"; }
//END FIX MAJOR DEFECTS FIRST

//FIX EASY DEFECTS FIRST
StrategyFixEasyDefectsFirst::StrategyFixEasyDefectsFirst(int a, int b, int c) : Strategy(a,b,c), keepGoing(true)
{}
void StrategyFixEasyDefectsFirst::update(Simulation& s)
{
	edrm.update(s);
	keepGoing = s.weekNumber < AT_LEAST_WEEKS || edrm.values.back() >= 1;
	if (keepGoing && edrm.values.size() > 1) // stability?
		keepGoing = fabs(edrm.values.back() - edrm.values[edrm.values.size()-2]) > 0.5;
}
// note that we will always want to fix defects that are found earlier fast
bool StrategyFixEasyDefectsFirst::comparison(const Defect& a, const Defect& b)
{
	if (a.isEasy() && !b.isEasy())
		return false;
	else if (!a.isEasy() && b.isEasy())
		return true;
	else if (a.isMajor() && !b.isMajor())
		return false;
	else if (!a.isMajor() && b.isMajor())
		return true;
	return a.weekFound > b.weekFound;
}
void StrategyFixEasyDefectsFirst::update_defects(std::vector<Defect>& defects) const
{
	std::sort(defects.begin(), defects.end(), StrategyFixEasyDefectsFirst::comparison);
}
bool StrategyFixEasyDefectsFirst::simContinue() const { return keepGoing; }
string StrategyFixEasyDefectsFirst::getName() const { return "fixEasyFirst"; }
//END FIX EASY DEFECTS FIRST

//FIFO
StrategyFIFO::StrategyFIFO(int a, int b, int c) : Strategy(a,b,c), keepGoing(true)
{}
void StrategyFIFO::update(Simulation& s)
{
	edrm.update(s);
	keepGoing = s.weekNumber < AT_LEAST_WEEKS || edrm.values.back() >= 1;
	if (keepGoing && edrm.values.size() > 1) // stability?
		keepGoing &= fabs(edrm.values.back() - edrm.values[edrm.values.size()-2]) > 0.5;
}
// note that we will always want to fix defects that are found earlier fast
bool StrategyFIFO::comparison(const Defect& a, const Defect& b) { return a.weekFound > b.weekFound; }
void StrategyFIFO::update_defects(std::vector<Defect>& defects) const
{
	std::sort(defects.begin(), defects.end(), StrategyFIFO::comparison);
}
bool StrategyFIFO::simContinue() const { return keepGoing; }
string StrategyFIFO::getName() const { return "fixFIFO"; }
//END FIFO
