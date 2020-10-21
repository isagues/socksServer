CC = gcc
CCFLAGS = -Wall -pedantic


all: 


test: test-hello

test-hello: hello.c hello_test.c
	$(CC) $(CCFLAGS) hello_test.c buffer.c -o $@

clean:
	rm test-hello

.PHONY: test, clean, test-hello, all