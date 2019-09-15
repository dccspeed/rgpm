#ifndef PATTERNCODE_H
#define PATTERNCODE_H

#include "constants.h"
#include "utils.h"
#include <vector>
#include <boost/functional/hash.hpp>



struct PatternItem {
	int srcId;
	int srcLabel;
	int destId;
	int destLabel;
	bool type;

	PatternItem():srcId(0), srcLabel(0), destId(0), destLabel(0), type(true) {};
	PatternItem(int sId, int sLabel, int dId, int dLabel, bool t) {
		srcId = sId;
		srcLabel = sLabel;
		destId = dId;
		destLabel = dLabel;
		type = t;
	}

	static bool comparePatternItemEqual(const PatternItem &, const PatternItem &);
	static bool comparePatternItem(const PatternItem &, const PatternItem &);
	int compareTo(const PatternItem &) const; 
	void print() const;
 	PatternItem & operator=(const PatternItem &other) {
		srcId = other.srcId;
		srcLabel = other.srcLabel;
		destId = other.destId;
		destLabel = other.destLabel;
		type = other.type;
		return *this;
	}


};

struct PatternCode:public std::vector<PatternItem> {

	public :
		std::vector<int> vertexMap;

	public :
		PatternCode():std::vector<PatternItem>(){
			//vertexMap.clear();
			//this->clear();
		};

		void reset() {this->clear(); vertexMap.clear();};
		int compareTo(const PatternCode &) const;
		bool operator==(const PatternCode &) const;

		std::pair<int, bool> getIndexMap(int);
		bool add(int srcId, int srcLabel, int destId, int destLabel);
		void removeLast();
		std::list<int> getRightMostPathExtensibleIds();

		PatternCode::iterator find(int srcId, int destId);
		void print();
};

namespace std {
	template <>
		struct hash<std::pair<int, int>>
		{
			std::size_t operator()(const std::pair<int, int>& i) const
			{
				size_t seed = 0;
				boost::hash_combine(seed, i.first);
				boost::hash_combine(seed, i.second);

				return seed;
			}
		};

	template <>
		struct hash<PatternItem>
		{
			std::size_t operator()(const PatternItem& i) const
			{
				size_t seed = 0;
				boost::hash_combine(seed, i.srcId);
				boost::hash_combine(seed, i.srcLabel);
				boost::hash_combine(seed, i.destId);
				boost::hash_combine(seed, i.destLabel);
				boost::hash_combine(seed, i.destLabel);
				boost::hash_combine(seed, i.type);

				return seed;
			}
		};

	template <>
		struct hash<PatternCode>
		{
			std::size_t operator()(const PatternCode& code) const
			{
				size_t seed = 0;
				
				std::hash<PatternItem> hasher;
				for(auto& i : code) {
					boost::hash_combine(seed, hasher(i));
				}

				return seed;
			}
		};

}

#endif
