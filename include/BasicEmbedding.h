#ifndef BASICEMBEDDING_H
#define BASICEMBEDDING_H

#include "constants.h"
#include "utils.h"
#include "graph.h"
#include <utils.hh>
#include <graph.hh>
#include <bliss_C.h>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <list>
#include <iostream>

struct Mod {
	public:
		int addId;
		int rmId;
		//int total;
		Mod():addId(-1), rmId(-1) {};
		//Mod():addId(-1), rmId(-1), total(0) {};
		Mod(int a, int r): addId(a), rmId(r) {}
		//Mod(int a, int r, int t): addId(a), rmId(r), total(t) { }

		Mod inverse() { return Mod(rmId, addId); }
		void print() const { std::cout << "Mod: " << addId << " " << rmId << std::endl; }
		friend std::ostream& operator<<(std::ostream& os, const Mod& mod) {
			os.setf(std::ios::fixed);
			os << "Mod: { addId: " <<  mod.addId << " rmId: " << mod.rmId << " }"; 
			return os;
		}

		bool operator==(const Mod &other) const {
		   if(this->addId==other.addId && this->rmId==other.rmId) return true;
		   else return false;
		}	
};

struct mod_hash {
    std::size_t operator()(Mod const& mod) const {
	size_t seed = 0;
	boost::hash_combine(seed,  mod.addId * 2654435761);
	boost::hash_combine(seed,  mod.rmId * 2654435761);
	return seed;
    }
};

typedef std::unordered_set<Mod, mod_hash> ModSet;

class BasicEmbedding {

protected:
   // Basic structure 
   Graph *graph;
   int numNeighbors;
   double degree;

public : 
     bool isPoisonPill;

     BasicEmbedding();
     BasicEmbedding(Graph *g);
     virtual ~BasicEmbedding();
     
     void reset();
     bool isEmpty();
     void init();
     virtual std::vector<int> getValidElementsForExpansionSorted() = 0;
     virtual std::vector<int> &getValidElementsForExpansionSorted(int) = 0;
     virtual std::unordered_set<int> getValidElementsForExpansion() = 0;
     virtual NeighborhoodSet &getValidElementsForExpansion(int) = 0;
     virtual std::unordered_set<int> getValidElementsForContraction() = 0;
     virtual std::unordered_set<int> getValidElementsForContractionWithWord(int) = 0;
     virtual std::vector<int> &getVertices() = 0;
     virtual std::vector<int> &getEdges() = 0;
     virtual NeighborhoodSet getWordNeighbors(int) = 0;
     NeighborhoodSet getWordNeighbors();
     int getNumberOfWordNeighbors(int);
     int getWordDegree(int);
     std::pair<size_t,int> getWordConnectionHash(int);	
     std::pair<size_t,int> getWordConnectionHash(int, ModSet &);	
     virtual int getTotalNumWords() = 0;
     virtual int getRandomWordBiased() = 0;
     int findWordPosition(int word); 
     virtual int getNumWords() = 0;
     virtual int getNumVertices() = 0;
     virtual int getNumEdges() = 0;
     virtual void removeWord(int wordId) = 0;
     virtual void removeLastWord() = 0;
     virtual void addWord(int) = 0;
     virtual void replaceWord(int, int) = 0;
     virtual int getLargestNodeDegree() = 0;
     virtual std::vector<int> &getWords() = 0; 
     bool isCanonicalEmbeddingWithWordAtPosition(int, int);
     bool isCanonicalEmbeddingWithWord(int);
     bool existWord(int wordId);
     virtual bool hasVertex(int) = 0;
     bool hasHighDegreeNode(double);
     virtual bool hasEdge(int) = 0;
     bool hasEdgeLabel(int);
     std::string toString();
     size_t getNaiveCodeHashValue();
     size_t getBlissCodeHashValue();
     size_t getBlissCodeHashValue(std::vector<int> &);
     virtual bliss::Graph getBlissGraph() = 0;
     virtual bliss::Graph getBlissGraph(std::vector<int>&) = 0;
     int getNumberOfSharedWordIds(BasicEmbedding &);
     int getNumberOfNonSharedWordIds(BasicEmbedding &);
     std::vector<int> getSharedWordIds(BasicEmbedding &);
     std::vector<int> getNonSharedWordIds(BasicEmbedding &);
     std::vector<int> getDiffWordIds(BasicEmbedding &);
     bool isSamePattern(BasicEmbedding&);
     bool isSmaller(BasicEmbedding&);
     bool isSameEmbedding(BasicEmbedding&);
     virtual std::unordered_map<int,int> getVertexLabels() = 0;
     bool isNeighbor(BasicEmbedding &);
     std::pair<Mod, bool> getModificationTo(BasicEmbedding &);
    
     bool isConnected();
     size_t getHash();
     int getNumberOfNeighbors() const;
     void setNumberOfNeighbors(int );
     double getDegree() const;
     void setDegree(double );
     void setGraph(Graph *);
     virtual bool areWordsNeighbours(int wordId1, int wordId2) = 0;
     virtual void print();
     void loadFromString(std::string &);
     void writeWordsToFile(std::ofstream &);

     friend std::ostream& operator<<(std::ostream&os , BasicEmbedding& e){
		os.setf(std::ios::fixed);
       	std::vector<int> vertices = e.getVertices();
       	std::vector<int> edges = e.getEdges();
	os << "Embedding: { ";
	os << "vertices: [ ";
	for (int i = 0; i< (int)vertices.size(); i++) {
		os << vertices[i] << " ";
	//for (std::unordered_set<int>::iterator it = vertexSet.begin(); it != vertexSet.end(); it++) {
	//	std::cout << *it << " ";
	}
	os << "]";
	os << " edges: [ ";
	for (int i = 0; i< (int)edges.size(); i++) {
		os << edges[i] << " ";
	}
	os << "]";
	os << " }";
	os << " HASH: " << e.getHash();
	os << " PHASH: " << e.getBlissCodeHashValue();

	return os;	
   }; 

/**
	 * The hook function that prints the found automorphisms.
	 * \a param must be a file descriptor (FILE *).
	 */
	static void report_aut(void* param, const unsigned int n, const unsigned int* aut)
	{
		  assert(param);
		  fprintf((FILE*)param, "Generator: ");
		  bliss::print_permutation((FILE*)param, n, aut, 0);
		  fprintf((FILE*)param, "\n");
	}


};
     

#endif
