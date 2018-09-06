#ifndef BASICEMBEDDINGSPACE_PAR_H
#define BASICEMBEDDINGSPACE_PAR_H

#include "BasicEmbeddingSpace.h"

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


template <class T, class A>
class EmbeddingSpacePar:public EmbeddingSpace<T,A> {

	public:
	
	EmbeddingSpacePar(int , Graph *); 
	EmbeddingSpacePar(int , int, Graph *); 
	EmbeddingSpacePar(int , int, Graph *, std::string); 
	EmbeddingSpacePar(int , int, Graph *, int); 
	EmbeddingSpacePar(int , int, Graph *, std::string, int); 

	tbb::atomic<unsigned> producer_count, consumer_count;	
	bool done;
	tbb::recursive_mutex mu;
	tbb::recursive_mutex *mus;

	//tbb::concurrent_vector<T> queue;
	tbb::concurrent_bounded_queue<T> queue;
	std::vector<A> aggs;

	const int producer_thread_count = 1;
	int nthreads;
	
	void producer();
	void consumer(int);

	void run_rw();
	void processEmbedding(T &, int);
	void summarizeAggregator(A &);
	void groupQualityAnalysis(T &);
	TourStats groupEstimateAllTours(T&, SuperEmbedding<T>&, int);
	//virtual bool isSameGroup(T &, T &) = 0;
        //virtual bool canReachGroup(T &, T &) = 0;
        //virtual bool isValidEmbeddingGroup(T &, T&) = 0;
        //virtual std::vector<int> getGroupKeys(T &, T &) = 0;
	
	const int MAX_QUEUE_SIZE = 100000;

};

#endif
