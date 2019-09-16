#include "EmbeddingSpaceTraditional.h"


//template <class T>
//EmbeddingSpaceTraditional<T>::EmbeddingSpaceTraditional(){  
//}

template <class T>
EmbeddingSpaceTraditional<T>::EmbeddingSpaceTraditional(int s, Graph *g): EmbeddingSpace<T,AggregatorPatternCounter>(s,g) {  
	this->isSingleGroup=true;
};

template <class T>
EmbeddingSpaceTraditional<T>::EmbeddingSpaceTraditional(int s, int sns, Graph *g): EmbeddingSpace<T,AggregatorPatternCounter>(s, sns, g) {
	this->isSingleGroup=true;
};

template <class T>
EmbeddingSpaceTraditional<T>::EmbeddingSpaceTraditional(int s, int sns, Graph *g, std::string config): EmbeddingSpace<T,AggregatorPatternCounter>(s, sns, g, config) {
	this->isSingleGroup=true;
};

template <class T>
std::vector<int> EmbeddingSpaceTraditional<T>::getGroupKeys(T &e1, T &e2) {
	std::vector<int> groups ({1});
	return groups;
}

template <class T>
bool EmbeddingSpaceTraditional<T>::isSameGroup(T &e1, T &e2) {
	return true;
}

template <class T>
bool EmbeddingSpaceTraditional<T>::isValidEmbeddingGroup(T &e1, T &e2) {
	return false;
}

template <class T>
bool EmbeddingSpaceTraditional<T>::filterEmbedding(T &e){
	double MIN_DENSITY=.0;
	if (Config::existKey(std::string("MIN_DENSITY"))) 
		MIN_DENSITY = Config::getKeyAsDouble(std::string("MIN_DENSITY"));

	return !EmbeddingUtils::isQuasiClique(e,MIN_DENSITY);
	//return true;
}

template class EmbeddingSpaceTraditional<EdgeInducedEmbedding>;
template class EmbeddingSpaceTraditional<VertexInducedEmbedding>;
