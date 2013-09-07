#include "sim.h"

Simulation::Simulation(const std::string& filename, const Strategy& legit)
	: defect_strategy(legit), weekNumber(-1)
{
	std::ifstream file(filename.c_str());

	csv_file = CSVIterator(file);
}

void Simulation::simulate()
{
	for (weekNumber = 0; csv_file != CSVIterator(); ++csv_file, ++weekNumber)
	{
		for (int detectors = 0; detectors < defect_strategy.nSWETesting; detectors++)
		{
			for (int dt = MIN_HARD; dt <= MAJ_EASY; dt++)
			{
				defect_type dtt = (defect_type) dt;
				std::string s = (*csv_file)[dt+1];
				std::istringstream iss(s);
				int dfct_amount;
				iss >> dfct_amount;
				for (int i = 0; i < dfct_amount; i++)
					defects.push_back(Defect(dtt,weekNumber,0));
			}
		}
		for (int fixers = 0; fixers < defect_strategy.nSWEFixing; fixers++)
		{
			// put the defects back in order
			defect_strategy.update_defects(defects);
			double sweHours = SWE_HOURS;
			// while there are enough hours to fix either a hard or easy defect
			while(sweHours >= HARD_DEFECT_FIX_TIME * (1.0+FIX_TIME_ERROR) ||
					sweHours >= EASY_DEFECT_FIX_TIME * (1.0+FIX_TIME_ERROR))
			{
				double f = (double)rand() / RAND_MAX;
				f *= FIX_TIME_ERROR;
				f *= (rand() % 2 ? 1 : -1);
				f += 1.0;
				f = std::max(f,0.25); //clamp to min of 0.25
				for (int ind_dfct = defects.size()-1; ind_dfct >= 0; ind_dfct--)
				{
					if (sweHours >= f * defects[ind_dfct].getFixTime())
					{
						fixed_defects.push_back(Defect(defects[ind_dfct].classification,
																			defects[ind_dfct].weekFound,
																			1.0));
						defects.erase(defects.begin()+ind_dfct);
					}
				}
			}
			if (ALLOW_PART_FIXES)
			{
				double f = (double)rand() / RAND_MAX;
				f *= FIX_TIME_ERROR;
				f *= (rand() % 2 ? 1 : -1);
				f += 1.0;
				f = std::max(f,0.25); //clamp to min of 0.25
				for (int ind_dfct = defects.size()-1; ind_dfct >= 0; ind_dfct--)
				{
					if (sweHours >= f * defects[ind_dfct].getFixTime())
					{
						defects.push_back(Defect(defects[ind_dfct].classification,
																			defects[ind_dfct].weekFound,
																			sweHours / f * defects[ind_dfct].getFixTime()));
						defects.erase(defects.begin()+ind_dfct);
					}
				}
			}
		}
		defect_strategy.update(*this);
		for (std::vector<Metric>::iterator metric = metrics.begin(); metric != metrics.end(); metric++)
			metric->update(*this);
	}
}
