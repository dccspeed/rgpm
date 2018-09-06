#include "EmbeddingSpaceTraditionalPar.h"


template <class T>
EmbeddingSpaceTraditionalPar<T>::EmbeddingSpaceTraditionalPar(int s, Graph *g): EmbeddingSpacePar<T,AggregatorPatternCounter>(s,g) {  
	this->isSingleGroup=true;
};

template <class T>
EmbeddingSpaceTraditionalPar<T>::EmbeddingSpaceTraditionalPar(int s, int sns, Graph *g): EmbeddingSpacePar<T,AggregatorPatternCounter>(s, sns, g) {
	this->isSingleGroup=true;
};

template <class T>
EmbeddingSpaceTraditionalPar<T>::EmbeddingSpaceTraditionalPar(int s, int sns, Graph *g, std::string config): EmbeddingSpacePar<T,AggregatorPatternCounter>(s, sns, g, config) {
	this->isSingleGroup=true;
};

template <class T>
EmbeddingSpaceTraditionalPar<T>::EmbeddingSpaceTraditionalPar(int s, int sns, Graph *g, int t): EmbeddingSpacePar<T,AggregatorPatternCounter>(s, sns, g, t) {
	this->isSingleGroup=true;
};

template <class T>
EmbeddingSpaceTraditionalPar<T>::EmbeddingSpaceTraditionalPar(int s, int sns, Graph *g, std::string config, int t): EmbeddingSpacePar<T,AggregatorPatternCounter>(s, sns, g, config, t) {
	this->isSingleGroup=true;
};

template <class T>
std::vector<int> EmbeddingSpaceTraditionalPar<T>::getGroupKeys(T &e1, T &e2) {
	std::vector<int> groups ({1});
	return groups;
}

template <class T>
bool EmbeddingSpaceTraditionalPar<T>::isSameGroup(T &e1, T &e2) {
	return false;
}

template <class T>
bool EmbeddingSpaceTraditionalPar<T>::isValidEmbeddingGroup(T &e1, T &e2) {
	return false;
}


template <class T>
bool EmbeddingSpaceTraditionalPar<T>::filterEmbedding(T &e){
	double MIN_DENSITY=.0;
	if (Config::existKey(std::string("MIN_DENSITY"))) 
		MIN_DENSITY = Config::getKeyAsDouble(std::string("MIN_DENSITY"));

	return !EmbeddingUtils::isQuasiClique(e,MIN_DENSITY);
	//return true;
}

template class EmbeddingSpaceTraditionalPar<EdgeInducedEmbedding>;
template class EmbeddingSpaceTraditionalPar<VertexInducedEmbedding>;
