#ifndef EMBEDDING_H
#define EMBEDDING_H

#include "constants.h"
#include "utils.h"
#include "patternCode.h"
#include "graph.h"
#include "domain.h"
#include <stdio.h>
#include <ostream>
#include <vector>
#include <unordered_set>

class Embedding{

    protected:
    //virtual std::vector<Uint> getWords() = 0;
    virtual std::vector<Uint> getVertices() = 0;
    virtual Uint getNumVertices() = 0;
    virtual std::vector<Uint> getEdges() = 0;
    virtual Uint getNumEdges() = 0;
    //virtual Uint getNumWords() = 0;
    //Pattern getPattern() = 0;
    //virtual Uint getNumVerticesAddedWithExpansion() = 0;
    //virtual Uint getNumEdgesAddedWithExpansion() = 0;
    //virtual void addWord(int word) = 0;
    //virtual void removeLastWord() = 0;
    //virtual std::unordered_set<Uint> getExtensibleWordIds() = 0;
    virtual std::unordered_set<Uint> getNeighborhoodVertices() = 0;
    virtual std::unordered_set<Uint> getNeighborhoodEdges() = 0;
    //virtual bool isCanonicalEmbeddingWithWord(int wordId) = 0;
    virtual std::string toString() = 0;

};

#endif
