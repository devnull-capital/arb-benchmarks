// standard libs
#include <sys/time.h>
#include <sys/resource.h>

// local libs
#include "../main.h"
#include "../lib/dbg.h"

int main() {
  rate *r1 = (rate *) malloc(sizeof(rate));
  check_mem(r1);

  r1->from = "a";
  r1->to = "b";
  r1->rate = 2.0;

  rate *r2 = (rate *) malloc(sizeof(rate));
  check_mem(r2);

  r2->from = "a";
  r2->to = "c";
  r2->rate = 2.0;

  rate *r3 = (rate *) malloc(sizeof(rate));
  check_mem(r3);

  r3->from = "a";
  r3->to = "d";
  r3->rate = 2.0;

  rate *r4 = (rate *) malloc(sizeof(rate));
  check_mem(r4);

  r4->from = "a";
  r4->to = "e";
  r4->rate = 2.0;

  rate *r5 = (rate *) malloc(sizeof(rate));
  check_mem(r5);

  r5->from = "b";
  r5->to = "a";
  r5->rate = 2.0;

  rate *r6 = (rate *) malloc(sizeof(rate));
  check_mem(r6);

  r6->from = "b";
  r6->to = "c";
  r6->rate = 2.0;

  rate *r7 = (rate *) malloc(sizeof(rate));
  check_mem(r7);

  r7->from = "b";
  r7->to = "d";
  r7->rate = 2.0;

  rate *r8 = (rate *) malloc(sizeof(rate));
  check_mem(r8);

  r8->from = "b";
  r8->to = "e";
  r8->rate = 2.0;

  rate *r9 = (rate *) malloc(sizeof(rate));
  check_mem(r9);

  r9->from = "c";
  r9->to = "a";
  r9->rate = 2.0;

  rate *r10 = (rate *) malloc(sizeof(rate));
  check_mem(r10);

  r10->from = "c";
  r10->to = "b";
  r10->rate = 2.0;

  rate *r11 = (rate *) malloc(sizeof(rate));
  check_mem(r11);

  r11->from = "c";
  r11->to = "d";
  r11->rate = 2.0;

  rate *r12 = (rate *) malloc(sizeof(rate));
  check_mem(r12);

  r12->from = "c";
  r12->to = "e";
  r12->rate = 2.0;

  rate *r13 = (rate *) malloc(sizeof(rate));
  check_mem(r13);

  r13->from = "d";
  r13->to = "a";
  r13->rate = 2.0;

  rate *r14 = (rate *) malloc(sizeof(rate));
  check_mem(r14);

  r14->from = "d";
  r14->to = "b";
  r14->rate = 2.0;

  rate *r15 = (rate *) malloc(sizeof(rate));
  check_mem(r15);

  r15->from = "d";
  r15->to = "c";
  r15->rate = 2.0;

  rate *r16 = (rate *) malloc(sizeof(rate));
  check_mem(r16);

  r16->from = "d";
  r16->to = "e";
  r16->rate = 2.0;

  rate *r17 = (rate *) malloc(sizeof(rate));
  check_mem(r17);

  r17->from = "e";
  r17->to = "a";
  r17->rate = 2.0;

  rate *r18 = (rate *) malloc(sizeof(rate));
  check_mem(r18);

  r18->from = "e";
  r18->to = "b";
  r18->rate = 2.0;

  rate *r19 = (rate *) malloc(sizeof(rate));
  check_mem(r19);

  r19->from = "e";
  r19->to = "c";
  r19->rate = 2.0;

  rate *r20 = (rate *) malloc(sizeof(rate));
  check_mem(r20);

  r20->from = "e";
  r20->to = "d";
  r20->rate = 2.0;

  rate_ptr_vec_t_one *l = (rate_ptr_vec_t_one *) malloc(sizeof(rate_ptr_vec_t_one));
  check_mem(l);
  vec_init(l);
  vec_push(l, r1);
  vec_push(l, r2);
  vec_push(l, r3);
  vec_push(l, r4);
  vec_push(l, r5);
  vec_push(l, r6);
  vec_push(l, r7);
  vec_push(l, r8);
  vec_push(l, r9);
  vec_push(l, r10);
  vec_push(l, r11);
  vec_push(l, r12);
  vec_push(l, r13);
  vec_push(l, r14);
  vec_push(l, r15);
  vec_push(l, r16);
  vec_push(l, r17);
  vec_push(l, r18);
  vec_push(l, r19);
  vec_push(l, r20);

  size_t i;
  rate_ptr_vec_t_three *tmp = (rate_ptr_vec_t_three *) malloc(sizeof(rate_ptr_vec_t_three));

  for (i = 0; i < 100; i++) {
    tmp = arbFromRates(l, 5);
  }

  log_info("tmp len: %d", tmp->length);

  return 0;

error:
  log_err("err");
  return 1;
}
