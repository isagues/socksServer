CC = gcc
CCFLAGS = -Wall -pedantic

all: 
	echo $(TEST_SOURCE)

test: hello.test

hello.test: hello_test.c hello.c
	$(CC) $(CCFLAGS) hello_test.c buffer.c -o $@

clean:
	rm *.test

.PHONY: test, clean, test-hello, all