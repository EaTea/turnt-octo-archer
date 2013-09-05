#include "sim.h"

Simulation::Simulation(const string& filename, Strategy legit)
	: defect_strategy(legit), weekNumber(-1)
{
	std::ifstream file(filename.c_str());

	csv_file(file);
}

void	Simulation::simulate()
{
	for (weekNumber = 0; csv_file != CSVIterator(); ++csv_file, ++weekNumber)
	{
		for (int detectors = 0; detectors < defect_strategy.nSWETesting; detectors++)
		{
			for (defect_type dt = MIN_HARD; dt > MAJ_EASY; dt++)
			{
				int dfct_amount = (*csv_file)[dt+1];
				for (int i = 0; i < dfct_amount; i++)
					defects.push_back(defect(dt,weekNumber));
			}
		}
		std::sort(defects.begin(), defects.end(), defect_strategy.comparison);
		// might need to reverse defects
		for (int fixers = 0; fixers < defect_strategy.nSWEFixing; fixers++)
		{
			double sweHours = SWE_HOURS;
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
						fixed_defects.push_back(defects[ind_dfct]);
						defects.erase(defects.begin()+ind_dfct);
					}
				}
			}
		}
		if (part_fixes)
		{
			for (int ind_dfct = defects.size()-1; ind_dfct >= 0; ind_dfct--)
			{
				if (sweHours >= f * defects[ind_dfct].getFixTime())
				{
					fixed_defects.push_back(defects[ind_dfct]);
					defects.erase(defects.begin()+ind_dfct);
				}
			}
		}
		defect_strategy.update(*this);
		for (std::vector<Metric>::iterator metric = metrics.begin(); it != metrics.end(); metric++)
			metric->update(*this);
	}
	csv_file.close();
}
