#include <stdio.h>
#include <time.h>

int main() {
  int num, i, resultado = 0;
  float vetor[30], media;
  
  num = 7;
  for(int f=0 ; f < 30; f++){
    clock_t tempo;
    tempo = clock();

    for (i = 2; i <= num / 2; i++) {
      if (num % i == 0) {
        resultado=0;
        break;
      }
      resultado=1;
    }
    
    printf("%i\n",resultado);
    
    printf("Tempo:%f\n",(clock() - tempo)*1000 / (double)CLOCKS_PER_SEC);
    vetor[f] = ((clock() - tempo)*1000 / (double)CLOCKS_PER_SEC);
    media += vetor[f];
  }

  media = media/30;
  printf("%f", media);
  return 0;
}
