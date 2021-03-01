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
  printf("%i\n",ehPrimo(2147483647));

  //  Verifica o tempo de execução.
  clock_t tempo;
  tempo = clock();
  printf("%i\n",ehPrimo(2147483647));

  printf("Tempo:%f\n",(clock() - tempo) / (double)CLOCKS_PER_SEC);

  return 0;
}
