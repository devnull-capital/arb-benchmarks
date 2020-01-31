all: build

clean:
	-rm bench
	-rm ../main.o

build/deps:
	@make -C ../lib/map-3
	@make -C ../lib/vec
	@gcc -Wall -Ofast -g -c ../main.c -o ../main.o 

build: clean build/deps
	@gcc -Wall -Ofast -g time.c ../main.o ../lib/map-3/map-3.o ../lib/vec/vec.o -o time
