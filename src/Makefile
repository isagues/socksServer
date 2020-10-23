CC = gcc
CCFLAGS = -Wall -pedantic

all:

test: hello.test request.test
	@ echo "\nHELLO TEST\n" 		> test.log
	@ ./hello.test 				>> test.log
	@ echo "\nREQUEST TEST\n"		>> test.log
	@ ./request.test				>> test.log

hello.test: hello_test.c hello.c
	@ $(CC) $(CCFLAGS) hello_test.c buffer.c -o $@

request.test: request_test.c request.c
	@ $(CC) $(CCFLAGS) request_test.c buffer.c -o $@

clean:
	@ rm *.test *.log

.PHONY: test, clean, test-hello, all