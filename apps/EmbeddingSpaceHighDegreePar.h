#ifndef EMBEDDINGSPACEHIGHDEGREEPAR_H
#define EMBEDDINGSPACEHIGHDEGREEPAR_H

#include "BasicEmbeddingSpacePar.h"
#include "AggregatorPatternCounter.h"

template <class T>
class EmbeddingSpaceHighDegreePar:public EmbeddingSpacePar<T, AggregatorPatternCounter> {
	
public:

	EmbeddingSpaceHighDegreePar(int , Graph *); 
	EmbeddingSpaceHighDegreePar(int s, int sns, Graph *g);
	EmbeddingSpaceHighDegreePar(int s, int sns, Graph *g, std::string);
	EmbeddingSpaceHighDegreePar(int s, int sns, Graph *g, int);
	EmbeddingSpaceHighDegreePar(int s, int sns, Graph *g, std::string, int);
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
