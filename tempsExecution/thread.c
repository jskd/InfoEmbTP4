
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>

#define NUMBER_OF_NS_IN_ONE_S 1000000000L
#define NUMBER_OF_MS_IN_ONE_S 1000000L

double diff_time( struct timespec start, struct timespec end) {
  return ((double) end.tv_sec -(double) start.tv_sec )
    +    ((double) end.tv_nsec-(double) start.tv_nsec) / NUMBER_OF_NS_IN_ONE_S;
}

void *thread_exit_func(){
  pthread_exit(NULL);
}

void bench_thread(int max_thread) {

  struct timespec timeStart, timeEnd;
  clock_gettime(CLOCK_REALTIME, &timeStart);

  pthread_t pid;

  for(int n_thread=0;  n_thread < max_thread; n_thread++){
    pthread_create(&pid, NULL, thread_exit_func, NULL);
  }

  clock_gettime(CLOCK_REALTIME, &timeEnd);

  // moyenne en ms
  double moyenne= (diff_time(timeStart, timeEnd) / max_thread) * NUMBER_OF_MS_IN_ONE_S;

  printf("Un thread (échantillon de %d) prend en moyenne: %lf ms.\n", max_thread, moyenne );

}

int main (int argc, char **argv) {
  if(argc < 2) {
    printf("Usage: thread [nombre-echantillon]\n");
    exit(1);
  }

  int max_thread= atoi(argv[1]);
  if(max_thread < 1) {
    printf("Nombre-echantillon incorect.\n");
    exit(1);
  }

  bench_thread(max_thread);

  exit(0);
}
