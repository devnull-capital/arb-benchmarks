#include "arb.hpp"
#include <benchmark/benchmark.h>

constexpr unsigned int depth = 5;

static void BM_ArbFromRates(benchmark::State& state) {
  // Perform setup here
  arb::rate *r1 = new arb::rate{"a", "b", "foo", 2.0, 0.0};
  arb::rate *r2 = new arb::rate{"a", "c", "foo", 2.0, 0.0};
  arb::rate *r3 = new arb::rate{"a", "d", "foo", 2.0, 0.0};
  arb::rate *r4 = new arb::rate{"a", "e", "foo", 2.0, 0.0};
  arb::rate *r5 = new arb::rate{"b", "a", "foo", 2.0, 0.0};
  arb::rate *r6 = new arb::rate{"b", "c", "foo", 2.0, 0.0};
  arb::rate *r7 = new arb::rate{"b", "d", "foo", 2.0, 0.0};
  arb::rate *r8 = new arb::rate{"b", "e", "foo", 2.0, 0.0};
  arb::rate *r9 = new arb::rate{"c", "a", "foo", 2.0, 0.0};
  arb::rate *r10 = new arb::rate{"c", "b", "foo", 2.0, 0.0};
  arb::rate *r11 = new arb::rate{"c", "d", "foo", 2.0, 0.0};
  arb::rate *r12 = new arb::rate{"c", "e", "foo", 2.0, 0.0};
  arb::rate *r13 = new arb::rate{"d", "a", "foo", 2.0, 0.0};
  arb::rate *r14 = new arb::rate{"d", "b", "foo", 2.0, 0.0};
  arb::rate *r15 = new arb::rate{"d", "c", "foo", 2.0, 0.0};
  arb::rate *r16 = new arb::rate{"d", "e", "foo", 2.0, 0.0};
  arb::rate *r17 = new arb::rate{"e", "a", "foo", 2.0, 0.0};
  arb::rate *r18 = new arb::rate{"e", "b", "foo", 2.0, 0.0};
  arb::rate *r19 = new arb::rate{"e", "c", "foo", 2.0, 0.0};
  arb::rate *r20 = new arb::rate{"e", "d", "foo", 2.0, 0.0};
  
  vector<arb::rate*> rates = {
    r1,
    r2,
    r3,
    r4,
    r5,
    r6,
    r7,
    r8,
    r9,
    r10,
    r11,
    r12,
    r13,
    r14,
    r15,
    r16,
    r17,
    r18,
    r19,
    r20,
  };

  vector< vector< vector<arb::rate*> > > arbitrage(depth);
  for (auto _ : state) {
    // This code gets timed
    arbitrage = arb::arbFromRates(rates, depth);
  }
  cout << "tmp len: " << arbitrage.size() << "\n";

  delete r1;
  delete r2;
  delete r3;
  delete r4;
  delete r5;
  delete r6;
  delete r7;
  delete r8;
  delete r9;
  delete r10;
  delete r11;
  delete r12;
  delete r13;
  delete r14;
  delete r15;
  delete r16;
  delete r17;
  delete r18;
  delete r19;
  delete r20;

}

// Register the function as a benchmark
BENCHMARK(BM_ArbFromRates);
// Run the benchmark
BENCHMARK_MAIN();
