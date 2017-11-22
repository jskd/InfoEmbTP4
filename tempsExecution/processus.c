
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define NUMBER_OF_NS_IN_ONE_S 1000000000L
#define NUMBER_OF_MS_IN_ONE_S 1000000L

double diff_time( struct timespec start, struct timespec end) {
  return ((double) end.tv_sec -(double) start.tv_sec )
    +    ((double) end.tv_nsec-(double) start.tv_nsec) / NUMBER_OF_NS_IN_ONE_S;
}

void bench_processus(int max_processus) {

  int n_processus=0;

  struct timespec timeStart, timeEnd;
  clock_gettime(CLOCK_REALTIME, &timeStart);

  while(n_processus <max_processus){
    pid_t pid=fork();
    if (pid == 0)
     exit(0);
    else if(pid > 0)
      n_processus++;
    else // fail
      break;
  }

  clock_gettime(CLOCK_REALTIME, &timeEnd);

  if(n_processus != max_processus)
    printf("Attention: nombre d'échantillon non attein. (fork failed)\n");

  // moyenne en ms
  double moyenne= (diff_time(timeStart, timeEnd) / n_processus) * NUMBER_OF_MS_IN_ONE_S;

  printf("Un processus (échantillon de %d) prend en moyenne: %lf ms.\n", n_processus, moyenne );

}

int main (int argc, char **argv) {
  if(argc < 2) {
    printf("Usage: processus [nombre-echantillon]\n");
    exit(1);
  }

  int max_processus= atoi(argv[1]);
  if(max_processus < 1) {
    printf("Nombre-echantillon incorect.\n");
    exit(1);
  }

  bench_processus(max_processus);

  exit(0);
}
