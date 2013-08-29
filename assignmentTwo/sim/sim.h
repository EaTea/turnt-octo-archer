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
#include <boost/foreach.hpp>
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

class Defect
{
		const defect_type classification;
		const int weekFound;
    double defectCompletion;
	public:
		Defect(defect_type&, int&, double&);
		defect_type getClassification() const { return classification; }
		bool isMajor() const;
		bool isEasy() const;
};

// STRATEGIES

class Strategy
{
  private:
    int nSWE;
    int nSWETesting;
    int nSWEFixing;
    int weeksPassed;
	public:
    virtual Strategy(int, int, int);
    virtual bool operator()(const Defect&, const Defect&);
};

// METRICS

class Metric
{
		vector<double> values;
	public:
		Metric(string&);
    virtual void update(const priority_queue<Defect>&);
};

// SIMULATION

class Simulation
{
		vector<Metric> metrics;
		Strategy defect_strategy;
		const double FIX_TIME_HARD, FIX_TIME_EASY, IMPACT_MINOR, IMPACT_MAJOR;
		const double ERROR;
		const bool PART_FIXES;
		CSVIteratir csv_file;
		int weekNumber;
		configure(const string&);
		vector<Defect> defects;
	public:
		Simulation(const string&, Strategy&);
		void addMetric(Metric&);
		void setStrategy(Strategy&);
		void simulate();
    void updateMetrics();
};
