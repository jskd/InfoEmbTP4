/* Jérôme SKODA , Joaquim LEFRANC  TP° 4 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <string.h>

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

// only = result only
void bench_context_change(int max_context_change, char only) {

  if(max_context_change < 1 && max_context_change > 10000) {
    return;
  }


  pthread_t t1;
  pthread_t t2;

  if((sem_init(&sem_one, 0, 0)) == 1){
    perror("Error initializing semaphore");
    exit(1);
  }

  if((sem_init(&sem_two, 0, 1)) == 1){
    perror("Error initializing semaphore");
    exit(1);
  }

  max_context_change/= 2; // TODO: quick fix pour les deux changement de context fois 2

  struct timespec timeStart, timeEnd;
  clock_gettime(CLOCK_REALTIME, &timeStart);

  pthread_create(&t1, NULL, context_change1, (void *) (intptr_t) max_context_change);
  pthread_create(&t2, NULL, context_change2, (void *) (intptr_t) max_context_change);

  pthread_join(t1, NULL);
  pthread_join(t2, NULL);

  clock_gettime(CLOCK_REALTIME, &timeEnd);

  sem_destroy(&sem_one);
  sem_destroy(&sem_two);

  max_context_change*= 2; // TODO: quick fix pour les deux changement de context fois 2

  // moyenne en ms
  double moyenne= (diff_time(timeStart, timeEnd) / max_context_change) * NUMBER_OF_MS_IN_ONE_S;

  if(only)
    printf("%lf\n", moyenne );
  else
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

  char only=0;
  if(argc >= 3)
    if(strcmp(argv[2], "-o") == 0)
      only=1;

  bench_context_change(max_context_change, only);

  exit(0);
}
