
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

sem_t sem_one, sem_two;

#define NUMBER_OF_NS_IN_ONE_S 1000000000L
#define NUMBER_OF_MS_IN_ONE_S 1000000L

double diff_time( struct timespec start, struct timespec end) {
  return ((double) end.tv_sec -(double) start.tv_sec )
    +    ((double) end.tv_nsec-(double) start.tv_nsec) / NUMBER_OF_NS_IN_ONE_S;
}

void *context_change1(void* arg){
  int max_context_change = (intptr_t) arg;

  for(int n_context_change=0; n_context_change<max_context_change; n_context_change++){
    sem_wait(&sem_one);
    sem_post(&sem_two);
  }
  return NULL;
}

void *context_change2(void* arg){
  int max_context_change = (intptr_t) arg;

  for(int n_context_change=0; n_context_change<max_context_change; n_context_change++){
    sem_wait(&sem_two);
    sem_post(&sem_one);
  }
  return NULL;
}

void bench_context_change(int max_context_change) {

  if(max_context_change < 1 && max_context_change > 10000) {
    return;
  }

  pthread_t t1;
  pthread_t t2;

  if((sem_init(&sem_one, 1, 0)) == 1){
    perror("Error initializing semaphore");
    exit(1);
  }

  if((sem_init(&sem_two, 1, 1)) == 1){
    perror("Error initializing semaphore");
    exit(1);
  }

  struct timespec timeStart, timeEnd;
  clock_gettime(CLOCK_REALTIME, &timeStart);

  pthread_create(&t1, NULL, context_change1, (void *) (intptr_t) max_context_change);
  pthread_create(&t2, NULL, context_change2, (void *) (intptr_t) max_context_change);

  pthread_join(t1, NULL);
  pthread_join(t2, NULL);

  clock_gettime(CLOCK_REALTIME, &timeEnd);

  // moyenne en ms
  double moyenne= (diff_time(timeStart, timeEnd) / max_context_change) * NUMBER_OF_MS_IN_ONE_S;

  printf("Un context_change (échantillon de %d) prend en moyenne: %lf ms.\n", max_context_change, moyenne );

}

int main (int argc, char **argv) {
  if(argc < 2) {
    printf("Usage: context_change [nombre-echantillon]\n");
    exit(1);
  }

  // Pour eviter le débordement
  int max_context_change= atoi(argv[1]);
  if(max_context_change < 1 && max_context_change > 10000) {
    printf("Nombre-echantillon incorect. (Entre 0 et 10000)\n");
    exit(1);
  }

  bench_context_change(max_context_change);

  exit(0);
}
