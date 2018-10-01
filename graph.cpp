#include "graph.h"
#include "canonical.h"
#include <set>
#include <iostream>
#include <algorithm>
#include <boost/functional/hash.hpp>

//Graph Functions
//static variable
//std::unordered_map<size_t, size_t> Graph::pattern_to_canonical = std::unordered_map<size_t, size_t> ();

Graph::Graph(): id(0), label(0), numNodeLabels(0), numEdgeLabels(0), largestDegree(-1) {
    nodes = std::vector<Node> ();
    edges = std::vector<Edge> ();
    //neighborhood = std::vector<NeighborhoodSet>();
    //neighborhoodEdge = std::vector<NeighborhoodSet>();
}

Graph Graph::init(){
	return Graph();
}

int Graph::getId() const{
    return (this->id);
}	

void Graph::setId(int id){
    this->id = id;
}

int Graph::getLabel() const{
    return (this->label);
}

void Graph::setLabel(int label){
    this->label = label;
}

int Graph::getType() const{
    return (this->type);
}

void Graph::setType(int type){
    this->type = type;
}

int Graph::getNumberOfNodes() const {
    return (int)nodes.size(); 
}

int Graph::getNumberOfEdges() const {
    return (int)edges.size(); 
}

int Graph::getNumberOfNodeLabels() const {
    return numNodeLabels; 
}

void Graph::setNumberOfNodeLabels(int n){
    this->numNodeLabels = n;
}

int Graph::getNumberOfEdgeLabels() const {
    return numEdgeLabels; 
}

void Graph::setNumberOfEdgeLabels(int n){
    this->numEdgeLabels = n;
}

std::vector<Node> &Graph::getNodes(){
    return (this->nodes);
}

std::vector<Edge> &Graph::getEdges(){
    return (this->edges);
}

Node &Graph::getNodeAt(int i) {
    return nodes[i];
}

Edge &Graph::getEdgeAt(int i){
    return edges[i];
}

std::pair<int,bool> Graph::getEdgeIdFromPair(int i, int j){
    std::unordered_map<std::pair<int, int>, int, boost::hash<std::pair<int, int>>>::iterator it = nodes2edge.find(std::make_pair(i, j));
    return std::pair<int,bool> (it->second,it!=nodes2edge.end());	
}
	
int Graph::getDegreeOfNodeAt(int i) {
	return neighborhood[i].size();
}

int Graph::getLargestNodeDegree() {
	if (largestDegree<0) {
		std::cout << "Computing largest node degree of the graph" << std::endl;
		for (std::vector<NeighborhoodSet>::iterator it = neighborhood.begin(); it != neighborhood.end(); it++) {
			if (largestDegree < (int) it->size())
				largestDegree = (int) it->size();
		}
	}
	return largestDegree;
}
	
NeighborhoodSet &Graph::getNeighborhoodVertexOfVertexAt(int id){
	return neighborhood[id];
}

std::vector<int> &Graph::getNeighborhoodIdxVertexOfVertexAt(int id){
	return neighborhoodIdx[id];
}


NeighborhoodSet &Graph::getNeighborhoodEdgeOfVertexAt(int id) {
	return neighborhoodEdge[id];
}

/*NeighborhoodSet Graph::getNeighborhoodEdgeOfEdgeAt(int id) {
	NeighborhoodSet neighs;
	Edge e = getEdgeAt(id);		
    
	neighs.insert(neighborhoodEdge[e.getFromNodeId()].begin(), neighborhoodEdge[e.getFromNodeId()].end());
	neighs.insert(neighborhoodEdge[e.getToNodeId()].begin(), neighborhoodEdge[e.getToNodeId()].end());

	neighs.erase(id);	
	return neighs;
}

NeighborhoodSet Graph::getNeighborhoodVertexOfEdgeAt(int id) {
	NeighborhoodSet neighs;
	Edge e = getEdgeAt(id);		

	neighs.insert(e.getFromNodeId());
	neighs.insert(e.getToNodeId());

	return neighs;
}*/

void Graph::insertNode(Node node){
    nodes.push_back(node);
}

void Graph::insertEdge(Edge e){
    std::pair<int, int> pedge(e.getFromNodeId(), e.getToNodeId()); 

    std::unordered_map<std::pair<int, int>, int, boost::hash<std::pair<int, int>>>::iterator ret;
    ret = nodes2edge.find(pedge);
    if (ret!=nodes2edge.end())
	return;
   
    nodes2edge.insert(std::make_pair(pedge, edges.size())) ;
    edges.push_back(e);
}

bool Graph::isNeighborEdge(int i, int j) {
    Edge &e1 = getEdgeAt(i);
    Edge &e2 = getEdgeAt(j);

    if (e1.getFromNodeId() ==  e2.getFromNodeId() ||
    	e1.getFromNodeId() == e2.getToNodeId() ||
    	e1.getToNodeId() == e2.getFromNodeId() ||
    	e1.getToNodeId() == e2.getToNodeId())
	return true;

    return false;
}

/*void Graph::removeDuplicatedEdges(){

    std::unordered_set<std::pair<int, int>, boost::hash<std::pair<int, int>>> u_edges;
    std::unordered_set<std::pair<int, int>, boost::hash<std::pair<int, int>>>::iterator ret;
    //std::pair <std::unordered_set<std::pair<int, int>, boost::hash<std::pair<int, int>>>::iterator, bool> ret;

    std::vector<Edge>::iterator it = edges.begin();
    int i = 0;
    while (it != edges.end())
    {
	std::pair<int, int> e(it->getFromNodeId(), it->getToNodeId()); 
	ret = u_edges.find(e);

	if (ret!=u_edges.end())
		it = edges.erase(it);
	else {
		u_edges.insert(e);
		it++;
	}
	i++;
	if (i%1000==0) std::cout << "edges number " << i << std::endl;
    }
}*/

//TODO fix for directed edges
void Graph::createNeighborhoodIndex(){
    neighborhood.resize(nodes.size(), NeighborhoodSet());
    neighborhoodIdx.resize(nodes.size(), std::vector<int>());

    //for (int i = 0; i < (int) neighborhood.size(); i++)
	//neighborhood[i].set_empty_key(-1);

    for (int j = 0; j < (int) edges.size(); j++) 
    {
	int from = edges[j].getFromNodeId();
	int to = edges[j].getToNodeId();
	if (from < 0 || from >= (int) neighborhood.size()) {
		std::cout << "invalid node from idx " << from << std::endl;
		std::cout << "neighborhood size " << (int)neighborhood.size();
		exit(1);
	}
	if (to < 0 || to >= (int) neighborhood.size()) {
		std::cout << "invalid node to idx " << to << std::endl;
		std::cout << "neighborhood size " << (int)neighborhood.size() << std::endl;
		exit(1);
	}
        neighborhood[to].insert(from);
        neighborhood[from].insert(to);
        neighborhoodIdx[to].push_back(from);
        neighborhoodIdx[from].push_back(to);
    }
}

//TODO fix for directed edges
void Graph::createNeighborhoodEdgeIndex(){
    neighborhoodEdge.resize(nodes.size(), NeighborhoodSet());
    //for (int i = 0; i < (int) neighborhoodEdge.size(); i++)
	//neighborhoodEdge[i].set_empty_key(-1);

    for (int j = 0; j < (int)edges.size(); j++) 
    {
        neighborhoodEdge[edges[j].getToNodeId()].insert(j);
        neighborhoodEdge[edges[j].getFromNodeId()].insert(j);
        //neighborhood.push_back(neighSet);
    }
}

/*
void Graph::createEdgeIndexSortedById(){
    int currentIdx = 0;
    int currentNodeId = 0;

    sort(edges.begin(), edges.end(), Edge::compareFromNodeId);
    currentNodeId = edges.front().fromNodeId;

    nodes[currentNodeId].idxEdgeBegin = currentIdx;	
    for (std::vector<Edge>::iterator iter=edges.begin(); iter!=edges.end(); iter++) {
        if (currentNodeId != iter->fromNodeId) {
            nodes[currentNodeId].idxEdgeEnd = currentIdx-1;	
            currentNodeId = iter->fromNodeId;
            nodes[currentNodeId].idxEdgeBegin = currentIdx;	
        }
        currentIdx++;
    }
    nodes[currentNodeId].idxEdgeEnd = currentIdx-1;	

    for (std::vector<Node>::iterator iter=nodes.begin(); iter!=nodes.end(); iter++) {
        //std::cout<<"sorting node: " << std::endl;
        //iter->print();
        if (iter->getDegree()!=0)
            sort(edges.begin()+iter->idxEdgeBegin, edges.begin()+iter->idxEdgeEnd, Edge::compareToNodeId);
    }
}*/

void Graph::print(){
    std::cout << "Graph ID " << this->id << " Label " << label << std::endl;
    std::cout << "---Data---"<< std::endl;
    for (std::vector<Node>::iterator iter=nodes.begin(); iter!=nodes.end(); iter++)
        iter->print();
    for (std::vector<Edge>::iterator iter=edges.begin(); iter!=edges.end(); iter++)
        iter->print();

}

void Graph::printToFile(const std::string output){
    std::ofstream ofs (output, std::ofstream::out);
   
    ofs << "t # " << getId() << std::endl; 
    for (std::vector<Node>::iterator iter=nodes.begin(); iter!=nodes.end(); iter++){
        ofs << "v " << iter->getId() << " " << iter->getLabel() << std::endl;
    }
    for (std::vector<Edge>::iterator iter=edges.begin(); iter!=edges.end(); iter++){
        ofs << "e " << iter->getFromNodeId() << " " << iter->getToNodeId() << " " << iter->getLabel() << std::endl;
    }

    ofs.close();
}

void Graph::printToFileArabesque(const std::string output){
    std::ofstream ofs (output, std::ofstream::out);
   
    for (std::vector<Node>::iterator iter=nodes.begin(); iter!=nodes.end(); iter++){
        ofs << iter->getId() << " " << iter->getLabel() << " ";
	int id = iter->getId();
    	for (NeighborhoodSet::iterator it = neighborhood[id].begin(); it != neighborhood[id].end(); it++) {
		ofs << *it << " ";
	}
	ofs << std::endl;	
    }
    ofs.close();
}

void Graph::printResume(){
    std::cout << "@Graph ID " << this->id << " Label " << label << std::endl;
    std::cout << "#nodes " << this->getNumberOfNodes() << " #edges " << this->getNumberOfEdges() << std::endl;
    std::cout << "#node's labels " << this->getNumberOfNodeLabels() << " #edge's labels " << this->getNumberOfEdgeLabels() << std::endl;
}


Graph Graph::swapNodeLabelsRandom(double p, int vl){
    Graph newGraph(*this);

    if (vl < 2) return newGraph;
    if (p > 1) p = 1;

    std::vector<Node> nodes = newGraph.getNodes();
    int numberOfNodes=newGraph.getNumberOfNodes();
    int currentLabel = 0, newLabel = 0, pos = 0;

    //renaming labels
    int numberOfLabelsRenamed=numberOfNodes*p;

    std::set<int> positions;
    while (numberOfLabelsRenamed != (int)positions.size()){
        pos = (nodes.size()-0.001)*drand48();
        positions.insert(pos);
    }
    for (std::set<int>::iterator it=positions.begin(); it !=positions.end(); it++){
        currentLabel = newLabel = nodes[*it].getLabel();
        while (newLabel==currentLabel){
            newLabel = (vl-0.001)*drand48();
        }
        nodes[*it].setLabel(newLabel);
    }		
    return newGraph;
}

Graph Graph::removeEdgesRandom(double p){
    Graph newGraph(*this);
    if (p > 1) p = 1;

    //get the number of edges
    int numberOfEdges=newGraph.getNumberOfEdges();
    // percent of edges that will be changed
    int numberOfEdgesRemoved = p*numberOfEdges;
    std::vector<Edge> edges = newGraph.getEdges();
    // deleting edges
    while (numberOfEdgesRemoved!=0){
        int pos = (edges.size()-0.001)*drand48();
        edges.erase(edges.begin()+pos);	
        numberOfEdgesRemoved--;
    }
    //newGraph.createEdgeIndexSortedById();
    
    //neighborhood.clear();
    newGraph.createNeighborhoodIndex();
    newGraph.createNeighborhoodEdgeIndex();
    return newGraph;
}

void Graph::modifyByAddingNodes(int n, double p) {
	IntIntMap nlabel2qtd = getNodeLabelDistribution();
        
	for (int i = 0; i < n; i++) {
		int l = (nlabel2qtd.size()-0.001)*drand48();
		//adding edges
		for (int j = 0; j < (int) nodes.size(); j++) {
			if (drand48() < p) {
				insertEdge(Edge(0, j, nodes.size(), 0));
			}
		}
		insertNode(Node(nodes.size(),l));
	}
}

std::vector<std::list<int>> Graph::getPossibleMatches(Graph &g) {
    std::vector<std::list<int>> domains(this->getNumberOfNodes());

    for (int i=0; i < g.getNumberOfNodes(); i++) {
        for (int j=0; j < this->getNumberOfNodes(); j++) {
            if (Graph::nodeMatches(g, i, *this, j)) {
                domains[j].push_back(i);
            }
        }
    }
    return domains;
}


//TODO FIX
bool Graph::nodeMatches(Graph &g, int idxG, Graph &q, int idxQ) {
    int count=0;
    Node &nodeQ = q.getNodeAt(idxQ);
    Node &nodeG = g.getNodeAt(idxG);

    //nodeG.print();
    //nodeQ.print();

    if (!Node::compareLabelEqual(nodeG,nodeQ)) 
        return false;

    if (q.getDegreeOfNodeAt(nodeQ.getId())>g.getDegreeOfNodeAt(nodeG.getId()))
        return false;


    for (NeighborhoodSet::iterator it = q.neighborhood[idxQ].begin(); it != q.neighborhood[idxQ].end(); it++) {
    	for (NeighborhoodSet::iterator it2 = g.neighborhood[idxG].begin(); it2 != g.neighborhood[idxG].end(); it2++) {
            if (Node::compareLabelEqual(g.getNodeAt(*it), q.getNodeAt(*it2))) {
                count++;
                break;
            }
        }
    }
    return (count==q.getDegreeOfNodeAt(nodeQ.getId()));
}


bool Graph::nodePartialMatch(Graph &g, Graph &q, std::vector<int> &match, int idx) {
    Node &nodeQ = q.getNodeAt(match.size());
    Node &nodeG = g.getNodeAt(idx);

    if (!Node::compareLabelEqual(nodeQ, nodeG))
        return false;

    for (NeighborhoodSet::iterator it = q.neighborhood[nodeQ.getId()].begin(); it != q.neighborhood[nodeQ.getId()].end(); it++) {
        if ((*it < (int)match.size()) && !g.isNeighbor(idx, match[*it])) {
            return false;
        }
    }

    return true;
}


bool Graph::subgraphMatching(Graph &q, Graph &g, std::vector<int> &match) {

    if ((int)match.size() == q.getNumberOfNodes()) {
        return true;
    }	

    //Node startNodeQ = q.getNodeAt(match.size());
    //find nodes to test in the position numberOfMatches
    /*int neighborNodeId;
      for (int i=startNodeQ.idxEdgeBegin; i<=startNodeQ.idxEdgeEnd; i++) {
      Edge edge = q.getEdgeAt(i);
      if (edge.getToNodeId()<match.size()) {
      neighborNodeId = edge.getToNodeId();
      break;
      }
      }*/

    //go to the candidates
    //Node neighborNode = g.getNodeAt(match[neighborNodeId]);
    //for (int i=neighborNode.idxEdgeBegin; i<=neighborNode.idxEdgeEnd; i++) {

    for (int i=0; i<g.getNumberOfNodes(); i++) {
        Node &node = g.getNodeAt(i);
        if (Graph::nodePartialMatch(g, q, match, node.getId())) {
            match.push_back(node.getId());
            if (Graph::subgraphMatching(q, g, match)) return true;
            match.pop_back();
        }
    }

    //for (int i=0; i<match.size();i++) {
    //	std::cout <<  "match[" << i << "] " << match[i] << " ";
    //}
    //std::cout << " false\n ";

    return false;

}

std::vector<int> Graph::bfsOrder(int idx) {

    std::deque<int> q;
    std::vector<bool> checked(this->getNumberOfNodes(), false);
    std::vector<int> bfs(this->getNumberOfNodes(), -1);

    q.push_back(idx);
    checked[idx]=true;
    bfs[0]=idx;
    int numberOfChecked=1;

    while (!q.empty()) {
        Node &node = this->getNodeAt(q.front());
    	for (NeighborhoodSet::iterator it = neighborhood[node.getId()].begin(); it != neighborhood[node.getId()].end(); it++) {
            if (!checked[*it]) {
                q.push_back(*it);
                checked[*it]=true;
                bfs[numberOfChecked]=*it;
                numberOfChecked++;
            }
        }
        q.pop_front();
    }

    return bfs;
}


bool Graph::isConnected() {
	std::vector<int> order = bfsOrder(0);
	for (int i = 0; i < getNumberOfNodes(); i++)
		if (order[i] == -1) return false;

	return true;
}

void Graph::printStatistics() {
	double d=0;
	double avg=0;

	for (std::vector<Node>::iterator iter=nodes.begin(); iter!=nodes.end(); iter++) {
		d+=getDegreeOfNodeAt(iter->getId());
	}
	d/=nodes.size();
	std::cout << "density: " << d << std::endl;
	std::cout << "avg-degree: " << avg << std::endl;
}

bliss::Graph Graph::getBlissGraph() {
	//creating bliss graph
	//this->code.print();
	
	//if (code>0) return code;
		
	//std::cout << "GBLISS CODE" << std::endl;
	bliss::Graph blissGraph;

	for (int i = 0; i < (int)nodes.size(); i++) {
		Node &vertex = getNodeAt(i);
		blissGraph.add_vertex(vertex.getLabel());
	}

	for (int i = 0; i < (int)edges.size(); i++) {
		Edge &edge = getEdgeAt(i);
		blissGraph.add_edge(edge.getFromNodeId(), edge.getToNodeId());
	}

	return blissGraph;
}

size_t Graph::getBlissCodeHashValue() {
	bliss::Graph bg = this->getBlissGraph();
	return Canonical::getHash(bg);
}

/*size_t Graph::getBlissCodeHashValue() {
	//creating bliss graph
	//this->code.print();
	
	//if (code>0) return code;
		
	//std::cout << "GBLISS CODE" << std::endl;
	bliss::Graph blissGraph;

	for (int i = 0; i < (int)nodes.size(); i++) {
		Node vertex = getNodeAt(i);
		blissGraph.add_vertex(vertex.getLabel());
	}

	for (int i = 0; i < (int)edges.size(); i++) {
		Edge edge = getEdgeAt(i);
		blissGraph.add_edge(edge.getFromNodeId(), edge.getToNodeId());
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


int Graph::getRandomNodeBiased() {
	int node = -1;
	int total = 0;
	for (int i = 0; i < (int)nodes.size(); i++) {
		double prob = Randness::instance().random_uni01();
        	if (prob <= (double) getDegreeOfNodeAt(i)/(total+getDegreeOfNodeAt(i))) {
			node = i; 
		}
		total+=getDegreeOfNodeAt(i);
	}
	return node;
}

int Graph::getRandomEdgeBiased() {
	int edge = -1;
	int total = 0;
	for (int i = 0; i < (int)edges.size(); i++) {
		Edge &e = getEdgeAt(i);
		double prob = Randness::instance().random_uni01();
		int w = getDegreeOfNodeAt(e.getFromNodeId()) + getDegreeOfNodeAt(e.getToNodeId());
        	if (prob <= (double) w/(total+w)) {
			edge = i; 
		}
		total+=w;
	}
	return edge;
}
		
IntIntMap Graph::getNodeLabelDistribution() {
	IntIntMap l2qtd;
	for (int i = 0; i < (int)nodes.size(); i++) {
		IntIntMap::iterator it = l2qtd.find(nodes[i].getLabel());
		if (it == l2qtd.end()) {
			l2qtd[nodes[i].getLabel()]=1;		
		}
		else {
			it->second++;
		}
	}
	return l2qtd;
}

IntIntMap Graph::getEdgeLabelDistribution() {
        IntIntMap l2qtd;
        for (int i = 0; i < (int)edges.size(); i++) {
                IntIntMap::iterator it = l2qtd.find(edges[i].getLabel());
                if (it == l2qtd.end()) {
                        l2qtd[nodes[i].getLabel()]=1;
                }
                else {
                        it->second++;
                }
        }
        return l2qtd;
}

void Graph::randomizeNodes() {
	std::random_shuffle( nodes.begin(), nodes.end() );
}

std::vector<int> Graph::getPathBetweenNodes(int id1, int id2, int k) {

	std::unordered_set<int> reach;
	std::stack<std::pair<int, int>> q;
	reach.insert(id1);
	q.push(std::pair<int, int> (id1,0));
	std::cout << "add "<< id1 << std::endl;


	while (!q.empty()) {
		std::pair<int,int> top  = q.top();
		std::vector<int> &neighs = getNeighborhoodIdxVertexOfVertexAt(top.first);
		
		//std::cout << "top first " << top.first << " second " << neighs[top.second] << " idx " << top.second << std::endl;
		
		//queue is full
		if ((int) q.size() == k) {
			reach.erase(top.first);
			q.pop();
			//std::cout << "remove full "<< top.first << std::endl;
		}
		//neighbors have finished
		else if (top.second >= (int) neighs.size()) {
			reach.erase(top.first);
			q.pop();
			//std::cout << "remove no-neigh " << top.first << std::endl;
		}
		//add new neighbor
		else {
			q.top().second++;
			if (reach.find(neighs[top.second]) == reach.end()) {
				reach.insert(neighs[top.second]);
				q.push(std::pair<int, int> (neighs[top.second],0));
				//std::cout << "add "<< neighs[top.second] << std::endl;
				
				// found a valid path
				if (neighs[top.second] == id2) {
					//std::cout << "valid path found" << std::endl;
					break;
				}
			}
		}

	}

	std::vector<int> path;
	//path was found
	if (!q.empty()) {
		while (!q.empty()) {
			path.push_back(q.top().first);
			q.pop();
		}
	}

	return path; 
}
