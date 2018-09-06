#include "random.h"

thread_local sfmt_t* Randness::sfmt_pt = NULL;
thread_local pcg32_random_t* Randness::pcg_pt = NULL;
thread_local boost::mt19937* Randness::mt_pt = NULL;


Randness::Randness() {
}

