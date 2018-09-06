#include "EmbeddingSpaceNodePairPar.h"
#include <boost/algorithm/string.hpp>
#include "graphSetReader.h"

template <class T>
EmbeddingSpaceNodePairPar<T>::EmbeddingSpaceNodePairPar(std::pair<int, int> nodep, int s, Graph *g): EmbeddingSpacePar<T,AggregatorPatternCounter>(s,g) { 
	this->nodep = nodep;
};

template <class T>
EmbeddingSpaceNodePairPar<T>::EmbeddingSpaceNodePairPar(std::pair<int, int> nodep, int s, int sns, Graph *g): EmbeddingSpacePar<T,AggregatorPatternCounter>(s, sns, g) {
	this->nodep = nodep;
};

template <class T>
EmbeddingSpaceNodePairPar<T>::EmbeddingSpaceNodePairPar(std::pair<int, int> nodep, int s, int sns, Graph *g, std::string config): EmbeddingSpacePar<T,AggregatorPatternCounter>(s, sns, g, config) {
	this->nodep = nodep;
};

template <class T>
EmbeddingSpaceNodePairPar<T>::EmbeddingSpaceNodePairPar(std::pair<int, int> nodep, int s, int sns, Graph *g, int t): EmbeddingSpacePar<T,AggregatorPatternCounter>(s, sns, g, t) {
	this->nodep = nodep;
}

template <class T>
EmbeddingSpaceNodePairPar<T>::EmbeddingSpaceNodePairPar(std::pair<int, int> nodep, int s, int sns, Graph *g, std::string config, int t): EmbeddingSpacePar<T,AggregatorPatternCounter>(s, sns, g, config, t) {
	this->nodep = nodep;
}

template <class T>
std::vector<int> EmbeddingSpaceNodePairPar<T>::getGroupKeys(T &e1, T &e2) {
	std::vector<int> keys({1});
	return keys;
}

template <class T>
bool EmbeddingSpaceNodePairPar<T>::isSameGroup(T &e1, T &e2) {
	if (e1.getHash() == e2.getHash()) return true;

	if (!e1.hasVertex(nodep.first) || !e1.hasVertex(nodep.second)) 
        	return false;

        if (!e2.hasVertex(nodep.first) || !e2.hasVertex(nodep.second)) 
        	return false;
	
	if (!e1.isSamePattern(e2)) 
		return false;
		

	return true;
}

template <class T>
bool EmbeddingSpaceNodePairPar<T>::isValidEmbeddingGroup(T &e1, T &e2) {
        if (e2.hasVertex(nodep.first) || e2.hasVertex(nodep.second)) { 
		//std::cout << "group valid emb " << e2 << std::endl;
        	return true;
	}

	return false;
}

template <class T>
bool EmbeddingSpaceNodePairPar<T>::filterEmbedding(T &e){
        if (!e.hasVertex(nodep.first) || !e.hasVertex(nodep.second)) {  
		//std::cout << "filtering by node pair: " << nodep.first << " " << nodep.second << std::endl;
		//std::cout << e << std::endl;
        	return true;
	}
        
	if (Config::existKey(std::string("MIN_DENSITY"))) {
                double MIN_DENSITY = Config::getKeyAsDouble(std::string("MIN_DENSITY"));
		if (!EmbeddingUtils::isQuasiClique(e,MIN_DENSITY)) {
			return true;
		}
	}
        
        if (Config::existKey(std::string("MAX_DENSITY"))) {
	        double MAX_DENSITY = Config::getKeyAsDouble(std::string("MAX_DENSITY"));
		if (EmbeddingUtils::isQuasiClique(e,MAX_DENSITY)) {
			return true;
		}
	}
	return false;
}

template <class T>
bool EmbeddingSpaceNodePairPar<T>::isValidEmbedding(T &e) {
	//std::cout << "valid emb " << e << std::endl;
        if (e.hasVertex(nodep.first)) 
        	return true;
        if (e.hasVertex(nodep.second)) 
        	return true;

	return false;
}

template class EmbeddingSpaceNodePairPar<EdgeInducedEmbedding>;
template class EmbeddingSpaceNodePairPar<VertexInducedEmbedding>;
