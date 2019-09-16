#include "EmbeddingUtils.h"
#include <algorithm>

//int EmbeddingUtils::time;
const int EmbeddingUtils::NIL;
//std::vector<int> EmbeddingUtils::parent;
//std::vector<int> EmbeddingUtils::disc;
//std::vector<int> EmbeddingUtils::low;
//std::vector<int> EmbeddingUtils::order;
//std::vector<bool> EmbeddingUtils::visited;
//std::vector<bool> EmbeddingUtils::ap;


EmbeddingUtils::EmbeddingUtils() {};

void EmbeddingUtils::articulationRec(int u, BasicEmbedding &embedding, std::vector<std::vector<int>> &adj, int *time, std::vector<int> &parent, std::vector<int>& disc, std::vector<int> &low, std::vector<bool> &visited, std::vector<bool> &ap) {
	// number of children in the DFS
	int children = 0;

	// visiting new node
	visited[u] = true;

	// discovery time and low value
	disc[u] = ++(*time);
	low[u] = *time;

	std::vector<int> words = embedding.getWords(); 
	int k=0;
	int v=adj[u][k];
	while (v != -1) {
		if (!visited[v]) {
			children++;
			parent[v] = u;
			articulationRec(v, embedding, adj, time, parent, disc, low, visited, ap);

			// Check if the subtree rooted with v has a connection to
			// one of the ancestors of u
			low[u] = std::min(low[u], low[v]);

			// u is an articulation point in following cases

			// (1) u is root of DFS tree and has two or more chilren.
			if (parent[u] == NIL && children > 1)
				ap[u] = true;

			// (2) If u is not root and low value of one of its child
			// is more than discovery value of u.
			if (parent[u] != NIL && low[v] >= disc[u])
				ap[u] = true;
		}

		// Update low value of u for parent function calls.
		else if (v != parent[u])
			low[u] = std::min(low[u], disc[v]);

		k++;
		v=adj[u][k];
	}
}

//void EmbeddingUtils::dfsRec(int u, BasicEmbedding &embedding, std::vector<std::vector<int>> &adj, std::vector<bool> &visited, std::vector<int> &order) {
void EmbeddingUtils::dfsRec(int u, BasicEmbedding &embedding, EmbMatrix adj, std::vector<bool> &visited, std::vector<int> &order) {
	// number of children in the DFS

	// visiting new node
	visited[u] = true;
	//embedding.print();
	//std::cout << "visiting node " << u << std::endl;
	std::vector<int> words = embedding.getWords();

	order.push_back(words[u]);
	int k=0;
	int v=adj[u][k];
	while (v != -1) {
		if (!visited[v]) {
			dfsRec(v, embedding, adj, visited, order);
		}
		k++;
		v=adj[u][k];
	}
}

std::vector<bool> EmbeddingUtils::articulation(BasicEmbedding &embedding) {
	int s = embedding.getNumWords();
	std::vector<int> words = embedding.getWords();

	int time = 0;
	std::vector<int> parent(s,NIL);
	std::vector<int> disc(s,NIL);
	std::vector<int> low(s,NIL);
	std::vector<bool> visited(s,false);
	std::vector<bool> ap(s,false);
	std::vector<int> order;

	//create embedding adj matrix.
	std::vector<std::vector<int>> adj( s, std::vector<int>(s+1,-1));
	int adjsize[s]={0};
	for (int u = 0; u < s; ++u) {
		for (int v = u; v < s; ++v) {
			// here we verify whether each vertice is adjacent to each vertice
			// In practive (embeddings << graph) this is better than accessing
			// directly the whole graph adjacency (the only one available)
			if (!embedding.areWordsNeighbours(words[u], words[v]))  
				continue;

			adj[u][adjsize[u]]=v;
			adj[v][adjsize[v]]=u;
			adjsize[u]++;
			adjsize[v]++;
		}
	}

	for (int u = 0; u < embedding.getNumWords(); ++u) {
		//System.out.println("Start from edge: " + u);
		if (visited[u] == false)
			articulationRec(u, embedding, adj, &time, parent, disc, low, visited, ap);
	}
	return ap;
}

std::vector<bool> EmbeddingUtils::articulation(BasicEmbedding &embedding, int wordId) {
	int s = embedding.getNumWords() + 1;
	std::vector<int> words = embedding.getWords();

	int time = 0;
	std::vector<int> parent(s,NIL);
	std::vector<int> disc(s,NIL);
	std::vector<int> low(s,NIL);
	std::vector<bool> visited(s,false);
	std::vector<bool> ap(s,false);
	std::vector<int> order;

	//create embedding adj matrix.
	std::vector<std::vector<int>> adj( s, std::vector<int>(s+1,-1));
	int adjsize[s]={0};
	for (int u = 0; u < s-1; ++u) {
		for (int v = u; v < s-1; ++v) {
			// here we verify whether each vertice is adjacent to each vertice
			// In practive (embeddings << graph) this is better than accessing
			// directly the whole graph adjacency (the only one available)
			if (!embedding.areWordsNeighbours(words[u], words[v]))
				continue;

			adj[u][adjsize[u]]=v;
			adj[v][adjsize[v]]=u;
			adjsize[u]++;
			adjsize[v]++;
		}
		//for the wordId
		if (embedding.areWordsNeighbours(words[u], wordId)) {
			adj[u][adjsize[u]]=s-1;
			adj[s-1][adjsize[s-1]]=u;
			adjsize[u]++;
			adjsize[s-1]++;
		}
	}

	for (int u = 0; u < embedding.getNumWords(); ++u) {
		//System.out.println("Start from edge: " + u);
		if (visited[u] == false)
			articulationRec(u, embedding, adj, &time, parent, disc, low, visited, ap);
	}
	return ap;
}

std::vector<int> EmbeddingUtils::dfs(BasicEmbedding &embedding) {
	int s = embedding.getNumWords();
	std::vector<int> words = embedding.getWords();

	std::vector<bool> visited(s,false);
	std::vector<int> order;

	//create embedding adj matrix.
	//std::vector<std::vector<int>> adj(s, std::vector<int>(s+1, -1));
	EmbMatrix adj;
	int adjsize[s]={0};
	for (int u = 0; u < s; ++u) {
		for (int v = u; v < s; ++v) {
			// here we verify whether each vertice is adjacent to each vertice
			// In practive (embeddings << graph) this is better than accessing
			// directly the whole graph adjacency (the only one available)
			if (!embedding.areWordsNeighbours(words[u], words[v]))
				continue;

			adj[u][adjsize[u]]=v;
			adj[v][adjsize[v]]=u;
			adjsize[u]++;
			adjsize[v]++;
		}
		adj[u][adjsize[u]]=-1;
	}
	for (int u = 0; u < embedding.getNumWords(); ++u) {
		//std::cout << "Start from word: " <<  u << std::endl;
		if (visited[u] == false)
			dfsRec(u, embedding, adj, visited, order);
	}
	return order;
}

bool EmbeddingUtils::isConnected(BasicEmbedding &embedding) {
	int s = embedding.getNumWords();
	std::vector<int> words = embedding.getWords();

	std::vector<bool> visited(s,false);
	std::vector<int> order;

	//std::cout << "create embedding adj matrix for embedding " << embedding << std::endl;
	//std::vector<std::vector<int>> adj(s, std::vector<int>(s+1,-1));
	EmbMatrix adj;
	int adjsize[s]={0};
	for (int u = 0; u < s; ++u) {
		for (int v = u; v < s; ++v) {
			// here we verify whether each vertice is adjacent to each vertice
			// In practive (embeddings << graph) this is better than accessing
			// directly the whole graph adjacency (the only one available)
			if (v == u || !embedding.areWordsNeighbours(words[u], words[v]))
				continue;

			adj[u][adjsize[u]]=v;
			adj[v][adjsize[v]]=u;
			adjsize[u]++;
			adjsize[v]++;
		}
		adj[u][adjsize[u]]=-1;
	}
	/*for (int u = 0; u < 10; ++u) {
		std::cout << "node " << u << " ";
		for (int v = u; v < 10; ++v) {
			std::cout << " " <<  adj[u][v];
		}
		std::cout << std::endl;
	}*/
	dfsRec(0, embedding, adj, visited, order);
	for (int u = 0; u < embedding.getNumWords(); ++u) 
		if (visited[u]==false) return false;

	return true;
}

bool EmbeddingUtils::isQuasiClique(BasicEmbedding &embedding, double a) {

	std::vector<int> words = embedding.getWords();
	double factor = a*(double)(embedding.getNumWords() - 1.);

	for (int u = 0; u < embedding.getNumWords(); ++u) {
		int d = embedding.getWordDegree(words[u]);
		//std::cout << "factor : " << factor << " degree node: " << d << std::endl;
		if (factor > d) return false;
	}
	return true;
}

std::vector<size_t> EmbeddingUtils::getSubsetWordsHash(BasicEmbedding &embedding) {
	std::vector<int> words(embedding.getWords());
	std::sort(words.begin(), words.end());

	std::vector<size_t> sub;
	for (int i = 0; i < (int) words.size(); i++) {
		size_t seed = 0;
		for (int j = 0; j < (int) words.size(); j++) {
			if (i != j)
				boost::hash_combine(seed,  words[j] * 2654435761);
		}
		sub.push_back(seed);
	}
	return sub;
}

