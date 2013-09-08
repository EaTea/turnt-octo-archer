// STL
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <fstream>
#include <iterator>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;

// Author: Edwin Tay, 20529864

// CONSTANTS
const int 		NUM_SWE								=		3;  	//number of SWEs
const double 	SWE_HOURS							=		25.0; //hours they work per week
const double 	MAJOR_DEFECT_IMPACT		=		7.0;  //perceived impact of major defect
const double	MINOR_DEFECT_IMPACT		=		1.0;  //perceived impact of minor defect
const double	HARD_DEFECT_FIX_TIME	=		5.0;  //hours to fix a hard defect
const double	EASY_DEFECT_FIX_TIME	=		2.0;  //hours to fix an easy defect
const int			AT_LEAST_WEEKS				=		5;

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
  void readNextRow(std::ifstream&);
  private:
  std::vector<std::string>  m_data;
};

std::ifstream& operator>>(std::ifstream&,CSVRow&);

class CSVIterator
{   
  public:
    typedef std::input_iterator_tag   iterator_category;
    typedef CSVRow            value_type;
    typedef std::size_t       difference_type;
    typedef CSVRow*           pointer;
    typedef CSVRow&           reference;

    CSVIterator(std::ifstream& str)  :m_str(str.good() ? & str : NULL)
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
				|| ((this->m_str == NULL) && (rhs.m_str == NULL)));
		}
    bool operator!=(CSVIterator const& rhs)
		{
			return !((*this) == rhs);
		}
  private:
    std::ifstream		 *m_str;
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

struct Defect
{
	defect_type classification;
	int weekFound;
	double defectCompletion;
	int weekFixed;
	Defect(defect_type c, int wf = -1, double comp = 0, int wkFxd = -1) :
		classification(c), weekFound(wf), defectCompletion(comp), weekFixed(wkFxd) { }
	defect_type getClassification() const { return classification; }
	bool isMajor() const
	{
		return classification == MAJ_HARD || classification == MAJ_EASY;
	}
	bool isEasy() const
	{
		return classification == MAJ_EASY || classification == MIN_EASY;
	}
	double getFixTime() const
	{
		return this->isEasy() ? EASY_DEFECT_FIX_TIME : HARD_DEFECT_FIX_TIME;
	}
};

struct Simulation;

// STRATEGIES

class Strategy
{
	public:
		int nSWE;
		int nSWETesting;
		int nSWEFixing;
		Strategy(int=-1, int=-1, int=-1);
		Strategy(const Strategy&);
		virtual void update(Simulation&) =0;
		virtual void update_defects(std::vector<Defect>&) const =0;
		virtual bool simContinue() const =0;
		virtual string getName() const =0;
};

// METRICS

class Metric
{
	public:
		std::vector<double> values;
		Metric();
		virtual void update(Simulation&) =0;
		virtual std::string getName() const =0;
};


void writeMetricToFile(const Metric&, const string&);

// SIMULATION

struct Simulation
{
	CSVIterator csv_file;
	int weekNumber;
	Strategy* defect_strategy;
	std::vector<Metric*> metrics;
	std::vector<Defect> defects;
	std::vector<Defect> fixed_defects;

	Simulation(ifstream&);

	void 	 addMetric(Metric* m) { metrics.push_back(m); }
	void	 setStrategy(Strategy* s) { defect_strategy = s; }
	void	 simulate();

	int    getWeekNumber()    const { return weekNumber; }
};

class DefectCountingMetric : public Metric
{
  public:
		int curWeek;
    DefectCountingMetric();
    std::string getName() const;
    std::vector<double> getValues() const;
    void update(Simulation& s);
};

class DefMajCountMetric : public Metric
{
  public:
		int curWeek;
    DefMajCountMetric();
    std::string getName() const;
    std::vector<double> getValues() const;
    void update(Simulation& s);
};

class DefectFixedMetric : public Metric
{
  public:
		int curWeek;
    DefectFixedMetric();
    std::string getName() const;
    std::vector<double> getValues() const;
    void update(Simulation& s);
};

class DefectMajorFixedMetric : public Metric
{
  public:
		int curWeek;
    DefectMajorFixedMetric();
    std::string getName() const;
    std::vector<double> getValues() const;
    void update(Simulation& s);
};

class RatioFoundFixedMetric : public Metric
{
  public:
		int curWeek;
		DefectCountingMetric dcm;
		DefectFixedMetric dfm;
    RatioFoundFixedMetric();
    std::string getName() const;
    std::vector<double> getValues() const;
    void update(Simulation& s);
};

class RatioMajFoundFixedMetric : public Metric
{
  public:
		int curWeek;
		DefMajCountMetric dmcm;
		DefectMajorFixedMetric dmfm;
    RatioMajFoundFixedMetric();
    std::string getName() const;
    std::vector<double> getValues() const;
    void update(Simulation& s);
};

class AverageTimeInQueueMetric : public Metric
{
  public:
		int curWeek;
    AverageTimeInQueueMetric();
    std::string getName() const;
    std::vector<double> getValues() const;
    void update(Simulation& s);
};

class AverageMajTimeInQueueMetric : public Metric
{
  public:
		int curWeek;
    AverageMajTimeInQueueMetric();
    std::string getName() const;
    std::vector<double> getValues() const;
    void update(Simulation& s);
};

class QueueImpactMetric : public Metric
{
  public:
		int curWeek;
    QueueImpactMetric();
    std::string getName() const;
    std::vector<double> getValues() const;
    void update(Simulation& s);
};

class QueueSizeMetric : public Metric
{
  public:
		int curWeek;
    QueueSizeMetric();
    std::string getName() const;
    std::vector<double> getValues() const;
    void update(Simulation& s);
};

class EstDefectsRemMetric : public Metric
{
  public:
		const static double estimation_rate = 0.2;
		int curWeek;
		DefectCountingMetric dcm;
    EstDefectsRemMetric();
    std::string getName() const;
    std::vector<double> getValues() const;
    void update(Simulation& s);
};

class EstMajDefRemMetric : public Metric
{
  public:
		const static double estimation_rate = 0.2;
		int curWeek;
		DefMajCountMetric dmcm;
    EstMajDefRemMetric();
    std::string getName() const;
    std::vector<double> getValues() const;
    void update(Simulation& s);
};

// VANILLA STRATS

class StrategyFixMajorDefectsFirst : public Strategy
{
	public:
		EstDefectsRemMetric edrm;
		bool keepGoing;
		StrategyFixMajorDefectsFirst(int,int,int);
		void update(Simulation& s);
		static bool comparison(const Defect& a, const Defect& b);
		void update_defects(std::vector<Defect>& defects) const;
		bool simContinue() const;
		string getName() const;
};

class StrategyFixEasyDefectsFirst : public Strategy
{
	public:
		EstDefectsRemMetric edrm;
		bool keepGoing;
		StrategyFixEasyDefectsFirst(int,int,int);
		void update(Simulation& s);
		static bool comparison(const Defect& a, const Defect& b);
		void update_defects(std::vector<Defect>& defects) const;
		bool simContinue() const;
		string getName() const;
};

class StrategyFIFO : public Strategy
{
	public:
		EstDefectsRemMetric edrm;
		bool keepGoing;
		StrategyFIFO(int,int,int);
		void update(Simulation& s);
		static bool comparison(const Defect& a, const Defect& b);
		void update_defects(std::vector<Defect>& defects) const;
		bool simContinue() const;
		string getName() const;
};

