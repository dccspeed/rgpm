#ifndef EMBEDDINGSPACEPERCOLATION_H
#define EMBEDDINGSPACEPERCOLATION_H

#include "BasicEmbeddingSpace.h"

template <class T>
class EmbeddingSpacePercolation:public EmbeddingSpace<T, AggregatorPatternCounter> {
	
public:

	EmbeddingSpacePercolation(int , Graph *); 
	EmbeddingSpacePercolation(int s, int sns, Graph *g);
	EmbeddingSpacePercolation(int s, int sns, Graph *g, std::string);
	std::vector<int> getGroupKeys(T &, T &);
	bool isSameGroup(T &, T &);
	bool isValidEmbeddingGroup(T &, T &);
        bool filterEmbedding(T &);

};

#endif
