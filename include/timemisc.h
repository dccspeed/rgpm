#ifndef GETTIME_H
#define GETTIME_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

unsigned int timeval_subtract (struct timeval *result, struct timeval *x, struct timeval *y);
double get_time(struct timeval *start, struct timeval *end);

#endif
