#include "EmbeddingSpaceHighDegree.h"
#include <boost/algorithm/string.hpp>
#include "graphSetReader.h"


//template <class T>
//EmbeddingSpaceHighDegree<T>::EmbeddingSpaceHighDegree(){  
//}

template <class T>
EmbeddingSpaceHighDegree<T>::EmbeddingSpaceHighDegree(int s, Graph *g): EmbeddingSpace<T,AggregatorPatternCounter>(s,g) { 
};

template <class T>
EmbeddingSpaceHighDegree<T>::EmbeddingSpaceHighDegree(int s, int sns, Graph *g): EmbeddingSpace<T,AggregatorPatternCounter>(s, sns, g) {
};

template <class T>
EmbeddingSpaceHighDegree<T>::EmbeddingSpaceHighDegree(int s, int sns, Graph *g, std::string config): EmbeddingSpace<T,AggregatorPatternCounter>(s, sns, g, config) {
};

template <class T>
std::vector<int> EmbeddingSpaceHighDegree<T>::getGroupKeys(T &e1, T &e2) {
	std::vector<int> keys({1});
	return keys;
}

template <class T>
bool EmbeddingSpaceHighDegree<T>::isSameGroup(T &e1, T &e2) {

	if (e1.getHash() == e2.getHash()) return true;

	std::vector<int> &v1 = e1.getVertices();
	std::vector<int> &v2 = e2.getVertices();
	int highest = 0;
	for (auto &i : v1) {
		int d = this->g->getDegreeOfNodeAt(i);
		if (highest < d) highest = d;
		if (d >= DEGREE_THRESHOLD && !e2.hasVertex(i)) {
			return false;
		}	
	}
		
	if (highest < DEGREE_THRESHOLD) return false;

	for (auto &i : v2) {
		int d = this->g->getDegreeOfNodeAt(i);
		if (d >= DEGREE_THRESHOLD && !e1.hasVertex(i)) {
			return false;
		}
	}

	if (!e1.isSamePattern(e2)) {
		return false;
	}

	return true;
}

template <class T>
bool EmbeddingSpaceHighDegree<T>::isValidEmbeddingGroup(T &e1, T &e2) {
	//return isSameGroup(e1,e2);
	
	std::vector<int> &v1 = e1.getVertices();

        for (auto &i : v1) {
                int d = this->g->getDegreeOfNodeAt(i);
                if (d >= DEGREE_THRESHOLD && e2.hasVertex(i)) {
                        return true;
                }
        }
	return false;
}

template <class T>
bool EmbeddingSpaceHighDegree<T>::filterEmbedding(T &e){
        double MIN_DENSITY=.0;
        if (Config::existKey(std::string("MIN_DENSITY")))
                MIN_DENSITY = Config::getKeyAsDouble(std::string("MIN_DENSITY"));
	if (!EmbeddingUtils::isQuasiClique(e,MIN_DENSITY)) {
		//std::cout << "filtered by mindensity!" << std::endl;
		return true;
	}
        
	double MAX_DENSITY=1.0001;
        if (Config::existKey(std::string("MAX_DENSITY")))
                MAX_DENSITY = Config::getKeyAsDouble(std::string("MAX_DENSITY"));
	if (EmbeddingUtils::isQuasiClique(e,MAX_DENSITY)) {
		//std::cout << "filtered by maxdensity!" << std::endl;
		return true;
	}
        
	std::set<int> labels;
	if (Config::existKey(std::string("TARGET_LABELS"))) 
               labels = getLabelsFromString(Config::getKeyAsString(std::string("TARGET_LABELS")));

	if (!labels.empty()) {
		std::vector<int> &v1 = e.getVertices();
	        for (auto &i : v1) {
			Node &n = this->g->getNodeAt(i);
			std::set<int>::iterator it = labels.find(n.getLabel());
			if (it!=labels.end()) labels.erase(it);
			if (labels.empty()) break;
		}
		if (!labels.empty()) {
			//std::cout << "filtered by labels!" << std::endl;
			return true;
		}
	}
	
	std::set<size_t> hashes;
	if (Config::existKey(std::string("NTARGET_PATTERNS"))) 
               hashes = getPatternCodesFromString(Config::getKeyAsString(std::string("NTARGET_PATTERNS")));

	if (!hashes.empty() && hashes.find(e.getBlissCodeHashValue())!=hashes.end()) {
		//std::cout << "filtered by pattern code!" << std::endl;
		return true;
	}
	return false;
}

template <class T>
std::set<int> EmbeddingSpaceHighDegree<T>::getLabelsFromString(std::string s) {
                std::vector<std::string> tokenList;
                boost::split(tokenList, s, boost::is_any_of(" ,;"));

		std::set<int> ls;
		for (auto &i : tokenList) {
			std::pair<bool, int> r = GraphSetReader::getLabelNodeId(i);
			//std::cout << "label: "	<< i << " id: " << r.second << std::endl;
			if (r.first) ls.insert(r.second);
		}
		return ls;
}

template <class T>
std::set<size_t> EmbeddingSpaceHighDegree<T>::getPatternCodesFromString(std::string s) {
                std::vector<std::string> tokenList;
                boost::split(tokenList, s, boost::is_any_of(" ,;"));
		//std::cout << "NTARGET_PATTERNS" << std::endl;
                std::set<size_t> pcs;
		size_t code = 0;
                for (auto &i : tokenList) {
			sscanf(i.c_str(), "%zu", &code);
                        pcs.insert(code);
			//std::cout << "pattern code: "	<< code <<  std::endl;
                }
                return pcs;
}

template class EmbeddingSpaceHighDegree<EdgeInducedEmbedding>;
template class EmbeddingSpaceHighDegree<VertexInducedEmbedding>;
