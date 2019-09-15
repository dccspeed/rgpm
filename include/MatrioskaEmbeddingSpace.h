#ifndef MATRIOSKAEMBEDDINGSPACE_H
#define MATRIOSKAEMBEDDINGSPACE_H

#include "BasicEmbeddingSpace.h"
#include "reservoir.h"
#include "TourStats.h"
#include <iostream>
#include <tbb/concurrent_hash_map.h>
#include <tbb/concurrent_queue.h>
#include <tbb/recursive_mutex.h>
#include <tbb/parallel_for.h>
#include <tbb/task_group.h>
#include <tbb/task_scheduler_init.h>
#include <tbb/blocked_range.h>
#include <tbb/parallel_reduce.h>
#include <array>
#include <chrono>
#include <sstream>      // std::stringstream
#include <bitset>
#include <boost/dynamic_bitset.hpp>

template <class T, class A>
class EmbeddingSpaceMSK:public EmbeddingSpace<T,A> {
	
	typedef std::unordered_map<std::pair<uint, uint>, double, boost::hash<std::pair<uint, uint>>> PartitionDoubleMap;
	typedef typename std::unordered_map<std::pair<uint, uint>, reservoir<T>, boost::hash<std::pair<uint, uint>>> PartitionReservoirMap;

	public:
	boost::dynamic_bitset<> lsn, lrw, lnext;
	std::vector<int> hops;
	Graph *g_complete;
	std::vector<uint> partitionMap;
	std::vector<uint> partitionDepthIdx;
	std::vector<std::unordered_set<int>> partitions;
	std::vector<double> degreeEstimateAux;
	std::vector<double> countEstimateAux;
	std::vector<double> countEstimate;
	//std::vector<double> degreeEstimatePartial; // it keeps the degree beteween two partitions
	PartitionDoubleMap degreeEstimatePartial;
	PartitionReservoirMap rs;
	//std::vector<reservoir<T>> rs;

	static const unsigned int maxlevelexp = 1000000;

	EmbeddingSpaceMSK(int , Graph *); 
	EmbeddingSpaceMSK(int , Graph *, std::string); 

	void run_rw();
	std::vector<TourStats> getGroupStats(T&, int);
	TourStats2<T> groupEstimateUsingTour(T&, uint, int);
	TourStats2<T> groupEstimateAllTours(uint, int);

	bool isSubset(boost::dynamic_bitset<> &, std::vector<int> &);
	bool isInPreviousLevels(uint, std::vector<int> &);
	uint getGraphLayerEdges(boost::dynamic_bitset<> &);
	ModSet getModificationsToLevel(T &, boost::dynamic_bitset<> &);
	ModSet getModificationsToLevel(T &, boost::dynamic_bitset<> &, ModSet &);
	ModSet getModificationsToPreviousLevels(T&, uint);
	ModSet getModificationsToPreviousLevels(T&, uint, ModSet &);
	void updateGroupStats(T &, T &, TourStats2<T> &);
	//void createLevelsFromSN();
	double computeModificationWeight(T&, Mod&);
	double getEmbeddingScore(T&);

	void initLevels(T &, Graph *);
	void initAllLevels(T &, Graph *);
	void expandLevels(Graph *);
	uint getGraphOutLevelEdges(boost::dynamic_bitset<> &);
	uint getGraphInLevelEdges(boost::dynamic_bitset<> &);
	T getRandomEmbeddingInLevel(uint);
	
	bool computeEmbeddingNeighborhood(T &, e_cache &, ModSet &);
	//std::pair<Mod, bool> getNextRandomModificationBiasedByDegree(T &);
	std::pair<Mod,bool> getNextRandomModificationByRejection(T &, ModSet &);
	//bool init_rw();

	int getMatrixPositionInArray(int r, int c) {
		return (c*(c-1))/2 + r;
		//return (r*(r-1))/2 + c - 1;
	}
};


#endif
