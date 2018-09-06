#include "BasicEmbedding.h"
#include "EmbeddingUtils.h"
#include "canonical.h"
#include <algorithm>
#include <boost/algorithm/string.hpp>

    BasicEmbedding::BasicEmbedding() {
        init();
	numNeighbors=0;
	degree=0;
	isPoisonPill = false;
    }
    
    BasicEmbedding::BasicEmbedding(Graph *g) {
        init();
	graph = g;
	numNeighbors=0;
	degree=0;
	isPoisonPill = false;
    }

    void BasicEmbedding::init() {
        reset();
    }
    
    void BasicEmbedding::reset() { 
    }
	
    bool BasicEmbedding::isEmpty() {
	return this->getNumWords()==0;
    }
    
    int BasicEmbedding::getNumberOfNeighbors() const {
	return numNeighbors;
    }

    void BasicEmbedding::setNumberOfNeighbors(int n) {
	numNeighbors = n;
    }
    
    double BasicEmbedding::getDegree() const {
	return degree;
    }

    void BasicEmbedding::setDegree(double d) {
	degree = d;
    }
    
    void BasicEmbedding::setGraph(Graph *g) {
	graph = g;
    }

     bool BasicEmbedding::existWord(int wordId) {
        std::vector<int> words = getWords();
	
	for (Uint i = 0; i < words.size(); i++) {
		if (words[i] == wordId) return true;
	}

        return false;
    }
 
    /*bool BasicEmbedding::isAutomorphic(BasicEmbedding &other) {
        if (getNumEdges() != other.getNumEdges()) return false;
        int size = getNumEdges();

        std::vector<int> edges = getEdges();
        std::vector<int> sortedEdges(edges.size());

        std::vector<int> otherEdges = other.getEdges();
        std::vector<int> sortedOtherEdges(otherEdges.size());
	
	std::copy(edges.begin(), edges.end(), sortedEdges.begin());
	std::copy(otherEdges.begin(), otherEdges.end(), sortedOtherEdges.begin());
	std::sort(sortedEdges.begin(), sortedEdges.end());
	std::sort(sortedOtherEdges.begin(), sortedOtherEdges.end());

        int i = 0;
        while (i < size) {
            if (sortedEdges[i]!=sortedOtherEdges[i])
                return false;
            i++;
        }
        return true;
    }*/
    
    bool BasicEmbedding::isSmaller(BasicEmbedding &other) {
	
	if (this->getNumWords() != other.getNumWords())
		return (this->getNumWords() < other.getNumWords());


	std::vector<int> words = this->getWords();
	std::vector<int> otherWords = other.getWords();
	int i=0;
	for (i = 0; i < this->getNumWords(); i++) {
		if (words[i] != otherWords[i]) break;
	}

	return (i==this->getNumWords() || words[i] < otherWords[i]);

    }


    // assumes that we are replacing the vertex in idx by wordId, and wordId is a valid neighbor 
    bool BasicEmbedding::isCanonicalEmbeddingWithWordAtPosition(int wordId, int idx) {
        std::vector<int> words = getWords();
        int numWords = getNumWords();
        if (numWords == 0) return true;
	
	if (idx == 0) {
		int i = 0;
		i++;
                while (i < numWords) {
                        // If one of those ids is higher or equal, not canonical
                        if (words[i] < wordId) {
                                return false;
                        }
                        i++;
                }
		return true;
	}
	else {
		if (wordId < words[0]) return false;
		int i = 0;
		// find the first neighbor
		for (i = 0; i < idx; ++i) {
			if (areWordsNeighbours(wordId, words[i])) {
				break;
			}
		}
		// not canonical because it's disconnected regarding the previous positions
		if (idx == i)
			return false;

		// If we found the first neighbour, all following words should have lower
		// ids than the one we are trying to add
		i++;
		while (i < idx) {
			// If one of those ids is higher or equal, not canonical
			if (words[i] > wordId) {
				return false;
			}
			i++;
		}

		return true;
	}
    }


bool BasicEmbedding::isCanonicalEmbeddingWithWord(int wordId) {
	std::vector<int> words = getWords();
	int numWords = getNumWords();

	if (numWords == 0) return true;
	if (wordId < words[0]) return false;

	int i;

	// find the first neighbor
	for (i = 0; i < numWords; ++i) {
		if (areWordsNeighbours(wordId, words[i])) {
			break;
		}
	}

	// if we didn't find any neighbour
	if (i == numWords) {
		// not canonical because it's disconnected
		std::cout << "try to add a invalid vertex!! canonicality checking fail!" << std::endl;
		exit(1);
		//return false;
	}

	// If we found the first neighbour, all following words should have lower
	// ids than the one we are trying to add
	i++;
	for (; i < numWords; ++i) {
		// If one of those ids is higher or equal, not canonical
		if (words[i] >= wordId) {
			return false;
		}
	}

	return true;
}

std::string BasicEmbedding::toString() {
	/*return "Embedding{" +
	  "vertices=" + vertices + ", " +
	  "edges=" + edges +
	  "} " + super.toString();
	 */
	return std::string("bla");
}

bool BasicEmbedding::isSameEmbedding(BasicEmbedding &e) {
	if (this->getNumberOfSharedWordIds(e) == this->getNumWords()) { 
		return true;
	}
	return false;
}

bool BasicEmbedding::isSamePattern(BasicEmbedding &e) {
	if (this->getBlissCodeHashValue()!=e.getBlissCodeHashValue()) 
		return false;
	return true;
}

/*int BasicEmbedding::getNumberOfAutomorphicVertices(BasicEmbedding &embedding) {
	if (this->getBlissCodeHashValue()!=e.getBlissCodeHashValue()) 
		return 0;

	int r 
	return r;
}*/

size_t BasicEmbedding::getBlissCodeHashValue() {
	bliss::Graph bg = this->getBlissGraph();	
	return Canonical::getHash(bg);
}

size_t BasicEmbedding::getBlissCodeHashValue(std::vector<int> &ls) {
	bliss::Graph bg = this->getBlissGraph(ls);	
	return Canonical::getHash(bg);
}

int BasicEmbedding::getNumberOfSharedWordIds(BasicEmbedding &embedding) {
	std::vector<int> shared =  this->getSharedWordIds(embedding);
	return shared.size();
}

std::vector<int> BasicEmbedding::getSharedWordIds(BasicEmbedding &embedding) {
	std::vector<int> shared;

	std::vector<int> sortedWords(this->getWords());
	std::vector<int> sortedOtherWords(embedding.getWords());

	std::sort(sortedWords.begin(), sortedWords.end());
	std::sort(sortedOtherWords.begin(), sortedOtherWords.end());

	int i=0, j=0;
	while (i < (int)sortedWords.size() && j < (int)sortedOtherWords.size()) {
		if (sortedWords[i] == sortedOtherWords[j]) {
			shared.push_back(sortedWords[i]);
			i++;
			j++;
		}
		else if (sortedWords[i] < sortedOtherWords[j])
			i++;
		else
			j++;
	}
	/*
	   std::cout << "shared words: ";
	   for (int i = 0; i < shared.size(); i++) 
	   std::cout << shared[i] << " ";
	   std::cout << std::endl;
	 */
	return shared;
}

std::vector<int> BasicEmbedding::getNonSharedWordIds(BasicEmbedding &other) {
	std::vector<int> nonshared;

	std::vector<int> words = this->getWords();
	std::vector<int> otherWords = other.getWords();
	std::vector<int> sortedWords(this->getNumWords());
	std::vector<int> sortedOtherWords(other.getNumWords());

	std::copy(words.begin(), words.end(), sortedWords.begin());
	std::copy(otherWords.begin(), otherWords.end(), sortedOtherWords.begin());
	std::sort(sortedWords.begin(), sortedWords.end());
	std::sort(sortedOtherWords.begin(), sortedOtherWords.end());

	int i=0, j=0;
	while (i < (int)sortedWords.size() && j < (int)sortedOtherWords.size()) {
		if (sortedWords[i] == sortedOtherWords[j]) {
			i++;
			j++;
		}
		else if (sortedWords[i] < sortedOtherWords[j]) {
			nonshared.push_back(sortedWords[i]);
			i++;
		}
		else {
			nonshared.push_back(sortedOtherWords[i]);
			j++;
		}
	}

	while (i < (int)sortedWords.size()) {
		nonshared.push_back(sortedWords[i]);
		i++;

	}
	while (j < (int)sortedOtherWords.size()) {
		nonshared.push_back(sortedOtherWords[i]);
		j++;
	}

	/*
	   std::cout << "shared words: ";
	   for (int i = 0; i < shared.size(); i++) 
	   std::cout << shared[i] << " ";
	   std::cout << std::endl;
	 */
	return nonshared;
}

std::vector<int> BasicEmbedding::getDiffWordIds(BasicEmbedding &other) {
	std::vector<int> diff;

	std::vector<int> sortedWords(this->getWords());
	std::vector<int> sortedOtherWords(other.getWords());

	std::sort(sortedWords.begin(), sortedWords.end());
	std::sort(sortedOtherWords.begin(), sortedOtherWords.end());

	int i=0, j=0;
	while (i < (int)sortedWords.size() && j < (int)sortedOtherWords.size()) {
		if (sortedWords[i] == sortedOtherWords[j]) {
			i++;
			j++;
		}
		else if (sortedWords[i] < sortedOtherWords[j]) {
			diff.push_back(sortedWords[i]);
			i++;
		}
		else {
			j++;
		}
	}

	while (i < (int)sortedWords.size()) {
		diff.push_back(sortedWords[i]);
		i++;
	}

	return diff;
}

int BasicEmbedding::findWordPosition(int word) {
	std::vector<int> words = this->getWords();
	for (int i = 0; this->getNumWords(); i++) {
		if (words[i] == word) 
			return i;
	}
	return -1;
}


bool BasicEmbedding::isConnected() {
	return EmbeddingUtils::isConnected(*this);
}

size_t BasicEmbedding::getHash() {
	std::vector<int> words(getWords());
	std::sort(words.begin(), words.end());

	/*
	   std::cout << "Embedding to hash: "; 
	   for (int i = 0; i < words.size(); i++) {
	   std::cout << words[i]  << " ";
	   }
	   std::cout << std::endl;
	 */

	container_hash<std::vector<int>> hash;

	return hash(words);
}

int BasicEmbedding::getWordDegree(int wordId) {
	std::vector<int> words = getWords();
	int d = 0;
	for (int i = 0; i < (int)words.size(); ++i) {
		if (wordId != words[i] && areWordsNeighbours(wordId, words[i])) {
			d++;
		}
	}
	return d;
}

std::pair<size_t,int> BasicEmbedding::getWordConnectionHash(int wordId) {
        std::vector<int> words = getWords();
	size_t seed = 0;
	int n = 0;
        for (int i = 0; i < (int)words.size(); ++i) {
                if (wordId != words[i] && areWordsNeighbours(wordId, words[i])) {
			boost::hash_combine(seed,  words[i] * 2654435761);
			n++;
                }
        }
        return std::pair<size_t, int> (seed,n);
}

std::pair<size_t,int> BasicEmbedding::getWordConnectionHash(int wordId, ModSet &amods) {
        std::vector<int> words = getWords();
        size_t seed = 0;
	int n = 0;

	Mod mod(wordId, 0);
        for (int i = 0; i < (int)words.size(); ++i) {
		mod.rmId = words[i];
                if (wordId != words[i] && areWordsNeighbours(wordId, words[i]) && amods.find(mod)==amods.end()) {
                        boost::hash_combine(seed,  words[i] * 2654435761);
			n++;
                }
        }
        return std::pair<size_t, int> (seed,n);
}

void BasicEmbedding::print() {
       	std::vector<int> &vertices = getVertices();
       	std::vector<int> &edges = getEdges();
	std::cout << "Embedding: { ";
	std::cout << "vertices: [ ";
	for (int i = 0; i< (int)vertices.size(); i++) {
		std::cout << vertices[i] << " ";
	//for (std::unordered_set<int>::iterator it = vertexSet.begin(); it != vertexSet.end(); it++) {
	//	std::cout << *it << " ";
	}
	std::cout << "]";
	std::cout << " edges: [ ";
	for (int i = 0; i< (int)edges.size(); i++) {
		std::cout << edges[i] << " ";
	}
	std::cout << "]";
	std::cout << " }";
	std::cout << " HASH: " << this->getHash();
	std::cout << std::endl;
	
} 
	
void BasicEmbedding::loadFromString(std::string &s) {
		std::vector<std::string> tokenList;
		boost::split(tokenList,s, boost::is_any_of(" ,;"));
	
		for (auto& i : tokenList) {
			addWord(atoi(i.c_str()));
			//std::cout << i << " ";
		}
		//std::cout << std::endl;
}
     
void BasicEmbedding::writeWordsToFile(std::ofstream & os) {
	assert(os.is_open());
       	std::vector<int> &vertices = getVertices();
	
	os << vertices[0];
	for (int i = 1; i< (int)vertices.size(); i++)
		os << "," << vertices[i];
	os << std::endl;
}

bool BasicEmbedding::isNeighbor(BasicEmbedding &other) {
        //e1.print();
        //e2.print();

        //using subsets hash
        std::vector<size_t> subsets1 = EmbeddingUtils::getSubsetWordsHash(*this);
        std::vector<size_t> subsets2 = EmbeddingUtils::getSubsetWordsHash(other);
        std::sort(subsets1.begin(), subsets1.end());
        std::sort(subsets2.begin(), subsets2.end());
        int i=0;
        int j=0;
        while (i< (int) subsets1.size() && j< (int)subsets2.size()) {
                if (subsets1[i] == subsets2[j])
                        return true;
                else if (subsets1[i] < subsets2[j])
                        i++;
                else
                        j++;
        }
        return false;
}

std::pair<Mod, bool> BasicEmbedding::getModificationTo(BasicEmbedding &other) {
        Mod mod;

        std::vector<int> removed = this->getDiffWordIds(other);
        if (removed.size()!=1) return std::pair<Mod, bool> (mod, false);
        std::vector<int> added = other.getDiffWordIds(*this);
        if (added.size()!=1) return std::pair<Mod, bool> (mod, false);

        /*std::unordered_set<int> exps = e1.getValidElementsForExpansion();
        if (exps.find(added[0])==exps.end())
        return false;

        std::unordered_set<int> conts = e1.getValidElementsForContractionWithWord(added[0]);
        if (conts.find(removed[0])==conts.end()) 
        return false;*/

        this->replaceWord(removed[0],added[0]);
        bool r = this->isSameEmbedding(other);
        this->replaceWord(added[0],removed[0]);
        mod.addId = added[0];
        mod.rmId = removed[0];

        return std::pair<Mod, bool>(mod, r);
}
    
int BasicEmbedding::getNumberOfWordNeighbors(int id) {
	return (getWordNeighbors(id)).size();
}

NeighborhoodSet BasicEmbedding::getWordNeighbors() {
	NeighborhoodSet neighs;
	std::vector<int> words = getWords();
	for (int i = 0; this->getNumWords(); i++) {
		NeighborhoodSet localNeigh = getWordNeighbors(words[i]);
		neighs.insert(localNeigh.begin(), localNeigh.end());
	}
	return neighs;
}

void BasicEmbedding::blissTest() {
      //bliss::Stats stats;
      bliss::Graph bg = getBlissGraph();

      //bg.write_dimacs(stdout);
      //const unsigned int* cl = bg.canonical_form(stats, &report_aut, stdout);

      //fprintf(stdout, "Canonical labeling: ");
      /*bliss::print_permutation(stdout, bg.get_nof_vertices(), cl, 0);
      fprintf(stdout, "\n");
      fprintf(stdout, "[ ");
      for (int i = 0; i < bg.get_nof_vertices(); i++)
      fprintf(stdout, "%d ", cl[i]);
      fprintf(stdout, "]\n");*/
	
      //bliss::Graph *bc = bg.permute(cl);
      //bc->write_dimacs(stdout);
      //bc->get_hash();

      Canonical::getHash(bg);
}

