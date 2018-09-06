#ifndef EMBEDDINGSPACEINTRAPAR_H
#define EMBEDDINGSPACEINTRAPAR_H

#include "AggregatorPatternCounter.h"
#include "BasicEmbeddingSpacePar.h"

template <class T>
class EmbeddingSpaceIntraPar:public EmbeddingSpacePar<T, AggregatorPatternCounter> {
	
public:

	EmbeddingSpaceIntraPar(int , Graph *); 
	EmbeddingSpaceIntraPar(int s, int sns, Graph *g);
	EmbeddingSpaceIntraPar(int s, int sns, Graph *g, std::string);
	EmbeddingSpaceIntraPar(int s, int sns, Graph *g, int);
	EmbeddingSpaceIntraPar(int s, int sns, Graph *g, std::string, int);
	std::vector<int> getGroupKeys(T &, T &);
	bool isSameGroup(T &, T &);
	bool isValidEmbeddingGroup(T &, T &);
        bool filterEmbedding(T &);

	
	const double EMB_DENSITY = 0.7;
};

#endif
