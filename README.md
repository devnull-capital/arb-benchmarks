Go vs. Rust vs. C++ arb bot tester. YMMV

# C++
In order to test the C++ code, first run `$ make gtest`.

In order to benchmark the c++ code, first run `$ make gbench`.

# Usage
## Benchmarks
Benchmarks were written in an attempt to measure each language's performance.

```bash
$ make bench


benchmarking go

goos: linux
goarch: amd64
pkg: github.com/devnull-capital/arb-go-vs-rust/go
BenchmarkArbFromRates-8              200           7027522 ns/op
PASS
ok      github.com/devnull-capital/arb-go-vs-rust/go    2.148s


benchmarking rust

Finished release [optimized] target(s) in 0.00s
Running /home/adam/go/src/github.com/devnull-capital/arb-go-vs-rust/rust/target/release/deps/arb-4b39695590f51228

running 8 tests
test test_arb_from_rates ... ignored
test test_build_base ... ignored
test test_combos_from_rates ... ignored
test test_is_arb ... ignored
test test_is_dupe ... ignored
test test_is_list_closing ... ignored
test test_is_rate_in_list ... ignored
test bench_arb ... bench:  10,937,480 ns/iter (+/- 1,013,008)

test result: ok. 0 passed; 0 failed; 7 ignored; 1 measured; 0 filtered out



benchmarking C++

2019-07-04 17:52:17
  Running ./cpp/arb_benchmark
Run on (8 X 2112 MHz CPU s)
  CPU Caches:
  L1 Data 32K (x4)
  L1 Instruction 32K (x4)
  L2 Unified 256K (x4)
L3 Unified 8192K (x1)
Load Average: 0.85, 0.67, 0.65
----------------------------------------------------------
Benchmark                Time             CPU   Iterations
----------------------------------------------------------
BM_ArbFromRates   18122723 ns     18118112 ns           35


done!
```

## Time
In order to sanity check the benchmarks, because it is easy for a compiler to over-optimize, the code was run 100 times and benchmarked with GNU's `time`. This is an imperfect check, for sure, but is only meant as a sanity check.

As can be seen, this method yields wildly different results, indicating possible errors in the benchmarks.

```bash
$ make time


timing go
2019/07/04 18:38:46 tmp len: 5
0.72user 0.00system 0:00.72elapsed 100%CPU (0avgtext+0avgdata 10296maxresident)k
0inputs+0outputs (0major+272minor)pagefaults 0swaps



warning: unused import: `test::Bencher`
--> ./rust/src/lib.rs:5:5
|
5 | use test::Bencher;
|     ^^^^^^^^^^^^^
|
= note: #[warn(unused_imports)] on by default



timing rust
tmp len: 5
34.02user 0.01system 0:34.03elapsed 100%CPU (0avgtext+0avgdata 2448maxresident)k
0inputs+0outputs (0major+202minor)pagefaults 0swaps





timing C++
tmp len: 5
2.33user 0.00system 0:02.33elapsed 99%CPU (0avgtext+0avgdata 5516maxresident)k
0inputs+0outputs (0major+1277minor)pagefaults 0swaps





done!
```
