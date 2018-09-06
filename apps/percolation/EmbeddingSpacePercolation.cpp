#include "EmbeddingSpacePercolation.h"


//template <class T>
//EmbeddingSpacePercolation<T>::EmbeddingSpacePercolation(){  
//}

template <class T>
EmbeddingSpacePercolation<T>::EmbeddingSpacePercolation(int s, Graph *g): EmbeddingSpace<T,AggregatorPatternCounter>(s,g) { 
	this->setTransitiveRelation(true); 
};

template <class T>
EmbeddingSpacePercolation<T>::EmbeddingSpacePercolation(int s, int sns, Graph *g): EmbeddingSpace<T,AggregatorPatternCounter>(s, sns, g) {
	this->setTransitiveRelation(true); 
};

template <class T>
EmbeddingSpacePercolation<T>::EmbeddingSpacePercolation(int s, int sns, Graph *g, std::string config): EmbeddingSpace<T,AggregatorPatternCounter>(s, sns, g, config) {
	this->setTransitiveRelation(true); 
};

template <class T>
std::vector<int> EmbeddingSpacePercolation<T>::getGroupKeys(T &e1, T &e2) {
	std::vector<int> keys({1});

	return keys;
}

template <class T>
bool EmbeddingSpacePercolation<T>::isSameGroup(T &e1, T &e2) {
	if (e1.getNumWords() - e1.getNumberOfSharedWordIds(e2) > 1) {
		return false;
	}
	else if ( !e1.isSamePattern(e2)) {
		return false;
	}
	return true;
}

template <class T>
bool EmbeddingSpacePercolation<T>::isValidEmbeddingGroup(T &e1, T &e2) {
	return isSameGroup(e1,e2);
}

template <class T>
bool EmbeddingSpacePercolation<T>::filterEmbedding(T &e){
	double MIN_DENSITY=.0;
	if (Config::existKey(std::string("MIN_DENSITY"))) 
		MIN_DENSITY = Config::getKeyAsDouble(std::string("MIN_DENSITY"));

	return !EmbeddingUtils::isQuasiClique(e,MIN_DENSITY);
	//return true;
}


template class EmbeddingSpacePercolation<EdgeInducedEmbedding>;
template class EmbeddingSpacePercolation<VertexInducedEmbedding>;
