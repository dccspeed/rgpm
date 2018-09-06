#ifndef _DATABASE_READER_SET_
#define _DATABASE_READER_SET_

#include<boost/tokenizer.hpp>
#include <string.h>
#include <string>
#include <math.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <list>
#include <sstream>
#include <typeinfo>
#include "constants.h"
#include "structure.h"
#include "type.h"
#include "strmisc.h"
#include "misc.h"
#include "sets.h"
//#include <gzstream.hpp>


class DatabaseReaderSet {
	
	private:
		std::ifstream ifs;
		virtual bool readDataset();

	public:

		std::vector<Set> sets;
		// These strings defines which characters identify the separation of the fields when reading the training and test database
		std::string DELIM;

		DatabaseReaderSet(): DELIM(" \n\t"){}
		virtual ~DatabaseReaderSet(){}
		std::vector<Set> &getSets() { return sets; };
		Uint getNumberOfSets() {return sets.size();};

		std::pair<Set, bool> readSetFromDataset();
		std::ifstream &openDataset(const std::string &DatasetFilename);
		std::ifstream &closeDataset();
		Uint read(const std::string &DatasetFilenames);
		void printSets();
		void clear() {
			sets.clear();
		}


};

class DatabaseReaderSetRuntimeError : public std::runtime_error{
	public:
		DatabaseReaderSetRuntimeError(std::string error="Generic error...")
		: std::runtime_error(error) {};
};

class ParsingDatabaseSetRuntimeError: public DatabaseReaderSetRuntimeError {
	public:
		ParsingDatabaseSetRuntimeError(std::string error="Generic error...")
		: DatabaseReaderSetRuntimeError(error) {};
};

#endif

