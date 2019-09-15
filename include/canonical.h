#ifndef CANONICAL_H
#define CANONICAL_H

#include <stdlib.h>
#include <unordered_map>
#include <utils.hh>
#include <graph.hh>
#include <bliss_C.h>
#include <tbb/concurrent_hash_map.h>
#include "utils.h"
#include "BasicEmbedding.h"
#include "graph.h"

class Canonical {
	public:
		struct PatternInfo {
			public:
			unsigned int *array;
			unsigned int N;

			void init(unsigned int _N) {
				N = _N;
				if (array != NULL) free(array);
				array = (unsigned int *) malloc(sizeof(int) * N);
			}

			PatternInfo() {	
				N = 0;
				array = NULL;
			}
			PatternInfo(unsigned int _N) {
				N = _N;
				array = (unsigned int *) malloc(sizeof(int) * N);
			}
			~PatternInfo() {
				if (array != NULL) free(array);
			}
		};

		static tbb::atomic<int> hits;
		static tbb::atomic<int> misses;
		static tbb::concurrent_hash_map<size_t, size_t> pattern_to_canonical; 
		//static tbb::concurrent_hash_map<size_t, PatternInfo*> naives; 
		//static tbb::concurrent_hash_map<size_t, PatternInfo*> canonicals; 

		//static size_t getHashScratch(bliss::Graph &);
		static size_t getHash(BasicEmbedding &);
		static size_t getHash(Graph &);
		static size_t getHash(bliss::Graph &);
		//static unsigned int *getPermutation(bliss::Graph &);
		
		static void report_aut(void*, const unsigned int, const unsigned int*);

		static void get_automorphism_map(unsigned int *, const unsigned int N, const unsigned int* perm, const unsigned int offset);
		
		//static void merge_sets(int *, size_t );

};

#endif
