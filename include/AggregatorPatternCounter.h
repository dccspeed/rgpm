#ifndef AGGREGATORPATTCOUNT_H
#define AGGREGATORPATTCOUNT_H

#include "Aggregator.h"
#include "Stats.h"
#include <limits>
#include <cmath>     

	class AggregatorPatternCounter: public Aggregator<size_t, PatternStats>  {
		public:

		bool fncomp (size_t i, size_t j) {
			return i==j;
		};

		unsigned int hash (size_t a) { 
			std::hash<int> h; return h(a);
		};

		PatternStats aggregateValue(PatternStats &a, PatternStats &b) {
			PatternStats r;
			r.supp = a.supp + b.supp;
			r.numGroups = a.numGroups + b.numGroups;
			r.numEmbs = a.numEmbs + b.numEmbs;
		
			return r;	
		};

		
		//assumes that the parameters is a subagg
		double computeMaxDiff(Aggregator<size_t, PatternStats> &agg) {
			double maxdiff = std::numeric_limits<double>::max();
			
			typename GenericMap<size_t,PatternStats>::iterator it = map.begin();
			while (it != map.end()) {
				typename GenericMap<size_t, PatternStats>::iterator itOther = agg.map.find(it->first);
	
				double diff = .0;
				//this item does not exist in the second map
				if (itOther==agg.map.end()) 
					diff = it->second.supp;
				else 
					diff = std::abs(it->second.supp-itOther->second.supp);
	
				if (maxdiff < diff) maxdiff = diff;
				it++;
			}	
			return maxdiff;
		}
		
	};

#endif
