/* Jérôme SKODA , Joaquim LEFRANC  TP° 4 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>

#define NUMBER_OF_NS_IN_ONE_S 1000000000L
#define NUMBER_OF_MS_IN_ONE_S 1000000L

double diff_time( struct timespec start, struct timespec end) {
  return ((double) end.tv_sec -(double) start.tv_sec )
    +    ((double) end.tv_nsec-(double) start.tv_nsec) / NUMBER_OF_NS_IN_ONE_S;
}

// only = result only
void bench_processus(int max_processus, char only) {

  if(max_processus < 1 && max_processus > 10000) {
    return;
  }

  int n_processus=0;
  pid_t pid;

  struct timespec timeStart, timeEnd;
  clock_gettime(CLOCK_REALTIME, &timeStart);

  while(n_processus < max_processus){
    pid=fork();
    if (pid == 0)
     exit(0);
    else if(pid > 0)
      n_processus++;
    else // fail
      break;
  }

  clock_gettime(CLOCK_REALTIME, &timeEnd);

  if(n_processus != max_processus)
    if(!only)
      printf("Attention: nombre d'échantillon non attein. (fork failed)\n");

  // moyenne en ms
  double moyenne= (diff_time(timeStart, timeEnd) / n_processus) * NUMBER_OF_MS_IN_ONE_S;

  if(only)
    printf("%lf\n", moyenne );
  else
    printf("Un processus (échantillon de %d) prend en moyenne: %lf ms.\n", n_processus, moyenne );
}

int main (int argc, char **argv) {
  if(argc < 2) {
    printf("Usage: processus [nombre-echantillon]\n");
    exit(1);
  }

  // Pour eviter le débordement
  int max_processus= atoi(argv[1]);
  if(max_processus < 1 && max_processus > 10000) {
    printf("Nombre-echantillon incorect. (Entre 0 et 10000)\n");
    exit(1);
  }

  char only=0;
  if(argc >= 3)
    if(strcmp(argv[2], "-o") == 0)
      only=1;

  bench_processus(max_processus, only);

  exit(0);
}
