#include "DatabaseReaderSet.h"

/**
 */
Uint DatabaseReaderSet::read(const std::string &DatasetFilename){

	// Reading Data Set with the document features
	printf("Reading Sets Files...\n");
	openDataset(DatasetFilename.c_str());
	readDataset();
    closeDataset();
	return 1;
}

std::ifstream &DatabaseReaderSet::openDataset(const std::string &DatasetFilename) {

    ifs.open(DatasetFilename.c_str(), std::ifstream::in);
    // Read the file gzipped
    //gz::igzstream ifs((*currentFile).c_str(), std::ifstream::in);

    if(ifs.good()==false){
        std::ostringstream exception;
        exception << "Dataset file " << DatasetFilename << " not found " << std::endl;
        std::cerr << exception.str();
        throw ParsingDatabaseSetRuntimeError(exception.str());
    }

    return ifs;
}

std::ifstream &DatabaseReaderSet::closeDataset() {

    ifs.close();
    
    return ifs;
}

//here the id of the graph is ignored!
std::pair<Set,bool> DatabaseReaderSet::readSetFromDataset() {
		
    //Set
    Set set;

    std::string currentLine;
    std::getline(ifs, currentLine);
    if (ifs.good()==false)
        return std::pair <Set, bool> (set, ifs.good());

		std::vector<std::string> tokenList = split(currentLine, " ");
		std::vector<std::string>::iterator currentToken = tokenList.begin();

		//std::cout << " Get Line " << currentLine << " Tokens " << tokenList.size() << std::endl;
		//std::cout << "Size of Set " << tokenList.size()-1 << std::endl;

		if (tokenList.size()-1 > 0)
		{   
			set.init_set(atoi(tokenList[0].c_str()));
			currentToken++;
		}
		else 
		{
			printf("Invalid Set! Undefined Size!\n");
			exit(1);
		}

		while (currentToken!=tokenList.end())
		{
			//std::cout << " TOKEN : " << *currentToken << std::endl;
			std::vector<std::string> values_set = split(*currentToken, ":");
			if (values_set.size()!=2)
			{
				printf("Invalid Set! Hash value undefined!\n");
				exit(1);
			}
			set.add_to_set(atoi(values_set[0].c_str()), atoi(values_set[1].c_str()));
			currentToken++;
		}

		return std::pair <Set, bool> (set, ifs.good());
}

bool DatabaseReaderSet::readDataset() {

#ifdef DEBUG
		if (DEBUG>=DEBUGGER){
			std::string DELIM_escaped(DELIM);
			escape(DELIM_escaped);
			std::cout << "Delimiters used to parse: <" << DELIM_escaped << ">" << std::endl;
		}
#endif

        while (ifs.good())

        {
            std::string currentLine;
            std::getline(ifs, currentLine);
            if (ifs.good()==false) break;

            std::vector<std::string> tokenList = split(currentLine," ");
            std::vector<std::string>::iterator currentToken = tokenList.begin();
    
            //std::cout << " Get Line " << currentLine << " Tokens " << tokenList.size() << std::endl;
            //std::cout << "Size of Set " << tokenList.size()-1 << std::endl;

            if (tokenList.size()-1 > 0)
            {   
                sets.push_back(Set (atoi(tokenList[0].c_str())));
                currentToken++;
            }
            else 
            {
                printf("Invalid Set! Undefined Size!\n");
                exit(1);
            }

            while (currentToken!=tokenList.end())
            {
								//std::cout << " TOKEN : " << *currentToken << std::endl;
                std::vector<std::string> values_set = split(*currentToken, ":");
                if (values_set.size()!=2)
                {
					printf("Invalid Set! Hash value undefined!\n");
                    exit(1);
                }
			    sets[sets.size()-1].add_to_set (atoi(values_set[0].c_str()), atoi(values_set[1].c_str()));
                currentToken++;
            }
        }

        return ifs.good();
}


void DatabaseReaderSet::printSets() {
	printf("..................................................\n");
	printf("Printing the Sets:\n");

	for (Uint i = 0; i < getNumberOfSets(); i++)
	{
		std::cout << "Structured Id[ " << sets[i].get_id() << " ]" << " ==> " << i << std::endl;
		sets[i].print_set();
	}
	printf("..................................................\n");
}

/*
void DatabaseReader::printLabels() const {
	printf("..................................................\n");
	printf("Printing the Labels:\n");

	for (int i = 0; i < getNumberOfLabels(); i++)
	{
		std::cout << "LABEL_ID[ " << i << " ]" << " ==> " << Label::SYMBOL_TABLE[i] << std::endl;
		labels[i].print();
	}
	printf("..................................................\n");
}
*/
