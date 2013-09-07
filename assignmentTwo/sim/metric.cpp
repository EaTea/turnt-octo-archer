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
	values.push_back(count);
	curWeek++;
}
