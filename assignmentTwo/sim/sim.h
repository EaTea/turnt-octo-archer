// LIBCONFIG
#include <libconfig.hh>

// STL
#include <iostream>
#include <fstream>
#include <iterator>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>

// BOOST
#include <boost/random/uniform_int_distribution.hpp>

// IO
// code taken from http://stackoverflow.com/questions/1120140/csv-parser-in-c 

class CSVRow
{
  public:
  std::string const& operator[](std::size_t index) const
  {
    return m_data[index];
  }
  std::size_t size() const
  {
    return m_data.size();
  }
  void readNextRow(std::istream&);
  private:
  std::vector<std::string>  m_data;
};

std::istream& operator>>(std::istream&,CSVRow&);

class CSVIterator
{   
  public:
    typedef std::input_iterator_tag   iterator_category;
    typedef CSVRow            value_type;
    typedef std::size_t         difference_type;
    typedef CSVRow*           pointer;
    typedef CSVRow&           reference;

    CSVIterator(std::istream& str)  :m_str(str.good() ? & str : NULL)
		{
			++(*this);
		}
    CSVIterator()           :m_str(NULL) {}

    // Pre Increment
    CSVIterator& operator++();
    // Post increment
    CSVIterator operator++(int);
    CSVRow const& operator*()   const     {return m_row;}
    CSVRow const* operator->()  const     {return &m_row;}

    bool operator==(CSVIterator const& rhs)
		{
			return ((this == &rhs)
				|| ((this->m_str == NULL)
				&& (rhs.m_str == NULL)));
		}
    bool operator!=(CSVIterator const& rhs)
		{
			return !((*this) == rhs);
		}
  private:
    std::istream*     m_str;
    CSVRow            m_row;
};

// CONFIGURATION

bool setup(const string&);

// DEFECTS

// constant integers representing a defect

enum defect_type {
 MIN_HARD = 0,
 MIN_EASY = 1,
 MAJ_HARD = 2,
 MAJ_EASY = 3
};

#define MAJOR_BITMASK 0x10
#define EASY_BITMASK 0x01

class Defect
{
		const defect_type classification;
		const int weekFound;
    double defectCompletion;
	public:
		Defect(defect_type&, int&, double&);
		defect_type getClassification() const { return classification; }
		bool isMajor() const
    {
      return classification & MAJOR_BITMASK ? true : false;
    }
		bool isEasy() const
    {
      return classification & EASY_BITMASK ? true : false;
    }
};

// STRATEGIES

class Strategy
{
  private:
    int nSWE;
    int nSWETesting;
    int nSWEFixing;
	public:
    Strategy(int, int, int);
    virtual void update(const Simulation&);
    virtual bool comparison(const Defect&, const Defect&);
};

// METRICS

class Metric
{
		vector<double> values;
	public:
		Metric(string&);
    virtual void update(const vector<Defect>&);
};

// SIMULATION

class Simulation
{
		vector<Metric> metrics;
		Strategy defect_strategy;
		const double FIX_TIME_HARD, FIX_TIME_EASY, IMPACT_MINOR, IMPACT_MAJOR;
		const double FIX_TIME_ERROR, IMPACT_ERROR;
		const bool PART_FIXES;
		CSVIterator csv_file;
		int weekNumber;
		configure(const string&);
		vector<Defect> defects;
    void updateMetrics();
    void updateStrategy();
    void nextRound();
	public:
		Simulation(const string&, Strategy&);
    ~Simulation();
		void addMetric(Metric&);
		void setStrategy(Strategy&);
		void simulate();
    void output(const string&);

    int    getWeekNumber()    const { return weekNumber; }
    double getFixTimeHard()   const { return FIX_TIME_HARD; }
    double getFixTimeEasy()   const { return FIX_TIME_EASY; }
    double getImpactMinor()   const { return IMPACT_MINOR; }
    double getImpactMajor()   const { return IMPACT_MAJOR; }
    double getFixTimeError()  const { return FIX_TIME_ERROR; }
    double getImpactError()   const { return IMPACT_ERROR; }
    bool   allowPartFixes()   const { return PART_FIXES; }
};
