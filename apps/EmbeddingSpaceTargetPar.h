#ifndef EMBEDDINGSPACETARGETPAR_H
#define EMBEDDINGSPACETARGETPAR_H

#include "BasicEmbeddingSpacePar.h"

template <class T>
class EmbeddingSpaceTargetPar:public EmbeddingSpacePar<T> {
	
public:

	EmbeddingSpaceTargetPar(int , Graph *); 
	EmbeddingSpaceTargetPar(int s, int sns, Graph *g);
	EmbeddingSpaceTargetPar(int s, int sns, Graph *g, int);
	EmbeddingSpaceTargetPar(int s, int sns, Graph *g, std::string);
	EmbeddingSpaceTargetPar(int s, int sns, Graph *g, std::string, int);
	std::vector<int> getGroupKeys(T &, T &);
	bool isSameGroup(T &, T &);
	bool canReachGroup(T &, T &);
	bool isValidEmbeddingGroup(T &, T &);
	bool isValidEmbedding(T &);
	void filterExtensionsByGroup(T &, T &, std::unordered_set<int> &);
};

#endif
