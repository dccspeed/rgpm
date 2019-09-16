#ifndef BASICEMBEDDINGSPACE_H
#define BASICEMBEDDINGSPACE_H

#include "logging.h"
#include "constants.h"
#include "utils.h"
#include "graph.h"
#include "random.h"
#include "EdgeInducedEmbedding.h"
#include "VertexInducedEmbedding.h"
#include "SuperEmbedding.h"
#include "EmbeddingUtils.h"
#include "AggregatorPatternCounter.h"
#include <stdio.h>
#include <vector>
#include <map>
#include <list>
#include <unordered_set>
#include <unordered_map>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <graph.hh>
#include <cache.h>
#include <fstream>


struct e_cache {
	public:
		static const int NUM_EXTRA_NEIGHBORS = 10;
		
		int numberOfNeighbors;
		double degree;
		int used_mods;
		//reservoir<Mod> rmods;
		Mod mods[NUM_EXTRA_NEIGHBORS];

		e_cache(): numberOfNeighbors(0), degree(0), used_mods(NUM_EXTRA_NEIGHBORS) {};
		//e_cache(): numberOfNeighbors(0), degree(0), used_mods(NUM_EXTRA_NEIGHBORS), rmods(NUM_EXTRA_NEIGHBORS) {};

		void reset() {
			numberOfNeighbors = 0;
			degree = 0;
			used_mods = NUM_EXTRA_NEIGHBORS;
		}
		void print() const {
			std::cout << "e_cache { number of neighbors: " << numberOfNeighbors << " degree: " << degree << " used_mods: " << used_mods << " } " << std::endl;
			for (int i = 0; i < NUM_EXTRA_NEIGHBORS; i++) {
				std::cout << "mods["<< i << "]: ";
				mods[i].print();
				//rmods.rvr[i].print();
			}
		}
};

typedef lru_cache<size_t, e_cache> EmbeddingCache;

template <class T, class A> 
class EmbeddingSpace {
		
	public : 
		Graph *g;
		T currEmbedding;
		int size; // embedding space related size
		int snSize; // embedding space related size
		bool isTransitive; 
		bool isSingleGroup; 

		//hashmap for aggregate patterns
		int numFlushes;
		A agg;
	
		//cache - it need to be enable in constants.h
		EmbeddingCache emb_neighbor_cacheg;

		//aggregator output files
		std::string aggOutput;

		ModSet mods_empty;

		EmbeddingSpace(int , Graph *); 
		EmbeddingSpace(int , int, Graph *); 
		EmbeddingSpace(int , int, Graph *, std::string); 
		void checkConfigFile();
		void setAggOutput(std::string output) {aggOutput = output;};
		
		virtual bool init_rw();
		void run_rw();
		//void run_rw_query(Graph &pattern);
		void run_exact_recursive();
		bool getValidEmbedding();
		bool getValidEmbeddingWithNode(int);
		bool getValidEmbeddingWithNodes(std::vector<int>&);
		bool getValidEmbeddingBiased(T&);
		void updateGroupStats(T &, T &, TourStats &);
		void updateGroupStatsExact(T &, T &, TourStats &);
		void filterGroupStatsByOwner(T &, TourStats &);
		TourStats getGroupStats(T &, SuperEmbedding<T> &, int);
		//void getExactGroupStatsDFS(T &, T&, TourStats &, std::unordered_set<size_t> &);
		//void getExactGroupStatsDFS(T &, T&, TourStats &, NeighborhoodSet & );
		void getExactGroupStatsDFS(T &, T&, TourStats &, std::set<size_t> & );
		void getExactGroupStatsBFS(T &, TourStats &, std::set<size_t> & );
		TourStats getExactGroupStats(T &);
		TourStats groupEstimateUsingTour(T, SuperEmbedding<T>&, int);
		virtual TourStats groupEstimateAllTours(T&, SuperEmbedding<T>&, int);
		bool getTransitiveRelation() {return isTransitive;};
		void setTransitiveRelation(bool t) { isTransitive = t;};

		SuperEmbedding<T> createBFSGroup(T &, int);
		SuperEmbedding<T> createSNTopEmbeddingsGroup(T &, int);
		SuperEmbedding<T> createSNFirstEmbeddingsGroup(T &, int);
		void test();

		void updateGroupsOwnerFromEmbeddingPair(T &, T &, std::unordered_map<int, int> &, std::vector<size_t> &);
		void getGroupsOwnerFromEmbeddingRec(T &, T &, int,  std::unordered_map<int, int> &, std::vector<size_t> &);
		//int getNumGroupsFromEmbedding(T &);
		//int getNumGroupsFromEmbedding(T &, std::vector<T> &);

		virtual double getEmbeddingScore(T &);
		int getEmbeddingNumNeighbors(T &);
		int getEmbeddingNumNeighbors(T &, ModSet &);
		double getEmbeddingDegree(T &);
		double getEmbeddingDegree(T &, ModSet &);
		std::pair<Mod,bool> getNextRandomModification(T &);
		std::pair<Mod,bool> getNextRandomModification(T &, ModSet &);
		std::pair<T,bool> getRandomNeighborSNByRejection(SuperEmbedding<T> &);
		std::pair<T,bool> getRandomNeighborSN(SuperEmbedding<T> &);
		bool getNextValidEmbedding(T &);
		bool getNextValidEmbeddingRW(T &);
		bool getNextValidEmbeddingFromFile(T &);
		virtual bool isSameGroup(T &, T &) = 0;
		virtual bool isValidEmbeddingGroup(T &, T&) = 0;
		virtual std::vector<int> getGroupKeys(T &, T &) = 0;
		virtual bool isValidEmbedding(T&);
		virtual bool filterEmbedding(T&);
		virtual void groupQualityAnalysis(T &);

		int MAX_EX_STEPS=1000000000;
		int MAX_EX_BUFF= 1000000000;
		int MAX_RW_STEPS=1000000;
		int INIT_RW_STEPS=1000000;
		int MAX_NUM_TOURS=10000;      
		int MAX_TOUR_STEPS=100000;
		int MAX_INIT_ATTEMPT=10000;
		int MAX_CACHE_SIZE=10000000;
		int MAX_AGG_SIZE=1000000;
		int INITIAL_NODE=-1;
		std::pair<int,int> INITIAL_PAIR_NODES=std::pair<int,int>(-1,-1);
		bool SET_TOUR=true;
		bool SET_SUPERNODE_RW=false;
		bool RW_NO_RETURN=true;
		bool SET_QLTY_ANALYSIS=false;
		T INITIAL_EMBEDDING;
		std::ifstream INPUT_EMB_FILE;
		std::ofstream OUTPUT_EMB_FILE;

		void expand_embedding(T &, int);
		void expand_embedding_group(T &, int, int *, int *);
		void getExactGroupDegreesRec(T &, T &, int, std::unordered_map<int, int> &, std::vector<double> &, std::vector<double> &);
		std::vector<T> getAllEmbeddings();
		void getAllEmbeddingsRec(T &, int, std::vector<T> &);
		double computeMaxDiffMap(std::unordered_map<size_t, PatternStats>, std::unordered_map<size_t, PatternStats>);
		void normalizeMap(std::unordered_map<size_t, PatternStats>&);
		virtual void aggregateEmbeddingClass(T &, TourStats &, A &);
		
//template <class T>
virtual bool computeEmbeddingNeighborhood(T &e, e_cache &neighborhood, ModSet &mods) {
	neighborhood.reset();
	//neighborhood.print();
	//std::unordered_set<int> checkedExpansions;
	std::unordered_map<size_t, std::unordered_set<int>> contractionMap;
        //size_t hash = e.getHash();

	std::vector<int> words = e.getWords();
	//checkedExpansions.insert(words.begin(), words.end());
	//T embeddingCopy = e;
	
	//std::cout << "start looking to the neighbors" << std::endl;
	
	std::unordered_set<int> expansions = e.getValidElementsForExpansion();
	//for (int i = 0; i < (int) words.size(); i++) {
	//	NeighborhoodSet expansions = e.getValidElementsForExpansion(words[i]);
		if (expansions.empty()) {	
			std::cout << "impossible to walk! ";
			std::cout << "num possible exp. equal to " << expansions.size() << std::endl;
			exit(1);
		}
		//for (NeighborhoodSet::iterator it = expansions.begin(); it!= expansions.end(); it++) {
		for (std::unordered_set<int>::iterator it = expansions.begin(); it!= expansions.end(); it++) {
			//std::cout << "exp: " << *it << std::endl;
	//		if (checkedExpansions.find(*it) != checkedExpansions.end()) continue;
	//		checkedExpansions.insert(*it);
			std::pair<size_t,int> connectionHash = e.getWordConnectionHash(*it, mods);
			//continue if there is no connections, which is possible if mods.size != 0;
			if (connectionHash.second==0) continue;

			std::unordered_map<size_t, std::unordered_set<int>>::iterator itContraction = contractionMap.find(connectionHash.first);
			if (itContraction == contractionMap.end()) {
				contractionMap.insert(std::make_pair(connectionHash.first, e.getValidElementsForContractionWithWord(*it)));
			}
			//checking if there is an error.
			itContraction = contractionMap.find(connectionHash.first);
			if (itContraction->second.empty()) {
				std::cout << "error: contractions empty!" << std::endl;
				exit(1);
			}
			//fill the neighborhood randomly with reservoir
			for (int k = 0; k < e_cache::NUM_EXTRA_NEIGHBORS; k++) {
				double prob = Randness::instance().random_uni01();
				if (prob <= (double) itContraction->second.size()/(neighborhood.degree+itContraction->second.size())) {
					neighborhood.mods[k].addId = *it;
				}
			}
			neighborhood.degree+=itContraction->second.size();
			/*for (int c : itContraction->second) {
				Mod mod(*it, c);
				//compute weight of this change in the HON
				double w = 1;
				neighborhood.degree+=w;
				double prob = Randness::instance().random_uni01();
				if (prob <= w/(neighborhood.degree)) {
					neighborhood.mods[0].addId = *it ;
					neighborhood.mods[0].rmId = c;
				}
			}*/
		}
	//}
	
	//checking if there is an error.
	if (neighborhood.degree == 0) {
		std::cout << "warning: there is no valid possible expansion!" << std::endl;
		return false;
	}

	for (int k = 0; k < e_cache::NUM_EXTRA_NEIGHBORS; k++) {
		//std::cout << " neighborhood.mods[k].addId: " << neighborhood.mods[k].addId << std::endl;
		std::pair<size_t,int> connectionHash = e.getWordConnectionHash(neighborhood.mods[k].addId, mods);
		std::unordered_map<size_t, std::unordered_set<int>>::iterator itContraction = contractionMap.find(connectionHash.first);
		if (itContraction == contractionMap.end()) {
			std::cout << "error: impossible to find a valid node for contraction! degree: " << e << std::endl; 
			//std::cout << "contractionMap size: " << contractionMap.size() << std::endl; 
			//std::cout << "mods to avoid size: " << mods.size();
			exit(1);
		}
		else if (itContraction->second.empty()) {
			std::cout << "error: impossible to find a valid node for contraction! set is empty!" << e;
			exit(1);
		}	
		std::unordered_set<int>::iterator it = Randness::instance().random_element(itContraction->second.begin(), itContraction->second.end()); 
		neighborhood.mods[k].rmId = *it;
	}

	// since there are neighbors set the number of used neighbors to zero
	neighborhood.used_mods=0;
	
	return true;
}

virtual std::vector<Mod> computeAllEmbeddingNeighborhood(T &e) {
	//td::unordered_set<int> checkedExpansions;
	std::unordered_map<size_t, std::unordered_set<int>> contractionMap;

	std::vector<int> words = e.getWords();
	//checkedExpansions.insert(words.begin(), words.end());

	std::vector<Mod> mods;
	
	std::unordered_set<int> expansions = e.getValidElementsForExpansion();
	//for (int i = 0; i < (int) words.size(); i++) {
	//	NeighborhoodSet expansions = e.getValidElementsForExpansion(words[i]);
		if (expansions.empty()) {	
			std::cout << "impossible to walk! ";
			std::cout << "num possible exp. equal to " << expansions.size() << std::endl;
			exit(1);
		}
		//for (NeighborhoodSet::iterator it = expansions.begin(); it!= expansions.end(); it++) {
		for (std::unordered_set<int>::iterator it = expansions.begin(); it!= expansions.end(); it++) {
			//std::cout << "exp: " << *it << std::endl;
			//if (checkedExpansions.find(*it) != checkedExpansions.end()) continue;
			//checkedExpansions.insert(*it);
			std::pair<size_t,int> connectionHash = e.getWordConnectionHash(*it);
			std::unordered_map<size_t, std::unordered_set<int>>::iterator itContraction = contractionMap.find(connectionHash.first);
			if (itContraction == contractionMap.end()) {
				contractionMap.insert(std::make_pair(connectionHash.first, e.getValidElementsForContractionWithWord(*it)));
			}
			//checking if there is an error.
			itContraction = contractionMap.find(connectionHash.first);
			if (itContraction->second.empty()) {
				std::cout << "error: contractions empty!" << std::endl;
				exit(1);
			}
			for (std::unordered_set<int>::iterator it2 = itContraction->second.begin(); it2 != itContraction->second.end(); it2++) {
				mods.push_back(Mod(*it,*it2));	
			}
		}
	//}
	
	return mods;
}

//template <class T>
virtual std::pair<Mod,bool> getNextRandomModificationByRejection(T &e, ModSet &mods) {
	std::pair<Mod, bool> mod;
	mod.second = false;

	std::vector<int> words = e.getWords();
	int totalExt = 0;
	for (int i = 0; i < (int) words.size(); i++) {
		totalExt+=g->getDegreeOfNodeAt(words[i]);
	}	

	//std::cout << "GETTING RANDOM NEIGHBOR OF EMBEDDING: " << std::endl;
	//e.print();
	int att = 0;
	do {
		//select node to leave
		int totalWeight=0;
		for (int i = 0; i < (int) words.size(); i++) {
			int weight = totalExt-g->getDegreeOfNodeAt(words[i]);
			double prob = Randness::instance().random_uni01();
			if (prob <= (double) weight/(totalWeight+weight)) {
				mod.first.rmId=words[i];
			}
			totalWeight+=weight;
		}
		//select the remain node responsable to set the new node	
		totalWeight=0;
		int neighId=-1;
		for (int i = 0; i < (int) words.size(); i++) {
			if (words[i]==mod.first.rmId) continue;
			int weight = g->getDegreeOfNodeAt(words[i]);
			double prob = Randness::instance().random_uni01();
			if (prob <= (double) weight/(totalWeight+weight)) {
				neighId=words[i];
			}
			totalWeight+=weight;
		}
	
		//if the embedding is of size one neighId = rmId
		if (e.getNumWords() == 1)
			mod.first.rmId = neighId; 
	
		//select the node to enter
		std::vector<int> &expansions = g->getNeighborhoodIdxVertexOfVertexAt(neighId);
		std::vector<int>::iterator it = Randness::instance().random_element(expansions.begin(), expansions.end()); 
		mod.first.addId = *it; 

		//std::cout << "rmId: " << mod.first.rmId << " addId: " << mod.first.addId << " neighId: " << neighId << std::endl;
		if (!e.existWord(mod.first.addId) && mods.find(mod.first)==mods.end()) {
			e.replaceWord(mod.first.rmId, mod.first.addId );
			//fix bias since the word can be connected to more than one vertex.
			int wDegree = e.getWordDegree(mod.first.addId);
			double prob = Randness::instance().random_uni01();
			if (prob <= (double)1./wDegree && EmbeddingUtils::isConnected(e)) {
				mod.second = true;
			}
			e.replaceWord(mod.first.addId, mod.first.rmId );
		}
		att++;
	} while (!mod.second && att < MAX_INIT_ATTEMPT);
	if (!mod.second) {
                std::cout << "warning: getNextRandomModificationByRejection fail. " << e << std::endl; 
		//exit(1);
	}
	return mod;
}

};


#endif
