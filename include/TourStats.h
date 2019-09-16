#ifndef TOURSTATS_H 
#define TOURSTATS_H 

#include <vector>
#include <unordered_map>
#include <iostream>
#include <limits>
#include "random.h"
#include "reservoir.h"

template <class T>
class TourStats2 {

	public: 
	int _p;
        int steps;
        bool ret;
	double n;
        std::vector<double> degrees;
	std::vector<reservoir<T>> rs;	

        /*TourStats2() {
		_p=0;
		steps = 0;
		ret = true;
		n = 0;
	};*/
        
	TourStats2(int p) {
		_p = p;
		steps = 0;
		ret = true;
		n = 0;
		degrees.resize(p,0);
		rs.resize(p,reservoir<T>());
	};

	void reset() {
		_p = 0;
		steps = 0;
		ret = true;
		n = 0;
		degrees.clear();

		for (int i = 0; i < _p ; i++)
			rs[i].reset();
	}

        TourStats2& operator += (const TourStats2 & tourStats) {
		if (!tourStats.ret) {
                   std::cout << "EXIT: Tour stats is incomplete. Some tour did not completed. " << ret << " " << steps << " " << tourStats.ret << " "<<  tourStats.steps << std::endl;
			std::_Exit( EXIT_FAILURE );

		} 

		steps+=tourStats.steps;	
		n+=tourStats.n;

		for (int i = 0; i < _p; i++){
			if (tourStats.degrees[i]>0) {
				this->degrees[i]+=tourStats.degrees[i];
				this->rs[i]+=tourStats.rs[i];
			}
		}

		return *this;
	}

        TourStats2 operator+ (const TourStats2 & tourStats) {

                if (!tourStats.ret || !this->ret) {
                   std::cout << "EXIT: Tour stats is incomplete. Some tour did not completed. " << this->ret << " " << this->steps << " " << tourStats.ret << " "<<  tourStats.steps << std::endl;
                        std::_Exit( EXIT_FAILURE );

                }
		
		TourStats2 aggStats(tourStats._p);
                aggStats.steps=tourStats.steps+this->steps;
		aggStats.n=tourStats.n+this->n;

		for (int i = 0; i < _p; i++){
			aggStats.degrees[i] = this->degrees[i] + tourStats.degrees[i];
			aggStats.rs[i] = this->rs[i] + tourStats.rs[i];
		}

                return aggStats;
        }

	void print() {
		std::cout.setf(std::ios::fixed);
                std::cout.precision(std::numeric_limits<double>::max_digits10);
		std::cout << "TourStats: { steps: " << steps;
		std::cout << " n: " << n ;
		double degreeTotal = 0;
		uint rsSize = 0;
		for (int i = 0; i < _p; i++){
			degreeTotal+=degrees[i];
			rsSize+=rs[i].filled;
		}
		std::cout << " degree total: " << degreeTotal << " rsSize: " << rsSize;
		std::cout << "}";
	}
        
	friend std::ostream& operator<<(std::ostream& os, TourStats2& stats) {
		os.setf(std::ios::fixed);
                os.precision(std::numeric_limits<double>::max_digits10);
		os << "TourStats: { steps: " << stats.steps;
		os << " n: " << stats.n; 
		double degreeTotal = 0;
		uint rsSize = 0;
		for (int i = 0; i < stats._p; i++){
			degreeTotal+=stats.degrees[i];
			rsSize+=stats.rs[i].filled;
		}
		os << " degree total: " << degreeTotal << " rsSize: " << rsSize;
		os << "}";
		return os;
        }

};


#endif
