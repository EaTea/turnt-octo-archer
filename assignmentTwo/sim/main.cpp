#include "sim.h"

int main()
{
	ifstream file("defect_detection.csv");
	Simulation sim(file);
	InRShftStratFixMajorDefectsFirst s(NUM_SWE,1,2);
	DefectCountingMetric dcm;
	DefectFixedMetric dfm;
	DefMajCountMetric dmcm;
	DefectMajorFixedMetric dmfm;
	RatioFoundFixedMetric rffm;
	RatioMajFoundFixedMetric rmffm;
	AverageTimeInQueueMetric atiqm;
	AverageMajTimeInQueueMetric amtiqm;
	QueueImpactMetric qim;
	QueueSizeMetric qsm;
	EstDefectsRemMetric edrm;
	EstMajDefRemMetric emdrm;
	sim.setStrategy(&s);
	sim.addMetric(&dcm);
	sim.addMetric(&dfm);
	sim.addMetric(&dmcm);
	sim.addMetric(&dmfm);
	sim.addMetric(&rffm);
	sim.addMetric(&rmffm);
	sim.addMetric(&atiqm);
	sim.addMetric(&amtiqm);
	sim.addMetric(&qim);
	sim.addMetric(&qsm);
	sim.addMetric(&edrm);
	sim.addMetric(&emdrm);
	sim.simulate();
	for (int i = 0; i < sim.metrics.size(); i++)
		writeMetricToFile(*sim.metrics[i], s.getName());
	exit(0);
}
