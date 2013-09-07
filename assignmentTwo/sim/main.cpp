#include "sim.h"

int main()
{
	ifstream file("defect_detection.csv");
	Simulation sim(file);
	StrategyFixMajorDefectsFirst s(NUM_SWE,1,2);
	DefectCountingMetric dcm;
	sim.setStrategy(&s);
	sim.addMetric(&dcm);
	sim.simulate();
	for (int i = 0; i < sim.metrics.size(); i++)
		writeMetricToFile(*sim.metrics[i]);
	exit(0);
}
