Go vs. Rust vs. C++ arb bot tester. YMMV

# C++
In order to test the C++ code, first run `$ make gtest`.

In order to benchmark the c++ code, first run `$ make gbench`.

# Usage
```
$ make bench


benchmarking go

goos: linux
goarch: amd64
pkg: github.com/devnull-capital/arb-go-vs-rust/go
BenchmarkArbFromRates-8              200           6977832 ns/op
PASS
ok      github.com/devnull-capital/arb-go-vs-rust/go    2.115s


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
test bench_arb ... bench:  11,048,080 ns/iter (+/- 1,900,042)

test result: ok. 0 passed; 0 failed; 7 ignored; 1 measured; 0 filtered out



benchmarking C++

2019-07-04 17:48:02
  Running ./cpp/arb_benchmark
Run on (8 X 2112 MHz CPU s)
  CPU Caches:
  L1 Data 32K (x4)
  L1 Instruction 32K (x4)
  L2 Unified 256K (x4)
L3 Unified 8192K (x1)
Load Average: 0.52, 0.54, 0.61
----------------------------------------------------------
Benchmark                Time             CPU   Iterations
----------------------------------------------------------
BM_ArbFromRates   83076138 ns     83075287 ns            8


done!
```
