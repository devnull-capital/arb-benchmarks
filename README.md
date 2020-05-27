Go vs. Rust vs. C++ arb bot tester. YMMV

# C++
In order to test the C++ code, first run `$ make gtest`.

In order to benchmark the c++ code, first install [gbench](https://github.com/google/benchmark#installation).

# Usage

## Benchmarks
Benchmarks were written in an attempt to measure each language's performance.

```bash
$ make time

\n\n timing C++
tmp len: 5

real    0m0.283s
user    0m0.276s
sys     0m0.003s
\n\n
done


\n\n timing C
[INFO] (time.c:181) tmp len: 5

real    0m0.855s
user    0m0.808s
sys     0m0.028s
\n\n
done


\n\n timing go
2020/01/26 05:16:06 tmp len: 5

real    0m2.952s
user    0m2.765s
sys     0m0.035s
\n\n
done


\n\n timing rust
tmp len: 5

real    1m32.767s
user    1m28.826s
sys     0m0.542s
\n\n
done
```
