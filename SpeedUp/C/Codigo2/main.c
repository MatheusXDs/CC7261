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


  //  Verifica o tempo de execução.
  clock_t tempo;
  tempo = clock();
  printf("%i\n",resultado);
  
  printf("Tempo:%f\n",(clock() - tempo) / (double)CLOCKS_PER_SEC);

  return 0;
}
