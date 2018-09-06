#include "graphSetReader.h"
#include "strmisc.h"
#include "utils.h"
#include <sstream>
#include <algorithm>
#include <boost/algorithm/string.hpp>


IntStrMap GraphSetReader::id2labelNode;
IntStrMap GraphSetReader::id2labelEdge;
StrIntMap GraphSetReader::label2idNode;
StrIntMap GraphSetReader::label2idEdge;

std::vector<Graph> GraphSetReader::read(const std::string &dataFilename) {
	// Reading Data Set with the document features
	printf("Reading Structured Files...\n");

	std::vector<Graph> graphs;	

	openData(dataFilename.c_str());
	//readGraph();
	closeData();
	mapIdToLabels(label2idNode, id2labelNode);
	mapIdToLabels(label2idEdge, id2labelEdge);
	
	return graphs;
}


Uint GraphSetReader::insertNodeLabel(const std::string &label){

	StrIntMap::iterator i = label2idNode.find(label);
	int current_size = getNumberOfNodeLabels();
	if(i==label2idNode.end())
	{
		label2idNode[label]=current_size;
		return  current_size;
	}
	return i->second;
}

Uint GraphSetReader::insertEdgeLabel(const std::string &label){

	StrIntMap::iterator i = label2idEdge.find(label);
	int current_size = getNumberOfEdgeLabels();

	if(i==label2idEdge.end())
	{
		label2idEdge[label]=current_size+1;
		return getNumberOfEdgeLabels();
	}
	return i->second;
}

std::pair<Graph,bool> GraphSetReader::readGraph() {

	uint graphId=0;
	Graph data;
	uint line=0;
	uint vertices=0;
	uint total_vertices=0;
	while (ifs.good())
	{
		line++;
		std::string currentLine;
		std::getline(ifs, currentLine);
		if (ifs.good()==false) break;

		std::vector<std::string> tokenList;
		boost::split(tokenList,currentLine,boost::is_any_of(" "));
		//tokenList = split(currentLine, " ", 4);
		std::vector<std::string>::iterator currentToken = tokenList.begin();
		//std::cout << " Get Line " << currentLine << " Tokens " << tokenList.size() << std::endl;
		if (!tokenList.empty() && currentToken!=tokenList.end() && (tokenList[0].compare("#")!=0))
		{	
			if (tokenList[0].compare("t")==0)
			{
				graphId = atoi(tokenList[2].c_str());
				if (graphId%1000 == 0) std::cout << "t new graph id " << graphId<< std::endl;
				//if (graphId>0) break;
				//FIXME label for each graph
				total_vertices+=vertices;
				vertices=0;
			}
			else if (tokenList[0].compare("v")==0 && tokenList.size()>=3)
			{
				int id = atoi(tokenList[1].c_str()) + total_vertices;
				std::string label(tokenList[2]);
				int l = insertNodeLabel(label);
				//std::cout << "v vertex " << "id " << id << " label " << l << std::endl;

				std::vector<int> extra; 
				for (int i = 3; i < (int)tokenList.size(); i++)			
					extra.push_back(atoi(tokenList[i].c_str()));

				data.insertNode(Node(id,l,extra));

				vertices++;
			}
			else if (tokenList[0].compare("e")==0 && tokenList.size()==4)
			{
				int id1 = atoi(tokenList[1].c_str()) + total_vertices;
				int id2 = atoi(tokenList[2].c_str()) + total_vertices;
				int l = insertEdgeLabel(tokenList[3]);
				//std::cout << "u edge " << "id1 " << id1 << " id2 " << id2 << " label " << l << std::endl;
				data.insertEdge(Edge(0, std::min(id1,id2), std::max(id1,id2), l));
				//data.insertEdge(Edge(0,id2, id1,l));
			}
			else if (tokenList[0].compare("a")==0 && tokenList.size()==4)
			{
				int id1 = atoi(tokenList[1].c_str()) + total_vertices;
				int id2 = atoi(tokenList[2].c_str()) + total_vertices;
				std::string label(tokenList[3]);
				int l = insertEdgeLabel(label);
				data.insertEdge(Edge(0, std::min(id1,id2), std::max(id1,id2), l));
			}
			else
			{
				printf("label not defined! transaction %d line %d\n", graphId, line);
				printf("%s\n\n", currentLine.c_str());
				exit(1);
#ifdef DEBUG
				if (DEBUG>=INFO){
					std::cout << "INFO - Parsing Database: Invalid line found when parsing "  << "..." << std::endl;
					std::cout << "Ignoring Line " << line << ": " << currentLine << std::endl;
				}
				if (DEBUG>=VERBOSE){
					std::cout << "Printing the tokens found on the line: " << std::endl;
					int count=0;
					for(std::vector<std::string>::iterator field=tokenList.begin(); field!=tokenList.end(); ++field, ++count)
						std::cout << "Token[ " << count << "]: <" << *field << ">"<< std::endl;
				}
#endif
			}
		}
	}


	mapIdToLabels(label2idNode, id2labelNode);
	mapIdToLabels(label2idEdge, id2labelEdge);

	//setting number of node labels
	data.setNumberOfNodeLabels(label2idNode.size());	 
	data.setNumberOfEdgeLabels(label2idEdge.size());	 

	return std::pair <Graph, bool> (data, ifs.good());
}



std::pair<Graph,bool> GraphSetReader::readTransactionData() {

	uint graphId=0;
	Graph data;
	uint line=0;
	uint vertices=0;
	uint total_vertices=0;
	while (ifs.good())
	{
		line++;
		std::string currentLine;
		std::getline(ifs, currentLine);
		if (ifs.good()==false) break;

		std::vector<std::string> tokenList;
		boost::split(tokenList,currentLine,boost::is_any_of(" "));
		//tokenList = split(currentLine, " ", 4);
		std::vector<std::string>::iterator currentToken = tokenList.begin();
		//std::cout << " Get Line " << currentLine << " Tokens " << tokenList.size() << std::endl;
		if (!tokenList.empty() && currentToken!=tokenList.end() && (tokenList[0].compare("#")!=0))
		{	
			if (tokenList[0].compare("t")==0)
			{
				graphId = atoi(tokenList[2].c_str());

				if(graphId>0) break;

				//std::cout << "Reading transaction 0\n"; 
				total_vertices+=vertices;
				vertices = 0;
			}
			else if (tokenList[0].compare("v")==0 && tokenList.size()>=3)
			{

				int id = atoi(tokenList[1].c_str()) + total_vertices;
				std::string label(tokenList[2]);
				int l = insertNodeLabel(label);
				//std::cout << "v vertex " << "id " << id << " label " << l << std::endl;

				std::vector<int> extra; 
				for (int i = 3; i < (int)tokenList.size(); i++)			
					extra.push_back(atoi(tokenList[i].c_str()));

				data.insertNode(Node(id,l,extra));

				vertices++;
			}
			else if (tokenList[0].compare("e")==0 && tokenList.size()==4)
			{
				int id1 = atoi(tokenList[1].c_str()) + total_vertices;
				int id2 = atoi(tokenList[2].c_str()) + total_vertices;
				int l = insertEdgeLabel(tokenList[3]);
				//std::cout << "u edge " << "id1 " << id1 << " id2 " << id2 << " label " << l << std::endl;
				data.insertEdge(Edge(0, std::min(id1,id2), std::max(id1,id2), l));
				//data.insertEdge(Edge(0,id2, id1,l));
			}
			else if (tokenList[0].compare("a")==0 && tokenList.size()==4)
			{
				int id1 = atoi(tokenList[1].c_str()) + total_vertices;
				int id2 = atoi(tokenList[2].c_str()) + total_vertices;
				std::string label(tokenList[3]);
				int l = insertEdgeLabel(label);
				data.insertEdge(Edge(0, std::min(id1,id2), std::max(id1,id2), l));
			}
			else
			{
				printf("label not defined! transaction %d line %d\n", graphId, line);
				printf("%s\n\n", currentLine.c_str());
				exit(1);
#ifdef DEBUG
				if (DEBUG>=INFO){
					std::cout << "INFO - Parsing Database: Invalid line found when parsing " << "..." << std::endl;
					std::cout << "Ignoring Line " << line << ": " << currentLine << std::endl;
				}
				if (DEBUG>=VERBOSE){
					std::cout << "Printing the tokens found on the line: " << std::endl;
					int count=0;
					for(std::vector<std::string>::iterator field=tokenList.begin(); field!=tokenList.end(); ++field, ++count)
						std::cout << "Token[ " << count << "]: <" << *field << ">"<< std::endl;
				}
#endif
			}
		}
	}


	mapIdToLabels(label2idNode, id2labelNode);
	mapIdToLabels(label2idEdge, id2labelEdge);

	//setting number of node labels
	data.setNumberOfNodeLabels(label2idNode.size());	 
	data.setNumberOfEdgeLabels(label2idEdge.size());	 

	return std::pair <Graph, bool> (data, ifs.good());
}


