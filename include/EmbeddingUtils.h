#ifndef EMBEDDINGUTILS_H
#define EMBEDDINGUTILS_H

#include "BasicEmbedding.h"
#include "graph.h"
#include <algorithm>

class EmbeddingUtils {

   private :
   
   static const int NIL = -1; 
   static void articulationRec(int u, BasicEmbedding &, std::vector<std::vector<int> > & ,  int *, std::vector<int> &, std::vector<int> &, std::vector<int> &, std::vector<bool> &, std::vector<bool> &);
   //static void dfsRec(int u, BasicEmbedding &, std::vector<std::vector<int> > &, std::vector<bool>&, std::vector<int>&);
   static void dfsRec(int u, BasicEmbedding &, int[10][10], std::vector<bool>&, std::vector<int>&);

   EmbeddingUtils();
   
   public:
   /**
    * Clients call for articulation brigdes and points of an embedding
    */
   static std::vector<bool> articulation(BasicEmbedding &);
   static std::vector<bool> articulation(BasicEmbedding &, int);

   /**
    * Clients call for dfs order of an embedding
    */
   static std::vector<int> dfs(BasicEmbedding &);


   static bool isConnected(BasicEmbedding &);
   
   static bool isQuasiClique(BasicEmbedding &, double );
   
   static std::vector<size_t> getSubsetWordsHash(BasicEmbedding &);
};

class CompareEmbeddings{
    public:
    bool operator() (const BasicEmbedding &l, const BasicEmbedding &r) const {
        return (l.getNumberOfNeighbors() > r.getNumberOfNeighbors());
    }
};


#endif
