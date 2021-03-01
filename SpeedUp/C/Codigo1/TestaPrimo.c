//
//  TestaPrimo.c
//  TestaPrimo
//
//  Created by Ricardo de Carvalho Destro on 12/08/20.
//  Copyright © 2020 Ricardo de Carvalho Destro. All rights reserved.
//

#include "TestaPrimo.h"
#include <time.h>


int TestaPrimo(int n) {
    int EhPrimo = 1,
        d=2;
    if (n <= 1)
    EhPrimo = 0;

    while (EhPrimo == 1 && d <= n /2) {
    if (n % d  == 0)
        EhPrimo = 0;
    d = d + 1;
    }
    return EhPrimo;
}

int main(){
  int n = 2147483647;
  printf("%i\n",TestaPrimo(n));

  //  Verifica o tempo de execução.
  clock_t tempo;
  tempo = clock();

    // código de teste
  for(int i = 0; i < 999999999; ++i){}
  printf("Tempo:%f\n",(clock() - tempo) / (double)CLOCKS_PER_SEC);
}
