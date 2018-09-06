#ifndef _DATABASE_READER_SIG_
#define _DATABASE_READER_SIG_

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
#include "signatures.h"


class DatabaseReaderSig {

	public:
		std::vector<Signature> sigs;
		std::string DELIM;

	private:
		std::ifstream ifs;
		virtual bool readDataset();

	public:
		DatabaseReaderSig(): DELIM(" \n\t"){}
		virtual ~DatabaseReaderSig(){}

		std::vector<Signature> &getSigs() { return sigs; };

		Uint getNumberOfSignatures() {return sigs.size();};

		std::pair<Signature, bool> readSignatureFromDataset();
		std::ifstream &openDataset(const std::string &DatasetFilename);
		std::ifstream &closeDataset();
		Uint read(const std::string &DatasetFilenames);
		void printSignatures();

		void clear() {
			sigs.clear();
		}

};

class DatabaseReaderSigRuntimeError : public std::runtime_error{
	public:
		DatabaseReaderSigRuntimeError(std::string error="Generic error...")
		: std::runtime_error(error) {};
};

class ParsingDatabaseSigRuntimeError: public DatabaseReaderSigRuntimeError {
	public:
		ParsingDatabaseSigRuntimeError(std::string error="Generic error...")
		: DatabaseReaderSigRuntimeError(error) {};
};

#endif

