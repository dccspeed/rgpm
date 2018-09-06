#include "EmbeddingSpaceIntraPar.h"

template <class T>
EmbeddingSpaceIntraPar<T>::EmbeddingSpaceIntraPar(int s, Graph *g): EmbeddingSpacePar<T,AggregatorPatternCounter>(s,g) {  
};

template <class T>
EmbeddingSpaceIntraPar<T>::EmbeddingSpaceIntraPar(int s, int sns, Graph *g): EmbeddingSpacePar<T,AggregatorPatternCounter>(s, sns, g) {
};

template <class T>
EmbeddingSpaceIntraPar<T>::EmbeddingSpaceIntraPar(int s, int sns, Graph *g, std::string conf): EmbeddingSpacePar<T,AggregatorPatternCounter>(s, sns, g, conf) {
};

template <class T>
EmbeddingSpaceIntraPar<T>::EmbeddingSpaceIntraPar(int s, int sns, Graph *g, int t): EmbeddingSpacePar<T,AggregatorPatternCounter>(s, sns, g, t) {
};

template <class T>
EmbeddingSpaceIntraPar<T>::EmbeddingSpaceIntraPar(int s, int sns, Graph *g, std::string conf, int t): EmbeddingSpacePar<T,AggregatorPatternCounter>(s, sns, g, conf, t) {
};


template <class T>
std::vector<int> EmbeddingSpaceIntraPar<T>::getGroupKeys(T &e1, T &e2) {
	std::vector<int> keys;

	std::vector<int> shared = e1.getSharedWordIds(e2);
	container_hash<std::vector<int>> hash;
	
	keys.push_back(hash(shared));
	return keys;
}

template <class T>
bool EmbeddingSpaceIntraPar<T>::isSameGroup(T &e1, T &e2) {
	if (e1.getNumWords() - e1.getNumberOfSharedWordIds(e2) > 1) {
		//std::cout << "It is not in the same group: edges difference! " << std::min(e1.getNumEdges(), e2.getNumEdges()) << " "  <<  e1.getNumberOfSharedEdgeIds(e2) << std::endl;
		return false;
	}
	else if ( !e1.isSamePattern(e2)) {
		//std::cout << "It is not in the same group: pattern difference!" << std::endl;
		return false;
	}
	return true;
}

template <class T>
bool EmbeddingSpaceIntraPar<T>::isValidEmbeddingGroup(T &e1, T &e2) {
	return isSameGroup(e1,e2);
}

template <class T>
bool EmbeddingSpaceIntraPar<T>::filterEmbedding(T &e){
	/*double MIN_DENSITY=.0;

	if (Config::existKey(std::string("MIN_DENSITY"))) 
		MIN_DENSITY = Config::getKeyAsDouble(std::string("MIN_DENSITY"));

	if (MIN_DENSITY == 0.) return true;
	//std::cout << "MIN_DENSITY: " << MIN_DENSITY << std::endl;
	return EmbeddingUtils::isQuasiClique(e,MIN_DENSITY);*/

	return false;
}

template class EmbeddingSpaceIntraPar<EdgeInducedEmbedding>;
template class EmbeddingSpaceIntraPar<VertexInducedEmbedding>;
