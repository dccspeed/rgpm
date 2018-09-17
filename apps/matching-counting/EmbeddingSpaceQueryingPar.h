#ifndef EMBEDDINGSPACETRADITIONALPAR_H
#define EMBEDDINGSPACETRADITIONALPAR_H

#include "BasicEmbeddingSpacePar.h"
#include "AggregatorPatternCounter.h"

template <class T>
class EmbeddingSpaceQueryingPar:public EmbeddingSpacePar<T, AggregatorPatternCounter> {
	
public:

	Graph *q;
	size_t qcode;

	EmbeddingSpaceQueryingPar(int , Graph *); 
	EmbeddingSpaceQueryingPar(int s, int sns, Graph *g);
	EmbeddingSpaceQueryingPar(int s, int sns, Graph *g, std::string);
	EmbeddingSpaceQueryingPar(int s, int sns, Graph *g, int);
	EmbeddingSpaceQueryingPar(int s, int sns, Graph *g, std::string, int);
	std::vector<int> getGroupKeys(T &, T &);
	bool isSameGroup(T &, T &);
	bool isValidEmbeddingGroup(T &, T &);
	void loadQuery(Graph *);

};

#endif
