#include "DatabaseReader.h"

std::vector<structure_t> DatabaseReader::structures;
IntStrMap DatabaseReader::id2label_v;
IntStrMap DatabaseReader::id2label_e;
StrIntMap DatabaseReader::label2id_v;
StrIntMap DatabaseReader::label2id_e;

Uint DatabaseReader::read(const std::string &DatasetFilename) {

	// Reading Data Set with the document features
	printf("Reading Structured Files...\n");
	openDataset(DatasetFilename.c_str());
	readDataset();
	closeDataset();
	mapIdToLabels(label2id_v, id2label_v);
	mapIdToLabels(label2id_e, id2label_e);
	return 1;
}

std::ifstream &DatabaseReader::openDataset(const std::string &DatasetFilename) {

    ifs.open(DatasetFilename.c_str(), std::ifstream::in);

    if(ifs.good()==false){
        std::ostringstream exception;
        exception << "Dataset file " << DatasetFilename << " not found " << std::endl;
        std::cerr << exception.str();
        throw ParsingDatabaseRuntimeError(exception.str());
    }

    return ifs;
}

std::ifstream &DatabaseReader::closeDataset() {

    ifs.close();
    
    return ifs;
}

//here the id of the graph is ignored!
std::pair<structure_t,bool> DatabaseReader::readTransactionFromDataset() {

	Uint transaction_id=0;
	structure_t data;
	Uint line=0;
	while (ifs.good())
	{
		line++;
		std::string currentLine;
		std::getline(ifs, currentLine);
		if (ifs.good()==false) break;

		std::vector<std::string> tokenList = split(currentLine, " ", 4);
		std::vector<std::string>::iterator currentToken = tokenList.begin();
		if (currentToken!=tokenList.end() && (tokenList[0].compare(std::string ("#"))!=0))
		{	
			if (tokenList[0].compare(std::string ("t"))==0)
			{
				transaction_id = atoi(tokenList[2].c_str());
				if (transaction_id>0) break;
				//FIXME label for each graph
			}
			else if (tokenList[0].compare(std::string("v"))==0)
			{
				Uint id = atoi(tokenList[1].c_str());
				std::string label(tokenList[2]);
				Uint l = insertVertexLabel(label);
				data.insertList(id,l);
			}
			else if (tokenList[0].compare(std::string("e"))==0)
			{
				Uint id1 = atoi(tokenList[1].c_str());
				Uint id2 = atoi(tokenList[2].c_str());
				std::string label(tokenList[3]);
				Uint l = insertEdgeLabel(label);
				data.insertInList(id1, id2, l);
				data.insertInList(id2, id1, l);
			}
			else if (tokenList[0].compare(std::string("a"))==0)
			{
				Uint id1 = atoi(tokenList[1].c_str());
				Uint id2 = atoi(tokenList[2].c_str());
				std::string label(tokenList[3]);
				Uint l = insertEdgeLabel(label);
				data.insertInList(id1, id2, l);
			}

			else
			{
				printf("label not defined! transaction %d line %d\n", transaction_id, line);
				printf("%s\n\n", currentLine.c_str());
				exit(1);
#ifdef DEBUG
				if (DEBUG>=INFO){
					std::cout << "INFO - Parsing Database: Invalid line found when parsing " << DatasetFilename << "..." << std::endl;
					std::cout << "Ignoring Line " << numLines << ": " << currentLine << std::endl;
				}
				if (DEBUG>=DEBUGGER_VERBOSE){
					std::cout << "Printing the tokens found on the line: " << std::endl;
					Uint count=0;
					for(std::vector<std::string>::iterator field=tokenList.begin(); field!=tokenList.end(); ++field, ++count)
						std::cout << "Token[ " << count << "]: <" << *field << ">"<< std::endl;
				}
#endif
			}
		}
	}

	return std::pair <structure_t, bool> (data, ifs.good());
}

bool DatabaseReader::readDataset() {

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

		std::vector<std::string> tokenList = split(currentLine, " ", 4);
		std::vector<std::string>::iterator currentToken = tokenList.begin();
		if (currentToken!=tokenList.end() && (tokenList[0].compare(std::string ("#"))!=0))
		{	
			if (tokenList[0].compare(std::string ("t"))==0)
			{
				Uint id = atoi(tokenList[2].c_str());
				structures.push_back(structure_t());
				structures[structures.size()-1].set_id(id);
				if (structures.size()%100==0) printf("read transaction %i\n", structures.size());
				//FIXME label for each graph
			}
			else if (tokenList[0].compare(std::string("v"))==0)
			{
				Uint id = atoi(tokenList[1].c_str());
				std::string label(tokenList[2]);
				Uint l = insertVertexLabel(label);
				structures[structures.size()-1].insertList(id,l);
			}
			else if (tokenList[0].compare(std::string("e"))==0)
			{
				Uint id1 = atoi(tokenList[1].c_str());
				Uint id2 = atoi(tokenList[2].c_str());
				std::string label(tokenList[3]);
				Uint l = insertEdgeLabel(label);
				//std::cout << "e edge " << "id1 " << id1 << " id2 " << id2 << " label " << l << std::endl;
				structures[structures.size()-1].insertInList(id1, id2, l);
				structures[structures.size()-1].insertInList(id2, id1, l);
			}
			else if (tokenList[0].compare(std::string("a"))==0)
			{
				Uint id1 = atoi(tokenList[1].c_str());
				Uint id2 = atoi(tokenList[2].c_str());
				std::string label(tokenList[3]);
				Uint l = insertEdgeLabel(label);
				structures[structures.size()-1].insertInList(id1, id2, l);
			}

#ifdef DEBUG
			else
			{
				if (DEBUG>=INFO){
					std::cout << "INFO - Parsing Database: Invalid line found when parsing " << DatasetFilename << "..." << std::endl;
					std::cout << "Ignoring Line " << numLines << ": " << currentLine << std::endl;
				}
				if (DEBUG>=DEBUGGER_VERBOSE){
					std::cout << "Printing the tokens found on the line: " << std::endl;
					Uint count=0;
					for(std::vector<std::string>::iterator field=tokenList.begin(); field!=tokenList.end(); ++field, ++count)
						std::cout << "Token[ " << count << "]: <" << *field << ">"<< std::endl;
				}
			}
#endif
		}
	}

	if (getNumberOfStructures() == 0){
		std::ostringstream exception;
		exception << "Failed parsing Database! No transactions computed!" << std::endl;
		exception << "Aborting ..." << std::endl;
        	throw ParsingDatabaseRuntimeError(exception.str());
	}

	return ifs.good();
}


Uint DatabaseReader::insertVertexLabel(const std::string &label) {

	StrIntMap::iterator i= label2id_v.find(label);
	Uint current_size = getNumberOfVertexLabels();
	if(i==label2id_v.end())
	{
		label2id_v[label]=current_size+1;
		return  getNumberOfVertexLabels(); 
	}
	return i->second;
}

Uint DatabaseReader::insertEdgeLabel(const std::string &label){

	StrIntMap::iterator i= label2id_e.find(label);
	Uint current_size = getNumberOfEdgesLabels();

	if(i==label2id_e.end())
	{
		label2id_e[label]=current_size;
		return getNumberOfEdgesLabels() - 1; 
	}
	return i->second;
}

void DatabaseReader::reorderEdgesLabel(){
	for (Uint i = 0; i < getNumberOfStructures(); i++)
        structures[i].sortListsByLabel();
}

void DatabaseReader::printStructures() const {
	printf("..................................................\n");
	printf("Printing the Structures:\n");

	for (Uint i = 0; i < getNumberOfStructures(); i++)
	{
		std::cout << "Structured Id[ " << structures[i].get_id() << " ]" << " ==> " << i << std::endl;
		structures[i].print();
	}
	printf("..................................................\n");
}

void DatabaseReader::printDataStatistics(FILE *os) {
    fprintf(os, "%d\n", structures.size());
    fprintf(os, "%d\n", label2id_v.size());
    fprintf(os, "%d\n", label2id_e.size());
    
    double v_avg=0;
    double e_avg=0;
    
    for (Uint i = 0; i< structures.size(); i++)
    {
        v_avg+=structures[i].size();
        e_avg+=structures[i].get_number_of_edges();
    }

    v_avg/=structures.size();
    e_avg/=structures.size();
    fprintf(os, "%lf\n", v_avg);
    fprintf(os, "%lf\n", e_avg/2);
}

void printGph(structure_t &s, FILE *os) {
	fprintf(os, "t # %d\n", s.get_id());
	std::vector<structure_list_t>::iterator iter;
	for (iter=s.begin(); iter!=s.end(); iter++)
	{
		IntStrMap::iterator v_label = DatabaseReader::id2label_v.find(iter->get_label());
		fprintf(os,"v %d %s\n", iter->get_id(), (v_label->second).c_str());
	}

	structure_list_t::iterator iter_e_l;
	for (iter=s.begin(); iter!=s.end(); iter++) 
	{
		for (iter_e_l = iter->begin(); iter_e_l != iter->end(); iter_e_l++)
		{
			if (iter->get_id() < iter_e_l->id )
			{
				IntStrMap::iterator e_label = DatabaseReader::id2label_e.find(iter_e_l->label);
				fprintf(os,"e %d %d %s\n", iter->get_id(), iter_e_l->id, (e_label->second).c_str() );
			}
		}
	}
}

