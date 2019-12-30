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
    return "failed";
  }
  r = NULL;

  log_info("succeed");
  return NULL;

error:
  log_err("could not allocate rate");
  return "failed";
}

char *all_tests() {
    mu_suite_start();

    mu_run_test(test_rate);

    return NULL;
}

RUN_TESTS(all_tests);
