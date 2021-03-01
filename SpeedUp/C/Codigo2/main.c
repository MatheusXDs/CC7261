int main() {
  int num, i, resultado = 0;
  
  num = 7;
  
  for (i = 2; i <= num / 2; i++) {
      if (num % i == 0) {
        resultado=0;
        break;
      }
      resultado=1;
  }
  printf("%i\n",resultado);

  //  Verifica o tempo de execução.
  clock_t tempo;
  tempo = clock();

    // código de teste
  for(int i = 0; i < 999999999; ++i){}
  printf("Tempo:%f\n",(clock() - tempo) / (double)CLOCKS_PER_SEC);

  return 0;
}
