#ifndef EMBEDDINGSPACETRADITIONALPAR_H
#define EMBEDDINGSPACETRADITIONALPAR_H

#include "BasicEmbeddingSpacePar.h"
#include "AggregatorPatternCounter.h"

template <class T>
class EmbeddingSpaceTraditionalPar:public EmbeddingSpacePar<T, AggregatorPatternCounter> {
	
public:

	EmbeddingSpaceTraditionalPar(int , Graph *); 
	EmbeddingSpaceTraditionalPar(int s, int sns, Graph *g);
	EmbeddingSpaceTraditionalPar(int s, int sns, Graph *g, std::string);
	EmbeddingSpaceTraditionalPar(int s, int sns, Graph *g, int);
	EmbeddingSpaceTraditionalPar(int s, int sns, Graph *g, std::string, int);
	std::vector<int> getGroupKeys(T &, T &);
	bool isSameGroup(T &, T &);
	bool isValidEmbeddingGroup(T &, T &);
        bool filterEmbedding(T &);

};

#endif
