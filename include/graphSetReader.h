#ifndef GRAPHSETREADER_H
#define GRAPHSETREADER_H

#include "utils.h"
#include "dataReader.h"
#include "graph.h"
//#include<boost/tokenizer.hpp>
#include <iostream>
#include <fstream>
#include <stdexcept>

class GraphSetReader : public DataReader {

	public:
		static IntStrMap id2labelNode;
		static StrIntMap label2idNode;
		static IntStrMap id2labelEdge;
		static StrIntMap label2idEdge;

	public:
		static int getNumberOfNodeLabels()  {return label2idNode.size();};
		static int getNumberOfEdgeLabels()  {return label2idEdge.size();};

		Uint insertNodeLabel(const std::string &label);
		Uint insertEdgeLabel(const std::string &label);
		void reorderEdgesLabel();
		
		std::vector<Graph> read(const std::string &);
		std::pair<Graph, bool> readGraph(); //read just one graph from the data
		std::pair<Graph, bool> readTransactionData();
		std::pair<Graph, bool> readGraphEdgeList();
		StrIntMap &getLabelNodeMap() {
			return label2idNode;
		};

		static std::pair<bool,int> getLabelNodeId(const std::string &s) {
			StrIntMap::iterator it = label2idNode.find(s);
			if (it != label2idNode.end())
				return std::pair<bool, int> (true, it->second);
			return std::pair<bool, int> (false, -1);
		}

};

#endif

