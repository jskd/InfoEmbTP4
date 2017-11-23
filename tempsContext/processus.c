
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <semaphore.h>

#define NUMBER_OF_NS_IN_ONE_S 1000000000L
#define NUMBER_OF_MS_IN_ONE_S 1000000L

double diff_time( struct timespec start, struct timespec end) {
  return ((double) end.tv_sec -(double) start.tv_sec )
    +    ((double) end.tv_nsec-(double) start.tv_nsec) / NUMBER_OF_NS_IN_ONE_S;
}

void bench_context_change(int max_context_change) {

  if(max_context_change < 1 && max_context_change > 10000) {
    return;
  }

  pid_t pid;

  sem_t sem_parent;
  sem_t sem_child;

  if((sem_init(&sem_parent, 1, 0)) == 1){
    perror("Error initializing semaphore");
    exit(1);
  }

  if((sem_init(&sem_child, 1, 1)) == 1){
    perror("Error initializing semaphore");
    exit(1);
  }

  struct timespec timeStart, timeEnd;
  clock_gettime(CLOCK_REALTIME, &timeStart);

  if((pid = fork()) < 0) {
    perror("Fork Failed");
    exit(1);
  }
  else if(pid == 0){ // child
    for(int n_context_change=0; n_context_change<max_context_change; n_context_change++){
      printf("Child wait\n");
      sem_wait(&sem_child);
      printf("Child post\n");
      sem_post(&sem_parent);
    }
    exit(0);
  }
  else { // Parent
    for(int n_context_change=0; n_context_change<max_context_change; n_context_change++){
      printf("Parent enter\n");
      sem_wait(&sem_parent);
      printf("Parent post\n");
      sem_post(&sem_child);
    }
  }

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
