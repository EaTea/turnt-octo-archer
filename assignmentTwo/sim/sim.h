// STL
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iterator>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>

// Author: Edwin Tay, 20529864

// CONSTANTS
const int 		NUM_SWE								=		3;  	//number of SWEs
const double 	SWE_HOURS							=		25.0; //hours they work per week
const double 	MAJOR_DEFECT_IMPACT		=		7.0;  //perceived impact of major defect
const double	MINOR_DEFECT_IMPACT		=		1.0;  //perceived impact of minor defect
const double	HARD_DEFECT_FIX_TIME	=		5.0;  //hours to fix a hard defect
const double	EASY_DEFECT_FIX_TIME	=		2.0;  //hours to fix an easy defect

// OPTIONS
const bool		ALLOW_PART_FIXES			=		false;//do we allow part fixes?
const double	DEFECT_IMPACT_ERROR		=		0.0;	//error in percentages
const double	FIX_TIME_ERROR		    =		0.0;	//error in percentages

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
		void closeStream();

    bool operator==(CSVIterator const& rhs)
		{
			return ((this == &rhs)
				|| ((this->m_str == NULL) && (rhs.m_str == NULL)));
		}
    bool operator!=(CSVIterator const& rhs)
		{
			return !((*this) == rhs);
		}
  private:
    std::istream*     m_str;
    CSVRow            m_row;
};

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
	public:
		const defect_type classification;
		const int weekFound;
    const double defectCompletion;
		Defect(defect_type& c, int& wf, double& comp) : classification(c), weekFound(wf), defectCompletion(comp) { };
		defect_type getClassification() const { return classification; }
		bool isMajor() const
    {
      return classification & MAJOR_BITMASK ? true : false;
    }
		bool isEasy() const
    {
      return classification & EASY_BITMASK ? true : false;
    }
		double getFixTime() const
		{
			return this->isEasy() ? EASY_DEFECT_FIX_TIME : HARD_DEFECT_FIX_TIME;
		}
};

class Simulation;

// STRATEGIES

class Strategy
{
	public:
    const int nSWE;
    int nSWETesting;
    int nSWEFixing;
    Strategy(int, int, int);
    virtual void update(const Simulation&);
    virtual bool comparison(const Defect&, const Defect&);
};

// METRICS

class Metric
{
	public:
    std::vector<double> values;
		Metric();
    virtual void update(const Simulation&);
    virtual std::string getName() const;
    virtual std::vector<double> getValues() const;
};


class OutputMetric
{
	public:
		static void writeMetricToFile(const Metric&);
};

// SIMULATION

class Simulation
{
	private:
		CSVIterator csv_file;
	public:
		int weekNumber;
		Strategy defect_strategy;
    std::vector<Metric> metrics;
    std::vector<Defect> defects;
		std::vector<Defect> fixed_defects;

		Simulation(const string&, Strategy);
    ~Simulation();

		void 	 addMetric(Metric&);
		void	 setStrategy(Strategy&);
		void	 simulate();
    void	 output(const std::string&);

    int    getWeekNumber()    const { return weekNumber; }
};
