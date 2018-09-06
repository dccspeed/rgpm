#ifndef EMBEDDINGSPACEINTRA_H
#define EMBEDDINGSPACEINTRA_H

#include "BasicEmbeddingSpace.h"

template <class T>
class EmbeddingSpaceIntra:public EmbeddingSpace<T, AggregatorPatternCounter> {
	
public:

	EmbeddingSpaceIntra(int , Graph *); 
	EmbeddingSpaceIntra(int s, int sns, Graph *g);
	EmbeddingSpaceIntra(int s, int sns, Graph *g, std::string);
	std::vector<int> getGroupKeys(T &, T &);
	bool isSameGroup(T &, T &);
	bool canReachGroup(T &, T &);
	bool isValidEmbeddingGroup(T &, T &);
        bool filterEmbedding(T &);

};

#endif
