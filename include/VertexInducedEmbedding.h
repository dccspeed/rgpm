#ifndef VERTEXINDUCEDEMBEDDING_H
#define VERTEXINDUCEDEMBEDDING_H

#include "BasicEmbedding.h"
#include "graph.h"
#include "random.h"
#include <set>
#include <boost/container/flat_set.hpp>

class VertexInducedEmbedding: public BasicEmbedding {

protected:
   std::vector<int> vertices;
   std::vector<int> edges;
   std::unordered_set<int> vertexSet;
   //std::unordered_set<int> edgeSet;
   
   //bool areWordsNeighbours(int wordId1, int wordId2);

public :
    VertexInducedEmbedding();
    VertexInducedEmbedding(Graph *g);
    void setFromEmbedding(VertexInducedEmbedding other);
    void copy(VertexInducedEmbedding other);
    void reset();
    std::unordered_set<int> &getVertexSet();
    std::unordered_set<int> computeEdgeSet();
    std::vector<int> &getVertices();
    std::vector<int> &getEdges();
    std::vector<int> &getWords();
    int getNumVertices();
    int getNumWords();
    int getNumEdges();
    int getTotalNumWords();
    void addWord(int);
    void replaceWord(int, int);
    void removeWord(int);
    void removeLastWord();
    bool hasVertex(int v);
    bool hasEdge(int v);
    NeighborhoodSet getWordNeighbors(int);

    std::unordered_set<int> getValidElementsForExpansion();
    NeighborhoodSet &getValidElementsForExpansion(int);
    std::unordered_set<int> getValidElementsForContraction();
    std::unordered_set<int> getValidElementsForContractionWithWord(int);
    std::unordered_map<int,int> getVertexLabels();
    int getLargestNodeDegree();
    
    bliss::Graph getBlissGraph();
    bliss::Graph getBlissGraph(std::vector<int>&);

inline bool areWordsNeighbours(int wordId1, int wordId2) {
	return graph->isNeighbor(wordId1, wordId2);
}
    
    int getRandomWordBiased(); 
    static void test(Graph &g);
};

#endif
