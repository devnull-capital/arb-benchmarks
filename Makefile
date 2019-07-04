all: bench

.PHONY: bench
bench: bench/go bench/rust
	@echo "done"

.PHONY: bench/go
bench/go:
	@echo "benchmarking go\n" && (cd ./go && go test -bench=.)

.PHONY: bench/rust
bench/rust:
	@echo "benchmarking rust\n" && (cd ./rust/src/ && cargo bench)

.PHONY: test
test: test/go test/rust
	@echo "done"

.PHONY: test/go
test/go:
	@echo "testing go\n" && (cd ./go && go test .)

.PHONY: test/rust
test/rust:
	@echo "testing rust\n" && (cd ./rust/src/ && cargo test)

.PHONY: build/cpp
build/cpp:
	@g++ ./cpp/main.cpp -o ./cpp/main -Ofast -march=native -flto -fno-signed-zeros -fno-trapping-math -frename-registers -funroll-loops
