#include <stdio.h>
#include <time.h>

int ehDivisaoExata(int dividendo, int divisor) {
    return (dividendo % divisor) == 0;
}

int ehPrimo(int num) {
  int i, quantDivisores = 0;

  for(i = 1; i <= num; i++) {
      if (ehDivisaoExata(num, i)) {
          quantDivisores++;
      }
  }
  if (quantDivisores == 2) {
      return 1;
  } else {
      return 0;
  }
}

int main() {
  float vetor[30], media;
  printf("1 para primo 0 para não primo");

  for(int i=0 ; i < 30 ; i++){

  clock_t tempo;
  tempo = clock();
  printf("%i\n",ehPrimo(7));

  printf("Tempo:%f\n",(clock() - tempo)*1000 / (double)CLOCKS_PER_SEC);
  vetor[i] = ((clock() - tempo)*1000 / (double)CLOCKS_PER_SEC);
  media += vetor[i];
  }

  media = media/30;
  printf("Media: %f", media);
  return 0;
}
