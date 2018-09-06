#include "SuperEmbedding.h"
#include "BasicEmbeddingSpace.h"
#include "EdgeInducedEmbedding.h"
#include "VertexInducedEmbedding.h"
#include <assert.h> 
#include <utility>      // std::pair


template <class T>   
SuperEmbedding<T>::SuperEmbedding():inDegree(0) {
	embQueue.setMaxSize(MAX_QUEUE_SIZE);
	embQueue.reserve(160000);
	hasAll=false;
}

template <class T>   
SuperEmbedding<T>::SuperEmbedding(int size):inDegree(0) {
	embQueue.setMaxSize(size);
	embQueue.reserve(MAX_QUEUE_SIZE);
	hasAll=false;
}

template <class T>   
void SuperEmbedding<T>::reset() {
	inDegreeEmbeddings.clear();
	degreeEmbeddings.clear();
	code2index.clear();
	embQueue.clear();
	inDegree = 0;
	degree = 0;
	hasAll = false;
}

template <class T>   
bool SuperEmbedding<T>::isEmpty() {
	return embQueue.empty();
}

template <class T>   
int SuperEmbedding<T>::getNumberOfEmbeddings() {
	std::vector<T> embs = embQueue.getVector();
	return embs.size();
	//return embQueue.size();
}

template <class T>
bool SuperEmbedding<T>::getHasAll() {
	return hasAll;
}

template <class T>
void SuperEmbedding<T>::setHasAll(bool b) {
	hasAll = b;
}

template <class T>   
bool SuperEmbedding<T>::hasEmbedding(T &embedding) {
    size_t code  = embedding.getHash();
    std::unordered_map<size_t, int>::iterator ret = code2index.find(code);

    if (ret!=code2index.end()) {
	//std::cout << "Embedding " << code << " is in SuperNode! Embedding Number: " << ret->second << std::endl;
	//embedding.print();
    }
    //else {
//	std::cout << "Embedding " <<  code  << " is NOT in SuperNode!" << std::endl;
//    }

   return ret!=code2index.end();
}

template <class T>   
bool SuperEmbedding<T>::insertEmbeddingQueue(T &embedding) {
    size_t code  = embedding.getHash();
    if (code2index.find(code)==code2index.end()) {
	code2index.insert(std::make_pair(code, embQueue.size()));
	embQueue.insert(embedding);
	return true;
    }
    return false;
}

template <class T>   
bool SuperEmbedding<T>::insertEmbedding(T &embedding) {
    size_t code  = embedding.getHash();
    if (code2index.find(code)==code2index.end()) {
	code2index.insert(std::make_pair(code, embQueue.size()));
	embQueue.insertVectorOnly(embedding);
	return true;
    }
    return false;
}

//TODO FIX IT
template <class T>   
void SuperEmbedding<T>::removeEmbedding(T &embedding) {
    size_t code  = embedding.getHash();
    std::unordered_map<size_t, int>::iterator ret = code2index.find(code);
    if (ret!=code2index.end()) {
	embQueue.remove(embedding);	
	code2index.erase(ret);
    }
}

template <class T>   
void SuperEmbedding<T>::removeEmbeddingQueue() {
	T embedding  = embQueue.top();
    	size_t code  = embedding.getHash();
        std::unordered_map<size_t, int>::iterator ret = code2index.find(code);
	if (ret!=code2index.end()) {
		code2index.erase(ret);
		embQueue.pop();
	}
	else {
		std::cout << "error: embedding is not in index!" << std::endl;
		exit(1);
	}
}

template <class T>   
void SuperEmbedding<T>::removeEmbedding() {
	std::vector<T> embs = embQueue.getVector();
	T embedding = embs.back();	
	size_t code  = embedding.getHash();
        std::unordered_map<size_t, int>::iterator ret = code2index.find(code);
	if (ret!=code2index.end()) {
		code2index.erase(ret);
		embs.pop_back();
	}
	else {
		std::cout << "error: embedding is not in index!" << std::endl;
		exit(1);
	}
}


template <class T>   
std::vector<T> &SuperEmbedding<T>::getEmbeddings() {
	return embQueue.getVector();

}

template <class T>   
void SuperEmbedding<T>::setEmbeddings(std::vector<T> &es) {
	embQueue.setVector(es);
}

template <class T>   
double SuperEmbedding<T>::getExternalDegree() {
	return degree;
}

template <class T>   
void SuperEmbedding<T>::setExternalDegree(double n) {
	degree = n;
}

template <class T>   
double SuperEmbedding<T>::getExternalDegree(T &embedding) {
    	size_t code  = embedding.getHash();
	std::unordered_map<size_t, int>::iterator ret = code2index.find(code);
	
	if (ret == code2index.end()) return 0;
	return getExternalDegree(ret->second);
}

template <class T>   
double SuperEmbedding<T>::getExternalDegree(int n) {
	if ((int) degreeEmbeddings.size()>n)
		return degreeEmbeddings[n];
	return 0;
}

template <class T>   
double SuperEmbedding<T>::getInternalDegree(T &embedding) {
    	size_t code  = embedding.getHash();
	std::unordered_map<size_t, int>::iterator ret = code2index.find(code);
	
	if (ret == code2index.end()) return 0;
	return getInternalDegree(ret->second);
}

template <class T>   
double SuperEmbedding<T>::getInternalDegree(int n) {
	if ((int) inDegreeEmbeddings.size()>n)
		return inDegreeEmbeddings[n];
	return 0;
}
template <class T>   
double SuperEmbedding<T>::getInternalDegree() {
	return inDegree;
}

template <class T>   
void SuperEmbedding<T>::setInternalDegree(double n) {
	inDegree = n;
}

template <class T>   
void SuperEmbedding<T>::print() {
	std::vector<T> embs = embQueue.getVector();
	std::cout << "SUPERNODE" << " Size: " << embs.size() << " inDegree: " << inDegree << " Degree: " << degree << std::endl;
	
	assert(embs.size()==inDegreeEmbeddings.size());
	assert(embs.size()==degreeEmbeddings.size());
	for (int i = 0; i < (int) embs.size(); i++)  {
		std::cout << "embs[" << i << "]" << embs[i] << std::endl;
		std::cout << "InDegree: " << inDegreeEmbeddings[i] << " TotalDegree: " << degreeEmbeddings[i] << std::endl;
	}

	/*std::cout << "SN Queue: " << std::endl;
  	while (!embQueue.empty())
  	{
  	   T emb = embQueue.top();
	   emb.print();
	   std::cout << "value: " << emb.getNumberOfNeighbors() << std::endl;
  	   embQueue.pop();
  	}*/
}

template <class T>   
int SuperEmbedding<T>::getQueueSize() {
	return embQueue.size();
}

template <class T>   
int SuperEmbedding<T>::getQueueMaxSize() {
	return embQueue.getMaxSize();
}

template <class T>   
void SuperEmbedding<T>::setQueueMaxSize(int s) {
	embQueue.setMaxSize(s);
}

template <class T>   
TopQueue<T, CompareEmbeddings>& SuperEmbedding<T>::getEmbeddingQueue() {
	return embQueue;
}

template <class T>   
T  SuperEmbedding<T>::getRandomEmbedding() {
	std::vector<T> embs = embQueue.getVector();
	int n = Randness::instance().get_a_random_number(0, embs.size());
	return embs[n];
}
		
template <class T>   
T  SuperEmbedding<T>::getBiasedEmbedding() {
	double total=0;
	int n=0;
	std::vector<T> embs = embQueue.getVector();
	for (int i = 0; i < (int) embs.size(); i++)  {
		double prob = Randness::instance().random_uni01();
		double w = degreeEmbeddings[i];
		//int w = inDegreeEmbeddings[i] + degreeEmbeddings[i];
		if (w == 0) continue; 

        	if (prob < (double) w/(total+w)) {
			n = i;
		}
		total+=w;
	}
	return embs[n];
}

template <class T>   
std::pair<T,bool>  SuperEmbedding<T>::getBiasedEmbeddingWithExternalDegree() {
	double total=0;
	int n=0;
	std::vector<T> embs = embQueue.getVector();
	for (int i = 0; i < (int) embs.size(); i++)  {
		double prob = Randness::instance().random_uni01();
		double w = degreeEmbeddings[i]-inDegreeEmbeddings[i];
		if (w == 0) continue; 

        	if (prob < (double) w/(total+w)) {
			n = i;
		}
		total+=w;
	}
	return std::pair<T,bool> (embs[n],total>0);
}

template <class T>   
void  SuperEmbedding<T>::setExternalDegreeOfEmbedding(T& embedding, double degree) {
    size_t code  = embedding.getHash();
    std::unordered_map<size_t, int>::iterator ret = code2index.find(code);
    if (ret==code2index.end()) {
	return;
    }
    setExternalDegreeOfEmbedding(ret->second, degree);   
}

template <class T>   
void  SuperEmbedding<T>::setExternalDegreeOfEmbedding(int embId, double degree) {
    if (embId >=  (int) embQueue.size()) {
	std::cout << "problem in super node. number of embeddings and index conflit!" << std::endl;
	exit(1);
    }
    
    if (degreeEmbeddings.size() < embQueue.size()) {
	degreeEmbeddings.resize(embQueue.size(),0);
    }

    degreeEmbeddings[embId]=degree;
    //degree+=edges;
}

template <class T>   
void  SuperEmbedding<T>::setInternalDegreeOfEmbedding(T& embedding, double degree) {
    size_t code  = embedding.getHash();
    std::unordered_map<size_t, int>::iterator ret = code2index.find(code);
    if (ret==code2index.end()) {
	return;
    }
    setInternalDegreeOfEmbedding(ret->second, degree);   
}

template <class T>   
void  SuperEmbedding<T>::setInternalDegreeOfEmbedding(int embId, double degree) {
    if (embId >= (int) embQueue.size()) {
	std::cout << "problem in super node. number of embeddings and index conflit!" << std::endl;
	exit(1);
    }
    
    if (inDegreeEmbeddings.size() < embQueue.size()) {
	inDegreeEmbeddings.resize(embQueue.size(),0);
    }

    inDegreeEmbeddings[embId]=degree;
    //inDegree+=edges;
}

template <class T>   
void  SuperEmbedding<T>::updateEmbeddingCodeIndex() {
	code2index.clear();

	std::vector<T> &embs = embQueue.getVector();
	for (int i = 0; i < (int) embs.size(); i++)  {
    		size_t code  = embs[i].getHash();
    		if (code2index.find(code)==code2index.end()) {
			code2index.insert(std::make_pair(code, i));
		}
	}
}

template <class T>   
void  SuperEmbedding<T>::updateDegrees() {
	neighborhood.clear();
	std::vector<T> &embs = this->getEmbeddings();	
	double inDegreeTotal = 0;
	double outDegreeTotal = 0;	

	for (int i = 0; i < (int) embs.size(); i++) {
		std::vector<size_t> subsets = EmbeddingUtils::getSubsetWordsHash(embs[i]);
		for (int j = 0; j < (int) subsets.size(); j++) {
			std::unordered_map<size_t, std::vector<int>>::iterator it = neighborhood.find(subsets[j]);
			if (it==neighborhood.end()) {
				neighborhood.insert(std::make_pair(subsets[j], std::vector<int>({i})));
			}
			else {
				it->second.push_back(i);
			}
		}
	}

	for (int i = 0; i < (int) embs.size(); i++) {
		int inDegree = 0;
		std::vector<size_t> subsets = EmbeddingUtils::getSubsetWordsHash(embs[i]);
		for (int j = 0; j < (int) subsets.size(); j++) {
			std::unordered_map<size_t, std::vector<int>>::iterator it = neighborhood.find(subsets[j]);
			inDegree+=it->second.size();
		}
		inDegree-=embs[i].getNumWords();
		int outDegree = embs[i].getDegree();
		this->setInternalDegreeOfEmbedding(i, inDegree);			
		this->setExternalDegreeOfEmbedding(i, outDegree);			
		inDegreeTotal+=inDegree;
		outDegreeTotal+=outDegree;
		//std::cout << "#emb " << i << " ind " << inDegree << " outd " << outDegree << std::endl;
	}

	this->setExternalDegree(outDegreeTotal-inDegreeTotal);			
	this->setInternalDegree(inDegreeTotal/2.);			
}

template <class T>   
std::unordered_set<int> SuperEmbedding<T>::getEmbeddingsNeighborsIdx(T& e) {
	//std::vector<T> &embs = this->getEmbeddings();	
	std::unordered_set<int> idxs;
	std::vector<size_t> subsets = EmbeddingUtils::getSubsetWordsHash(e);
	for (int j = 0; j < (int) subsets.size(); j++) {
		std::unordered_map<size_t, std::vector<int>>::iterator it = neighborhood.find(subsets[j]);
		if (it == neighborhood.end()) continue;
		idxs.insert(it->second.begin(), it->second.end());
		//for (auto &i : it->second)
		//	std::cout << "emb in sn1: " << embs[i] << std::endl;
	}
	return idxs;
}

template <class T>   
T SuperEmbedding<T>::getRandomEmbeddingNeighbors(T& e) {
	int n = 0;
	int total =0;
	
	std::vector<size_t> subsets = EmbeddingUtils::getSubsetWordsHash(e);
	for (int j = 0; j < (int) subsets.size(); j++) {
		std::unordered_map<size_t, std::vector<int>>::iterator it = neighborhood.find(subsets[j]);
		double prob = Randness::instance().random_uni01();
                double w = (double) it->second.size();
                //int w = inDegreeEmbeddings[i] + degreeEmbeddings[i];
                if (w == 0) continue;

                if (prob < (double) w/(total+w)) {
                        n = j;
                }
                total+=w;
	}
	
	std::unordered_map<size_t, std::vector<int>>::iterator it = neighborhood.find(subsets[n]);

	std::vector<T> &embs = this->getEmbeddings();	
	return embs[*(Randness::instance().random_element(it->second.begin(), it->second.end()))];
}

template <class T>   
ModSet SuperEmbedding<T>::getModificationsToSN(T &e) {
	ModSet mods;

	std::vector<T> &embs = this->getEmbeddings();	
	std::unordered_set<int> idxs = getEmbeddingsNeighborsIdx(e);
		
	for (auto &j : idxs) {
		std::pair<Mod, bool> mod = e.getModificationTo(embs[j]);
		//std::cout << "emb in sn2: " << embs[j] << std::endl;
		if (mod.second) mods.insert(mod.first);
	}
	//if (mods.empty())
	//	std::cout << "warning : embedding is not a neighbor of SN." << std::endl;
	return mods;
}


template class SuperEmbedding<EdgeInducedEmbedding>;
template class SuperEmbedding<VertexInducedEmbedding>;
