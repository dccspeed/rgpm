#include "EmbeddingSpaceTargetPar.h"
#include <set>

//template <class T>
//EmbeddingSpaceTargetPar<T>::EmbeddingSpaceTargetPar(){  
//}

template <class T>
EmbeddingSpaceTargetPar<T>::EmbeddingSpaceTargetPar(int s, Graph *g): EmbeddingSpacePar<T>(s,g) {  
};

template <class T>
EmbeddingSpaceTargetPar<T>::EmbeddingSpaceTargetPar(int s, int sns, Graph *g): EmbeddingSpacePar<T>(s, sns, g) {
};

template <class T>
EmbeddingSpaceTargetPar<T>::EmbeddingSpaceTargetPar(int s, int sns, Graph *g, std::string config): EmbeddingSpacePar<T>(s, sns, g, config) {
};

template <class T>
EmbeddingSpaceTargetPar<T>::EmbeddingSpaceTargetPar(int s, int sns, Graph *g, int t): EmbeddingSpacePar<T>(s, sns, g, t) {
};

template <class T>
EmbeddingSpaceTargetPar<T>::EmbeddingSpaceTargetPar(int s, int sns, Graph *g, std::string config, int t): EmbeddingSpacePar<T>(s, sns, g, config, t) {
};

template <class T>
std::vector<int> EmbeddingSpaceTargetPar<T>::getGroupKeys(T &e1, T &e2) {
	std::vector<int> keys;
	keys.push_back(1);
	return keys;
}


// intersecao dos nos que retwitaram
template <class T>
bool EmbeddingSpaceTargetPar<T>::isSameGroup(T &e1, T &e2) {
	std::vector<int> e1Vertices = e1.getVertices();
	std::vector<int> e2Vertices = e2.getVertices();
	
	std::set<int> e2SetVertices(e2Vertices.begin(), e2Vertices.end());
	std::set<int> e1InterestNodes;
	std::set<int> e2InterestNodes;

	for(auto& v1 : e1Vertices) {
		Node n = this->g->getNodeAt(v1);
		std::vector<int> extra = n.getExtra();
		if(extra[0] == 2) {
			e1InterestNodes.insert(v1);
		}
	}

	int count = 0;
	for(auto& v2: e2Vertices) {
		Node n = this->g->getNodeAt(v2);
		std::vector<int> extra = n.getExtra();
		if(extra[0] == 2) {

			if(e1InterestNodes.find(v2) == e1InterestNodes.end()) {
				return false;
			} else {
				count += 1;
			}
		}
	}
	if(count != (int)e1InterestNodes.size()) {
		return false;
	}
	if(!e1.isSamePattern(e2)) {
		return false;
	}

	return true;
}

template <class T>
bool EmbeddingSpaceTargetPar<T>::isValidEmbeddingGroup(T &e1, T &e2) {
	std::vector<int> e1Vertices = e1.getVertices();
	std::vector<int> e2Vertices = e2.getVertices();
	
	std::set<int> e2SetVertices(e2Vertices.begin(), e2Vertices.end());
	std::set<int> e1InterestNodes;

	for(auto& v1 : e1Vertices) {
		Node n = this->g->getNodeAt(v1);
		std::vector<int> extra = n.getExtra();
		if(extra[0] == 2) {
			e1InterestNodes.insert(v1);
		}
	}

	int count = 0;
	for(auto& v2: e2Vertices) {
		Node n = this->g->getNodeAt(v2);
		std::vector<int> extra = n.getExtra();
		if(extra[0] == 2) {

			if(e1InterestNodes.find(v2) == e1InterestNodes.end()) {
				return false;
			} else {
				count += 1;
			}
		}
	}
	if(count != (int)e1InterestNodes.size()) {
		return false;
	}
	return true;
}

// Se tem algum cara com segundo label como 2 
template <class T>
bool EmbeddingSpaceTargetPar<T>::isValidEmbedding(T &e){

	std::vector<int> vertices = e.getVertices();
	bool ret = false;
	for(auto& v: vertices){
		Node n = this->g->getNodeAt(v);
		std::vector<int> extra = n.getExtra();
		if(extra[0] == 2) {
			ret = true;
		}	
	}

	return ret;
	//return ret && EmbeddingUtils::isQuasiClique(e,0.7);
}

template <class T>
void EmbeddingSpaceTargetPar<T>::filterExtensionsByGroup(T &e, T &eRef, std::unordered_set<int> &expansions) {
	return;
/*	int p = 0;
	for (std::unordered_set<int>::iterator it = expansions.begin(); it!= expansions.end();) {	
		
		bool r = false;
		Node n = this->g->getNodeAt(*it);
		double factor = 0.7*(double)(e.getNumWords()-1.);
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
	if (p > 0)
		std::cout << "pruned: " << p << " from: " << expansions.size()+p << std::endl;
	else {

		std::cout << "none was pruned\n";
	}
*/
}

template class EmbeddingSpaceTargetPar<EdgeInducedEmbedding>;
template class EmbeddingSpaceTargetPar<VertexInducedEmbedding>;
