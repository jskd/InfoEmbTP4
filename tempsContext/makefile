processus: processus.c
	gcc -g -Wall --std=gnu99 -o $@  $<

thread: thread.c
	gcc -g -Wall --std=gnu99  -pthread -o $@ $<

all: processus thread

bench-processus: processus
	taskset -c 0 ./processus 10000

bench-thread: thread
	taskset -c 0 ./thread 10000
