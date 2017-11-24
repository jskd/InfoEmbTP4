/* Jérôme SKODA , Joaquim LEFRANC  TP° 4 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <semaphore.h>
#include <fcntl.h>
#include <pthread.h>
#include <string.h>
#include <sys/wait.h>

#define NUMBER_OF_NS_IN_ONE_S 1000000000L
#define NUMBER_OF_MS_IN_ONE_S 1000000L

#define SEM_PARENT_NAME "/sem_parent.sem"
#define SEM_CHILD_NAME "/sem_child.sem"

double diff_time( struct timespec start, struct timespec end) {
  return ((double) end.tv_sec -(double) start.tv_sec )
    +    ((double) end.tv_nsec-(double) start.tv_nsec) / NUMBER_OF_NS_IN_ONE_S;
}

// only = result only
void bench_context_change(int max_context_change, char only) {

  if(max_context_change < 1 && max_context_change > 10000) {
    return;
  }

  pid_t pid;

  sem_t* sem_child= sem_open(SEM_CHILD_NAME, O_CREAT, 0644, 0);
  sem_t* sem_parent= sem_open(SEM_PARENT_NAME, O_CREAT, 0644, 1);

  struct sched_param sched_param;
  sched_param.sched_priority = 42; // Car c'est la réponse

  if(sem_parent == NULL || sem_child == NULL) {
    perror("Error initializing semaphore");
    return;
  }

  max_context_change/= 2; // TODO: quick fix pour les deux changement de context fois 2

  struct timespec timeStart, timeEnd;
  clock_gettime(CLOCK_REALTIME, &timeStart);

  if((pid = fork()) < 0)
  {
    perror("Fork Failed");
    exit(1);
  }
  else if(pid == 0)
  { // child
    sched_setscheduler(getpid(), SCHED_FIFO, &sched_param);
    for(int n_context_change=0; n_context_change<max_context_change; n_context_change++){
      sem_wait(sem_child);
      sem_post(sem_parent);
    }
    exit(0);
  }
  else
  { // Parent
    sched_setscheduler(getpid(), SCHED_FIFO, &sched_param);
    for(int n_context_change=0; n_context_change<max_context_change; n_context_change++){
      sem_wait(sem_parent);
      sem_post(sem_child);
    }
    waitpid(-1, NULL, 0);
  }

  clock_gettime(CLOCK_REALTIME, &timeEnd);

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
