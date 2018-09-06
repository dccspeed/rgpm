#ifndef EMBEDDINGSPACEPERCOPAR_H
#define EMBEDDINGSPACEPERCOPAR_H

#include "BasicEmbeddingSpacePar.h"
#include "AggregatorPatternCounter.h"

template <class T>
class EmbeddingSpacePercolationPar:public EmbeddingSpacePar<T, AggregatorPatternCounter> {
	
public:

	EmbeddingSpacePercolationPar(int , Graph *); 
	EmbeddingSpacePercolationPar(int s, int sns, Graph *g);
	EmbeddingSpacePercolationPar(int s, int sns, Graph *g, std::string);
	EmbeddingSpacePercolationPar(int s, int sns, Graph *g, int);
	EmbeddingSpacePercolationPar(int s, int sns, Graph *g, std::string, int);
	std::vector<int> getGroupKeys(T &, T &);
	bool isSameGroup(T &, T &);
	bool isValidEmbeddingGroup(T &, T &);
        bool filterEmbedding(T &);

};

#endif
