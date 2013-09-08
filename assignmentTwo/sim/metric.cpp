#include "sim.h"
Metric::Metric() {}

DefectCountingMetric::DefectCountingMetric() : curWeek(0) {}
std::string DefectCountingMetric::getName() const { return "defect_counting"; }
std::vector<double> DefectCountingMetric::getValues() const { return values; }
void DefectCountingMetric::update(Simulation& s)
{
	std::vector<Defect>* defects = &(s.defects);
	int count = 0;
	for (std::vector<Defect>::iterator dfct = defects->begin(); dfct != defects->end(); dfct++)
	{
		if (dfct->weekFound == curWeek)
			count++;
	}
	defects = &(s.fixed_defects);
	for (std::vector<Defect>::iterator dfct = defects->begin(); dfct != defects->end(); dfct++)
	{
		if (dfct->weekFound == curWeek)
			count++;
	}
	values.push_back(count);
	curWeek++;
}

DefMajCountMetric::DefMajCountMetric() : curWeek(0) {}
std::string DefMajCountMetric::getName() const { return "defect_major_detect"; }
std::vector<double> DefMajCountMetric::getValues() const { return values; }
void DefMajCountMetric::update(Simulation& s)
{
	std::vector<Defect>* defects = &(s.defects);
	int count = 0;
	for (std::vector<Defect>::iterator dfct = defects->begin(); dfct != defects->end(); dfct++)
	{
		if (dfct->weekFound == curWeek && dfct->isMajor())
			count++;
	}
	defects = &(s.fixed_defects);
	for (std::vector<Defect>::iterator dfct = defects->begin(); dfct != defects->end(); dfct++)
	{
		if (dfct->weekFound == curWeek && dfct->isMajor())
			count++;
	}
	values.push_back(count);
	curWeek++;
}

DefectFixedMetric::DefectFixedMetric() : curWeek(0) {}
std::string DefectFixedMetric::getName() const { return "defect_fixed"; }
std::vector<double> DefectFixedMetric::getValues() const { return values; }
void DefectFixedMetric::update(Simulation& s)
{
	std::vector<Defect>* defects = &(s.fixed_defects);
	int count = 0;
	for (std::vector<Defect>::iterator dfct = defects->begin(); dfct != defects->end(); dfct++)
	{
		if (dfct->weekFixed == curWeek)
			count++;
	}
	values.push_back(count);
	curWeek++;
}

DefectMajorFixedMetric::DefectMajorFixedMetric() : curWeek(0) {}
std::string DefectMajorFixedMetric::getName() const { return "major_defect_fixed"; }
std::vector<double> DefectMajorFixedMetric::getValues() const { return values; }
void DefectMajorFixedMetric::update(Simulation& s)
{
	std::vector<Defect>* defects = &(s.fixed_defects);
	int count = 0;
	for (std::vector<Defect>::iterator dfct = defects->begin(); dfct != defects->end(); dfct++)
	{
		if (dfct->weekFixed == curWeek && dfct->isMajor())
			count++;
	}
	values.push_back(count);
	curWeek++;
}

RatioFoundFixedMetric::RatioFoundFixedMetric() : curWeek(0) {}
std::string RatioFoundFixedMetric::getName() const { return "ratio_found_fixed"; }
std::vector<double> RatioFoundFixedMetric::getValues() const { return values; }
void RatioFoundFixedMetric::update(Simulation& s)
{
	dfm.update(s);
	dcm.update(s);
	values.push_back(dfm.values[values.size()-1]/dcm.values[values.size()-1]);
	curWeek++;
}

RatioMajFoundFixedMetric::RatioMajFoundFixedMetric() : curWeek(0) {}
std::string RatioMajFoundFixedMetric::getName() const { return "major_ratio_found_fixed"; }
std::vector<double> RatioMajFoundFixedMetric::getValues() const { return values; }
void RatioMajFoundFixedMetric::update(Simulation& s)
{
	dmfm.update(s);
	dmcm.update(s);
	values.push_back(dmfm.values[values.size()-1]/dmcm.values[values.size()-1]);
	curWeek++;
}
