#ifndef EMBEDDINGSPACEHIGHDEGREE_H
#define EMBEDDINGSPACEHIGHDEGREE_H

#include "BasicEmbeddingSpace.h"

template <class T>
class EmbeddingSpaceHighDegree:public EmbeddingSpace<T, AggregatorPatternCounter> {
	
public:

	EmbeddingSpaceHighDegree(int , Graph *); 
	EmbeddingSpaceHighDegree(int s, int sns, Graph *g);
	EmbeddingSpaceHighDegree(int s, int sns, Graph *g, std::string);
	std::vector<int> getGroupKeys(T &, T &);
	bool isSameGroup(T &, T &);
	bool isValidEmbeddingGroup(T &, T &);
        bool filterEmbedding(T &);

	void setDegreeThreshold(int d) {
		DEGREE_THRESHOLD = d;
	}

	std::set<int> getLabelsFromString(std::string );
	std::set<size_t> getPatternCodesFromString(std::string );
		
	int DEGREE_THRESHOLD = 1000;

};

#endif
