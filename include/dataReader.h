#ifndef DATAREADER_H
#define DATAREADER_H

//#include<boost/tokenizer.hpp>
#include <iostream>
#include <fstream>
#include <stdexcept>

class DataReader{

	protected:
		std::ifstream ifs;
		// These strings defines which characters identify the separation of the fields 
		std::string delim;

	public:
		//DataReader(std::string d):delim(d) {} ;
		DataReader();
		virtual ~DataReader();
		std::ifstream &openData(const std::string);
		std::ifstream &closeData();

};

class DataReaderRuntimeError : public std::runtime_error{
	public:
		DataReaderRuntimeError(std::string error="Generic error...")
		: std::runtime_error(error) {};
};

class ParsingDataRuntimeError: public DataReaderRuntimeError {
	public:
		ParsingDataRuntimeError(std::string error="Generic error...")
		: DataReaderRuntimeError(error) {};
};



#endif

