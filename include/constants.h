#ifndef CONSTANTS_H
#define CONSTANTS_H

//#define GET_TIME
//#define INSTRUMENTATION
//#define DEBUG INFO

#define INFO 1
#define VERBOSE 2

#define SEPARATORS " "
#define MAX_LINE_SIZE 400

//defaut algorithm parameters
#define DEFAUT_MAX_SUBSTRUCT_SIZE 10
#define DEFAUT_MAX_SUBSTRUCT_GAP 2
#define DEFAUT_SIGNATURE_SIZE 128
#define DEFAUT_NUM_VIEWS 0 //zero means all views

//how we select the projections
#define RANDOM_VIEWS 0
#define DEGREE_VIEWS 1
#define DEFAUT_SELECTION_VIEWS_TYPE RANDOM_VIEWS

// views or projection type
#define DFS_SEARCH 0 // use deep search to get a view
#define BFS_SEARCH 1 // use breath search algorithm to get a view
#define SP_SEARCH 2 // use shortest path algorithm to get a view
#define COMPLETE_SEARCH 3 // use all nodes in the neighborhood
#define DEFAUT_VIEW_TYPE COMPLETE_SEARCH

#define PATH_STRUCTURE 0
#define DEFAUT_STRUCTURE_TYPE PATH_STRUCTURE

//signature type
#define INT_SET 0
#define MIN_HASHING 1
#define BLOOM_FILTER 2
#define DEFAUT_SIGNATURE_TYPE MIN_HASHING

#define EDGE_C_TYPE 0
#define LABEL_C_TYPE 1

#define CACHE 1
//#define FLUSH_AGGREGATOR


#define RANDOM_TYPE RND_SFMT
#define RND_SFMT 0
#define RND_MT 1
#define RND_PCG 2

#endif 
