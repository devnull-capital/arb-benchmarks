#ifndef __main_h__
#define __main_h__

#include "lib/vec/vec.h"
#include "lib/map/map.h"

typedef struct rate {
  char *from;
  char *to;
  char *exchange;
  double rate;
  double vol;
} rate;

typedef vec_t(rate *) rate_ptr_vec_t_one;
typedef vec_t(rate_ptr_vec_t_one *) rate_ptr_vec_t_two;
typedef vec_t(rate_ptr_vec_t_two *) rate_ptr_vec_t_three;

int freeRate(rate *r);
int freeVecTOne(rate_ptr_vec_t_one *v);
int freeVecTTwo(rate_ptr_vec_t_two *v);
int freeVecTThree(rate_ptr_vec_t_three *v);

rate_ptr_vec_t_three *arbFromRates(rate_ptr_vec_t_one *rates, size_t depth);
rate_ptr_vec_t_three *arbFromCombos(rate_ptr_vec_t_three *combos);
int isDupe(rate_ptr_vec_t_two *list, rate_ptr_vec_t_one *arb);
int isArb(rate_ptr_vec_t_one *list);
rate_ptr_vec_t_three *combosFromRates(rate_ptr_vec_t_one *rates, size_t depth);
int isListClosing(rate_ptr_vec_t_one *list);
rate_ptr_vec_t_two *buildBase(rate_ptr_vec_t_one *rates);
int isRateInList(rate_ptr_vec_t_one *list,rate *r);

#endif
