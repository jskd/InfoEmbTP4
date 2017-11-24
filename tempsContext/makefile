all: processus thread

processus: processus.c
	gcc -g -Wall --std=gnu99 -pthread -o $@  $<

thread: thread.c
	gcc -g -Wall --std=gnu99 -pthread -o $@ $<

bench-processus: processus
	taskset -c 0 ./processus 100000

bench-thread: thread
	taskset -c 0 ./thread 100000
