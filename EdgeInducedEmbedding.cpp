#include "EdgeInducedEmbedding.h"
#include "EmbeddingUtils.h"
#include <graph.hh>
#include <algorithm>


void EdgeInducedEmbedding::reset() {
	BasicEmbedding::reset();
	edges.clear();
	verticesMap.clear();
	edgesSet.clear();
}

EdgeInducedEmbedding::EdgeInducedEmbedding(Graph *g):BasicEmbedding(g) {
	edges.reserve(1000); 
	verticesMap.reserve(1000);
	edgesSet.reserve(1000);
};

EdgeInducedEmbedding::EdgeInducedEmbedding():BasicEmbedding() { 
	edges.reserve(1000); 
	verticesMap.reserve(1000);
	edgesSet.reserve(1000);
};  

void EdgeInducedEmbedding::setFromEmbedding(EdgeInducedEmbedding other) {
}

void EdgeInducedEmbedding::copy(EdgeInducedEmbedding other) {
	graph = other.graph;
	edges = other.getEdges();
	edgesSet = other.getEdgesSet();
	verticesMap = other.getVerticesMap();
}

std::unordered_set<int> &EdgeInducedEmbedding::getEdgesSet()  {
	return edgesSet;
}

std::unordered_map<int,int> &EdgeInducedEmbedding::getVerticesMap() {
	return verticesMap;
}

std::vector<int> &EdgeInducedEmbedding::getVertices() {
	vertices.clear();
	for(auto kv : verticesMap) {
		vertices.push_back(kv.first);
	} 
	return vertices;
}

int EdgeInducedEmbedding::getNumVertices() {
	return verticesMap.size();
}
    
NeighborhoodSet EdgeInducedEmbedding::getWordNeighbors(int id) {
	NeighborhoodSet neighs;
	std::cout << "Function is not implemented!" << std::endl;
	exit(1);
}

NeighborhoodSet &EdgeInducedEmbedding::getValidElementsForExpansion(int vertexId) {
	return graph->getNeighborhoodEdgeOfVertexAt(vertexId);
}

std::unordered_set<int> EdgeInducedEmbedding::getValidElementsForContraction() {
	std::unordered_set<int> contractions;
	std::vector<bool> bridges = EmbeddingUtils::articulation(*this);

	for (int i = 0 ; i < (int)edges.size(); i++)
		if (!bridges[i])
			contractions.insert(edges[i]);	
	return contractions;
}

std::unordered_set<int> EdgeInducedEmbedding::getValidElementsForContractionWithWord(int wordId) {
	std::unordered_set<int> contractions;
	std::vector<bool> bridges = EmbeddingUtils::articulation(*this, wordId);

	for (int i = 0 ; i < (int)edges.size(); i++)
		if (!bridges[i])
			contractions.insert(edges[i]);	
	return contractions;
}

bool EdgeInducedEmbedding::areWordsNeighbours(int wordId1, int wordId2) {
	return graph->isNeighborEdge(wordId1, wordId2);
}

/*std::vector<int> EdgeInducedEmbedding::getValidElementsForExpansion() {
  std::vector<int> expansions;
  if (edges.size() == 0) {
//expansions.insert(-1);	
int vec[] = {-1};
return std::vector<int> (vec, vec + sizeof(vec) / sizeof(int)) ;
}

//std::cout << "extensions size: " << extensionsWords.size() << std::endl;
for (std::unordered_map<int,int>::iterator it = extensionsWords.begin(); it != extensionsWords.end() ; it++) {
if (!hasEdge(it->first)) {
expansions.push_back(it->first);
///std::cout << it->first << "\t";
}
}
//std::cout << std::endl;
return expansions;
}*/

std::unordered_set<int> EdgeInducedEmbedding::getValidElementsForExpansion() {
	std::unordered_set<int> expansions;
	if (edges.size() == 0) {
		//expansions.insert(-1);	
		//int vec[] = {-1};
		//return std::vector<int> (vec, vec + sizeof(vec) / sizeof(int)) ;
		expansions.insert(-1); 
	}
	for (std::unordered_map<int, int>::iterator it = verticesMap.begin(); it!=verticesMap.end(); it++) {
		NeighborhoodSet possibleExp = getValidElementsForExpansion(it->first);
		expansions.insert(possibleExp.begin(), possibleExp.end());
	}
	//remove edges already inserted
	for (int i = 0; i < (int)edges.size(); i++) {
		expansions.erase(edges[i]);
	}

	/*std::vector<int> bla (expansions.begin(), expansions.end());
	  std::vector<int> ble = getValidElementsForExpansion();

	  std::sort(bla.begin(), bla.end());
	  std::sort(ble.begin(), ble.end());
	  for (int i = 0; i < bla.size(); i++) {
	  if (bla[i]!=ble[i]) {
	  std::cout << "BLA != BLE\n";
	  exit(1);
	  }
	  }

	  std::cout << "before : " << bla.size() << " after " << ble.size() << std::endl;
	  return bla; 
	 */
	//return std::vector<int> (expansions.begin(), expansions.end());
	return expansions;
}


/**
 * Add word and update the number of vertices in this embedding.
 *
 * @param word
 */

void EdgeInducedEmbedding::addWord(int word) {
	edges.push_back(word);
	edgesSet.insert(word);

	//update vertices
	Edge e = graph->getEdgeAt(word);
	int v = e.getFromNodeId();
	int u = e.getToNodeId();

	bool hasV = hasVertex(v);
	bool hasU = hasVertex(u);
	if (!hasV) {
		//add vertex
		//vertices.push_back(v);
		verticesMap.insert(std::make_pair(v, 1));

		//update extensions
		/*std::unordered_set<int> possibleExp = getValidElementsForExpansion(v);
		  for (std::unordered_set<int>::iterator it = possibleExp.begin(); it != possibleExp.end() ; it++) {
		  std::unordered_map<int, int>::iterator itExp = extensionsWords.find(*it);
		  if (itExp==extensionsWords.end()) {
		  extensionsWords.insert(std::make_pair(*it, 1));
		  }
		  else {
		  itExp->second+=1;
		  }
		  }*/
	}
	else {
		std::unordered_map<int, int>::iterator itVert = verticesMap.find(v);
		itVert->second+=1;
	}

	if (!hasU) {
		//add vertex
		//vertices.push_back(u);
		verticesMap.insert(std::make_pair(u, 1));

		//update extensions
		/*std::unordered_set<int> possibleExp = getValidElementsForExpansion(u);
		  for (std::unordered_set<int>::iterator it = possibleExp.begin(); it != possibleExp.end() ; it++) {
		  std::unordered_map<int, int>::iterator itExp = extensionsWords.find(*it);
		  if (itExp==extensionsWords.end()) {
		  extensionsWords.insert(std::make_pair(*it, 1));
		  }
		  else {
		  itExp->second+=1;
		  }
		  }*/
	}
	else {
		std::unordered_map<int, int>::iterator itVert = verticesMap.find(u);
		itVert->second+=1;
	}
}

void EdgeInducedEmbedding::replaceWord(int word1, int word2) {
	std::cout << "PROBLEM: replaceWord for edge induced embeddings is not implemented!" << std::endl;
	exit(1);
}

void EdgeInducedEmbedding::removeLastWord() {
	if (edges.size()<=1) {
		edges.clear();
		edgesSet.clear();
		verticesMap.clear();
		return;
	}
	int word = edges.back();
	Edge e = graph->getEdgeAt(word);
	edges.pop_back();
	edgesSet.erase(word);

	int v = e.getFromNodeId();
	int u = e.getToNodeId();

	//confirm that removed edge is the only one with these nodes
	bool hasV = false;
	bool hasU = false;

	std::unordered_map<int, int>::iterator itVert = verticesMap.find(v);
	//if (itVert==verticesMap.end()) {
	//}
	if (itVert->second>1) {
		hasV = true;
		itVert->second--;
	}
	else if (itVert->second == 1) 
		verticesMap.erase(itVert); 
	else {
		std::cout << "problem node not found!" << std::endl;
		exit(1);
	}

	itVert = verticesMap.find(u);
	//if (itVert==verticesMap.end()) {
	//}
	if (itVert->second>1) {
		hasU= true;
		itVert->second--;
	}
	else if (itVert->second == 1) 
		verticesMap.erase(itVert); 
	else {
		std::cout << "problem node not found!" << std::endl;
		exit(1);
	}


	if (!hasV && !hasU) {
		std::cout << "error : an edge can not remove two vertices!\n";
		std::cout << "edge " << word << " vertices " << u << " " << v  <<  "\n";
		this->print();
		exit(1);
	}

}

void EdgeInducedEmbedding::removeWord(int word) {
	//std::assert(pos == -1);

	if (edges.size()<=1) {
		edges.clear();
		edgesSet.clear();
		//vertices.clear();
		verticesMap.clear();
		return;
	}

	//erase edge	
	bool edgeFound = false;
	std::vector<int>::iterator it = edges.begin(); 
	while (it!=edges.end()) {
		//std::cout << *it << " " << word << std::endl;
		if (*it == word) {
			edgeFound = true;
			edges.erase(it);
			edgesSet.erase(word);
			break;
		}
		it++;

	}

	if (!edgeFound) {
		std::cout << "problem edge not found!" << std::endl;
		exit(1);
	}

	Edge e = graph->getEdgeAt(word);
	int v = e.getFromNodeId();
	int u = e.getToNodeId();

	//confirm that removed edge is the only one with these nodes
	bool hasV = false;
	bool hasU = false;
	/*for (int i = 0; i < edges.size(); i++) {
	  Edge remainEdge = graph->getEdgeAt(edges[i]);
	  if (remainEdge.hasVertex(v)) {
	  hasV = true;
	  }
	  if (remainEdge.hasVertex(u)) {
	  hasU = true;
	  }
	  if (hasV && hasU) break;
	  }*/
	std::unordered_map<int, int>::iterator itVert = verticesMap.find(v);
	//if (itVert==verticesMap.end()) {
	//}
	if (itVert->second>1) {
		hasV = true;
		itVert->second--;
	}
	else if (itVert->second == 1) 
		verticesMap.erase(itVert); 
	else {
		std::cout << "problem node not found!" << std::endl;
		exit(1);
	}



	itVert = verticesMap.find(u);
	//if (itVert==verticesMap.end()) {
	//}
	if (itVert->second>1) {
		hasU= true;
		itVert->second--;
	}
	else if (itVert->second == 1) 
		verticesMap.erase(itVert); 
	else {
		std::cout << "problem node not found!" << std::endl;
		exit(1);
	}


	if (!hasV && !hasU) {
		std::cout << "error : an edge can not remove two vertices!\n";
		std::cout << "edge " << word << " vertices " << u << " " << v  <<  "\n";
		this->print();
		exit(1);
	}


	//std::cout << "#### extensions size: " << extensionsWords.size() << std::endl;
	//for (std::unordered_map<int,int>::iterator it = extensionsWords.begin(); it != extensionsWords.end() ; it++) 
	//		std::cout << it->first << ":" << it->second << "\t";
	//std::cout << std::endl;

	//update extensionsWords
	/*if (!hasV) {
	  std::unordered_set<int> possibleExp = getValidElementsForExpansion(v);
	  for (std::unordered_set<int>::iterator it = possibleExp.begin(); it != possibleExp.end() ; it++) {
	  std::unordered_map<int, int>::iterator itExp = extensionsWords.find(*it);
	  if (itExp==extensionsWords.end()) {
	  std::cout << "error! embedding problem! removeWord\n";
	  std::cout << "removed node "<< v << " not found " << *it << "\n";
	  exit(1);
	  }
	  else if (itExp->second>1){
	  itExp->second--;
	  }
	  else {
	//std::cout << "removing extension " << itExp->first << std::endl;
	extensionsWords.erase(itExp);
	}
	}
	}
	if (!hasU) {
	std::unordered_set<int> possibleExp = getValidElementsForExpansion(u);
	for (std::unordered_set<int>::iterator it = possibleExp.begin(); it != possibleExp.end() ; it++) {
	std::unordered_map<int, int>::iterator itExp = extensionsWords.find(*it);
	if (itExp==extensionsWords.end()) {
	std::cout << "error! embedding problem! removeWord";
	std::cout << "removed node "<< u << " not found " << *it << "\n";
	exit(1);
	}
	else if (itExp->second>1){
	itExp->second--;
	}
	else {
	extensionsWords.erase(itExp);
	}
	}
	}*/

	//std::cout << "NEW: ";
	//this->print(); 
}

int EdgeInducedEmbedding::getTotalNumWords(){
	return graph->getNumberOfEdges();
}

bool EdgeInducedEmbedding::hasVertex(int v) {
	/*for (int i = 0; i< vertices.size(); i++) {
	  if (vertices[i] == v) 
	  return true;
	  }*/

	std::unordered_map<int, int>::iterator itVert = verticesMap.find(v);
	return itVert!=verticesMap.end();
}

bool EdgeInducedEmbedding::hasEdge(int e) {
	return edgesSet.find(e) != edgesSet.end();
}

bliss::Graph EdgeInducedEmbedding::getBlissGraph() {
	std::vector<int> empty_vec;
	return getBlissGraph(empty_vec);
}

bliss::Graph EdgeInducedEmbedding::getBlissGraph(std::vector<int> &ls) {
	bliss::Graph blissGraph;

	//std::unordered_map<int,int> &verticesMap = getVerticesMap();
	std::vector<int> &vertices = getVertices();
	std::vector<int> &edges = getEdges();
	
	//if (ls.size() != verticesMap.size() && !ls.empty()) {
	if (ls.size() != vertices.size() && !ls.empty()) {
		std::cout << "error: getBlissGraph" << std::endl;
		exit(1);
	}

	int idx = 0;
	std::unordered_map<int, int> map;
	//for (std::unordered_map<int,int>::iterator itVert = verticesMap.begin(); itVert != verticesMap.end(); itVert++) {
	for (std::vector<int>::iterator itVert = vertices.begin(); itVert != vertices.end(); itVert++) {
		//Node &vertex = graph->getNodeAt(itVert->first);
		Node &vertex = graph->getNodeAt(*itVert);
		if (ls.empty()) blissGraph.add_vertex(vertex.getLabel());
		else blissGraph.add_vertex(ls[idx]);
		//std::unordered_map<int, int>::iterator it = map.find(itVert->first);
		std::unordered_map<int, int>::iterator it = map.find(*itVert);
		if (it != map.end()) {
			std::cout << "gbliss vertex problem!" << std::endl;
			exit(1);
		}
		//map.insert(std::make_pair(itVert->first, map.size()));
		map.insert(std::make_pair(*itVert, map.size()));
		idx++;
	}

	for (int i = 0; i < (int)edges.size(); i++) {
		Edge &edge = graph->getEdgeAt(edges[i]);

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
		//std::cout << "add new edge: " << v << " " << u<< std::endl;
		blissGraph.add_edge(v, u);
	}

	return blissGraph;
}

/*    size_t EdgeInducedEmbedding::getBlissCodeHashValue() {
//creating bliss graph
//this->code.print();

//if (code>0) return code;

//std::cout << "GBLISS CODE" << std::endl;
bliss::Graph blissGraph;

//std::vector<int> vertices = e.getVertices();
std::unordered_map<int,int> verticesMap = getVerticesMap();
std::vector<int> edges = getEdges();

std::unordered_map<int, int> map;

//for (int i = 0; i < vertices.size(); i++) {
for (std::unordered_map<int,int>::iterator itVert = verticesMap.begin(); itVert != verticesMap.end(); itVert++) {
Node vertex = graph->getNodeAt(itVert->first);
blissGraph.add_vertex(vertex.getLabel());
//blissGraph.add_vertex(0);
std::unordered_map<int, int>::iterator it = map.find(itVert->first);
if (it != map.end()) { 
std::cout << "gbliss vertex problem!" << std::endl;
exit(1);
}

map.insert(std::make_pair(itVert->first, map.size()));
}

for (int i = 0; i < (int)edges.size(); i++) {
Edge edge = graph->getEdgeAt(edges[i]);

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
//std::cout << "add new edge: " << v << " " << u<< std::endl;
blissGraph.add_edge(v, u);
}

size_t hash1 = blissGraph.get_hash();
std::unordered_map<size_t, size_t>::iterator it_canonical = Graph::pattern_to_canonical.find(hash1);
if (it_canonical!=Graph::pattern_to_canonical.end()) {
return it_canonical->second;
}

//std::cout << "naive pattern not found!" << std::endl; 
//running canonical form func
bliss::Stats stats;
const unsigned int* labelling = blissGraph.canonical_form(stats, NULL, NULL);
bliss::Graph *blissGraph2 = blissGraph.permute(labelling);

size_t code = blissGraph2->get_hash();
Graph::pattern_to_canonical.insert(std::make_pair(hash1, code) );

delete blissGraph2;

return code;
}*/

std::vector<int> &EdgeInducedEmbedding::getEdges() {
	return edges;
}

std::vector<int> &EdgeInducedEmbedding::getWords() {
	return getEdges();
}

int EdgeInducedEmbedding::getNumEdges() {
	return edges.size();
}

int EdgeInducedEmbedding::getNumWords() {
	return getNumEdges();
}

int EdgeInducedEmbedding::getRandomWordBiased() {
	return graph->getRandomEdgeBiased();
}

int EdgeInducedEmbedding::getLargestNodeDegree() {
	int l = -1;
	/*for (int i = 0; i < (int)vertices.size(); i++) {
		if (l < graph->getDegreeOfNodeAt(vertices[i])){
			l = graph->getDegreeOfNodeAt(vertices[i]);
		}
	}*/
	return l;
}

std::unordered_map<int,int> EdgeInducedEmbedding::getVertexLabels() {
	std::unordered_map<int,int> ls;
	for(auto kv : verticesMap) {
		Node n = graph->getNodeAt(kv.first);
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

