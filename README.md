Go vs. Rust arb bot tester. YMMV

# Usage
```
$ make bench/go
benchmarking go

goos: linux
goarch: amd64
pkg: github.com/adam-hanna/arb-test/go
BenchmarkArbFromRates-8              500           3856434 ns/op
PASS
ok      github.com/adam-hanna/arb-test/go       2.253s


$ make bench/rust
benchmarking rust

   Compiling arb v0.1.0 (/home/adam/go/src/github.com/adam-hanna/arb-test/rust)
       Finished release [optimized] target(s) in 3.91s
            Running /home/adam/go/src/github.com/adam-hanna/arb-test/rust/target/release/deps/arb-4b39695590f51228

running 8 tests
test test_arb_from_combos ... ignored
test test_build_base ... ignored
test test_combos_from_rates ... ignored
test test_is_arb ... ignored
test test_is_dupe ... ignored
test test_is_list_closing ... ignored
test test_is_rate_in_list ... ignored
test bench_arb ... bench:  18,828,400 ns/iter (+/- 1,704,960)

test result: ok. 0 passed; 0 failed; 7 ignored; 1 measured; 0 filtered out
```
