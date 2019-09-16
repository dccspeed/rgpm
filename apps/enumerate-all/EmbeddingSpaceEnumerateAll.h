#ifndef EMBEDDINGSPACEENUMALL_H
#define EMBEDDINGSPACEENUMALL_H

#include "BasicEmbeddingSpace.h"

template <class T>
class EmbeddingSpaceEnumerateAll:public EmbeddingSpace<T,AggregatorPatternCounter> {
	
public:

	EmbeddingSpaceEnumerateAll(int , Graph *); 
	EmbeddingSpaceEnumerateAll(int s, int sns, Graph *g);
	EmbeddingSpaceEnumerateAll(int s, int sns, Graph *g, std::string);
	std::vector<int> getGroupKeys(T &, T &);
	bool isSameGroup(T &, T &);
	bool isValidEmbeddingGroup(T &, T &);
        bool filterEmbedding(T &);

};

#endif
