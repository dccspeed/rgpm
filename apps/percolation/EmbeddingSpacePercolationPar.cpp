#include "EmbeddingSpacePercolationPar.h"


//template <class T>
//EmbeddingSpacePercolationPar<T>::EmbeddingSpacePercolationPar(){  
//}

template <class T>
EmbeddingSpacePercolationPar<T>::EmbeddingSpacePercolationPar(int s, Graph *g): EmbeddingSpacePar<T,AggregatorPatternCounter>(s,g) { 
	this->setTransitiveRelation(true); 
};

template <class T>
EmbeddingSpacePercolationPar<T>::EmbeddingSpacePercolationPar(int s, int sns, Graph *g): EmbeddingSpacePar<T,AggregatorPatternCounter>(s, sns, g) {
	this->setTransitiveRelation(true); 
};

template <class T>
EmbeddingSpacePercolationPar<T>::EmbeddingSpacePercolationPar(int s, int sns, Graph *g, std::string config): EmbeddingSpacePar<T,AggregatorPatternCounter>(s, sns, g, config) {
	this->setTransitiveRelation(true); 
};

template <class T>
EmbeddingSpacePercolationPar<T>::EmbeddingSpacePercolationPar(int s, int sns, Graph *g, int t): EmbeddingSpacePar<T,AggregatorPatternCounter>(s, sns, g, t) {
	this->setTransitiveRelation(true); 
}

template <class T>
EmbeddingSpacePercolationPar<T>::EmbeddingSpacePercolationPar(int s, int sns, Graph *g, std::string config, int t): EmbeddingSpacePar<T,AggregatorPatternCounter>(s, sns, g, config, t) {
	this->setTransitiveRelation(true); 
}

template <class T>
std::vector<int> EmbeddingSpacePercolationPar<T>::getGroupKeys(T &e1, T &e2) {
	std::vector<int> keys({1});

	return keys;
}

template <class T>
bool EmbeddingSpacePercolationPar<T>::isSameGroup(T &e1, T &e2) {
	if (e1.getNumWords() - e1.getNumberOfSharedWordIds(e2) > 1) {
		return false;
	}
	else if ( !e1.isSamePattern(e2)) {
		return false;
	}
	return true;
}

template <class T>
bool EmbeddingSpacePercolationPar<T>::isValidEmbeddingGroup(T &e1, T &e2) {
	return isSameGroup(e1,e2);
}

template <class T>
bool EmbeddingSpacePercolationPar<T>::filterEmbedding(T &e){
        double MIN_DENSITY=.0;
        if (Config::existKey(std::string("MIN_DENSITY"))) {
                MIN_DENSITY = Config::getKeyAsDouble(std::string("MIN_DENSITY"));
	}        

	if (!EmbeddingUtils::isQuasiClique(e,MIN_DENSITY)) {
		//std::cout << e << " embedding filtered by min density." << std::endl;
		return true;
	}

	return false;
}


template class EmbeddingSpacePercolationPar<EdgeInducedEmbedding>;
template class EmbeddingSpacePercolationPar<VertexInducedEmbedding>;
