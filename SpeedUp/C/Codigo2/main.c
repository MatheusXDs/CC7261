#include "TestaPrimo.h"
#include <time.h>

int main() {
  int num, i, resultado = 0;
  
  num = 2147483647;

  clock_t tempo;
  tempo = clock();

  for (i = 2; i <= num / 2; i++) {
      if (num % i == 0) {
        resultado=0;
        break;
      }
      resultado=1;
  }
  
  //  Verifica o tempo de execução.
  printf("%i\n",resultado);
  
  printf("Tempo:%f\n",(clock() - tempo)*1000 / (double)CLOCKS_PER_SEC);

  return 0;
}
