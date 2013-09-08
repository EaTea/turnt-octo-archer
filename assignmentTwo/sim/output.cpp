#include "sim.h"

void writeMetricToFile(const Metric& m, const string& after)
{
  std::string name = m.getName();
	name += "_" + after;
  std::vector<double> values = m.values;
  std::vector<std::string> lines;
	for (int i = 0; i < values.size(); i++)
	{
		std::ostringstream oss;
		oss << values[i];
		lines.push_back(oss.str());
	}
  std::ofstream fileWriter(name.c_str(), std::fstream::out | std::fstream::trunc);
  for (std::vector<std::string>::iterator ln = lines.begin(); ln != lines.end(); ln++)
    fileWriter << *ln << std::endl;
  fileWriter.close();
}
