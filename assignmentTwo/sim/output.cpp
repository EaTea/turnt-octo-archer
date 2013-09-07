#include "sim.h"

void writeMetricToFile(const Metric& m)
{
  std::string name = m.getName();
  std::vector<double> values = m.values;
  std::ifstream fileReader(name.c_str(), std::fstream::in);
  std::vector<std::string> lines;
  std::string tmp;
  while(!fileReader.eof())
  {
    getline(fileReader>>std::ws,tmp);
    lines.push_back(tmp);
  }
  fileReader.close();
  if (!lines.empty())
  {
    int i = 0;
    for (std::vector<std::string>::iterator ln = lines.begin(); ln != lines.end() && i < values.size(); ln++, i++)
    {
      std::ostringstream oss;
      oss << '\t' << values[i];
      ln->append(oss.str());
    }
  }
  else
  {
    for (int i = 0; i < values.size(); i++)
    {
      std::ostringstream oss;
      oss << values[i];
      lines.push_back(oss.str());
    }
  }
  std::ofstream fileWriter(name.c_str(), std::fstream::out | std::fstream::trunc);
  for (std::vector<std::string>::iterator ln = lines.begin(); ln != lines.end(); ln++)
  {
    fileWriter << *ln << std::endl;
  }
  fileWriter.close();
}
