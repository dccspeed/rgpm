#include "VertexInducedEmbedding.h"
#include "EmbeddingUtils.h"
#include <graph.hh>
#include <algorithm>

void VertexInducedEmbedding::reset() {
	BasicEmbedding::reset();
	vertices.clear();
	vertexSet.clear();
}

VertexInducedEmbedding::VertexInducedEmbedding():BasicEmbedding() { 
};

VertexInducedEmbedding::VertexInducedEmbedding(Graph *g, std::vector<int> ws):BasicEmbedding(g, ws) { 
	for (int w : ws) addWord(w);
};

VertexInducedEmbedding::VertexInducedEmbedding(Graph *g):BasicEmbedding(g) { 
};

VertexInducedEmbedding::~VertexInducedEmbedding() { 
};

void VertexInducedEmbedding::setFromEmbedding(VertexInducedEmbedding other) {
}

/*void VertexInducedEmbedding::copy(VertexInducedEmbedding other) {
	graph = other.graph;
	vertices = other.getVertices();
	vertexSet = other.getVertexSet();
}*/

std::unordered_set<int> &VertexInducedEmbedding::getVertexSet() {
	return vertexSet;
}
    
NeighborhoodSet VertexInducedEmbedding::getWordNeighbors(int id) {
	assert(graph!=NULL);
	assert(id < graph->getNumberOfNodes());
        return graph->getNeighborhoodVertexOfVertexAt(id);
}

std::unordered_set<int> VertexInducedEmbedding::computeEdgeSet() {
	std::unordered_set<int> es;
	//for (int i = 0; i < (int)vertices.size(); i++) {
	//	for (int j = i; j < (int)vertices.size(); j++) {

	for (auto& i: vertices) {
		assert(i < graph->getNumberOfNodes());
		for (auto& j: vertices) {
 			assert(j < graph->getNumberOfNodes());
			if (i == j) continue;
			if (graph->isNeighbor(j, i)) {
				int min = std::min(j, i);
				int max = std::max(j, i);
				std::pair<int, bool> ret = graph->getEdgeIdFromPair(min, max);
				if (ret.second) es.insert(ret.first);	
				else {
					std::cout << "error: edge id not found!" << std::endl;
					exit(1);
				}
			}
		}
	}
	return es;
}

std::vector<int> &VertexInducedEmbedding::getVertices() {
	return vertices;
}

std::vector<int> &VertexInducedEmbedding::getWords() {
	return vertices;
}

inline int VertexInducedEmbedding::getNumVertices() {
	return vertices.size();
}

inline int VertexInducedEmbedding::getNumWords() {
	return getNumVertices();
}

int VertexInducedEmbedding::getLargestNodeDegree() {
	int l = -1;
	for (int i = 0; i < (int)vertices.size(); i++) {
		if (l < graph->getDegreeOfNodeAt(vertices[i])){
			l = graph->getDegreeOfNodeAt(vertices[i]);
		}
	}
	return l;
}

inline NeighborhoodSet &VertexInducedEmbedding::getValidElementsForExpansion(int vertexId) {
	return graph->getNeighborhoodVertexOfVertexAt(vertexId);
}

inline std::vector<int> &VertexInducedEmbedding::getValidElementsForExpansionSorted(int vertexId) {
	return graph->getNeighborhoodIdxVertexOfVertexAt(vertexId);
}

inline std::unordered_set<int> VertexInducedEmbedding::getValidElementsForContraction() {
	std::unordered_set<int> contractions;
	std::vector<bool> points = EmbeddingUtils::articulation(*this);

	for (int i = 0 ; i < (int)vertices.size(); i++)
		if (!points[i])
			contractions.insert(vertices[i]);	
	return contractions;
}

inline std::unordered_set<int> VertexInducedEmbedding::getValidElementsForContractionWithWord(int wordId) {
	std::unordered_set<int> contractions;
	std::vector<bool> points = EmbeddingUtils::articulation(*this, wordId);

	for (int i = 0 ; i < (int)vertices.size(); i++)
		if (!points[i] && wordId!=vertices[i])
			contractions.insert(vertices[i]);	
	return contractions;
}

//it assumes that the neighbor vector is sorted
std::vector<int> VertexInducedEmbedding::getValidElementsForExpansionSorted() {
	std::vector<int> expansions;
	if (vertices.size() == 0) {
		expansions.push_back(-1);
	}

	std::unordered_set<int> v(vertices.begin(), vertices.end()); 
	std::vector<std::vector<int>::const_iterator> fwdIterators, endIterators;
	uint maxsize = 0;
	for (int i = 0; i < (int)vertices.size(); i++) {
		std::vector<int> &neighs = getValidElementsForExpansionSorted(vertices[i]);
		//std::cout << "neighs " << i;
		//for (int j : neighs) 
		//	std::cout << " " << j;
		//std::cout << std::endl;
		fwdIterators.push_back(neighs.begin());
		endIterators.push_back(neighs.end());
		maxsize+=neighs.size();
	}
	expansions.reserve(maxsize);

	while (!fwdIterators.empty())
	{
		// Find out which iterator carries the smallest value
		size_t index = 0;
		for (size_t i = 1; i < fwdIterators.size(); ++i) {
			if (*fwdIterators[i] < *fwdIterators[index])
				index = i;
		}

		if (v.find(*fwdIterators[index])==v.end() && (expansions.empty() || expansions.back() < *fwdIterators[index])) {
			expansions.push_back(*fwdIterators[index]);
			//std::cout << "add " << *fwdIterators[index] << std::endl;
		}

		++fwdIterators[index];
		if (fwdIterators[index] == endIterators[index]) {
			fwdIterators.erase(fwdIterators.begin() + index);
			endIterators.erase(endIterators.begin() + index);
		}
	}

	//for (int i : expansions) 
	//	std::cout << " " << i;
	//std::cout << "size:" << expansions.size() << std::endl;
	
	return expansions;
}

std::unordered_set<int> VertexInducedEmbedding::getValidElementsForExpansion() {
	std::unordered_set<int> expansions;
	if (vertices.size() == 0) {
		expansions.insert(-1); 
	}
	for (int i = 0; i < (int)vertices.size(); i++) {
		NeighborhoodSet possibleExp = getValidElementsForExpansion(vertices[i]);
		expansions.insert(possibleExp.begin(), possibleExp.end());
	}

	//remove edges already inserted
	for (int i = 0; i < (int)vertices.size(); i++) {	
		//std::cout << "removing word: " << vertices[i] << std::endl;
		expansions.erase(vertices[i]);
	}

	return expansions;
}

std::unordered_set<int> VertexInducedEmbedding::getValidElementsForExpansionWith(std::unordered_set<int> &l) {
	std::unordered_set<int> expansions;
	if (vertices.size() == 0) {
		expansions.insert(-1); 
	}
	for (int i = 0; i < (int)vertices.size(); i++) {
		NeighborhoodSet possibleExp = getValidElementsForExpansion(vertices[i]);
		for (int j : possibleExp)
			if (l.find(j)!=l.end()) expansions.insert(j);
	}

	//remove vertices already inserted
	for (int i = 0; i < (int)vertices.size(); i++) {	
		//std::cout << "removing word: " << vertices[i] << std::endl;
		expansions.erase(vertices[i]);
	}

	return expansions;
}


/**
 * Add word and update the number of vertices in this embedding.
 *
 * @param word
 */

/**
 * Add word and update the number of vertices in this embedding.
 *
 * @param word
 */

inline void VertexInducedEmbedding::addWord(int word) {
	if (vertexSet.find(word)!=vertexSet.end()) {
		std::cout << "addWord error! word already exist! " << word << std::endl;
		//this->print();
		exit(1);
	}
	vertices.push_back(word);
	vertexSet.insert(word);
}

inline void VertexInducedEmbedding::replaceWord(int word1, int word2) {
	if (vertexSet.find(word1)==vertexSet.end()) {
		std::cout << "replaceWord error! word to be replaced does not exist! " << word1 << std::endl;
		this->print();
		std::cout << "Hash value: " << this->getHash() << std::endl;
		exit(1);
	}
	if (vertexSet.find(word2)!=vertexSet.end()) {
		std::cout << "addWord error! word already exist! " << word2 << std::endl;
		this->print();
		std::cout << "Hash value: " << this->getHash() << std::endl;
		exit(1);
	}

	for (int i = 0; i < (int)vertices.size(); i++) {
		if (vertices[i] == word1) vertices[i] = word2;

	}
	vertexSet.erase(word1);
	vertexSet.insert(word2);
}


inline void VertexInducedEmbedding::removeLastWord() {
	if (vertices.size()<=1) {
		vertices.clear();
		vertexSet.clear();
		//edgeSet.clear();
		return;
	}

	int word = vertices.back();
	if (vertexSet.find(word)==vertexSet.end()) {
		std::cout << "problem removeWord! vertex not found!" << std::endl;
		exit(1);
	}
	vertexSet.erase(word);
	vertices.pop_back();
}

inline void VertexInducedEmbedding::removeWord(int word) {
	//std::assert(pos == -1);

	//std::cout << "Remove word " << word << std::endl;
	if (vertices.size()<=1) {
		vertices.clear();
		vertexSet.clear();
		//edgeSet.clear();
		return;
	}

	if (vertexSet.find(word)==vertexSet.end()) {
		std::cout << "problem removeWord! vertex not found!" << std::endl;
		exit(1);
	}
	vertexSet.erase(word);

	bool vertexFound = false;
	std::vector<int>::iterator it = vertices.begin(); 
	while (it!=vertices.end()) {
		//std::cout << *it << " " << word << std::endl;
		if (*it == word) {
			vertexFound = true;
			vertices.erase(it);
			break;
		}
		it++;
	}

	if (!vertexFound) {
		std::cout << "problem removeWord! vertex not found!" << std::endl;
		exit(1);
	}
}

int VertexInducedEmbedding::getTotalNumWords(){
	return graph->getNumberOfNodes();
}

bool VertexInducedEmbedding::hasVertex(int v) {
	std::unordered_set<int>::iterator it = vertexSet.find(v);
	return it!=vertexSet.end();
}

bool VertexInducedEmbedding::hasEdge(int e) {
	std::vector<int> &edges = getEdges();
	for (auto &i : edges) {
		if (e == i) return true;
	}
	return false;
}

bliss::Graph VertexInducedEmbedding::getBlissGraph() {
	std::vector<int> empty_vec;
	return getBlissGraph(empty_vec);
}

bliss::Graph VertexInducedEmbedding::getBlissGraph(std::vector<int> &ls) {
	//creating bliss graph
	//if (code>0) return code;
	//std::cout << "blissCode for: " << *this << std::endl << std::flush;


	//std::cout << "GBLISS CODE" << std::endl;
	bliss::Graph blissGraph;

	if (ls.size() != vertices.size() && !ls.empty()) {
		std::cout << "error: getBlissGraph" << std::endl;
		exit(1);
	}

	int idx=0;
	std::unordered_map<int, int> map;
	for (std::vector<int>::iterator it = vertices.begin(); it != vertices.end(); it++) {
		assert(*it < graph->getNumberOfNodes());
		Node &vertex = graph->getNodeAt(*it);
		if (ls.empty()) blissGraph.add_vertex(vertex.getLabel());
		else blissGraph.add_vertex(ls[idx]);
		std::unordered_map<int, int>::iterator it2 = map.find(*it);
		if (it2 != map.end()) { 
			std::cout << "gbliss vertex problem!" << std::endl;
			exit(1);
		}
		map.insert(std::make_pair(*it, map.size()));
		idx++;
	}

	std::unordered_set<int> edgeSet = computeEdgeSet();
	for (std::unordered_set<int>::iterator it = edgeSet.begin(); it != edgeSet.end(); it++) {
		Edge &edge = graph->getEdgeAt(*it);


		std::unordered_map<int, int>::iterator itv = map.find(edge.getFromNodeId());
		if (itv == map.end()) { 
			std::cout << "gbliss edge problem!" << std::endl;
			exit(1);
		}
		std::unordered_map<int, int>::iterator itu = map.find(edge.getToNodeId());
		if (itu == map.end()) {
			std::cout << "gbliss edge problem!" << std::endl;
			exit(1);
		}
		int v = std::min(itv->second, itu->second);
		int u = std::max(itv->second, itu->second);
		//std::cout << edge;
		//std::cout << "add new edge: " << " " << v << " " << u << std::endl;
		blissGraph.add_edge(v, u);
	}
	return blissGraph;
}

std::vector<int> &VertexInducedEmbedding::getEdges() {
	std::unordered_set<int> edgeSet = computeEdgeSet();

	if (vertices.size()>1 && edgeSet.empty()) {
		std::cout << "error: embedding is not connected!" << std::endl;
		exit(1);
	} 

	/*edges.clear();
	  for (std::unordered_set<int>::iterator it = edgeSet.begin(); it != edgeSet.end(); it++) {
	  edges.push_back(*it);
	  }*/

	edges.assign(edgeSet.begin(), edgeSet.end());
	return edges;
}

int VertexInducedEmbedding::getNumEdges() {
	std::unordered_set<int> edgeSet = computeEdgeSet();
	return edgeSet.size();
}

int VertexInducedEmbedding::getRandomWordBiased() {
	return graph->getRandomNodeBiased();
}

std::unordered_map<int,int> VertexInducedEmbedding::getVertexLabels() {
	std::unordered_map<int,int> ls;
	for (int i = 0; i< (int)vertices.size(); i++) {
		Node &n = graph->getNodeAt(vertices[i]);
		std::unordered_map<int,int>::iterator it = ls.find(n.getLabel());
		if (it == ls.end()) {
			ls.insert(std::make_pair(n.getLabel(), 1));
		}
		else {
			it->second++;
		}
	} 
	return ls;
}

