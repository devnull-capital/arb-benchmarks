Go vs. Rust vs. C++ arb bot tester. YMMV

# C++
In order to test the C++ code, first run `$ make gtest`.

In order to benchmark the c++ code, first run `$ make gbench`.

# Usage
## Benchmarks
Benchmarks were written in an attempt to measure each language's performance.

```bash
make bench


benchmarking go

2019/07/04 19:46:06 tmp len: 5
goos: linux
goarch: amd64
pkg: github.com/devnull-capital/arb-go-vs-rust/go
BenchmarkArbFromRates-8         2019/07/04 19:46:06 tmp len: 5
2019/07/04 19:46:08 tmp len: 5
200           7117200 ns/op
testing: BenchmarkArbFromRates-8 left GOMAXPROCS set to 1
PASS
ok      github.com/devnull-capital/arb-go-vs-rust/go    1.845s


benchmarking rust

Finished release [optimized] target(s) in 0.01s
Running /home/adam/go/src/github.com/devnull-capital/arb-go-vs-rust/rust/target/release/deps/arb-4b39695590f51228

running 8 tests
test test_arb_from_rates ... ignored
test test_build_base ... ignored
test test_combos_from_rates ... ignored
test test_is_arb ... ignored
test test_is_dupe ... ignored
test test_is_list_closing ... ignored
test test_is_rate_in_list ... ignored
test bench_arb ... bench:  10,746,280 ns/iter (+/- 1,236,243)

test result: ok. 0 passed; 0 failed; 7 ignored; 1 measured; 0 filtered out



benchmarking C++

2019-07-04 19:46:13
  Running ./cpp/arb_benchmark
Run on (8 X 2112 MHz CPU s)
  CPU Caches:
  L1 Data 32K (x4)
  L1 Instruction 32K (x4)
  L2 Unified 256K (x4)
  L3 Unified 8192K (x1)
Load Average: 0.80, 0.69, 0.68
tmp len: 5
tmp len: 5
tmp len: 5
----------------------------------------------------------
Benchmark                Time             CPU   Iterations
----------------------------------------------------------
BM_ArbFromRates    5282183 ns      5282185 ns          114


done!
```

## Time
In order to sanity check the benchmarks, because it is easy for a compiler to over-optimize, the code was run 100 times and benchmarked with GNU's `time`. This is an imperfect check, for sure, but is only meant as a sanity check.

As can be seen, this method yields wildly different results, indicating possible errors in the benchmarks. In particular, the rust benchmark does not look to be correct, but the go and C++ appear reasonable. The go and C++ timed tests are roughly 100x the avg. ns/op from the benchmark. However, the rust is orders of magnitude slower.

```bash
$ make time


timing go
2019/07/04 19:47:31 tmp len: 5
0.75user 0.00system 0:00.76elapsed 99%CPU (0avgtext+0avgdata 10284maxresident)k
0inputs+0outputs (0major+266minor)pagefaults 0swaps



warning: unused import: `test::Bencher`
--> ./rust/src/lib.rs:5:5
|
5 | use test::Bencher;
|     ^^^^^^^^^^^^^
|
= note: #[warn(unused_imports)] on by default



timing rust
tmp len: 5
35.12user 0.01system 0:35.13elapsed 100%CPU (0avgtext+0avgdata 2588maxresident)k
0inputs+0outputs (0major+203minor)pagefaults 0swaps





timing C++
tmp len: 5
0.55user 0.00system 0:00.55elapsed 100%CPU (0avgtext+0avgdata 3544maxresident)k
0inputs+0outputs (0major+216minor)pagefaults 0swaps





done!

```
