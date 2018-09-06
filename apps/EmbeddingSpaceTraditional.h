#ifndef EMBEDDINGSPACETRADITIONAL_H
#define EMBEDDINGSPACETRADITIONAL_H

#include "BasicEmbeddingSpace.h"

template <class T>
class EmbeddingSpaceTraditional:public EmbeddingSpace<T,AggregatorPatternCounter> {
	
public:

	EmbeddingSpaceTraditional(int , Graph *); 
	EmbeddingSpaceTraditional(int s, int sns, Graph *g);
	EmbeddingSpaceTraditional(int s, int sns, Graph *g, std::string);
	std::vector<int> getGroupKeys(T &, T &);
	bool isSameGroup(T &, T &);
	bool isValidEmbeddingGroup(T &, T &);
        bool filterEmbedding(T &);

};

#endif
