#include "sim.h"

void CSVRow::readNextRow(std::ifstream& str)
{
	std::string         line;
	std::getline(str,line);

	std::stringstream   lineStream(line);
	std::string         cell;

	this->m_data.clear();
	while(std::getline(lineStream,cell,','))
	{
		this->m_data.push_back(cell);
	}
}

std::ifstream& operator>>(std::ifstream& str,CSVRow& data)
{
	data.readNextRow(str);
	return str;
}   

CSVIterator& CSVIterator::operator++()
{
	if (this->m_str)
	{
		(*(this->m_str)) >> m_row;
		this->m_str = m_str->good() ? m_str : NULL;
	}
	return *this;
}

CSVIterator CSVIterator::operator++(int)
{
	CSVIterator tmp(*this);
	++(*this);
	return tmp;
}
