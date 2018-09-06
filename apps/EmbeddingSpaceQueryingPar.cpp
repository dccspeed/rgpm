#include "EmbeddingSpaceQueryingPar.h"


template <class T>
EmbeddingSpaceQueryingPar<T>::EmbeddingSpaceQueryingPar(int s, Graph *g): EmbeddingSpacePar<T,AggregatorPatternCounter>(s,g) {  
};

template <class T>
EmbeddingSpaceQueryingPar<T>::EmbeddingSpaceQueryingPar(int s, int sns, Graph *g): EmbeddingSpacePar<T,AggregatorPatternCounter>(s, sns, g) {
};

template <class T>
EmbeddingSpaceQueryingPar<T>::EmbeddingSpaceQueryingPar(int s, int sns, Graph *g, std::string config): EmbeddingSpacePar<T,AggregatorPatternCounter>(s, sns, g, config) {
};

template <class T>
EmbeddingSpaceQueryingPar<T>::EmbeddingSpaceQueryingPar(int s, int sns, Graph *g, int t): EmbeddingSpacePar<T,AggregatorPatternCounter>(s, sns, g, t) {
};

template <class T>
EmbeddingSpaceQueryingPar<T>::EmbeddingSpaceQueryingPar(int s, int sns, Graph *g, std::string config, int t): EmbeddingSpacePar<T,AggregatorPatternCounter>(s, sns, g, config, t) {
};

template <class T>
std::vector<int> EmbeddingSpaceQueryingPar<T>::getGroupKeys(T &e1, T &e2) {
	std::vector<int> groups ({1});
	return groups;
}

template <class T>
bool EmbeddingSpaceQueryingPar<T>::isSameGroup(T &e1, T &e2) {
	if ( e2.getBlissCodeHashValue() != qcode ) {
		return false;
	}
	return true;
}

template <class T>
bool EmbeddingSpaceQueryingPar<T>::isValidEmbeddingGroup(T &e1, T &e2) {
        return true;
}

template <class T>
void EmbeddingSpaceQueryingPar<T>::loadQuery(Graph *_q) {
	q = _q;
     	qcode = q->getBlissCodeHashValue();
}

template class EmbeddingSpaceQueryingPar<EdgeInducedEmbedding>;
template class EmbeddingSpaceQueryingPar<VertexInducedEmbedding>;
