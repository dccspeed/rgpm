#ifndef EMBEDDINGSPACEDOMINANT_H
#define EMBEDDINGSPACEDOMINANT_H

#include "BasicEmbeddingSpace.h"

template <class T>
class EmbeddingSpaceDominant:public EmbeddingSpace<T> {
	
public:

	EmbeddingSpaceDominant(int , Graph *); 
	EmbeddingSpaceDominant(int s, int sns, Graph *g);
	EmbeddingSpaceDominant(int s, int sns, Graph *g, std::string);
	std::vector<int> getGroupKeys(T &, T &);
	bool isSameGroup(T &, T &);
	bool canReachGroup(T &, T &);
	bool isValidEmbeddingGroup(T &, T &);
        bool isValidEmbedding(T &);
        //void filterExtensionsByGroup(T &, T &, std::unordered_set<int> &);
	int getHighestDegreeNode(T &);	

};

#endif
