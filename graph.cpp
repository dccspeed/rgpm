#include "graph.h"
#include "canonical.h"
#include <set>
#include <iostream>
#include <algorithm>
#include <boost/functional/hash.hpp>

//Graph Functions
//static variable
//std::unordered_map<size_t, size_t> Graph::pattern_to_canonical = std::unordered_map<size_t, size_t> ();

Graph::Graph(): id(0), label(0), numNodeLabels(0), numEdgeLabels(0), largestDegree(-1), numNodesIdx(0), numEdgesIdx(0) {
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

uint Graph::getNumberOfNodes() const {
    return nodes.size(); 
}

uint Graph::getNumberOfEdges() const {
    return edges.size(); 
}

uint Graph::getNumberOfNodeLabels() const {
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

void Graph::setNodes(std::vector<Node> &nodes){
    this->nodes=nodes;
}

std::vector<Edge> &Graph::getEdges(){
    return (this->edges);
}

void Graph::setEdges(std::vector<Edge> &edges){
    this->edges = edges;
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

int Graph::getWeightOfNodeAt(int i) {
	double w = 0;
	for (int e : neighborhoodEdge[i]) {
		w+=edges[e].getWeight();
	}
	return w;
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

std::vector<int> &Graph::getNeighborhoodIdxEdgeOfVertexAt(int id){
	return neighborhoodIdxEdge[id];
}

NeighborhoodSet Graph::getKNeighborhoodVertexOfVertexAt(int id, int k){
	NeighborhoodSet kneighborhood;
	std::queue<std::pair<int,int>> reachQueue;

	reachQueue.push(std::pair<int, int> (id,0));
	while (!reachQueue.empty()) {
	        std::pair<int,int> reachNode = reachQueue.front();
		reachQueue.pop();

		NeighborhoodSet &neigh = getNeighborhoodVertexOfVertexAt(reachNode.first);
		for (int i : neigh) {
			if (kneighborhood.find(id)==kneighborhood.end() && reachNode.second<k) {
				reachQueue.push(std::pair<int, int> (i,reachNode.second+1));
				kneighborhood.insert(i);
			}
		}
	}
	return kneighborhood;

}

NeighborhoodSet Graph::getKNeighborhoodVertexOfVertexAt(int id, int k, std::unordered_set<int> &l){
        NeighborhoodSet kneighborhood;
        std::queue<std::pair<int,int>> reachQueue;

        reachQueue.push(std::pair<int, int> (id,0));
        while (!reachQueue.empty()) {
                std::pair<int,int> reachNode = reachQueue.front();
                reachQueue.pop();

                NeighborhoodSet &neigh = getNeighborhoodVertexOfVertexAt(reachNode.first);
                for (int i : neigh) {
			if (l.find(i)==l.end()) continue;
                        if (kneighborhood.find(id)==kneighborhood.end() && reachNode.second<k) {
                                reachQueue.push(std::pair<int, int> (i,reachNode.second+1));
                                kneighborhood.insert(i);
                        }
                }
        }
        return kneighborhood;

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

bool Graph::insertEdge(Edge e){
    //std::cout << "try to insert edge " << e << std::endl;
    std::pair<int, int> pedge(e.getFromNodeId(), e.getToNodeId()); 

    std::unordered_map<std::pair<int, int>, int, boost::hash<std::pair<int, int>>>::iterator ret;
    ret = nodes2edge.find(pedge);
    if (ret!=nodes2edge.end())
	return false;
   
    //std::cout << "inserting edge " << e << std::endl;
    
    nodes2edge.insert(std::make_pair(pedge, edges.size())) ;
    edges.push_back(e);
    return true;
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
void Graph::createNeighborhoodIndex() {
   
    if (numNodesIdx==nodes.size() && numEdgesIdx==edges.size()) return;

    std::cout << "creating or updating graph idx" << std::endl;
    neighborhood.resize(nodes.size(), NeighborhoodSet());
    neighborhoodIdx.resize(nodes.size(), std::vector<int>());
    neighborhoodEdge.resize(nodes.size(), NeighborhoodSet());
    dirty_nodes.resize(nodes.size(), false);

    //for (int i = 0; i < (int) neighborhood.size(); i++)
	//neighborhood[i].set_empty_key(-1);

    //update only the remaing edges
    for (uint j = numEdgesIdx; j < edges.size(); j++) {
	int from = edges[j].getFromNodeId();
	int to = edges[j].getToNodeId();
	if (from < 0 || from >= (int) neighborhood.size()) {
		std::cout << "invalid node from idx " << from << std::endl;
		std::cout << "neighborhood size " << neighborhood.size();
		exit(1);
	}
	if (to < 0 || to >= (int) neighborhood.size()) {
		std::cout << "invalid node to idx " << to << std::endl;
		std::cout << "neighborhood size " << neighborhood.size() << std::endl;
		exit(1);
	}
        neighborhood[to].insert(from);
        neighborhood[from].insert(to);
        neighborhoodIdx[to].push_back(from);
        neighborhoodIdx[from].push_back(to);
        neighborhoodEdge[to].insert(j);
        neighborhoodEdge[from].insert(j);
	//std::cout << "new idx edge " << j << " " << edges[j] <<  std::endl;
	
	//set dirty
	dirty_nodes.set(to, true);
	dirty_nodes.set(from, true);
    }

    // sort indexes
    size_t i = dirty_nodes.find_first();
    while (i!=boost::dynamic_bitset<>::npos) {
	std::sort(neighborhoodIdx[i].begin(), neighborhoodIdx[i].end());
    	dirty_nodes.set(i, false);
	//std::cout << i << " " << std::endl;
        i = dirty_nodes.find_next(i);
    }

    numNodesIdx = nodes.size();
    numEdgesIdx = edges.size();
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

void Graph::print() {
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
   
    for (std::vector<Node>::iterator iter=nodes.begin(); iter!=nodes.end(); iter++) {
        ofs << iter->getId() << " " << iter->getLabel() << " ";
	int id = iter->getId();
    	for (NeighborhoodSet::iterator it = neighborhood[id].begin(); it != neighborhood[id].end(); it++) {
		ofs << *it << " ";
	}
	ofs << std::endl;	
    }
    ofs.close();
}

void Graph::printResume() {
    std::cout << "@Graph ID " << this->id << " Label " << label << std::endl;
    std::cout << "#nodes " << this->getNumberOfNodes() << " #edges " << this->getNumberOfEdges() << std::endl;
    std::cout << "#node's labels " << this->getNumberOfNodeLabels() << " #edge's labels " << this->getNumberOfEdgeLabels() << std::endl;
}


Graph Graph::swapNodeLabelsRandom(double p, int vl) {
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
	pos = Randness::instance().get_a_random_number(0,nodes.size());
        positions.insert(pos);
    }
    for (std::set<int>::iterator it=positions.begin(); it !=positions.end(); it++){
        currentLabel = newLabel = nodes[*it].getLabel();
        while (newLabel==currentLabel){
            newLabel = Randness::instance().get_a_random_number(0,vl);
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
	int pos = Randness::instance().get_a_random_number(0,edges.size());
        edges.erase(edges.begin()+pos);	
        numberOfEdgesRemoved--;
    }
    //newGraph.createEdgeIndexSortedById();
    
    //neighborhood.clear();
    newGraph.createNeighborhoodIndex();
    return newGraph;
}

void Graph::modifyByAddingNodes(int n, double p) {
	IntIntMap nlabel2qtd = getNodeLabelDistribution();
        
	for (int i = 0; i < n; i++) {
		int l = Randness::instance().get_a_random_number(0,nlabel2qtd.size());
		//adding edges
		for (int j = 0; j < (int) nodes.size(); j++) {
			if (Randness::instance().random_uni01() < p) {
				insertEdge(Edge(0, j, nodes.size(), 0));
			}
		}
		insertNode(Node(nodes.size(),l));
	}
}

std::vector<std::list<int>> Graph::getPossibleMatches(Graph &g) {
    std::vector<std::list<int>> domains(this->getNumberOfNodes());

    for (uint i=0; i < g.getNumberOfNodes(); i++) {
        for (uint j=0; j < this->getNumberOfNodes(); j++) {
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

    if (match.size() == q.getNumberOfNodes()) {
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

    for (uint i=0; i<g.getNumberOfNodes(); i++) {
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


// A recursive function that uses visited[] and parent to detect 
// cycle in subgraph reachable from vertex v. 
bool Graph::isCyclicUtil(int v, std::vector<bool> &visited, int parent, std::unordered_set<int> &l) 
{ 
    // Mark the current node as visited 
    visited[v] = true; 
  
    // Recur for all the vertices adjacent to this vertex 
    std::vector<int> neighs = getNeighborhoodIdxVertexOfVertexAt(v);
    for (int i : neighs) {
	if (l.find(i) == l.end()) continue;

        // If an adjacent is not visited, then recur for that adjacent 
        if (!visited[i]) 
        { 
           if (isCyclicUtil(i, visited, v, l)) 
              return true; 
        } 
  
        // If an adjacent is visited and not parent of current vertex, 
        // then there is a cycle. 
        else if (i != parent) 
           return true; 
    } 
    return false; 
} 
  
// Returns true if the graph contains a cycle, else false. 
bool Graph::isCyclic(std::unordered_set<int> &l) 
{ 
    // Mark all the vertices as not visited and not part of recursion 
    // stack 
    std::vector<bool> visited(getNumberOfNodes(), false); 

    for (int i : l) 
        visited[i] = false; 
  
    // Call the recursive helper function to detect cycle in different 
    // DFS trees 
    for (int u : l) 
        if (!visited[u]) // Don't recur for u if it is already visited 
          if (isCyclicUtil(u, visited, -1, l)) 
             return true; 
  
    return false; 
} 

std::vector<int> &Graph::getNodesDegrees() {
	degrees.resize(getNumberOfNodes(), 0);

	for (int i = 0; i < getNumberOfNodes(); i++) 
		degrees[i] = getDegreeOfNodeAt(i);

	return degrees;
}

std::vector<int> &Graph::getNodesCoreness() {
	coreness.resize(getNumberOfNodes(), 0);
	std::vector<std::unordered_set<int>> D(getLargestNodeDegree()+1, std::unordered_set<int> ());
	std::vector<int> d(getNumberOfNodes(), 0);

	for (int i = 0; i < getNumberOfNodes(); i++) {
		//std::cout << "bulding structs for degrees using node: " << i << std::endl;
		d[i] = getDegreeOfNodeAt(i);
		D[getDegreeOfNodeAt(i)].insert(i);
	}

	int largestCore = 0;
	for (int k = 0; k <= getLargestNodeDegree(); k++) {
		//std::cout << "getting nodes with degree: " << k << std::endl;
		while (!D[k].empty()) {
			int i = *(D[k].begin());
			//std::cout << "=checking node " << i << std::endl;
			D[k].erase(i);
			coreness[i] = k;
			if (largestCore < k) largestCore = k;
			std::vector<int> neighs = getNeighborhoodIdxVertexOfVertexAt(i);
			for (int j : neighs) {
				if (d[j] > k) {
					D[d[j]].erase(j);
					D[d[j]-1].insert(j);
					d[j]--;
				}
			}
		}
	}

	return coreness;
}

std::vector<int> &Graph::getNodesLayers() {
	layer.resize(getNumberOfNodes(), 1);
	coreness.resize(getNumberOfNodes(), 1);
	std::vector<int> d(getNumberOfNodes(), 0);

	for (int i = 0; i < getNumberOfNodes(); i++) {
		//std::cout << "bulding structs for degrees using node: " << i << std::endl;
		d[i] = getDegreeOfNodeAt(i);
	}

	int core = 1, lay = 1;
	boost::dynamic_bitset<> covered(getNumberOfNodes(), false);
	covered.set();
	//std::cout << "init covered count " << covered.count() <<  " size " << covered.size() << std::endl;
	while (!covered.none()){
		//std::cout << "starting over!" << std::endl;
		size_t i = covered.find_first();
		while (i!=boost::dynamic_bitset<>::npos) {
			//std::cout << "checking node " << i << std::endl;
			if (d[i]<=core) {
				//std::cout << "node " << i << " has core <= " << core << std::endl;
				layer[i] = lay;
				coreness[i] = core;
				std::vector<int> &neigh = getNeighborhoodIdxVertexOfVertexAt(i);
				for (uint j : neigh) {
					if (covered.test(j)) d[j]--;
				}
				covered.set(i, false);
			}	
			i = covered.find_next(i);
		}
		lay++;
		int mindegree = getNumberOfNodes();
		i = covered.find_first();
		while (i!=boost::dynamic_bitset<>::npos) {
			if (mindegree>d[i]) mindegree = d[i];
			i = covered.find_next(i);
		}
		if (mindegree >= core+1) core = mindegree;
		//std::cout << "covered count " << covered.count() << std::endl;
	}
	
	//for (int i = 0; i < getNumberOfNodes(); i++) {
	//	std::cout << "node: " << i << " layer: " << layer[i] << std::endl;
	//}
	return layer;
}

// A recursive function to print DFS starting from v. 
// It returns true if degree of v after processing is less 
// than k else false 
// It also updates degree of adjacent if degree of v 
// is less than k.  And if degree of a processed adjacent 
// becomes less than k, then it reduces of degree of v also, 
bool Graph::kcoreDFSUtil(int v, std::vector<bool> &visited, 
                    std::vector<int> &vDegree, int k) 
{ 
    // Mark the current node as visited and print it 
    visited[v] = true; 
  
    // Recur for all the vertices adjacent to this vertex 
    std::vector<int> neighs = getNeighborhoodIdxVertexOfVertexAt(v); 
    for (int i : neighs) 
    { 
        // degree of v is less than k, then degree of adjacent 
        // must be reduced 
        if (vDegree[v] < k) 
            vDegree[i]--; 
  
        // If adjacent is not processed, process it 
        if (!visited[i]) 
        { 
            // If degree of adjacent after processing becomes 
            // less than k, then reduce degree of v also. 
            if (kcoreDFSUtil(i, visited, vDegree, k)) 
                vDegree[v]--; 
        } 
    } 
  
    // Return true if degree of v is less than k 
    return (vDegree[v] < k); 
} 

// Prints k cores of an undirected graph 
std::vector<int> Graph::computeKCore(int k) 
{ 
    // INITIALIZATION 
    // Mark all the vertices as not visited and not 
    // processed. 
    std::vector<bool> visited(getNumberOfNodes(), false); 
    std::vector<bool> processed(getNumberOfNodes(), false); 
  
    int mindeg = std::numeric_limits<int>::max(); 
    int startvertex = 0; 
  
    // Store degrees of all vertices 
    std::vector<int> vDegree(getNumberOfNodes()); 
    for (int i=0; i<getNumberOfNodes(); i++) 
    { 
        vDegree[i]  = getDegreeOfNodeAt(i); 
  
        if (vDegree[i] < mindeg) 
        { 
            mindeg = vDegree[i]; 
            startvertex=i; 
        } 
    } 
  
    kcoreDFSUtil(startvertex, visited, vDegree, k); 
  
    // DFS traversal to update degrees of all 
    // vertices. 
    for (int i=0; i<getNumberOfNodes(); i++) 
        if (visited[i] == false) 
            kcoreDFSUtil(i, visited, vDegree, k); 
  
    // RETURN K CORES 
    std::vector<int> kcores;
    std::cout << "K-Cores : \n"; 
    for (int v=0; v<getNumberOfNodes(); v++) 
    { 
        // Only considering those vertices which have degree 
        // >= K after BFS 
        if (vDegree[v] >= k) 
        {	
            kcores.push_back(v); 
            //std::cout << "\n[" << v << "]"; 
            // Traverse adjacency list of v and print only 
            // those adjacent which have vDegree >= k after 
            // BFS. 
	    //std::vector<int> neighs = getNeighborhoodIdxVertexOfVertexAt(v);
            //for (int i : neighs) 
            //    if (vDegree[i] >= k) 
            //        std::cout << " -> " << i; 
        } 
    } 
 
    return kcores;
} 

bool Graph::isConnected() {
	std::vector<int> order = bfsOrder(0);
	for (uint i = 0; i < getNumberOfNodes(); i++)
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

size_t Graph::getNaiveCodeHashValue() {
	size_t seed = 0;
	for (Edge e : edges) {
		boost::hash_combine(seed, e.getLabel() * 2654435761);
		
		//including node src
		Node src = this->getNodeAt(e.getFromNodeId());
		boost::hash_combine(seed, e.getFromNodeId() * 2654435761);
		boost::hash_combine(seed, src.getLabel() * 2654435761);
		
		//including node dest
		Node dest = this->getNodeAt(e.getToNodeId());
		boost::hash_combine(seed, e.getToNodeId() * 2654435761);
		boost::hash_combine(seed, dest.getLabel() * 2654435761);
	}

	return seed;
}

size_t Graph::getBlissCodeHashValue() {
	//bliss::Graph bg = this->getBlissGraph();
	//return Canonical::getHash(bg);
	return Canonical::getHash(*this);
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
	std::random_shuffle(nodes.begin(), nodes.end());
}

void Graph::colorNodes(int c) {
	colors.resize(getNumberOfNodes());
	for (int i = 0; i < getNumberOfNodes(); i++)
		colors[i] = Randness::instance().get_a_random_number(0,c);
}

//assumes that the node vector has them all, thus just update edges and indexes
void Graph::updateWithEdgesInGraph(boost::dynamic_bitset<> &bs, Graph *g) {
	//for (int i : ids) {
	size_t i = bs.find_first();
	while (i != boost::dynamic_bitset<>::npos) {
		if (i >= nodes.size()) {
			std::cout << "error: updateWithNodesInGraph. id is out of range!" << std::endl;
			exit(1);
		}
		NeighborhoodSet neighEdges = g->getNeighborhoodEdgeOfVertexAt(i); 
		for (NeighborhoodSet::iterator it = neighEdges.begin(); it != neighEdges.end(); it++) {
			Edge e = g->getEdgeAt(*it);
    			int to = e.getToNodeId(); 
			int from = e.getFromNodeId();
			//update if the nodes are set as true in the bitset
			if (!bs.test(to) || !bs.test(from) || !insertEdge(e)) continue;
			//check if there is an error
			if (from < 0 || from >= (int) neighborhood.size()) {
	               		std::cout << "invalid update node from idx " << from << std::endl;
	        	        std::cout << "neighborhood size " << (int)neighborhood.size();
			        exit(1);
			}
		        if (to < 0 || to >= (int) neighborhood.size()) {
                		std::cout << "invalid update node to idx " << to << std::endl;
		                std::cout << "neighborhood size " << (int)neighborhood.size() << std::endl;
                		exit(1);
		        }	
			//update node indexes
			neighborhood[to].insert(from);
		        neighborhood[from].insert(to);
		        neighborhoodIdx[to].push_back(from);
		        neighborhoodIdx[from].push_back(to);

			//update edge index
        		neighborhoodEdge[to].insert(edges.size()-1);
		        neighborhoodEdge[from].insert(edges.size()-1);
		}
		i = bs.find_next(i);
	}
}

std::vector<int> Graph::getPathBetweenNodes(int id1, int id2, int k) {

	std::unordered_set<int> reach;
	std::stack<std::pair<int, int>> q;
	reach.insert(id1);
	q.push(std::pair<int, int> (id1,0));
	//std::cout << "add "<< id1 << std::endl;

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

std::vector<int> Graph::getConnectedNodes(std::unordered_set<int> &nodes, int k) {
	std::cout << "looking for connected nodes of size: " << k << std::endl;
	std::vector<int> cnodes;
	std::unordered_set<int> reach;
	std::stack<int> q;

	if (!nodes.empty()) {
		reach.insert(*nodes.begin());
		q.push(*nodes.begin());
	}

	while (reach.size()!=k && !q.empty()) {
		int top  = q.top();
		q.pop();
		std::vector<int> &neighs = getNeighborhoodIdxVertexOfVertexAt(top);
		std::cout << "node " << top << " number of neighbors " << neighs.size() << " current size "<< reach.size() << std::endl;

		//neighbors have finished
		//add new neighbor if it is in nodes set
		for (int i : neighs){
			if (reach.find(i) == reach.end() && nodes.find(i) != nodes.end()) {
				std::cout << "add " << i << std::endl;
				reach.insert(i);
				q.push(i);
				if (reach.size()==k) break;
			}
		}
	}

	for (int i : reach) cnodes.push_back(i);

	if (cnodes.size()!=k)
		std::cout << "connected nodes with size " << k << " not found!" << std::endl;

	return cnodes; 
}

/*
   std::map<int, int> Graph::markConnectedComponentsWithinSet(boost::dynamic_bitset<> &bs) { 
   if (comp.size() != nodes.size()) comp.resize(nodes.size());

//representative set
std::map<int, int> rs; 

//set number of components to zero
int numComps=0;

//mark all the vertices as not visited 
size_t i = bs.find_first();
while (i != boost::dynamic_bitset<>::npos) {
comp[i] = 0; 
i = bs.find_next(i);
}

i = bs.find_first();
while (i != boost::dynamic_bitset<>::npos) {
//std::cout << "node " << i << " " << comp[i] << std::endl; 
if (comp[i] == 0) {
numComps++;
connectedComponentsDFSWithinSet(bs, i, numComps); 
}
if (rs.find(comp[i])==rs.end() && comp[i]!=0) {
rs[comp[i]] = i;
}
//std::cout << "node " << i << " " << comp[i] << std::endl;
i = bs.find_next(i);
} 
return rs;
} 

void Graph::connectedComponentsDFSWithinSet(boost::dynamic_bitset<> &bs, int i, int numComps) { 
//mark the current node as visited and print it 
comp[i]=numComps; 
//std::cout << ">>>node " << i << std::endl; 

// Recur for all the vertices 
// adjacent to this vertex 
for(int j : neighborhood[i]) { 
if(bs.test(j) && comp[j] == 0) 
connectedComponentsDFSWithinSet(bs, j, numComps); 
}
} 

//find the connected components ignoring nodes set in bs and returns a representative node of each component
std::map<int, int> Graph::markConnectedComponentsWithoutSet(boost::dynamic_bitset<> &bs) { 
if (comp.size() != nodes.size()) comp.resize(nodes.size());

//representative set
std::map<int, int> rs; 

//set number of components to zero
int numComps=0;

//mark all the vertices as not visited 
for (uint i = 0; i < nodes.size(); i++) 
comp[i] = 0; 

for (uint i=0; i < nodes.size(); i++) { 
//std::cout << "node " << i << " " << comp[i] << std::endl; 
if (!bs.test(i) && comp[i] == 0) {
numComps++;
connectedComponentsDFSWithoutSet(bs, i, numComps); 
}
if (rs.find(comp[i])==rs.end() && comp[i]!=0) {
rs[comp[i]] = i;
}
//std::cout << "node " << i << " " << comp[i] << std::endl;
} 
return rs;
} 

void Graph::connectedComponentsDFSWithoutSet(boost::dynamic_bitset<> &bs, int i, int numComps) { 
	//mark the current node as visited and print it 
	comp[i]=numComps; 
	//std::cout << ">>>node " << i << std::endl; 

	// Recur for all the vertices 
	// adjacent to this vertex 
	for(int j : neighborhood[i]) { 
		if(!bs.test(j) && comp[j] == 0) 
			connectedComponentsDFSWithoutSet(bs, j, numComps); 
	}
}*/ 

std::map<int, std::vector<int> > Graph::markConnectedComponentsWithinSet(std::unordered_set<int> &os) { 
	if (comp.size() != nodes.size()) comp.resize(nodes.size());

	//representative set
	std::map<int, std::vector<int> > rs; 

	//set number of components to zero
	int numComps=0;

	//mark all the vertices as not visited 
	for (int i : os) {
		comp[i] = -1; 
	}

	for (int i : os) {
		//std::cout << "node " << i << " " << comp[i] << std::endl; 
		if (comp[i]==-1) {
			connectedComponentsDFSWithinSet(os, i, numComps); 
			numComps++;
		}

		std::map<int, std::vector<int> >::iterator map_it = rs.find(comp[i]);
		if (map_it==rs.end()) {
			//rs[comp[i]] = i;
			rs.insert(std::make_pair(comp[i], std::vector<int>()));
			map_it = rs.find(comp[i]);
		}
		//insert this node in its component
		map_it->second.push_back(i);
		//std::cout << "node " << i << " " << comp[i] << std::endl;
	} 
	return rs;
} 

void Graph::connectedComponentsDFSWithinSet(std::unordered_set<int> &os, int i, int numComps) { 
	//mark the current node as visited and print it 
	comp[i]=numComps; 
	//std::cout << ">>>node " << i << std::endl; 

	// Recur for all the vertices 
	// adjacent to this vertex 
	for(int j : neighborhood[i]) { 
		if(os.find(j)!=os.end() && comp[j] == -1) 
			connectedComponentsDFSWithinSet(os, j, numComps); 
	}
}

/*
//find the connected components ignoring nodes set in bs and returns a representative node of each component
std::map<int, int> Graph::markConnectedComponentsWithoutSet(std::unordered_set<int> &os) { 
if (comp.size() != nodes.size()) comp.resize(nodes.size());

//representative set
std::map<int, int> rs; 

//set number of components to zero
int numComps=0;

//mark all the vertices as not visited 
for (uint i = 0; i < nodes.size(); i++) 
comp[i] = 0; 

for (uint i=0; i < nodes.size(); i++) { 
//std::cout << "node " << i << " " << comp[i] << std::endl; 
if (os.find(i)==os.end() && comp[i] == 0) {
numComps++;
connectedComponentsDFSWithoutSet(os, i, numComps); 
}
if (rs.find(comp[i])==rs.end() && comp[i]!=0) {
rs[comp[i]] = i;
}
//std::cout << "node " << i << " " << comp[i] << std::endl;
} 
return rs;
} 

void Graph::connectedComponentsDFSWithoutSet(std::unordered_set<int> &os, int i, int numComps) { 
//mark the current node as visited and print it 
comp[i]=numComps; 
//std::cout << ">>>node " << i << std::endl; 

// Recur for all the vertices 
// adjacent to this vertex 
for(int j : neighborhood[i]) { 
if(os.find(j)==os.end() && comp[j] == 0) 
connectedComponentsDFSWithoutSet(os, j, numComps); 
}
} */


std::map<int, std::vector<int> > Graph::markConnectedComponents() { 
	if (comp.size() != nodes.size()) comp.resize(nodes.size());
	//representative set
	std::map<int, std::vector<int> > rs; 

	//set number of components to zero
	int numComps=0;

	//mark all the vertices as not visited 
	for (int i = 0; i < getNumberOfNodes(); i++) {
		comp[i] = -1; 
	}

	for (int i = 0; i < getNumberOfNodes(); i++) {
		//std::cout << "node " << i << " " << comp[i] << std::endl; 
		if (comp[i]==-1) {
			connectedComponentsDFS(i, numComps); 
			numComps++;
		}

		std::map<int, std::vector<int> >::iterator map_it = rs.find(comp[i]);
		if (map_it==rs.end()) {
			//rs[comp[i]] = i;
			rs.insert(std::make_pair(comp[i], std::vector<int>()));
			map_it = rs.find(comp[i]);
		}
		//insert this node in its component
		map_it->second.push_back(i);
		//std::cout << "node " << i << " " << comp[i] << std::endl;
	} 
	return rs;
} 

void Graph::connectedComponentsDFS(int i, int numComps) { 
	//mark the current node as visited and print it 
	comp[i]=numComps; 
	//std::cout << ">>>node " << i << std::endl; 

	// Recur for all the vertices 
	// adjacent to this vertex 
	for(int j : neighborhood[i]) { 
		if(comp[j] == -1) 
			connectedComponentsDFS(j, numComps); 
	}
}
