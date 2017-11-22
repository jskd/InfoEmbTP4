processus: processus.c
	gcc -g -Wall --std=gnu99 -o $@ $<

thread: thread.c
	gcc -g -Wall --std=gnu99  -pthread -o $@ $<

all: processus thread

bench: all
	taskset -c 0 ./processus 10000
	taskset -c 0 ./thread 10000
