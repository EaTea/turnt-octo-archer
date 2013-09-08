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

//RSFHTERS

//FIX MAJOR DEFECTS FIRST
ExRShftStratFixMajorDefectsFirst::ExRShftStratFixMajorDefectsFirst(int a, int b, int c) : Strategy(a,b,c), keepGoing(true)
{}
void ExRShftStratFixMajorDefectsFirst::update(Simulation& s)
{
	edrm.update(s);
	qim.update(s);
	if (qim.values.back() > 100)
	{
		nSWETesting = 0;
		nSWEFixing = nSWE;
	}
	else
	{
		nSWETesting = 1;	
		nSWEFixing = nSWE-nSWETesting;
	}
	keepGoing = s.weekNumber < AT_LEAST_WEEKS || edrm.values.back() >= 1;
	if (keepGoing && edrm.values.size() > 1 && edrm.values.back() < 10) // stability?
		keepGoing &= fabs(edrm.values.back() - edrm.values[edrm.values.size()-2]) > 0.5;
}
// note that we will always want to fix defects that are found earlier fast
bool ExRShftStratFixMajorDefectsFirst::comparison(const Defect& a, const Defect& b)
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
void ExRShftStratFixMajorDefectsFirst::update_defects(std::vector<Defect>& defects) const
{
	std::sort(defects.begin(), defects.end(), ExRShftStratFixMajorDefectsFirst::comparison);
}
bool ExRShftStratFixMajorDefectsFirst::simContinue() const { return keepGoing; }
string ExRShftStratFixMajorDefectsFirst::getName() const { return "exrsfixMajorFirst"; }
//END FIX MAJOR DEFECTS FIRST

//FIX EASY DEFECTS FIRST
ExRShftStratFixEasyDefectsFirst::ExRShftStratFixEasyDefectsFirst(int a, int b, int c) : Strategy(a,b,c), keepGoing(true)
{}
void ExRShftStratFixEasyDefectsFirst::update(Simulation& s)
{
	edrm.update(s);
	qim.update(s);
	if (qim.values.back() > 100)
	{
		nSWETesting = 0;
		nSWEFixing = nSWE;
	}
	else
	{
		nSWETesting = 1;	
		nSWEFixing = nSWE-nSWETesting;
	}
	keepGoing = s.weekNumber < AT_LEAST_WEEKS || edrm.values.back() >= 1;
	if (keepGoing && edrm.values.size() > 1 && edrm.values.back() < 10) // stability?
		keepGoing = fabs(edrm.values.back() - edrm.values[edrm.values.size()-2]) > 0.5;
}
// note that we will always want to fix defects that are found earlier fast
bool ExRShftStratFixEasyDefectsFirst::comparison(const Defect& a, const Defect& b)
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
void ExRShftStratFixEasyDefectsFirst::update_defects(std::vector<Defect>& defects) const
{
	std::sort(defects.begin(), defects.end(), ExRShftStratFixEasyDefectsFirst::comparison);
}
bool ExRShftStratFixEasyDefectsFirst::simContinue() const { return keepGoing; }
string ExRShftStratFixEasyDefectsFirst::getName() const { return "exrsfixEasyFirst"; }
//END FIX EASY DEFECTS FIRST

//FIFO
ExRShftStratFIFO::ExRShftStratFIFO(int a, int b, int c) : Strategy(a,b,c), keepGoing(true)
{}
void ExRShftStratFIFO::update(Simulation& s)
{
	edrm.update(s);
	qim.update(s);
	if (qim.values.back() > 100)
	{
		nSWETesting = 0;
		nSWEFixing = nSWE;
	}
	else
	{
		nSWETesting = 1;	
		nSWEFixing = nSWE-nSWETesting;
	}
	keepGoing = s.weekNumber < AT_LEAST_WEEKS || edrm.values.back() >= 1;
	if (keepGoing && edrm.values.size() > 1 && edrm.values.back() < 10) // stability?
		keepGoing &= fabs(edrm.values.back() - edrm.values[edrm.values.size()-2]) > 0.5;
}
// note that we will always want to fix defects that are found earlier fast
bool ExRShftStratFIFO::comparison(const Defect& a, const Defect& b) { return a.weekFound > b.weekFound; }
void ExRShftStratFIFO::update_defects(std::vector<Defect>& defects) const
{
	std::sort(defects.begin(), defects.end(), ExRShftStratFIFO::comparison);
}
bool ExRShftStratFIFO::simContinue() const { return keepGoing; }
string ExRShftStratFIFO::getName() const { return "exrsfixFIFO"; }
//END FIFO

//FIX MAJOR DEFECTS FIRST
InRShftStratFixMajorDefectsFirst::InRShftStratFixMajorDefectsFirst(int a, int b, int c) : Strategy(a,b,c), keepGoing(true)
{}
void InRShftStratFixMajorDefectsFirst::update(Simulation& s)
{
	edrm.update(s);
	qim.update(s);
	if (qim.values.back() > 50)
	{
		nSWETesting = 0;
		nSWEFixing = nSWE;
	}
	else
	{
		nSWETesting = 1;	
		nSWEFixing = nSWE-nSWETesting;
	}
	keepGoing = s.weekNumber < AT_LEAST_WEEKS || edrm.values.back() >= 1;
	if (keepGoing && edrm.values.size() > 1 && edrm.values.back() < 10) // stability?
		keepGoing &= fabs(edrm.values.back() - edrm.values[edrm.values.size()-2]) > 0.5;
}
// note that we will always want to fix defects that are found earlier fast
bool InRShftStratFixMajorDefectsFirst::comparison(const Defect& a, const Defect& b)
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
void InRShftStratFixMajorDefectsFirst::update_defects(std::vector<Defect>& defects) const
{
	std::sort(defects.begin(), defects.end(), InRShftStratFixMajorDefectsFirst::comparison);
}
bool InRShftStratFixMajorDefectsFirst::simContinue() const { return keepGoing; }
string InRShftStratFixMajorDefectsFirst::getName() const { return "inrsfixMajorFirst"; }
//END FIX MAJOR DEFECTS FIRST

//FIX EASY DEFECTS FIRST
InRShftStratFixEasyDefectsFirst::InRShftStratFixEasyDefectsFirst(int a, int b, int c) : Strategy(a,b,c), keepGoing(true)
{}
void InRShftStratFixEasyDefectsFirst::update(Simulation& s)
{
	edrm.update(s);
	qim.update(s);
	if (qim.values.back() > 50)
	{
		nSWETesting = 0;
		nSWEFixing = nSWE;
	}
	else
	{
		nSWETesting = 1;	
		nSWEFixing = nSWE-nSWETesting;
	}
	keepGoing = s.weekNumber < AT_LEAST_WEEKS || edrm.values.back() >= 1;
	if (keepGoing && edrm.values.size() > 1 && edrm.values.back() < 10) // stability?
		keepGoing = fabs(edrm.values.back() - edrm.values[edrm.values.size()-2]) > 0.5;
}
// note that we will always want to fix defects that are found earlier fast
bool InRShftStratFixEasyDefectsFirst::comparison(const Defect& a, const Defect& b)
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
void InRShftStratFixEasyDefectsFirst::update_defects(std::vector<Defect>& defects) const
{
	std::sort(defects.begin(), defects.end(), InRShftStratFixEasyDefectsFirst::comparison);
}
bool InRShftStratFixEasyDefectsFirst::simContinue() const { return keepGoing; }
string InRShftStratFixEasyDefectsFirst::getName() const { return "inrsfixEasyFirst"; }
//END FIX EASY DEFECTS FIRST

//FIFO
InRShftStratFIFO::InRShftStratFIFO(int a, int b, int c) : Strategy(a,b,c), keepGoing(true)
{}
void InRShftStratFIFO::update(Simulation& s)
{
	edrm.update(s);
	qim.update(s);
	if (qim.values.back() > 50)
	{
		nSWETesting = 0;
		nSWEFixing = nSWE;
	}
	else
	{
		nSWETesting = 1;	
		nSWEFixing = nSWE-nSWETesting;
	}
	keepGoing = s.weekNumber < AT_LEAST_WEEKS || edrm.values.back() >= 1;
	if (keepGoing && edrm.values.size() > 1 && edrm.values.back() < 10) // stability?
		keepGoing &= fabs(edrm.values.back() - edrm.values[edrm.values.size()-2]) > 0.5;
}
// note that we will always want to fix defects that are found earlier fast
bool InRShftStratFIFO::comparison(const Defect& a, const Defect& b) { return a.weekFound > b.weekFound; }
void InRShftStratFIFO::update_defects(std::vector<Defect>& defects) const
{
	std::sort(defects.begin(), defects.end(), InRShftStratFIFO::comparison);
}
bool InRShftStratFIFO::simContinue() const { return keepGoing; }
string InRShftStratFIFO::getName() const { return "inrsfixFIFO"; }
//END FIFO
