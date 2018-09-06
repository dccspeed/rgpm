#ifndef GRAPH_H
#define GRAPH_H

#include <stdlib.h>
#include <string.h>
#include <vector>
#include <stack>
#include <list>
#include <unordered_set>
#include <unordered_map>
#include <deque>
#include <graph.hh>
#include "utils.h"
#include "node.h"
#include "edge.h"
#include "random.h"
//#include <boost/container/flat_set.hpp>
#include <google/dense_hash_set> // or sparse_hash_set, dense_hash_map, ...

/*class EdgeList:public std::list<Edge> {

	public: 
		EdgeList();
		EdgeList(int);
		EdgeList(int, int);
		int getId();
		void setId(int);
		int getLabel();
		void setLabel(int);
		EdgeList::iterator insertOrdered(Edge &);
		EdgeList::iterator find(int);
		EdgeList::iterator getAtPosition(int);
		void print();
};*/

//typedef std::unordered_set<int> NeighborhoodSet;
//typedef google::dense_hash_set<int> NeighborhoodSet;
//typedef boost::container::flat_set<int> NeighborhoodSet;

struct NeighborhoodSet: public google::dense_hash_set<int> {
	NeighborhoodSet():google::dense_hash_set<int>() {
		this->set_empty_key(-1);
	}
};

struct Graph {
	private:
		int id;
		int label;
		int type;
		int numNodeLabels;
		int numEdgeLabels;
		int largestDegree;
		std::vector<Node> nodes;
		std::vector<Edge> edges;

	public:
		//static std::unordered_map<size_t, size_t> pattern_to_canonical; 
		std::vector<NeighborhoodSet> neighborhood;
		std::vector<std::vector<int>> neighborhoodIdx;
		std::vector<NeighborhoodSet> neighborhoodEdge;
		std::unordered_map<std::pair<int, int>, int, boost::hash<std::pair<int, int>>> nodes2edge;

		Graph();
		static Graph init();
		Graph(int size);
		int getId() const;
		void setId(int);
		int getLabel() const;
		void setLabel(int);
		int getType() const;
		void setType(int);
		int getNumberOfNodes() const;
		int getNumberOfEdges() const;
		int getNumberOfNodeLabels() const;
		void setNumberOfNodeLabels(int);
		int getNumberOfEdgeLabels() const;
		void setNumberOfEdgeLabels(int);
		Node &getNodeAt(int);
		int getDegreeOfNodeAt(int);
		int getLargestNodeDegree();
		NeighborhoodSet &getNeighborhoodVertexOfVertexAt(int);
		std::vector<int> &getNeighborhoodIdxVertexOfVertexAt(int);
		NeighborhoodSet &getNeighborhoodEdgeOfVertexAt(int);
		NeighborhoodSet getNeighborhoodEdgeOfEdgeAt(int);
		NeighborhoodSet getNeighborhoodVertexOfEdgeAt(int);
		Edge &getEdgeAt(int);
		std::pair<int, bool> getEdgeIdFromPair(int i, int j);
		void insertEdge(Edge);
		void insertNode(Node);
		std::vector<Node> &getNodes();
		std::vector<Edge> &getEdges();
		void createEdgeIndexSortedById();
		void removeDuplicatedEdges();
		void createNeighborhoodIndex();
		void createNeighborhoodEdgeIndex();
		void sortListsByLabel();
		Graph removeEdgesRandom(double);
		Graph swapNodeLabelsRandom(double, int);
		void print();
		void printToFile(const std::string);
		void printToFileArabesque(const std::string);
		void printResume();
		inline bool isNeighbor(Node& iNode, Node& jNode) {
    			return isNeighbor(iNode.getId(), jNode.getId());
		};
		inline bool isNeighbor(int i, int j) {
    			return neighborhood[i].find(j)!=neighborhood[i].end();	
		};
		bool isNeighborEdge(int, int) ;
		std::vector<std::list<int> > getPossibleMatches(Graph &);
		static bool nodeMatches(Graph &, int , Graph &, int );
		static bool nodePartialMatch(Graph&, Graph&, std::vector<int>&, int);
		static bool subgraphMatching(Graph&, Graph&, std::vector<int>&);

		std::vector<int> bfsOrder(int);
		bool isConnected();

		void printStatistics();
		bliss::Graph getBlissGraph();
		size_t getBlissCodeHashValue();

		int getRandomNodeBiased();
		int getRandomEdgeBiased();
		void randomizeNodes();
		IntIntMap getNodeLabelDistribution();
		IntIntMap getEdgeLabelDistribution();
		void modifyByAddingNodes(int, double); 
		std::vector<int> getPathBetweenNodes(int, int, int);

};


#endif 
