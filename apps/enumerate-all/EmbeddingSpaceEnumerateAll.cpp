#include "EmbeddingSpaceEnumerateAll.h"


//template <class T>
//EmbeddingSpaceEnumerateAll<T>::EmbeddingSpaceEnumerateAll(){  
//}

template <class T>
EmbeddingSpaceEnumerateAll<T>::EmbeddingSpaceEnumerateAll(int s, Graph *g): EmbeddingSpace<T,AggregatorPatternCounter>(s,g) {  
	this->isSingleGroup=true;
};

template <class T>
EmbeddingSpaceEnumerateAll<T>::EmbeddingSpaceEnumerateAll(int s, int sns, Graph *g): EmbeddingSpace<T,AggregatorPatternCounter>(s, sns, g) {
	this->isSingleGroup=true;
};

template <class T>
EmbeddingSpaceEnumerateAll<T>::EmbeddingSpaceEnumerateAll(int s, int sns, Graph *g, std::string config): EmbeddingSpace<T,AggregatorPatternCounter>(s, sns, g, config) {
	this->isSingleGroup=true;
};

template <class T>
std::vector<int> EmbeddingSpaceEnumerateAll<T>::getGroupKeys(T &e1, T &e2) {
	std::vector<int> groups ({1});
	return groups;
}

template <class T>
bool EmbeddingSpaceEnumerateAll<T>::isSameGroup(T &e1, T &e2) {
	return true;
}

template <class T>
bool EmbeddingSpaceEnumerateAll<T>::isValidEmbeddingGroup(T &e1, T &e2) {
	return false;
}

template <class T>
bool EmbeddingSpaceEnumerateAll<T>::filterEmbedding(T &e){
	double MIN_DENSITY=.0;
	if (Config::existKey(std::string("MIN_DENSITY"))) 
		MIN_DENSITY = Config::getKeyAsDouble(std::string("MIN_DENSITY"));

	return !EmbeddingUtils::isQuasiClique(e,MIN_DENSITY);
	//return true;
}

template class EmbeddingSpaceEnumerateAll<EdgeInducedEmbedding>;
template class EmbeddingSpaceEnumerateAll<VertexInducedEmbedding>;
