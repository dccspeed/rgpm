#ifndef EDGEINDUCEDEMBEDDING_H
#define EDGEINDUCEDEMBEDDING_H

#include "BasicEmbedding.h"
#include "graph.h"
#include "random.h"
#include <set>

class EdgeInducedEmbedding: public BasicEmbedding {

public: 
   std::vector<int> edges;
   std::vector<int> vertices; 
   std::unordered_set<int> edgesSet;
   std::unordered_map<int, int> verticesMap; 

protected:
   bool areWordsNeighbours(int wordId1, int wordId2);

public :
    bool hasVertex(int v);
    bool hasEdge(int v);
    EdgeInducedEmbedding();
    EdgeInducedEmbedding(Graph *g);
    EdgeInducedEmbedding(Graph *g, std::vector<int>);
    void setFromEmbedding(EdgeInducedEmbedding other);
    //void copy(EdgeInducedEmbedding other);
    void reset();
    std::unordered_set<int> &getEdgesSet();
    std::vector<int> &getEdges();
    std::vector<int> &getWords();
    std::vector<int> &getVertices();
    std::unordered_map<int,int> &getVerticesMap();
    std::unordered_map<int,int> &getExtensionsWords();
    int getNumVertices();
    int getNumEdges();
    int getNumWords();
    int getTotalNumWords();
    int getLargestNodeDegree();
    void addWord(int);
    void replaceWord(int, int);
    void removeWord(int);
    void removeLastWord();
    NeighborhoodSet getWordNeighbors(int);

    std::vector<int> getValidElementsForExpansionSorted();
    std::vector<int> &getValidElementsForExpansionSorted(int);
    std::unordered_set<int> getValidElementsForExpansionWith(std::unordered_set<int> &);
    std::unordered_set<int> getValidElementsForExpansion();
    NeighborhoodSet &getValidElementsForExpansion(int);
    std::unordered_set<int> getValidElementsForContraction();
    std::unordered_set<int> getValidElementsForContractionWithWord(int);
    std::unordered_map<int,int> getVertexLabels();
    
    bliss::Graph getBlissGraph();
    bliss::Graph getBlissGraph(std::vector<int>&);

    int getRandomWordBiased();    
    static void test(Graph &g);
};

#endif
