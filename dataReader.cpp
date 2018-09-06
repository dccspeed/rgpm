#include "dataReader.h"
#include <sstream>

DataReader::DataReader(){
}

DataReader::~DataReader(){
}
	
std::ifstream &DataReader::openData(const std::string dataFilename) {

    ifs.open(dataFilename.c_str(), std::ifstream::in);

    if(ifs.good()==false){
        std::ostringstream exception;
        exception << "data file " << dataFilename << " not found " << std::endl;
        std::cerr << exception.str();
        throw ParsingDataRuntimeError(exception.str());
    }

    return ifs;
}

std::ifstream &DataReader::closeData() {

    ifs.close();
    
    return ifs;
}

