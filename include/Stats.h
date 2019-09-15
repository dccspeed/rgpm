#ifndef STATS_H 
#define STATS_H 

#include <vector>
#include <unordered_map>
#include <iostream>
#include <limits>
#include "random.h"
#include "reservoir.h"

//template <class T>
class TourStats {

	public: 
        int steps;
        bool ret;
        std::unordered_map<int, int> groups;
        std::vector<double> groupsDegree;
        std::vector<double> groupsSize;
        std::vector<size_t> groupsOwner;
	

        TourStats() {steps=0;ret=true;};

	void reset() {
		steps = 0;
		ret = true;
		groups.clear();
		groupsDegree.clear();
		groupsSize.clear();
		groupsOwner.clear();
	}

	bool empty() {return groups.empty();};
        TourStats( int _steps, std::unordered_map<int, int> & _groups, std::vector<double> & _groupsDegree, std::vector<double> & _groupsSize) {
                ret=true;
                steps=_steps;
                groups = _groups;
                groupsDegree = _groupsDegree;
                groupsSize = _groupsSize;
        };

        TourStats& operator += (const TourStats & tourStats) {
		if (tourStats.ret) {
			steps+=tourStats.steps;	
		}
		else {
                   std::cout << "EXIT: Tour stats is incomplete. Some tour did not completed. " << ret << " " << steps << " " << tourStats.ret << " "<<  tourStats.steps << std::endl;
			std::_Exit( EXIT_FAILURE );

		} 

		for (std::unordered_map<int, int>::const_iterator itP = tourStats.groups.begin(); itP!=tourStats.groups.end(); itP++) {
			//increment final results with partials
			std::unordered_map<int, int>::const_iterator it = groups.find(itP->first);
			if (it==groups.end()) {
				int nextKey = groups.size();
				groups.insert(std::make_pair(itP->first, nextKey));
				groupsDegree.push_back(tourStats.groupsDegree[itP->second]);				
				groupsSize.push_back(tourStats.groupsSize[itP->second]);
			} else {
				groupsDegree[it->second]+=tourStats.groupsDegree[itP->second];				
				groupsSize[it->second]+=tourStats.groupsSize[itP->second];
			}

		}
		return *this;
	}

        TourStats operator + (const TourStats & tourStats) {
		TourStats aggStats;

                if (tourStats.ret && this->ret) {
                        aggStats.steps=tourStats.steps+this->steps;
                }
                else {
                   std::cout << "EXIT: Tour stats is incomplete. Some tour did not completed. " << this->ret << " " << this->steps << " " << tourStats.ret << " "<<  tourStats.steps << std::endl;
                        std::_Exit( EXIT_FAILURE );

                }
                
		for (std::unordered_map<int, int>::const_iterator itP = this->groups.begin(); itP!=this->groups.end(); itP++) {
                        //increment final results with partials
                        std::unordered_map<int, int>::const_iterator it = aggStats.groups.find(itP->first);
                        if (it==aggStats.groups.end()) {
                                int nextKey = aggStats.groups.size();
                                aggStats.groups.insert(std::make_pair(itP->first, nextKey));
                                aggStats.groupsDegree.push_back(this->groupsDegree[itP->second]);
                                aggStats.groupsSize.push_back(this->groupsSize[itP->second]);
                        } else {
                                aggStats.groupsDegree[it->second]+=this->groupsDegree[itP->second];
                                aggStats.groupsSize[it->second]+=this->groupsSize[itP->second];
                        }

                }

                for (std::unordered_map<int, int>::const_iterator itP = tourStats.groups.begin(); itP!=tourStats.groups.end(); itP++) {
                        //increment final results with partials
                        std::unordered_map<int, int>::const_iterator it = aggStats.groups.find(itP->first);
                        if (it==aggStats.groups.end()) {
                                int nextKey = aggStats.groups.size();
                                aggStats.groups.insert(std::make_pair(itP->first, nextKey));
                                aggStats.groupsDegree.push_back(tourStats.groupsDegree[itP->second]);
                                aggStats.groupsSize.push_back(tourStats.groupsSize[itP->second]);
                        } else {
                                aggStats.groupsDegree[it->second]+=tourStats.groupsDegree[itP->second];
                                aggStats.groupsSize[it->second]+=tourStats.groupsSize[itP->second];
                        }

                }


                return aggStats;
        }

	void randomStats() {
			ret = true;
			steps = 1;
                        int nextKey = groups.size();
                        groups.insert(std::make_pair(0, nextKey));
                        groupsDegree.push_back(Randness::instance().get_a_random_number(20,30));
                        groupsSize.push_back(Randness::instance().get_a_random_number(10,20));

	}

	void print() {
		std::cout.setf(std::ios::fixed);
                std::cout.precision(std::numeric_limits<double>::max_digits10);
		std::cout << "TourStats=> steps: " << steps << std::endl;
		for (std::unordered_map<int, int>::const_iterator itP = groups.begin(); itP!=groups.end(); itP++) {
			std::cout << "Group[" << itP->first <<  "]: degree=" << groupsDegree[itP->second] << " size=" << groupsSize[itP->second];
			if (!groupsOwner.empty()) std::cout << " owner=" << groupsOwner[itP->second];
			std::cout << " ret: " << ret;
			std::cout << std::endl;
		}
	}
        
	friend std::ostream& operator<<(std::ostream& os, const TourStats& stats) {
		os.setf(std::ios::fixed);
                os.precision(std::numeric_limits<double>::max_digits10);
		os << "TourStats: { steps: " << stats.steps << " groups: [";
		for (std::unordered_map<int, int>::const_iterator itP = stats.groups.begin(); itP!=stats.groups.end(); itP++) {
			os << "( gid: " << itP->first <<  " degree: " << stats.groupsDegree[itP->second] << " size: " << stats.groupsSize[itP->second];
			if (!stats.groupsOwner.empty()) os << " owner: " << stats.groupsOwner[itP->second];
			os << " ret: " << stats.ret;
			os << " )";
		}
		os << "] }";
		return os;
        }

};

struct PatternStats {
        double supp;
        double numEmbs;
        double numGroups;

        PatternStats() {supp = 0; numEmbs = 0; numGroups = 0;};
        PatternStats(double _supp, double _numEmbs, double _numGroups) {supp = _supp; numEmbs = _numEmbs; numGroups = _numGroups;};

        friend std::ostream& operator<<(std::ostream& os, const PatternStats& stats) {
                os.precision(std::numeric_limits<double>::max_digits10);
		os << "{ supp: " << stats.supp << " numEmbs: " << stats.numEmbs << " numGroups: " << stats.numGroups << " }";
		return os;
        }

};

#endif
