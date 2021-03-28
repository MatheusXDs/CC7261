#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#define Filosofos 5
#define Tcomendo 10
#define Tpensando 10
#define ESQUERDA (f_num + Filosofos - 1) % Filosofos
#define DIREITA (f_num + 1) % Filosofos

pthread_mutex_t mutex;
pthread_cond_t condition[Filosofos];

enum { Pensando, Faminto, Comendo } state[Filosofos];
int filo_num[Filosofos];

void *pensa (void *arg);
void pega_garfo(int f_num);
void devolve_garfo(int f_num);
void Testa(int f_num);

int main(int argc, char *argv[])
{
  /* Setup */
  pthread_t ph_thread[Filosofos];
  pthread_mutex_init(&mutex, NULL);
  int i = 0;
  for (i = 0; i < Filosofos; i++) {
    pthread_cond_init(&condition[i], NULL);
    filo_num[i] = i;
  }

  /* Meat */
  for (i = 0; i < Filosofos; i++) {
    pthread_create(&ph_thread[i], NULL, pensa, &filo_num[i]);
    printf("Filosofo #%d senta na mesa.\n", i + 1);
    sleep(1);
  }
  for (i = 0; i < Filosofos; i++)
    pthread_join(ph_thread[i], NULL);

  /* Cleanup */
  pthread_mutex_destroy(&mutex);
  for (i = 0; i < Filosofos; i++)
    pthread_cond_destroy(&condition[i]);

  return(0);
}

void *pensa(void *arg)
{
  while(1) {
    int *f_num = arg;
    printf("Filosofo #%d esta Pensando.\n", *f_num + 1);
    sleep(Tpensando); // tempo pensando
    pega_garfo(*f_num);
    devolve_garfo(*f_num);
  }
}

void pega_garfo(int f_num) {
  pthread_mutex_lock(&mutex);

  printf("Filosofo #%d esta Faminto, tenta pegar os garfos.\n", f_num + 1);
  state[f_num] = Faminto;
  Testa(f_num);
  while (state[f_num] != Comendo) 
    pthread_cond_wait(&condition[f_num], &mutex);

  pthread_mutex_unlock(&mutex);
}

void devolve_garfo(int f_num) {
  pthread_mutex_lock(&mutex);

  printf("Filosofo #%d coloca os garfos na mesa.\n", f_num + 1);
  state[f_num] = Pensando;
  Testa(ESQUERDA);
  Testa(DIREITA);

  pthread_mutex_unlock(&mutex);
}

void Testa(int f_num) {
  if (state[f_num] == Faminto && state[ESQUERDA] != Comendo && state[DIREITA] != Comendo){
    printf("Filosofo #%d esta Comendo.\n", f_num + 1);
    state[f_num] = Comendo;
    sleep(Tcomendo);
    pthread_cond_signal(&condition[f_num]);
  }
}