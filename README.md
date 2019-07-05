Go vs. Rust vs. C++ arb bot tester. YMMV

# C++
In order to test the C++ code, first run `$ make gtest`.

In order to benchmark the c++ code, first run `$ make gbench`.

# Usage
## Benchmarks
Benchmarks were written in an attempt to measure each language's performance.

```bash
$ make bench                                                                                                                                        [41/3502]

benchmarking go

2019/07/05 11:06:47 tmp len: 5
goos: linux
goarch: amd64
pkg: github.com/devnull-capital/arb-go-vs-rust/go
BenchmarkArbFromRates-8         2019/07/05 11:06:47 tmp len: 5
2019/07/05 11:06:48 tmp len: 5
100          14364787 ns/op
testing: BenchmarkArbFromRates-8 left GOMAXPROCS set to 1
PASS
ok      github.com/devnull-capital/arb-go-vs-rust/go    1.905s


benchmarking rust

Finished release [optimized] target(s) in 0.09s
Running /home/adam/go/src/github.com/devnull-capital/arb-go-vs-rust/rust/target/release/deps/arb-4b39695590f51228

running 8 tests
test test_arb_from_rates ... ignored
test test_build_base ... ignored
test test_combos_from_rates ... ignored
test test_is_arb ... ignored
test test_is_dupe ... ignored
test test_is_list_closing ... ignored
test test_is_rate_in_list ... ignored
test bench_arb ... bench:  20,876,580 ns/iter (+/- 23,577,000)

test result: ok. 0 passed; 0 failed; 7 ignored; 1 measured; 0 filtered out



benchmarking C++

#@g++ -o ./cpp/arb_benchmark ./cpp/arb_benchmark.cpp ./cpp/arb.cpp -std=c++17 -Ofast -march=native -flto -fno-signed-zeros -fno-trapping-math -frename-registers -funroll-loops -lbenchmark $lpthread -isystem -I./cpp/benchmark/include -L./cpp/build/src
2019-07-05 11:07:00
  Running ./cpp/arb_benchmark
Run on (8 X 2112 MHz CPU s)
  CPU Caches:
  L1 Data 32K (x4)
  L1 Instruction 32K (x4)
  L2 Unified 256K (x4)
  L3 Unified 8192K (x1)
Load Average: 0.37, 0.31, 0.38
tmp len: 5
tmp len: 5
tmp len: 5
----------------------------------------------------------
Benchmark                Time             CPU   Iterations
----------------------------------------------------------
BM_ArbFromRates    6314294 ns      6314129 ns          100


done!
```

## Time
In order to sanity check the benchmarks, because it is easy for a compiler to over-optimize, the code was run 100 times and benchmarked with GNU's `time`. This is an imperfect check, for sure, but is only meant as a sanity check.

As can be seen, this method yields wildly different results, indicating possible errors in the benchmarks. In particular, the rust benchmark does not look to be correct, but the go and C++ appear reasonable. The go and C++ timed tests are roughly 100x the avg. ns/op from the benchmark. However, the rust is orders of magnitude slower.

```bash
$ make time


timing go
2019/07/05 11:07:10 tmp len: 5
1.38user 0.00system 0:01.42elapsed 98%CPU (0avgtext+0avgdata 10260maxresident)k
0inputs+0outputs (14major+260minor)pagefaults 0swaps



warning: unused import: `test::Bencher`
--> ./rust/src/lib.rs:5:5
|
5 | use test::Bencher;
|     ^^^^^^^^^^^^^
|
= note: #[warn(unused_imports)] on by default



timing rust
tmp len: 5
59.16user 0.00system 0:59.16elapsed 100%CPU (0avgtext+0avgdata 2420maxresident)k
0inputs+0outputs (0major+200minor)pagefaults 0swaps



#@g++ -o ./cpp/main ./cpp/main.cpp ./cpp/arb.cpp -std=c++11 -Ofast -march=native -flto -fno-signed-zeros -fno-trapping-math -frename-registers -funroll-loops


timing C++
tmp len: 5
0.60user 0.00system 0:00.60elapsed 100%CPU (0avgtext+0avgdata 3648maxresident)k
0inputs+0outputs (0major+222minor)pagefaults 0swaps





done!
```
