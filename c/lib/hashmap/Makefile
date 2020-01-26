all: build

clean: clean/threads clean/hashmap

clean/threads:
	-rm threads.o

clean/hashmap:
	-rm hashmap.o

build: clean build/threads build/hashmap

build/threads:
	@gcc -Wall -g -c threads.c -o threads.o

build/hashmap:
	@gcc -Wall -g -c hashmap.c -o hashmap.o
