#include "../main.h"
#include "../lib/minunit.h"
#include "../lib/dbg.h"

char *test_rate() {
  rate *r = (rate *) malloc(sizeof(rate));
  check_mem(r)

  r->from = "BTC";
  r->to = "ETH";
  r->exchange = "binance";
  r->rate = 100.0;
  r->vol = 5.0;
  
  int i = freeRate(r);
  if (i) {
    log_err("could not free rate; i%d; r: %p", i, r);
    goto error;
  }
  r = NULL;

  return NULL;

error:
  if (r)
    freeRate(r);

  log_err("error");
  return "failed";
}

typedef struct test_is_rate_in_list_test {
  rate_ptr_vec_t_one list;
  rate *r;
  int expected;
} test_is_rate_in_list_test;
typedef vec_t(test_is_rate_in_list_test *) test_is_rate_in_list_test_ptr_vec_t_one;

char *test_is_rate_in_list() {
  rate *r = (rate *) malloc(sizeof(rate));
  check_mem(r);

  r->from = "foo";
  r->to = "bar";
  r->exchange = "baz";
  r->rate = 1.0;
  r->vol = 1.0;

  rate *r1 = (rate *) malloc(sizeof(rate));
  check_mem(r1);

  r1->from = "foo";
  r1->to = "bar";
  r1->exchange = "baz";
  r1->rate = 1.0;
  r1->vol = 1.0;

  rate *r2 = (rate *) malloc(sizeof(rate));
  check_mem(r2);

  r2->from = "baz";
  r2->to = "bar";
  r2->exchange = "foo";
  r2->rate = 4.0;
  r2->vol = 8.0;

  test_is_rate_in_list_test *t1 = (test_is_rate_in_list_test *) malloc(sizeof(test_is_rate_in_list_test));
  check_mem(t1);
  vec_init(&t1->list);
  vec_push(&t1->list, r1);
  vec_push(&t1->list, r2);
  t1->r = r;
  t1->expected = 0;

  test_is_rate_in_list_test *t2 = (test_is_rate_in_list_test *) malloc(sizeof(test_is_rate_in_list_test));
  check_mem(t2);
  vec_init(&t2->list);
  vec_push(&t2->list, r1);
  vec_push(&t2->list, r2);
  vec_push(&t2->list, r);
  t2->r = r;
  t2->expected = 1;

  test_is_rate_in_list_test_ptr_vec_t_one tests;
  vec_init(&tests);
  vec_push(&tests, t1);
  vec_push(&tests, t2);

  size_t i = 0;
  int received = 0;
  for (i = 0; i < tests.length; i++) {
    received = isRateInList(&tests.data[i]->list, tests.data[i]->r);
    if (tests.data[i]->expected != received) {
      if (r)
        freeRate(r);
      r = NULL; 

      if (r1)
        freeRate(r1);
      r1 = NULL; 

      if (r2)
        freeRate(r2);
      r2 = NULL; 

      vec_deinit(&t1->list);
      vec_deinit(&t2->list);

      log_err("test #%d failed; expected %d, received %d", i+1, tests.data[i]->expected, received);
      vec_deinit(&tests);

      return "failed";
    }
  }


  if (r)
    freeRate(r);
  r = NULL; 

  if (r1)
    freeRate(r1);
  r1 = NULL; 

  if (r2)
    freeRate(r2);
  r2 = NULL; 

  vec_deinit(&t1->list);
  vec_deinit(&t2->list);
  vec_deinit(&tests);

  return NULL;

error:
  if (r)
    freeRate(r);
  r = NULL; 

  if (r1)
    freeRate(r1);
  r1 = NULL; 

  if (r2)
    freeRate(r2);
  r2 = NULL; 

  vec_deinit(&t1->list);
  vec_deinit(&t2->list);
  vec_deinit(&tests);

  log_err("error");
  return "failed";
}

char *test_build_base() {
  // note: it should not build when to's and from's don't match
  rate *r = (rate *) malloc(sizeof(rate));
  check_mem(r);

  r->from = "foo";
  r->to = "bar";
  r->exchange = "baz";
  r->rate = 1.0;
  r->vol = 1.0;

  rate *r1 = (rate *) malloc(sizeof(rate));
  check_mem(r1);

  r1->from = "foo";
  r1->to = "bar";
  r1->exchange = "baz";
  r1->rate = 1.0;
  r1->vol = 1.0;

  rate_ptr_vec_t_one l1;
  vec_init(&l1);
  vec_push(&l1, r);
  vec_push(&l1, r1);

  rate_ptr_vec_t_two *base = buildBase(&l1);
  if (!base || base->length != 0) {
    if (!base) {
      log_err("test #1 failed; expected base");
      if (r)
        freeRate(r);
      r = NULL; 

      if (r1)
        freeRate(r1);
      r1 = NULL; 

      vec_deinit(&l1);

      if (base)
        vec_deinit(base);

      return "failed";
    }

    log_err("test #1 failed; expected 0, received %d", base->length);

    if (r)
      freeRate(r);
    r = NULL; 

    if (r1)
      freeRate(r1);
    r1 = NULL; 

    vec_deinit(&l1);

    if (base)
      vec_deinit(base);

    return "failed";
  }
  
  // note: it should build on proper input
  rate *r2 = (rate *) malloc(sizeof(rate));
  check_mem(r2);

  r2->from = "a";
  r2->to = "b";
  r2->exchange = "baz";
  r2->rate = 1.0;
  r2->vol = 1.0;

  rate *r3 = (rate *) malloc(sizeof(rate));
  check_mem(r3);

  r3->from = "b";
  r3->to = "c";
  r3->exchange = "baz";
  r3->rate = 1.0;
  r3->vol = 1.0;

  rate *r4 = (rate *) malloc(sizeof(rate));
  check_mem(r4);

  r4->from = "c";
  r4->to = "d";
  r4->exchange = "baz";
  r4->rate = 1.0;
  r4->vol = 1.0;

  rate *r5 = (rate *) malloc(sizeof(rate));
  check_mem(r5);

  r5->from = "y";
  r5->to = "z";
  r5->exchange = "baz";
  r5->rate = 1.0;
  r5->vol = 1.0;

  rate_ptr_vec_t_one l2;
  vec_init(&l2);
  vec_push(&l2, r2);
  vec_push(&l2, r3);
  vec_push(&l2, r4);
  vec_push(&l2, r5);

  rate_ptr_vec_t_two *base2 = buildBase(&l2);
  if (!base2 || base2->length != 2 || base2->data[0]->length != 2 || base2->data[1]->length != 2) {
    if (!base) {
      log_err("test #2 failed; expected base");
      if (r)
        freeRate(r);
      r = NULL; 

      if (r1)
        freeRate(r1);
      r1 = NULL; 

      if (r2)
        freeRate(r2);
      r2 = NULL; 

      if (r3)
        freeRate(r3);
      r3 = NULL; 

      if (r4)
        freeRate(r4);
      r4 = NULL; 

      if (r5)
        freeRate(r5);
      r5 = NULL; 

      vec_deinit(&l1);
      vec_deinit(&l2);

      if (base)
        vec_deinit(base);

      if (base2)
        vec_deinit(base2);

      return "failed";
    }

    log_err("test #2 failed; expected 2, received %d", base2->length);
    if (r)
      freeRate(r);
    r = NULL; 

    if (r1)
      freeRate(r1);
    r1 = NULL; 

    if (r2)
      freeRate(r2);
    r2 = NULL; 

    if (r3)
      freeRate(r3);
    r3 = NULL; 

    if (r4)
      freeRate(r4);
    r4 = NULL; 

    if (r5)
      freeRate(r5);
    r5 = NULL; 

    vec_deinit(&l1);
    vec_deinit(&l2);

    if (base)
      vec_deinit(base);

    if (base2)
      vec_deinit(base2);

    return "failed";

  }

  if (r)
    freeRate(r);
  r = NULL; 

  if (r1)
    freeRate(r1);
  r1 = NULL; 

  if (r2)
    freeRate(r2);
  r2 = NULL; 

  if (r3)
    freeRate(r3);
  r3 = NULL; 

  if (r4)
    freeRate(r4);
  r4 = NULL; 

  if (r5)
    freeRate(r5);
  r5 = NULL; 

  vec_deinit(&l1);
  vec_deinit(&l2);

  if (base)
    vec_deinit(base);

  if (base2)
    vec_deinit(base2);

  return NULL;

error:
  if (r)
    freeRate(r);
  r = NULL; 

  if (r1)
    freeRate(r1);
  r1 = NULL; 

  if (r2)
    freeRate(r2);
  r2 = NULL; 

  if (r3)
    freeRate(r3);
  r3 = NULL; 

  if (r4)
    freeRate(r4);
  r4 = NULL; 

  if (r5)
    freeRate(r5);
  r5 = NULL; 

  vec_deinit(&l1);
  vec_deinit(&l2);

  if (base)
    vec_deinit(base);

  if (base2)
    vec_deinit(base2);

  log_err("error");
  return "failed";
}

char *test_combos_from_rates() {
  // note: it should build on proper input
  rate *r2 = (rate *) malloc(sizeof(rate));
  check_mem(r2);

  r2->from = "a";
  r2->to = "b";
  r2->exchange = "baz";
  r2->rate = 1.0;
  r2->vol = 1.0;

  rate *r3 = (rate *) malloc(sizeof(rate));
  check_mem(r3);

  r3->from = "b";
  r3->to = "c";
  r3->exchange = "baz";
  r3->rate = 1.0;
  r3->vol = 1.0;

  rate *r4 = (rate *) malloc(sizeof(rate));
  check_mem(r4);

  r4->from = "c";
  r4->to = "d";
  r4->exchange = "baz";
  r4->rate = 1.0;
  r4->vol = 1.0;

  rate *r5 = (rate *) malloc(sizeof(rate));
  check_mem(r5);

  r5->from = "y";
  r5->to = "z";
  r5->exchange = "baz";
  r5->rate = 1.0;
  r5->vol = 1.0;

  rate_ptr_vec_t_one l2;
  vec_init(&l2);
  vec_push(&l2, r2);
  vec_push(&l2, r3);
  vec_push(&l2, r4);
  vec_push(&l2, r5);
  
  rate_ptr_vec_t_three *combos = combosFromRates(&l2, 4);
  /*if (!combos || combos->length != 4 || combos->data[0]->length != 2 || combos->data[0]->data[0]->length != 2 || combos->data[0]->data[1]->length != 2 || combos->data[1]->data[0]->length != 2 && combos->data[1]->data[1]->length != 3) { */
  if (!combos || combos->length != 4 || combos->data[0]->length != 2 || combos->data[0]->data[0]->length != 2 || combos->data[0]->data[1]->length != 2 || combos->data[1]->length != 1 || combos->data[1]->data[0]->length != 3) { 
    log_info("combos len: %d; 0 len: %d; 00 len: %d; 01 len: %d; 1 len: %d, 10 len: %d", combos->length, combos->data[0]->length, combos->data[0]->data[0]->length, combos->data[0]->data[1]->length, combos->data[1]->length, combos->data[1]->data[0]->length);
    if (r2)
      freeRate(r2);
    r2 = NULL; 

    if (r3)
      freeRate(r3);
    r3 = NULL; 

    if (r4)
      freeRate(r4);
    r4 = NULL; 

    if (r5)
      freeRate(r5);
    r5 = NULL; 

    vec_deinit(&l2);

    if (combos)
      vec_deinit(combos);

    return "failed";
  }

  return NULL;

error:
  if (r2)
    freeRate(r2);
  r2 = NULL; 

  if (r3)
    freeRate(r3);
  r3 = NULL; 

  if (r4)
    freeRate(r4);
  r4 = NULL; 

  if (r5)
    freeRate(r5);
  r5 = NULL; 

  vec_deinit(&l2);

  if (combos)
    vec_deinit(combos);
}

char *all_tests() {
    mu_suite_start();

    mu_run_test(test_rate);
    mu_run_test(test_is_rate_in_list);
    mu_run_test(test_build_base);
    mu_run_test(test_combos_from_rates);

    return NULL;
}

RUN_TESTS(all_tests);
