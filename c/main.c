// standard libs
#include <stdio.h>
#include <stdint.h>
/*#include <cstdint>*/

// custom libs
#include "lib/dbg.h"
#include "lib/vec/vec.h"
#include "lib/map-3/map-3.h"

// package header
#include "main.h"

int freeRate(rate *r) {
  if (r)
    free(r);
  r = NULL;

  if (r)
    goto error;

  return 0;

error:
  return 1;
}

int freeVecTOne(rate_ptr_vec_t_one *v) {
  if (v)
    vec_deinit(v);
  v = NULL;

  return 0;
}

int freeVecTTwo(rate_ptr_vec_t_two *v) {
  size_t i = 0;
  for (i = 0; i < v->length; i++) {
    if (v->data[i] && freeVecTOne(v->data[i]) != 0)
      return 1;

    v->data[i] = NULL;
  }

  return 0;
}

int freeVecTThree(rate_ptr_vec_t_three *v) {
  size_t i = 0;
  for (i = 0; i < v->length; i++) {
    if (v->data[i] && freeVecTTwo(v->data[i]) != 0)
      return 1;

    v->data[i] = NULL;
  }

  return 0;
}

rate_ptr_vec_t_three *arbFromRates(rate_ptr_vec_t_one *rates, size_t depth) {
  return arbFromCombos(combosFromRates(rates, depth));
}

rate_ptr_vec_t_three *arbFromCombos(rate_ptr_vec_t_three *combos) {
  rate_ptr_vec_t_three *ret = (rate_ptr_vec_t_three *) malloc(sizeof(rate_ptr_vec_t_three));
  vec_init(ret);

  size_t i = 0;
  size_t j = 0;

  for (i = 0; i < combos->length; i++) {
    rate_ptr_vec_t_two *tmp_ret = (rate_ptr_vec_t_two *) malloc(sizeof(rate_ptr_vec_t_two));
    vec_init(tmp_ret);
    vec_push(ret, tmp_ret);
  }

  for (i = 0; i < combos->length; i++) {
    for (j = 0; j < combos->data[i]->length; j++) {
      if (isArb(combos->data[i]->data[j]) && !isDupe(ret->data[i], combos->data[i]->data[j])) {
        vec_push(ret->data[i], combos->data[i]->data[j]);
      }
    }
  }

  return ret;
}

int isDupe(rate_ptr_vec_t_two *list, rate_ptr_vec_t_one *arb) {
  if (!list || list->length == 0)
    return 0;

  struct table *m = createTable(arb->length);

  size_t i = 0;
  for (i = 0; i < arb->length; i++) {
    insert(m, (uintptr_t)arb->data[i], 1);
  }

  size_t j = 0;
  int count = 0;
  for (i = 0; i < list->length; i++) {
    count = 0;
    for (j = 0; j < list->data[i]->length; j++) {
      if (lookup(m, (uintptr_t)list->data[i]->data[j]) == 1)
        count++;
    }

    if (count == list->data[i]->length) {
      free(m);
      return 1;
    }
  }

  free(m);
  return 0;
}

int isArb(rate_ptr_vec_t_one *list) {
  if (list->length < 2)
    return 0;

  if (list->data[0]->from != list->data[list->length - 1]->to)
    return 0;

  double prod = list->data[0]->rate;
  size_t i = 0;
  for (i = 1; i < list->length; i++) {
    if (list->data[i-1]->to != list->data[i]->from)
      return 0;

    prod *= list->data[i]->rate;
  }
    
  return prod > 1.0;
}

rate_ptr_vec_t_three *combosFromRates(rate_ptr_vec_t_one *rates, size_t depth) {
  rate_ptr_vec_t_three *ret = (rate_ptr_vec_t_three *) malloc(sizeof(rate_ptr_vec_t_three));
  vec_init(ret);

  size_t i = 0;
  size_t j = 0;
  size_t k = 0;
  size_t z = 0;

  rate_ptr_vec_t_two *base_rates = buildBase(rates);
  vec_push(ret, base_rates);

  for (i = 0; i < depth - 1; i++) {
    rate_ptr_vec_t_two *tmp_ret = (rate_ptr_vec_t_two *) malloc(sizeof(rate_ptr_vec_t_two));
    vec_init(tmp_ret);
    vec_push(ret, tmp_ret);
  }

	for (i = 1; i < depth; i++) {
    for (j = 0; j < ret->data[i-1]->length; j++) {
      for (k = 0; k < rates->length; k++) {
				if (ret->data[i-1]->data[j]->data[ret->data[i-1]->data[j]->length-1]->to == rates->data[k]->from && !isRateInList(ret->data[i-1]->data[j], rates->data[k]) && !isListClosing(ret->data[i-1]->data[j])) {
          rate_ptr_vec_t_one *tmp = (rate_ptr_vec_t_one *) malloc(sizeof(rate_ptr_vec_t_one));
          vec_init(tmp);
          
          for (z = 0; z < ret->data[i-1]->data[j]->length; z++) {
            vec_push(tmp, ret->data[i-1]->data[j]->data[z]);
          }
          vec_push(tmp, rates->data[k]);

          vec_push(ret->data[i], tmp);
        }
      }
    }
  }

  return ret;
}

int isListClosing(rate_ptr_vec_t_one *list) {
  if (!list || list->length < 2) {
    return 0;
  }

  return list->data[0]->from == list->data[list->length - 1]->to;
}

rate_ptr_vec_t_two *buildBase(rate_ptr_vec_t_one *rates) {
  rate_ptr_vec_t_two *ret = (rate_ptr_vec_t_two *) malloc(sizeof(rate_ptr_vec_t_two));
  vec_init(ret);

  size_t i = 0;
  size_t j = 0;
  for (i = 0; i < rates->length; i++) {
    for (j = i + 1; j < rates->length; j++) {
      if (rates->data[i]->to == rates->data[j]->from) {
        rate_ptr_vec_t_one *tmp = (rate_ptr_vec_t_one *) malloc(sizeof(rate_ptr_vec_t_one));
        vec_init(tmp);

        vec_push(tmp, rates->data[i]);
        vec_push(tmp, rates->data[j]);

        vec_push(ret, tmp);
      }
    }
  }

  return ret;
}

int isRateInList(rate_ptr_vec_t_one *list, rate *r) {
  size_t i = 0;
  for (i = 0; i < list->length; i++) {
    if (list->data[i] == r) {
      return 1;
    }
  }

  return 0;
}
