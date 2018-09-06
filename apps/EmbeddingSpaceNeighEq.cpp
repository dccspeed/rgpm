#include "EmbeddingSpaceNeighEq.h"


//template <class T>
//EmbeddingSpaceNeighEq<T>::EmbeddingSpaceNeighEq(){  
//}

template <class T>
EmbeddingSpaceNeighEq<T>::EmbeddingSpaceNeighEq(int s, Graph *g): EmbeddingSpace<T>(s,g) {  
};

template <class T>
EmbeddingSpaceNeighEq<T>::EmbeddingSpaceNeighEq(int s, int sns, Graph *g): EmbeddingSpace<T>(s, sns, g) {
};

template <class T>
std::vector<int> EmbeddingSpaceNeighEq<T>::getGroupKeys(T &e1, T &e2) {
	std::vector<int> keys;

	container_hash<std::vector<int>> hash;
	size_t key1 = hash(computeTopKNeighbors(e1,4));
	size_t key2 = hash(computeTopKNeighbors(e2,4));

	if (key1==key2)
		keys.push_back(hash(shared));
	return keys;
}

template <class T>
bool EmbeddingSpaceNeighEq<T>::isSameGroup(T &e1, T &e2) {
	size_t key1 = hash(computeTopKNeighbors(e1,4));
	size_t key2 = hash(computeTopKNeighbors(e2,4));
	if (key1!=key2)
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
bool EmbeddingSpaceNeighEq<T>::isValidEmbeddingGroup(T &e1, T &e2) {
	#if (topK neighs of e1 are not neighs of e2 or in e2)
	return true;
}

std::vector<int> EmbeddingSpaceNeighEq<T>::computeTopKNeighbors(T &e) {
	std::vector<int> topNeigh;

	return topNeigh;	
}


template class EmbeddingSpaceNeighEq<EdgeInducedEmbedding>;
template class EmbeddingSpaceNeighEq<VertexInducedEmbedding>;
