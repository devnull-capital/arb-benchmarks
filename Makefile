all: bench

.PHONY: bench
bench: bench/go bench/rust bench/cpp bench/c
	@echo "\n\ndone!"

.PHONY: bench/c
bench/c:
	@echo "\n\nbenchmarking c\n" && (cd ./c/benchmark && make)

.PHONY: bench/go
bench/go:
	@echo "\n\nbenchmarking go\n" && (cd ./go && go test -bench=.)

.PHONY: bench/rust
bench/rust:
	@echo "\n\nbenchmarking rust\n" && (cd ./rust/src/ && cargo bench)

.PHONY: time
time: time/go time/rust time/cpp time/c
	@echo "done"

.PHONY: test
test: test/go test/rust test/cpp test/c
	@echo "done"

.PHONY: test/c
test/c:
	@echo "testing c\n" && (cd ./c/tests && make)

.PHONY: test/go
test/go:
	@echo "testing go\n" && (cd ./go && go test .)

.PHONY: build/c
build/c:
	@(cd c/time && make)

.PHONY: build/go
build/go:
	@(cd go && go build -o main)

.PHONY: time/c
time/c: build/c
	@echo "\n\n timing c" && (cd ./c/time && make && time ./time) && echo "\n\n"

.PHONY: time/go
time/go: build/go
	@echo "\n\n timing go" && time ./go/main && echo "\n\n"

.PHONY: test/rust
test/rust:
	@echo "testing rust\n" && (cd ./rust/src/ && cargo test)

.PHONY: build/rust
build/rust:
	@rustc ./rust/src/lib.rs -o ./rust/main

.PHONY: time/rust
time/rust: build/rust
	@echo "\n\n timing rust" && time ./rust/main && echo "\n\n"

.PHONY: build/cpp
build/cpp:
	#@g++ -o ./cpp/main ./cpp/main.cpp ./cpp/arb.cpp -std=c++11 -Ofast -march=native -flto -fno-signed-zeros -fno-trapping-math -frename-registers -funroll-loops
	@g++ -o ./cpp/main ./cpp/main.cpp ./cpp/arb.cpp -std=c++11 -O3

.PHONY: time/cpp
time/cpp: build/cpp
	@echo "\n\n timing C++" && time ./cpp/main && echo "\n\n"

.PHONY: valgrind/cpp
valgrind/cpp:
	@valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose --log-file=valgrind-out.txt ./cpp/main

.PHONY: clean/cpp/test
clean/cpp/test:
	-@rm ./cpp/arb_unit_test > /dev/null || true

.PHONY: clean/cpp/bench
clean/cpp/bench:
	-@rm ./cpp/arb_benchmark > /dev/null || true

.PHONY: test/cpp
test/cpp: clean/cpp/test
	@g++ -o ./cpp/arb_unit_test ./cpp/arb_unit_test.cpp ./cpp/arb.cpp -std=c++11 -lgtest -lpthread -L./cpp/gtest/lib -I./cpp/gtest/headers
	@./cpp/arb_unit_test

.PHONY: gtest
gtest:
	@(mkdir -p cpp/gtest && cd cpp/gtest && make)

.PHONY: gbench
gbench:
	@(mkdir -p cpp/build && cd cpp/build && cmake -DCMAKE_BUILD_TYPE=Release ../benchmark/ && make)

.PHONY: bench/cpp
bench/cpp: clean/cpp/bench
	@echo "\n\nbenchmarking C++\n"
	#@g++ -o ./cpp/arb_benchmark ./cpp/arb_benchmark.cpp ./cpp/arb.cpp -std=c++17 -Ofast -march=native -flto -fno-signed-zeros -fno-trapping-math -frename-registers -funroll-loops -lbenchmark -lpthread -isystem -I./cpp/benchmark/include -L./cpp/build/src
	@g++ -o ./cpp/arb_benchmark ./cpp/arb_benchmark.cpp ./cpp/arb.cpp -std=c++17 -O3 -lbenchmark -lpthread -isystem -I./cpp/benchmark/include -L./cpp/build/src
	@./cpp/arb_benchmark
