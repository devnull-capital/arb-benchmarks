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

  log_err("error");
  return "failed";
}

typedef struct test_is_arb_test {
  rate_ptr_vec_t_one list;
  int expected;
} test_is_arb_test;
typedef vec_t(test_is_arb_test *) test_is_arb_test_ptr_vec_t_one;

char *test_is_arb() {
  rate *r = (rate *) malloc(sizeof(rate));
  check_mem(r);

  r->from = "a";
  r->to = "b";
  r->exchange = "baz";
  r->rate = 1.0;
  r->vol = 1.0;

  test_is_arb_test *t1 = (test_is_arb_test *) malloc(sizeof(test_is_arb_test));
  check_mem(t1);
  vec_init(&t1->list);
  vec_push(&t1->list, r);
  t1->expected = 0;

  rate *r1 = (rate *) malloc(sizeof(rate));
  check_mem(r1);

  r1->from = "y";
  r1->to = "z";
  r1->exchange = "baz";
  r1->rate = 1.0;
  r1->vol = 1.0;

  test_is_arb_test *t2 = (test_is_arb_test *) malloc(sizeof(test_is_arb_test));
  check_mem(t2);
  vec_init(&t2->list);
  vec_push(&t2->list, r);
  vec_push(&t2->list, r1);
  t2->expected = 0;

  rate *r2 = (rate *) malloc(sizeof(rate));
  check_mem(r2);

  r2->from = "b";
  r2->to = "c";
  r2->exchange = "baz";
  r2->rate = 1.0;
  r2->vol = 1.0;

  test_is_arb_test *t3 = (test_is_arb_test *) malloc(sizeof(test_is_arb_test));
  check_mem(t3);
  vec_init(&t3->list);
  vec_push(&t3->list, r);
  vec_push(&t3->list, r2);
  t3->expected = 0;

  rate *r3 = (rate *) malloc(sizeof(rate));
  check_mem(r3);

  r3->from = "a";
  r3->to = "b";
  r3->exchange = "baz";
  r3->rate = 2.0;
  r3->vol = 1.0;

  rate *r4 = (rate *) malloc(sizeof(rate));
  check_mem(r4);

  r4->from = "b";
  r4->to = "c";
  r4->exchange = "baz";
  r4->rate = 2.0;
  r4->vol = 1.0;

  test_is_arb_test *t4 = (test_is_arb_test *) malloc(sizeof(test_is_arb_test));
  check_mem(t4);
  vec_init(&t4->list);
  vec_push(&t4->list, r3);
  vec_push(&t4->list, r4);
  t4->expected = 0;

  rate *r5 = (rate *) malloc(sizeof(rate));
  check_mem(r5);

  r5->from = "c";
  r5->to = "a";
  r5->exchange = "baz";
  r5->rate = 2.0;
  r5->vol = 1.0;

  test_is_arb_test *t5 = (test_is_arb_test *) malloc(sizeof(test_is_arb_test));
  check_mem(t5);
  vec_init(&t5->list);
  vec_push(&t5->list, r3);
  vec_push(&t5->list, r4);
  vec_push(&t5->list, r5);
  t5->expected = 1;

  rate *r6 = (rate *) malloc(sizeof(rate));
  check_mem(r6);

  r6->from = "c";
  r6->to = "a";
  r6->exchange = "baz";
  r6->rate = 1.0;
  r6->vol = 1.0;

  rate *r7 = (rate *) malloc(sizeof(rate));
  check_mem(r7);

  r7->from = "c";
  r7->to = "a";
  r7->exchange = "baz";
  r7->rate = 1.0;
  r7->vol = 1.0;

  rate *r8 = (rate *) malloc(sizeof(rate));
  check_mem(r8);

  r8->from = "c";
  r8->to = "a";
  r8->exchange = "baz";
  r8->rate = 1.0;
  r8->vol = 1.0;

  test_is_arb_test *t6 = (test_is_arb_test *) malloc(sizeof(test_is_arb_test));
  check_mem(t6);
  vec_init(&t6->list);
  vec_push(&t6->list, r6);
  vec_push(&t6->list, r7);
  vec_push(&t6->list, r8);
  t6->expected = 0;

  test_is_arb_test_ptr_vec_t_one tests;
  vec_init(&tests);
  vec_push(&tests, t1);
  vec_push(&tests, t2);
  vec_push(&tests, t3);
  vec_push(&tests, t4);
  vec_push(&tests, t5);
  vec_push(&tests, t6);

  size_t i = 0;
  int received = 0;
  for (i = 0; i < tests.length; i++) {
    received = isArb(&tests.data[i]->list);
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

      if (r3)
        freeRate(r3);
      r3 = NULL; 

      if (r4)
        freeRate(r4);
      r4 = NULL; 

      if (r5)
        freeRate(r5);
      r5 = NULL; 

      if (r6)
        freeRate(r6);
      r6 = NULL; 

      if (r7)
        freeRate(r7);
      r7 = NULL; 

      if (r8)
        freeRate(r8);
      r8 = NULL; 

      vec_deinit(&t1->list);
      vec_deinit(&t2->list);
      vec_deinit(&t3->list);
      vec_deinit(&t4->list);
      vec_deinit(&t5->list);
      vec_deinit(&t6->list);

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

  if (r3)
    freeRate(r3);
  r3 = NULL; 

  if (r4)
    freeRate(r4);
  r4 = NULL; 

  if (r5)
    freeRate(r5);
  r5 = NULL; 

  if (r6)
    freeRate(r6);
  r6 = NULL; 

  if (r7)
    freeRate(r7);
  r7 = NULL; 

  if (r8)
    freeRate(r8);
  r8 = NULL; 

  vec_deinit(&t1->list);
  vec_deinit(&t2->list);
  vec_deinit(&t3->list);
  vec_deinit(&t4->list);
  vec_deinit(&t5->list);
  vec_deinit(&t6->list);

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

  if (r6)
    freeRate(r6);
  r6 = NULL; 

  if (r7)
    freeRate(r7);
  r7 = NULL; 

  if (r8)
    freeRate(r8);
  r8 = NULL; 

  vec_deinit(&t1->list);
  vec_deinit(&t2->list);
  vec_deinit(&t3->list);
  vec_deinit(&t4->list);
  vec_deinit(&t5->list);
  vec_deinit(&t6->list);

  log_err("error");
  return "failed";
}

char *test_arb_from_combos() {
  rate *r = (rate *) malloc(sizeof(rate));
  check_mem(r);

  r->from = "a";
  r->to = "b";
  r->exchange = "baz";
  r->rate = 2.0;
  r->vol = 1.0;

  rate *r1 = (rate *) malloc(sizeof(rate));
  check_mem(r1);

  r1->from = "b";
  r1->to = "c";
  r1->exchange = "baz";
  r1->rate = 2.0;
  r1->vol = 1.0;

  rate *r2 = (rate *) malloc(sizeof(rate));
  check_mem(r2);

  r2->from = "c";
  r2->to = "a";
  r2->exchange = "baz";
  r2->rate = 2.0;
  r2->vol = 1.0;

  rate *r3 = (rate *) malloc(sizeof(rate));
  check_mem(r3);

  r3->from = "y";
  r3->to = "z";
  r3->exchange = "baz";
  r3->rate = 2.0;
  r3->vol = 1.0;

  rate_ptr_vec_t_one rates;
  vec_init(&rates);
  vec_push(&rates, r);
  vec_push(&rates, r1);
  vec_push(&rates, r2);
  vec_push(&rates, r3);

  rate_ptr_vec_t_three *arb = arbFromCombos(combosFromRates(&rates, 4));
  if (!arb || arb->length != 4 || arb->data[0]->length != 0 || arb->data[1]->length != 1 || arb->data[1]->data[0]->length != 3 || arb->data[2]->length != 0 || arb->data[3]->length != 0) {
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

  log_err("error");
  return "failed";
}

char *test_is_dupe() {
  rate *r1 = (rate *) malloc(sizeof(rate));
  check_mem(r1);

  rate *r2 = (rate *) malloc(sizeof(rate));
  check_mem(r2);

  rate *r3 = (rate *) malloc(sizeof(rate));
  check_mem(r3);

  rate_ptr_vec_t_one *arb1 = (rate_ptr_vec_t_one *) malloc(sizeof(rate_ptr_vec_t_one));
  vec_init(arb1);
  vec_push(arb1, r1);
  vec_push(arb1, r2);
  vec_push(arb1, r3);

  rate *r4 = (rate *) malloc(sizeof(rate));
  check_mem(r4);

  rate *r5 = (rate *) malloc(sizeof(rate));
  check_mem(r5);

  rate_ptr_vec_t_one *arb2 = (rate_ptr_vec_t_one *) malloc(sizeof(rate_ptr_vec_t_one));
  vec_init(arb2);
  vec_push(arb2, r4);
  vec_push(arb2, r5);

  rate *r6 = (rate *) malloc(sizeof(rate));
  check_mem(r6);

  rate *r7 = (rate *) malloc(sizeof(rate));
  check_mem(r7);

  rate_ptr_vec_t_one *arb3 = (rate_ptr_vec_t_one *) malloc(sizeof(rate_ptr_vec_t_one));
  vec_init(arb3);
  vec_push(arb3, r6);
  vec_push(arb3, r7);

  rate_ptr_vec_t_two *l1 = (rate_ptr_vec_t_two *) malloc(sizeof(rate_ptr_vec_t_two));
  check_mem(l1);
  vec_init(l1);
  vec_push(l1, arb1);
  vec_push(l1, arb2);

  int received;
  int expected;

  received = isDupe(l1, arb1);
  expected = 1;
  if (received != expected) {
    log_err("received: %d; expected %d", received, expected);
    goto failed;
  }

  rate_ptr_vec_t_two *l2 = (rate_ptr_vec_t_two *) malloc(sizeof(rate_ptr_vec_t_two));
  check_mem(l2);

  vec_init(l2);
  vec_push(l2, arb2);
  vec_push(l2, arb3);

  received = isDupe(l2, arb1);
  expected = 0;
  if (received != expected) {
    log_err("received: %d; expected %d", received, expected);
    goto failed;
  }

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

  if (r6)
    freeRate(r6);
  r6 = NULL; 

  if (r7)
    freeRate(r7);
  r7 = NULL; 

  if (arb1)
    freeRate(arb1);
  arb1 = NULL; 

  if (arb2)
    freeRate(arb2);
  arb2 = NULL; 

  if (arb3)
    freeRate(arb3);
  arb3 = NULL; 

  if (l1)
    vec_deinit(l1);

  if (l2)
    vec_deinit(l2);

  return NULL;

failed:
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

  if (r6)
    freeRate(r6);
  r6 = NULL; 

  if (r7)
    freeRate(r7);
  r7 = NULL; 

  if (arb1)
    freeRate(arb1);
  arb1 = NULL; 

  if (arb2)
    freeRate(arb2);
  arb2 = NULL; 

  if (arb3)
    freeRate(arb3);
  arb3 = NULL; 

  if (l1)
    vec_deinit(l1);

  if (l2)
    vec_deinit(l2);

  return "failed";

error:
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

  if (r6)
    freeRate(r6);
  r6 = NULL; 

  if (r7)
    freeRate(r7);
  r7 = NULL; 

  if (arb1)
    freeRate(arb1);
  arb1 = NULL; 

  if (arb2)
    freeRate(arb2);
  arb2 = NULL; 

  if (arb3)
    freeRate(arb3);
  arb3 = NULL; 

  if (l1)
    vec_deinit(l1);

  if (l2)
    vec_deinit(l2);

  log_err("error");
  return "failed";
}

char *test_is_closing() {
  rate *r1 = (rate *) malloc(sizeof(rate));
  check_mem(r1);

  r1->from = "a";
  r1->to = "b";

  rate *r2 = (rate *) malloc(sizeof(rate));
  check_mem(r2);

  r2->from = "b";
  r2->to = "a";

  rate_ptr_vec_t_one *l1 = (rate_ptr_vec_t_one *) malloc(sizeof(rate_ptr_vec_t_one));
  check_mem(l1);
  vec_init(l1);
  vec_push(l1, r1);
  vec_push(l1, r2);

  int expected;
  int received;

  received = isListClosing(l1); 
  expected = 1;
  if (received != expected) {
    log_info("received: %d, expected: %d", received, expected);
    goto failed;
  }

  rate *r3 = (rate *) malloc(sizeof(rate));
  check_mem(r3);

  r3->from = "b";
  r3->to = "c";

  rate_ptr_vec_t_one *l2 = (rate_ptr_vec_t_one *) malloc(sizeof(rate_ptr_vec_t_one));
  check_mem(l2);
  vec_init(l2);
  vec_push(l2, r1);
  vec_push(l2, r3);

  received = isListClosing(l2); 
  expected = 0;
  if (received != expected) {
    log_info("received: %d, expected: %d", received, expected);
    goto failed;
  }

  if (r1)
    free(r1);
  r1 = NULL;

  if (r2)
    free(r2);
  r2 = NULL;

  if (r3)
    free(r3);
  r3 = NULL;

  if (l1)
    vec_deinit(l1);

  if (l2)
    vec_deinit(l2);

  return NULL;

failed:
  if (r1)
    free(r1);
  r1 = NULL;

  if (r2)
    free(r2);
  r2 = NULL;

  if (r3)
    free(r3);
  r3 = NULL;

  if (l1)
    vec_deinit(l1);

  if (l2)
    vec_deinit(l2);

  return "failed";

error:
  if (r1)
    free(r1);
  r1 = NULL;

  if (r2)
    free(r2);
  r2 = NULL;

  if (r3)
    free(r3);
  r3 = NULL;

  if (l1)
    vec_deinit(l1);

  if (l2)
    vec_deinit(l2);

  log_err("err");
  return "failed";
}

char *all_tests() {
    mu_suite_start();

    mu_run_test(test_rate);
    mu_run_test(test_is_rate_in_list);
    mu_run_test(test_build_base);
    mu_run_test(test_combos_from_rates);
    mu_run_test(test_is_arb);
    mu_run_test(test_arb_from_combos);
    mu_run_test(test_is_dupe);
    mu_run_test(test_is_closing);

    return NULL;
}

RUN_TESTS(all_tests);
