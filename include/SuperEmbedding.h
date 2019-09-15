#ifndef SUPEREMBEDDING_H
#define SUPEREMBEDDING_H

#include "BasicEmbedding.h"
#include "EmbeddingUtils.h"
#include "graph.h"
#include <unordered_map>
#include <vector>

template <class T>
class SuperEmbedding {
   private :
	double inDegree;
	double degree;
	std::vector<double> inDegreeEmbeddings;
	std::vector<double> degreeEmbeddings;
  	std::unordered_map<size_t,int> code2index;
	TopQueue<T,CompareEmbeddings> embQueue;
	std::unordered_map<size_t, std::vector<int>> neighborhood;
	
	bool hasAll;
	const int MAX_QUEUE_SIZE=100000;
	const int MAX_ATTEMPT=10000;
   public :
	SuperEmbedding();
	SuperEmbedding(int);
	void reset();
	bool isEmpty();
	bool getHasAll();
	void setHasAll(bool);
	bool hasEmbedding(T &);
	bool insertEmbedding(T &);
	bool insertEmbeddingQueue(T &);
	void removeEmbedding(T &);
	void removeEmbedding();
	void removeEmbeddingQueue();
	std::vector<T> &getEmbeddings();
	void setEmbeddings(std::vector<T> &);
	int getNumberOfEmbeddings();
	int getQueueMaxSize();	
	int getQueueSize();	
	void setQueueMaxSize(int);	

	void  updateEmbeddingCodeIndex();
	void  updateDegrees();
	void  updateEmbeddingsUsingQueue(int );
	void print();
	double getExternalDegree();
	double getExternalDegree(T&);
	double getExternalDegree(int);
	void setExternalDegree(double);
	double getInternalDegree();
	double getInternalDegree(T&);
	double getInternalDegree(int);
	void setInternalDegree(double);
	void  setExternalDegreeOfEmbedding(T&, double);
	void  setExternalDegreeOfEmbedding(int, double);
	void  setInternalDegreeOfEmbedding(T&, double);
	void  setInternalDegreeOfEmbedding(int, double);
	T getRandomEmbedding();
	T getBiasedEmbedding();
	std::pair<T,bool>  getBiasedEmbeddingWithExternalDegree();
	TopQueue<T, CompareEmbeddings>& getEmbeddingQueue();
	std::unordered_set<int> getEmbeddingsNeighborsIdx(T&);
	ModSet getModificationsToSN(T&);
	T getRandomEmbeddingNeighbors(T&);

	friend std::ostream& operator<<(std::ostream& os, SuperEmbedding& se) {
		std::vector<T> &embs = se.getEmbeddings();
		os << "SuperEmbedding: { size: " << embs.size() << " Total InDegree: " << se.getInternalDegree() << " Total Degree: " << se.getExternalDegree() << " }";
		for (int i = 0; i < (int) embs.size(); i++)  {
			os << "SN[" << i << "]" <<  embs[i]  << " InDegree: " << se.getInternalDegree(i) << " Degree: " << se.getExternalDegree(i) << std::endl;
		}
		return os;
	}
};


#endif
