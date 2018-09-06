#include "DatabaseReaderSig.h"

/**
 */
Uint DatabaseReaderSig::read(const std::string &DatasetFilename){

	// Reading Data Set with the document features
	printf("Reading Signatures Files...\n");
	openDataset(DatasetFilename.c_str());
	readDataset();
    closeDataset();
	return 1;
}

std::ifstream &DatabaseReaderSig::openDataset(const std::string &DatasetFilename) {

    ifs.open(DatasetFilename.c_str(), std::ifstream::in);
    // Read the file gzipped
    //gz::igzstream ifs((*currentFile).c_str(), std::ifstream::in);

    if(ifs.good()==false){
        std::ostringstream exception;
        exception << "Dataset file " << DatasetFilename << " not found " << std::endl;
        std::cerr << exception.str();
        throw ParsingDatabaseSigRuntimeError(exception.str());
    }

    return ifs;
}

std::ifstream &DatabaseReaderSig::closeDataset() {

    ifs.close();
    
    return ifs;
}

//here the id of the graph is ignored!
std::pair<Signature,bool> DatabaseReaderSig::readSignatureFromDataset() {
		
    //sig
    Signature sig;

    std::string currentLine;
    std::getline(ifs, currentLine);
    if (ifs.good()==false)
        return std::pair <Signature, bool> (sig, ifs.good());

		std::vector<std::string> tokenList = split(currentLine, " ", 4);
		std::vector<std::string>::iterator currentToken = tokenList.begin();

		//std::cout << " Get Line " << currentLine << " Tokens " << tokenList.size() << std::endl;
		//std::cout << "Size of signature " << tokenList.size()-1 << std::endl;

		if (tokenList.size()-1 > 0)
		{   
			sig.init_signature(tokenList.size()-1, atoi(tokenList[0].c_str()));
			currentToken++;
		}
		else 
		{
			printf("Invalid signature! Undefined Size!\n");
			exit(1);
		}

		Uint idx=0;
		while (currentToken!=tokenList.end())
		{
			//std::cout << " TOKEN : " << *currentToken << std::endl;
			std::vector<std::string> values_sig = split(*currentToken, ":");
			if (values_sig.size()!=2)
			{
				printf("Invalid signature! Hash value undefined!\n");
				exit(1);
			}
			sig.set_sig_mhvalue(idx , atoi(values_sig[0].c_str()));
			sig.set_sig_mtcity(idx , atoi(values_sig[1].c_str()));
			currentToken++;
			idx++;
		}

		return std::pair <Signature, bool> (sig, ifs.good());
}

bool DatabaseReaderSig::readDataset() {

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
            //std::cout << "Size of signature " << tokenList.size()-1 << std::endl;

            if (tokenList.size()-1 > 0)
            {   
                sigs.push_back(Signature (tokenList.size()-1, atoi(tokenList[0].c_str())));
                currentToken++;
            }
            else 
            {
                printf("Invalid signature! Undefined Size!\n");
                exit(1);
            }

            Uint idx=0;
            while (currentToken!=tokenList.end())
            {
								//std::cout << " TOKEN : " << *currentToken << std::endl;
                std::vector<std::string> values_sig = split(*currentToken, ":");
                if (values_sig.size()!=2)
                {
										printf("Invalid signature! Hash value undefined!\n");
                    exit(1);
                }
                sigs[sigs.size()-1].set_sig_mhvalue(idx , atoi(values_sig[0].c_str()));
                sigs[sigs.size()-1].set_sig_mtcity(idx , atoi(values_sig[1].c_str()));
                currentToken++;
                idx++;
            }
        }

        return ifs.good();
}


void DatabaseReaderSig::printSignatures() {
	printf("..................................................\n");
	printf("Printing the Signatures:\n");

	for (Uint i = 0; i < getNumberOfSignatures(); i++)
	{
		std::cout << "Structured Id[ " << sigs[i].get_id() << " ]" << " ==> " << i << std::endl;
		sigs[i].print_signature();
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
