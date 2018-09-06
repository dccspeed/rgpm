#ifndef EMBEDDINGSPACETARGET_H
#define EMBEDDINGSPACETARGET_H

#include "BasicEmbeddingSpace.h"

template <class T>
class EmbeddingSpaceTarget:public EmbeddingSpace<T,AggregatorPatternCounter> {
	
public:

	EmbeddingSpaceTarget(int , Graph *); 
	EmbeddingSpaceTarget(int s, int sns, Graph *g);
	EmbeddingSpaceTarget(int s, int sns, Graph *g, std::string);
	std::vector<int> getGroupKeys(T &, T &);
	bool isSameGroup(T &, T &);
	bool canReachGroup(T &, T &);
	bool isValidEmbeddingGroup(T &, T &);
	bool isValidEmbedding(T &);
	void filterExtensionsByGroup(T &, T &, std::unordered_set<int> &);
};

#endif
