#include "sim.h"

int main()
{
	ifstream file("defect_detection.csv");
	Simulation sim(file);
	StrategyFixMajorDefectsFirst s(NUM_SWE,1,2);
	RatioFoundFixedMetric rffm;
	RatioMajFoundFixedMetric rmffm;
	sim.setStrategy(&s);
	sim.addMetric(&rffm);
	sim.addMetric(&rmffm);
	sim.simulate();
	for (int i = 0; i < sim.metrics.size(); i++)
		writeMetricToFile(*sim.metrics[i], s.getName());
	exit(0);
}
