#include "sim.h"

class DefectCountingMetric : protected Metric
{
  public:
    DefectCountingMetric() {}
    std::string getName() const { return "defect_counting"; }
    std::vector<double> getValues() const { return values; }
    void update(std::vector<Defect>& defects)
    {
      int latestWeek = -1, nLatestWeek = 0;
      for (std::vector<Defect>::iterator dfct = defects.begin(); dfct != defects.end(); dfct++)
      {
        if (dfct->weekFound > latestWeek)
        {
          latestWeek = dfct->weekFound;
          nLatestWeek = 1;
        }
        else nLatestWeek += (latestWeek == dfct->weekFound ? 1 : 0);
      }
      values.push_back(nLatestWeek);
    }
};
