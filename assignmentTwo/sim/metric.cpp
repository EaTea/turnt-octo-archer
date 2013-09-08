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
		if (dfct->weekFound == curWeek)
			count++;
	defects = &(s.fixed_defects);
	for (std::vector<Defect>::iterator dfct = defects->begin(); dfct != defects->end(); dfct++)
		if (dfct->weekFound == curWeek)
			count++;
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
		if (dfct->weekFound == curWeek && dfct->isMajor())
			count++;
	defects = &(s.fixed_defects);
	for (std::vector<Defect>::iterator dfct = defects->begin(); dfct != defects->end(); dfct++)
		if (dfct->weekFound == curWeek && dfct->isMajor())
			count++;
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
		if (dfct->weekFixed == curWeek)
			count++;
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
		if (dfct->weekFixed == curWeek && dfct->isMajor())
			count++;
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
	if (dfm.values[values.size()-1] == 0)
		values.push_back(0);
	else
		values.push_back(dcm.values[values.size()-1]/dfm.values[values.size()-1]);
	curWeek++;
}

RatioMajFoundFixedMetric::RatioMajFoundFixedMetric() : curWeek(0) {}
std::string RatioMajFoundFixedMetric::getName() const { return "major_ratio_found_fixed"; }
std::vector<double> RatioMajFoundFixedMetric::getValues() const { return values; }
void RatioMajFoundFixedMetric::update(Simulation& s)
{
	dmfm.update(s);
	dmcm.update(s);
	if (dmfm.values[values.size()-1] == 0)
		values.push_back(0);
	else
		values.push_back(dmcm.values[values.size()-1]/dmfm.values[values.size()-1]);
	curWeek++;
}

AverageTimeInQueueMetric::AverageTimeInQueueMetric() : curWeek(0) {}
std::string AverageTimeInQueueMetric::getName() const { return "avg_time_in_queue"; }
std::vector<double> AverageTimeInQueueMetric::getValues() const { return values; }
void AverageTimeInQueueMetric::update(Simulation& s)
{
	std::vector<Defect>* defects = &(s.defects);
	int count = 0;
	double timeInQueue = 0;
	for (std::vector<Defect>::iterator dfct = defects->begin(); dfct != defects->end(); dfct++)
	{
		timeInQueue += (curWeek - dfct->weekFound+1);
		count++;
	}
	values.push_back(count ? timeInQueue / count : 0);
	curWeek++;
}

AverageMajTimeInQueueMetric::AverageMajTimeInQueueMetric() : curWeek(0) {}
std::string AverageMajTimeInQueueMetric::getName() const { return "avg_maj_time_queue"; }
std::vector<double> AverageMajTimeInQueueMetric::getValues() const { return values; }
void AverageMajTimeInQueueMetric::update(Simulation& s)
{
	std::vector<Defect>* defects = &(s.defects);
	int count = 0;
	double timeInQueue = 0;
	for (std::vector<Defect>::iterator dfct = defects->begin(); dfct != defects->end(); dfct++)
		if (dfct->isMajor())
		{
			timeInQueue += (curWeek - dfct->weekFound+1);
			count++;
		}
	values.push_back(count ? timeInQueue / count : 0);
	curWeek++;
}

QueueImpactMetric::QueueImpactMetric() : curWeek(0) {}
std::string QueueImpactMetric::getName() const { return "queue_impact"; }
std::vector<double> QueueImpactMetric::getValues() const { return values; }
void QueueImpactMetric::update(Simulation& s)
{
	std::vector<Defect>* defects = &(s.defects);
	double totalImpact = 0;
	for (std::vector<Defect>::iterator dfct = defects->begin(); dfct != defects->end(); dfct++)
		totalImpact += dfct->isMajor() ? MAJOR_DEFECT_IMPACT : MINOR_DEFECT_IMPACT;
	values.push_back(totalImpact);
	curWeek++;
}

QueueSizeMetric::QueueSizeMetric() : curWeek(0) {}
std::string QueueSizeMetric::getName() const { return "queue_size"; }
std::vector<double> QueueSizeMetric::getValues() const { return values; }
void QueueSizeMetric::update(Simulation& s)
{
	values.push_back(s.defects.size());
	curWeek++;
}

EstDefectsRemMetric::EstDefectsRemMetric() : curWeek(0) {}
std::string EstDefectsRemMetric::getName() const { return "est_def_remaining"; }
std::vector<double> EstDefectsRemMetric::getValues() const { return values; }
void EstDefectsRemMetric::update(Simulation& s)
{
	dcm.update(s);
	if (!dcm.values.empty() && dcm.values.size() < 2) //use negative exponential
		values.push_back(dcm.values[dcm.values.size()-1] / (estimation_rate));
	else
	{
		vector<double> lg_vals;
		for (vector<double>::iterator val = dcm.values.begin(); val != dcm.values.end(); val++)
			lg_vals.push_back(*val ? log(*val) : 0);

		double meanx, meany, sumsqx, sumx, sumy, sumxy;
		meanx = meany = sumsqx = sumx = sumy = sumxy = 0;
		for (vector<double>::iterator val = lg_vals.begin(); val != lg_vals.end(); val++)
		{
			meanx += val - lg_vals.begin();
			meany += *val;
			sumsqx += (val - lg_vals.begin()) * (val - lg_vals.begin());
			sumxy += *val * (val - lg_vals.begin());
		}
		sumx = meanx;
		meanx /= lg_vals.size();
		sumy = meany;
		meany /= lg_vals.size();
		double m, b;
		m = (sumxy - (sumx) * (sumy) / lg_vals.size()) / (sumsqx - sumx * sumx / lg_vals.size());
		b = meany - m * meanx;
		// at the end 
		b = exp(b);
		values.push_back(-b/m*exp(m*curWeek));
	}
	curWeek++;
}

EstMajDefRemMetric::EstMajDefRemMetric() : curWeek(0) {}
std::string EstMajDefRemMetric::getName() const { return "est_maj_def_remaining"; }
std::vector<double> EstMajDefRemMetric::getValues() const { return values; }
void EstMajDefRemMetric::update(Simulation& s)
{
	dmcm.update(s);
	if (!dmcm.values.empty() && dmcm.values.size() < 2) //use negative exponential
		values.push_back(dmcm.values[dmcm.values.size()-1] / (estimation_rate));
	else
	{
		vector<double> lg_vals;
		for (vector<double>::iterator val = dmcm.values.begin(); val != dmcm.values.end(); val++)
			lg_vals.push_back(*val ? log(*val) : 0);

		double meanx, meany, sumsqx, sumx, sumy, sumxy;
		meanx = meany = sumsqx = sumx = sumy = sumxy = 0;
		for (vector<double>::iterator val = lg_vals.begin(); val != lg_vals.end(); val++)
		{
			meanx += val - lg_vals.begin();
			meany += *val;
			sumsqx += (val - lg_vals.begin()) * (val - lg_vals.begin());
			sumxy += *val * (val - lg_vals.begin());
		}
		sumx = meanx;
		meanx /= lg_vals.size();
		sumy = meany;
		meany /= lg_vals.size();
		double m, b;
		m = (sumxy - (sumx) * (sumy) / lg_vals.size()) / (sumsqx - sumx * sumx / lg_vals.size());
		b = meany - m * meanx;
		// at the end 
		b = exp(b);
		values.push_back(-b/m*exp(m*curWeek));
	}
	curWeek++;
}
