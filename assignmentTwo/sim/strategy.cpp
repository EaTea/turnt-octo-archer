Strategy::Strategy(int nswe, int nsweTest, int nsweFix) : nSWE(nswe), nSWETesting(nsweTest), nSWEFixing(nsweFix) { }

class RandomStrategy
{
  bool comparison(const Defect& a, const Defect& b)
  {
    return (bool)(rand() % 2);
  }
  void update(const Simulation& sim)
  {
    
  }
};
