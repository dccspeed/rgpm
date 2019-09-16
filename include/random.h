#ifndef UTILSRANDOM_H
#define UTILSRANDOM_H

#include <ctime>
#include <cmath>
#include <vector>
#include <iostream>
#include <chrono>
#include <random>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_real_distribution.hpp>
#include <boost/random/uniform_int_distribution.hpp>
#include <tbb/recursive_mutex.h>
#include <thread>
#include "constants.h"
#include "SFMT.h"
#include "pcg_variants.h"

class Randness {

//private:
public:

//thread_local static unsigned s;
thread_local static sfmt_t* sfmt_pt;
thread_local static boost::mt19937 *mt_pt;
thread_local static pcg32_random_t *pcg_pt;

Randness();
~Randness() {
	if (sfmt_pt!=NULL) free(sfmt_pt);
	if (mt_pt!=NULL) free(mt_pt);
	if (pcg_pt!=NULL) free(pcg_pt);
} 

static sfmt_t* init_sfmt_pt() {
        std::hash<std::thread::id> hash_fn;
	int id = hash_fn(std::this_thread::get_id());
	int s = std::chrono::system_clock::now().time_since_epoch().count() * id;
	//int s = 0;
	
	sfmt_t *sfmt_pt = (sfmt_t*) malloc(sizeof(sfmt_t));
	//std::cout << "thread id: " << std::this_thread::get_id() << " seed: " << s << std::endl;
	sfmt_init_gen_rand(sfmt_pt, s);
	return sfmt_pt;
}

static boost::mt19937* init_mt_pt() {
        std::hash<std::thread::id> hash_fn;
	int id = hash_fn(std::this_thread::get_id());
	int s = std::chrono::system_clock::now().time_since_epoch().count() * id;
	//int s = 0;

	boost::mt19937 *mt_pt = new  boost::mt19937(s);
	return mt_pt;
}

static pcg32_random_t* init_pcg_pt() {
        std::hash<std::thread::id> hash_fn;
	int id = hash_fn(std::this_thread::get_id());
	int s = std::chrono::system_clock::now().time_since_epoch().count() * id;
	//int s = 0;
	
	pcg32_random_t *pcg_pt = (pcg32_random_t *) malloc(sizeof(pcg32_random_t));
	pcg32_srandom_r(pcg_pt, s, (intptr_t)&pcg_pt);
	return pcg_pt;
}


static Randness& instance() {
    static Randness *instance = new Randness();
    return *instance;
}

inline double random_uni01() {
  	//with sfmt
	double r = 0.;
	if (RANDOM_TYPE == RND_SFMT) {
		if (sfmt_pt==NULL) {
			sfmt_pt = init_sfmt_pt();
		}
		r = sfmt_genrand_real1(sfmt_pt);
	}
  	//with boost mt
	else if (RANDOM_TYPE == RND_MT) {
		if (mt_pt==NULL) {
			mt_pt = init_mt_pt();
		}
     		boost::random::uniform_real_distribution<double> distribution(0.0,1.0);
  		r = distribution(*mt_pt);
	}
	else if (RANDOM_TYPE == RND_PCG) {
		// with pcg	
		if (pcg_pt==NULL) {
			pcg_pt = init_pcg_pt();
		}
		r = ldexp(pcg32_random_r(pcg_pt), -32);
	}
	else {
		exit(1);
		std::cout << "ERROR: random type does not exist!" << std::endl;
	}
	return r;
}

inline unsigned int get_a_random_number(int lowest, int highest) {
	//std::cout << "lowest: " << lowest << " highest: " << highest << std::endl;
	if (highest-1 == lowest) {
		return lowest;		
  	} 
  	else if (highest-1 < lowest) {
	    	std::cout << "ERROR In random_number_generator: Higher value is smaller than lower" << std::endl;
	    	exit(1);
  	}
	
	double r = 0.;
	//with sfmt
	if (RANDOM_TYPE == RND_SFMT) {
		if (sfmt_pt==NULL) {
		  	sfmt_pt = init_sfmt_pt();
		}
		r = sfmt_genrand_uint32(sfmt_pt)%(highest-lowest) + lowest;
	}
	//with boost mt
	else if (RANDOM_TYPE == RND_MT) {
		if (mt_pt==NULL) {
 			mt_pt = init_mt_pt();
  		}
		boost::random::uniform_int_distribution<int> distribution(lowest,highest-1);
		r = distribution(*mt_pt);
	}
	//with pcg
	else if (RANDOM_TYPE == RND_PCG){
		if (pcg_pt==NULL) {
  			pcg_pt = init_pcg_pt();
		}
		r = pcg32_random_r(pcg_pt)%(highest-lowest) + lowest;
	}
	else {
		exit(1);
		std::cout << "ERROR: random type does not exist!" << std::endl;
	}
	return r;


}

//get random element from container
template <typename I>
inline I random_element(const I begin, const I end)
{
    const unsigned long n = std::distance(begin, end);

    I r = begin;
    std::advance(r, get_a_random_number(0,n));
    
    return r;
}

/*! \fn unsigned int randomWithDiscreteProbability(const vector<double>& accum_prob_vec) 
 		*  \brief A function to return a random number with discrete probability; pass the cum. distribution vector.
 		*  \param accum_prob_vec a constant reference of double vector.
		*	 \return an unsigned integer
 		*/
inline unsigned int randomWithDiscreteProbability(const std::vector<double>& accum_prob_vec) {
  double x = random_uni01();
  return lower_bound(accum_prob_vec.begin(), accum_prob_vec.end(), x) - 
         accum_prob_vec.begin();
}

inline unsigned int randomWithDiscreteProbability(const std::vector<int>& accum_prob_vec) {
  int highest = accum_prob_vec.back()+1;
  int x = get_a_random_number(0, highest);
  return lower_bound(accum_prob_vec.begin(), accum_prob_vec.end(), x) - 
         accum_prob_vec.begin();
}

};

#endif
