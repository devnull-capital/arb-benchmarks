#include "arb.hpp"
#include <benchmark/benchmark.h>

constexpr unsigned int depth = 5;

static void BM_ArbFromRates(benchmark::State& state) {
  // Perform setup here
  rate r1 = rate{"a", "b", "foo", 2.0, 0.0};
  rate r2 = rate{"a", "c", "foo", 2.0, 0.0};
  rate r3 = rate{"a", "d", "foo", 2.0, 0.0};
  rate r4 = rate{"a", "e", "foo", 2.0, 0.0};
  rate r5 = rate{"b", "a", "foo", 2.0, 0.0};
  rate r6 = rate{"b", "c", "foo", 2.0, 0.0};
  rate r7 = rate{"b", "d", "foo", 2.0, 0.0};
  rate r8 = rate{"b", "e", "foo", 2.0, 0.0};
  rate r9 = rate{"c", "a", "foo", 2.0, 0.0};
  rate r10 = rate{"c", "b", "foo", 2.0, 0.0};
  rate r11 = rate{"c", "d", "foo", 2.0, 0.0};
  rate r12 = rate{"c", "e", "foo", 2.0, 0.0};
  rate r13 = rate{"d", "a", "foo", 2.0, 0.0};
  rate r14 = rate{"d", "b", "foo", 2.0, 0.0};
  rate r15 = rate{"d", "c", "foo", 2.0, 0.0};
  rate r16 = rate{"d", "e", "foo", 2.0, 0.0};
  rate r17 = rate{"e", "a", "foo", 2.0, 0.0};
  rate r18 = rate{"e", "b", "foo", 2.0, 0.0};
  rate r19 = rate{"e", "c", "foo", 2.0, 0.0};
  rate r20 = rate{"e", "d", "foo", 2.0, 0.0};
  
  vector<rate> rates = {
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

  vector< vector< vector<rate> > > arb(depth);
  for (auto _ : state) {
    // This code gets timed
    arb = arbFromRates(rates, depth);
  }
  cout << "tmp len: " << arb.size() << "\n";
}

// Register the function as a benchmark
BENCHMARK(BM_ArbFromRates);
// Run the benchmark
BENCHMARK_MAIN();
