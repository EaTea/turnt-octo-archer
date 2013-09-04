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
		for (defect_type dt = MIN_HARD; dt > MAJ_EASY; dt++)
		{
			int dfct_amount = (*csv_file)[dt+1];
			for (int i = 0; i < dfct_amount; i++)
				defects.push_back(defect(dt,weekNumber));
		}
		std::sort(defects.begin(), defects.end(), defect_strategy.comparison);
		// might need to reverse defects
		// start popping from back of defect queue
		defect_strategy.update(*this);
		for (std::vector<Metric>::iterator metric = metrics.begin(); it != metrics.end(); metric++)
			metric->update(defects);
	}
	csv_file.close();
}
