all: test

clean:
	-rm main_tests
	-rm ../main.o

build/deps:
	@make -C ../lib/map-3
	@make -C ../lib/vec

build: clean build/deps
	@gcc -Wall -g -c ../main.c -o ../main.o 
	@gcc -Wall -g main_tests.c ../main.o ../lib/map-3/map-3.o ../lib/vec/vec.o -o main_tests

test: build
	@./main_tests
