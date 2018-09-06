#ifndef EMBEDDINGSPACEPERCONEIGH_H
#define EMBEDDINGSPACEPERCONEIGH_H

#include "BasicEmbeddingSpace.h"

template <class T>
class EmbeddingSpacePercoNeigh:public EmbeddingSpace<T, AggregatorPatternCounter> {
	
public:

	EmbeddingSpacePercoNeigh(int , Graph *); 
	EmbeddingSpacePercoNeigh(int s, int sns, Graph *g);
	EmbeddingSpacePercoNeigh(int s, int sns, Graph *g, std::string);
	std::vector<int> getGroupKeys(T &, T &);
	bool isSameGroup(T &, T &);
	bool isValidEmbeddingGroup(T &, T &);
        bool filterEmbedding(T &);

};

#endif
