#ifndef RESERVOIR_HPP
#define RESERVOIR_HPP

#include <vector>
#include "random.h"
#include "utils.h"

// a cache which evicts the least recently used item when it is full
template<typename O>
class reservoir {

	protected:
	const static uint DEFAULT_CAPACITY = 100000;
	
	public:
	uint cpty;
	uint filled;
	uint nreplaces; 
	uint ninserts; 
	uint total; 
	double weight;
	std::vector<O> rvr;
	std::vector<double> weights;
	TopQueue<std::pair<int,double>, CompareIntDoublePairInc> weightsQ;
	
	reservoir(uint c): cpty(c), filled(0), nreplaces(0), ninserts(0), total(0), weight(0) { 
		//rvr.resize(cpty);
		//weights.resize(cpty);
		weightsQ.setMaxSize(cpty);
	};

	reservoir(): cpty(DEFAULT_CAPACITY), filled(0), nreplaces(0), ninserts(0), total(0), weight(0) { 
		//rvr.resize(cpty);
		//weights.resize(cpty);
		weightsQ.setMaxSize(cpty);
	};

	~reservoir() { 
	};

	bool empty() const { return filled==0; };
	
	/*inline bool insert(O obj) {
		total++;
		if (ninserts < cpty) {
			rvr[filled] = obj;
			ninserts++;
			filled++;
			return true;
		}
		
		// Pick a random index from 0 to i. 
		uint j = Randness::instance().get_a_random_number(0, total+1);
	
		if (j < cpty) {
			nreplaces++; 
			ninserts++;
			rvr[j] = obj;
			return true;
		}
		return false;
	};*/
	
	inline bool insert(O obj) {
		return insert(obj, 1);
	}

	/*inline bool insert(O obj, double w) {
                total++;
		weight += w/total;
               
		 if (ninserts < cpty) {
                        rvr[filled] = obj;
                        weights[filled] = w;
                        ninserts++;
                        filled++;
                        return true;
                }
                

		double p = w/weight;		
		double j = Randness::instance().random_uni01();
                if (j <= p) {
			uint k = Randness::instance().get_a_random_number(0, cpty);
                        nreplaces++; 
                        ninserts++;
                        rvr[k] = obj;
                        weights[k] = w;
                        return true;
                }
                return false;
        }*/
	
	inline bool insert(O obj, double w) {
                total++;
                weight+=w;
		double new_w = std::pow(Randness::instance().random_uni01(), 1/w);               
	
                 if (ninserts < cpty) {
			std::pair<int, double> w_item (filled, new_w);
			weightsQ.insert(w_item);
                        rvr.push_back(obj);
                        weights.push_back(w);
                        ninserts++;
                        filled++;
                        return true;
                }
	    	std::pair<int, double> top = weightsQ.top();
		if (top.second < new_w) {
			//std::cout << "inserting new weight:" << new_w << " removing:" << top.second << std::endl;
			std::pair<int, double> w_item (top.first, new_w);
			weightsQ.insert(w_item); // will discard the smallest
                        rvr[top.first] = obj;
                        weights[top.first] = w;
			nreplaces++; 
                        ninserts++;
                        return true;

		}
		//std::cout << "new weight: " << new_w << std::endl;
		//std::cout << "top reservoir: " << top.second << std::endl;
                
		return false;
			
	}

	void reset() {
		filled = 0;
		nreplaces = 0;
		ninserts = 0;
		total = 0;
		weight = 0;
		weightsQ.clear();
		rvr.clear();
		weights.clear();
	}

	O random() const {
		if (filled <=0) {
			std::cout << "error: impossible to get a random element in reservoir" << std::endl;
			exit(1);
		}
		uint j = Randness::instance().get_a_random_number(0, filled);
		//std::cout << "getting random element in reservoir position: " << j << std::endl;
		return rvr[j];
	}

	void copy(reservoir<O> r) {
		filled = r.filled;
		nreplaces = r.nreplaces;
		ninserts = r.ninserts;
		total = r.total;
		weight = r.weight;
		rvr = r.rvr;
		weights = r.weights;
		weightsQ = r.weightsQ;
	}

	/*reservoir<O> operator+ (const reservoir<O> &r2) {
		if (this->cpty != r2.cpty) {
			std::cout << "error: can not merge the reservoirs!" << std::endl;
			exit(1);
		}
		reservoir<O> r;

		uint _filled = this->filled + r2.filled;
		uint _ninserts = this->ninserts + r2.ninserts;
		uint _nreplaces = this->nreplaces + r2.nreplaces;
		uint _total = this->total + r2.total;
		double _weight = this->weight + r2.weight;

		//r2 is not a full sample
		if (r2.cpty > r2.filled) {
			r.copy(*this);
			for(uint i = 0; i < r2.filled; ++i) 
				r.insert(r2.rvr[i], r2.weights[i]);
		}
		//this is not a full sample
		else if (this->cpty > this->filled) {
			r.copy(r2);
			for(uint i = 0; i < this->filled; ++i) 
				r.insert(this->rvr[i], this->weights[i]);

		}
		//they are full samples
		else {
			double p = (double) r2.weights / (r2.weight+this->weight);
			for(uint i = 0; i < r.cpty; ++i) {
				double j = Randness::instance().random_uni01();
				int k = Randness::instance().get_a_random_number(0, cpty);
				if(j > p)
					r.insert(this->rvr[k], this->weights[k]);
				else
					r.insert(r2.rvr[k], r2.weights[k]);
			}
		}

		r.filled = std::min(cpty, _filled);
		r.ninserts = _ninserts;
		r.nreplaces = _nreplaces;
		r.total = _total;
		r.weight = _weight;	
		
		return r;
	}
	
	reservoir<O>& operator+= (const reservoir<O> &r2) {
		if (this->cpty != r2.cpty) {
			std::cout << "error: can not merge the reservoirs!" << std::endl;
			exit(1);
		}
		
		uint _filled = this->filled + r2.filled;
		uint _ninserts = this->ninserts + r2.ninserts;
		uint _nreplaces = this->nreplaces + r2.nreplaces;
		uint _total = this->total + r2.total;
		double _weight = this->weight + r2.weight;
	
		//r2 is not a full sample
		if (r2.cpty > r2.filled) {
			for(uint i = 0; i < r2.filled; ++i) 
				this->insert(r2.rvr[i], r2.weights[i]);
		}
		//this is not a full sample
		else if (this->cpty > this->filled) {
			reservoir<O> r(cpty);
			r.copy(*this);
			this->copy(r2);
			for(uint i = 0; i < r.filled; ++i) 
				this->insert(r.rvr[i], r.weights[i]);
		}
		//they are full samples
		else {
			reservoir<O> r(cpty);
			double p = (double) r2.weight / (r2.weight+this->weight);
			for(uint i = 0; i < cpty; ++i) {
				double j = Randness::instance().random_uni01();
				int k = Randness::instance().get_a_random_number(0, cpty);
				if(j > p)
					r.insert(this->rvr[k], this->weights[k]);
				else
					r.insert(r2.rvr[k], r2.weights[k]);
			}
			//replace vectors of objects and weights
			this->rvr = r.rvr;
			this->weights = r.weights;
		}

		this->filled = std::min(cpty, _filled);
		this->ninserts = _ninserts;
		this->nreplaces = _nreplaces;
		this->total = _total;	
		this->weight = _weight;	
		return *this;
	}*/
	
	reservoir<O> operator+ (const reservoir<O> &r2) {
		if (this->cpty != r2.cpty) {
			std::cout << "error: can not merge the reservoirs!" << std::endl;
			exit(1);
		}
		
		if (this->filled==0) {
			//reservoir<O> r = *this;
			return r2;
		}
		else if (r2.filled==0) {
			//reservoir<O> r = r2;
			return *this;
		}


		reservoir<O> r(this->cpty);
		
		uint _filled = this->filled + r2.filled;
		uint _ninserts = this->ninserts + r2.ninserts;
		uint _nreplaces = this->nreplaces + r2.nreplaces;
		uint _total = this->total + r2.total;
		double _weight = this->weight + r2.weight;

		// generate new random numbers remake heap	
		const std::vector<std::pair<int,double>> heap = this->weightsQ.getVector();
		for (std::pair<int,double> w_item : heap) {	
			double new_w = std::pow(Randness::instance().random_uni01(), 1/this->weights[w_item.first]);               
			r.insert(this->rvr[w_item.first], new_w);
		}

		//insert the items in the second reservoir
		const std::vector<std::pair<int,double>> heap2 = r2.weightsQ.getVectorConst();
		for (std::pair<int,double> w_item : heap2) {	
			double new_w = std::pow(Randness::instance().random_uni01(), 1/r2.weights[w_item.first]);               
			r.insert(r2.rvr[w_item.first], new_w);
		}
		
		r.filled = std::min(cpty, _filled);
		r.ninserts = _ninserts;
		r.nreplaces = _nreplaces;
		r.total = _total;	
		r.weight = _weight;	
		
		return r;
	}

	reservoir<O>& operator+= (const reservoir<O> &r2) {
		if (this->cpty != r2.cpty) {
			std::cout << "error: can not merge the reservoirs!" << std::endl;
			exit(1);
		}
	
		if (r2.filled==0) return *this;
 	
		uint _filled = this->filled + r2.filled;
		uint _ninserts = this->ninserts + r2.ninserts;
		uint _nreplaces = this->nreplaces + r2.nreplaces;
		uint _total = this->total + r2.total;
		double _weight = this->weight + r2.weight;

		// generate new random numbers remake heap	
		std::vector<std::pair<int,double>> &heap = weightsQ.getVector();
		for (std::pair<int,double> w_item : heap) {	
			w_item.second = std::pow(Randness::instance().random_uni01(), 1/this->weights[w_item.first]);               
		}
		this->weightsQ.remakeHeap();		

		//insert the items in the second reservoir
		std::vector<std::pair<int,double>> heap2 = r2.weightsQ.getVectorConst();
		for (std::pair<int,double> w_item : heap2) {	
			double new_w = std::pow(Randness::instance().random_uni01(), 1/r2.weights[w_item.first]);               
			this->insert(r2.rvr[w_item.first], new_w);
		}
	
		this->filled = std::min(cpty, _filled);
		this->ninserts = _ninserts;
		this->nreplaces = _nreplaces;
		this->total = _total;	
		this->weight = _weight;	
		return *this;
	}

	void print () {
		std::cout << "reservoir: ninserts=" << ninserts << " nreplaces=" << nreplaces << " total=" << total << " filled=" << filled << " capacity="<< cpty << " weight=" << weight << std::endl;
		for(uint i = 0; i < filled; ++i) {
			std::cout << rvr[i] << std::endl;	
		}	
	}
	
	friend std::ostream& operator<<(std::ostream& os, reservoir<O>& r) {
		os << "reservoir: ninserts=" << r.ninserts << " nreplaces=" << r.nreplaces << " total=" << r.total << " filled=" << r.filled << " capacity="<< r.cpty <<  " weight=" << r.weight << std::endl;
		for(uint i = 0; i < r.filled; ++i) {
			std::cout << r.rvr[i] << std::endl;	
		}	
	
		return os;	
	}
};


#endif 
