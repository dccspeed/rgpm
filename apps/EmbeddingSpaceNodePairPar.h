#ifndef EMBEDDINGSPACEHIGHDEGREEPAR_H
#define EMBEDDINGSPACEHIGHDEGREEPAR_H

#include "BasicEmbeddingSpacePar.h"
#include "AggregatorPatternCounter.h"

template <class T>
class EmbeddingSpaceNodePairPar:public EmbeddingSpacePar<T, AggregatorPatternCounter> {
	
public:

	std::pair<int, int> nodep;

	EmbeddingSpaceNodePairPar(std::pair<int, int>, int , Graph *); 
	EmbeddingSpaceNodePairPar(std::pair<int, int>, int, int, Graph *);
	EmbeddingSpaceNodePairPar(std::pair<int, int>, int, int, Graph *, std::string);
	EmbeddingSpaceNodePairPar(std::pair<int, int>, int, int, Graph *, int);
	EmbeddingSpaceNodePairPar(std::pair<int, int>, int, int, Graph *, std::string, int);
	std::vector<int> getGroupKeys(T &, T &);
	bool isSameGroup(T &, T &);
	bool isValidEmbeddingGroup(T &, T &);
	bool isValidEmbedding(T &);
        bool filterEmbedding(T &);

};

#endif
