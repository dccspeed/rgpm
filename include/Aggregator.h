#ifndef AGGREGATOR_H
#define AGGREGATOR_H

#include <unordered_map>
#include <boost/functional/hash.hpp>
#include <functional>
#include <iostream>
#include <fstream>

template <typename KK, typename VV>
using GenericMap = std::unordered_map<
			KK,
			VV,
			std::function<unsigned long(KK)>,
			std::function<bool(KK, KK)>
			//unsigned int(*)(KK),
			//bool(*)(KK,KK)
			>;
	

template <typename KK, typename VV>
	class Aggregator {
		public:
			GenericMap<KK, VV> map;
	
		Aggregator(){
			//std::cout << "AGGREGATOR CONSTRUCTOR\n";
			//using namespace std::placeholders;
			map = GenericMap<KK,VV> (1, std::bind(&Aggregator::hash, this, std::placeholders::_1) ,std::bind(&Aggregator::fncomp, this, std::placeholders::_1, std::placeholders::_2));
		};

		Aggregator(const Aggregator &a) {
			map = a.map;
		}
		
		unsigned int getMapSize(){ 
			return map.size(); 
		};
	
		void clear() {
			map.clear();
		}

		bool aggregate(KK k, VV v) {
			typename GenericMap<KK,VV>::iterator it = map.find(k);
			if (it==map.end()) {
				map.insert(std::make_pair(k,v));
				//std::cout << "new key" << std::endl;
				return true;
			}
	
			//std::cout << "old key" << std::endl;
			it->second = aggregateValue(it->second,v);
			return false;
		};


		void aggregate(Aggregator &a) {
			typename GenericMap<KK,VV>::iterator it = a.map.begin();
			while (it != a.map.end()) {
				aggregate(it->first, it->second);	
				it++;
			}
		};

		Aggregator &operator=(const Aggregator & rhs) {
			std::cout << "Aggregator's assignment operator. " << std::endl;
			this->map = rhs.map;
			return *this;
		}

		
		void print() {
			typename GenericMap<KK,VV>::iterator it = map.begin();
			while (it != map.end()) {
				std::cout << "Aggregator Pair: " << std::endl;
				std::cout << "key: " << it->first << std::endl;
				std::cout << "value: " << it->second << std::endl;
				
				it++;
			}
		};
		
		void flush(std::ofstream &os) {
			int i = 0;
			typename GenericMap<KK,VV>::iterator it = map.begin();
			while (it != map.end()) {
				os << "Aggregator Pair: " << std::endl;
				os << "key: " << it->first << std::endl;
				os << "value: " << it->second << std::endl;
				
				it++;
				i++;
			}
			//std::cout << "flushing " << i << " keys " << getMapSize() << std::endl;
			os.flush();
		};

		virtual bool fncomp (KK lhs, KK rhs) = 0;
		virtual unsigned int hash (KK a) = 0;
		virtual VV aggregateValue (VV &a, VV &b) = 0;
		virtual double computeMaxDiff(Aggregator<KK,VV> &agg) = 0;
	};

#endif
