#include "EmbeddingSpaceDominant.h"

template <class T>
EmbeddingSpaceDominant<T>::EmbeddingSpaceDominant(int s, Graph *g): EmbeddingSpace<T>(s,g) {  
};

template <class T>
EmbeddingSpaceDominant<T>::EmbeddingSpaceDominant(int s, int sns, Graph *g): EmbeddingSpace<T>(s, sns, g) {
};


template <class T>
EmbeddingSpaceDominant<T>::EmbeddingSpaceDominant(int s, int sns, Graph *g, std::string conf): EmbeddingSpace<T>(s, sns, g, conf) {
};

template <class T>
std::vector<int> EmbeddingSpaceDominant<T>::getGroupKeys(T &e1, T &e2) {
	std::vector<int> keys({1});
	return keys;
}

template <class T>
bool EmbeddingSpaceDominant<T>::isSameGroup(T &e1, T &e2) {
	int h1 = getHighestDegreeNode(e1);
	int h2 = getHighestDegreeNode(e2);
	if (h1 != h2) {
		return false;
	}
	else if ( !e1.isSamePattern(e2)) {
		return false;
	}
	return true;
}

template <class T>
bool EmbeddingSpaceDominant<T>::isValidEmbeddingGroup(T &e1, T &e2) {
	return isSameGroup(e1,e2);
}

template <class T>
bool EmbeddingSpaceDominant<T>::isValidEmbedding(T &e){
	double MIN_DENSITY=.0;

	if (Config::existKey(std::string("MIN_DENSITY"))) 
		MIN_DENSITY = Config::getKeyAsDouble(std::string("MIN_DENSITY"));

	if (MIN_DENSITY == 0.) return true;
	//std::cout << "MIN_DENSITY: " << MIN_DENSITY << std::endl;
	return EmbeddingUtils::isQuasiClique(e,MIN_DENSITY);
	//return true;
}

/*template <class T>
void EmbeddingSpaceDominant<T>::filterExtensionsByGroup(T &e, T &eRef, std::unordered_set<int> &expansions) {
	int p = 0;

	//prune by pattern' labels
	std::unordered_map<int,int> refLabels = eRef.getVertexLabels();
	for (std::unordered_set<int>::iterator it = expansions.begin(); it!= expansions.end();) {	
		Node n = this->g->getNodeAt(*it);
		std::unordered_map<int,int>::iterator itLabel = refLabels.find(n.getLabel());
		if (itLabel == refLabels.end()) {
			it = expansions.erase(it);		
			p++;
		}
		else {
			it++;
		} 
	}
	//prune by density
	double MIN_DENSITY=.0;
	if (Config::existKey(std::string("MIN_DENSITY"))) 
		MIN_DENSITY = Config::getKeyAsDouble(std::string("MIN_DENSITY"));
	//std::cout << "MIN_DENSITY: " << MIN_DENSITY << std::endl;
	if (MIN_DENSITY == 0.) {
		for (std::unordered_set<int>::iterator it = expansions.begin(); it!= expansions.end();) {	
			bool r = false;
			Node n = this->g->getNodeAt(*it);
			double factor = MIN_DENSITY*(double)(e.getNumWords()-1.);
			int dGlobal = this->g->getDegreeOfNodeAt(*it);
			if ((double) dGlobal < factor)
				r = true;
			else {
				int dLocal = e.getWordDegree(*it);
				if ( (double) dLocal < factor)
					r = true;
			}

			//std::cout << "factor: " << factor << " d: " << d << std::endl;
			if (r) {
				it = expansions.erase(it);
				p++;
			}
			else 
				it++;
		}
	}

	//if (p > 0)
	//  std::cout << "pruned: " << p << " from: " << expansions.size()+p << std::endl;
	//  else {
	//  std::cout << "none was pruned\n";
	//  }

}*/
   
template <class T>
int EmbeddingSpaceDominant<T>::getHighestDegreeNode(T &embedding) {
	std::vector<int> vertices = embedding.getVertices();		
	
	int maxDegree = 0;
	int idx = -1;
        for (int i = 0; i < (int) vertices.size(); i++) {
		if (maxDegree < this->g->getDegreeOfNodeAt(vertices[i])) {
			maxDegree = this->g->getDegreeOfNodeAt(vertices[i]);
			idx = i;
		}
	}

	if (idx < 0) {
		std::cout << "ERROR: in getHighestDegreeNode function!" << std::endl;
		exit(1);
	} 
	return vertices[idx];
}

template class EmbeddingSpaceDominant<EdgeInducedEmbedding>;
template class EmbeddingSpaceDominant<VertexInducedEmbedding>;
