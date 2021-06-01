#include <stdio.h>
#include <chrono>
#include <iostream>
#include <unistd.h>
#include <pthread.h>
#include <queue>
#include <time.h>

using namespace std;

#define Componentes 14

int Componente[Componentes], ciclos = 0;
float reator[3] = {0, 0, 0};
float Etanol, soda_caustica, oleo, decantador = 0, glicerina = 0, biodiesel = 0, secadorSolucao = 0, solucaoLavagem = 0, secadorEtanol = 0;
bool lavagemAtiva[3] = {false, false, false};
bool decantadorAtivo = true, secadorEtanolAtivo = true, secadorBiodieselAtivo = false, tanqueReatorEncheu = false, reatorAtivo = false, lavagemAtivo = false;

pthread_cond_t condicao[Componentes];

void *threadComponente(void *arg);
void oleoResidual();
void tanqueOleo();
void tanqueReator();
void tanqueDecantador();
void tanqueGlicerina();
void secadorDecantador();
void tanqueEtanol();
void tanque_soda_caustica_Etanol();
void tanqueLavagem1();
void tanqueLavagem2();
void tanqueLavagem3();
void secadorBiodiesel();
void tanqueBiodiesel();

int main(int argc, char *argv[]){

  pthread_t ph_thread[Componentes];

  for (int i = 0; i < Componentes; i++){
    pthread_cond_init(&condicao[i], NULL);
    Componente[i] = i;
  }

  for (int i = 0; i < Componentes; i++){
    pthread_create(&ph_thread[i], NULL, threadComponente, &Componente[i]);
    sleep(0);
  }

  for (int i = 0; i < Componentes; i++){
    pthread_join(ph_thread[i], NULL);
  }

  for (int i = 0; i < Componentes; i++){
    pthread_cond_destroy(&condicao[i]);
  }

  return 0;
}

void *threadComponente(void *arg){
  int valorThread = *(int *)arg;

  while (ciclos <= 3600){
    
    if (valorThread == 0){
      // Orquestrador
      ciclos++;
      usleep(1000 * 2000);
      cout << "Ciclo " << ciclos << endl;
      cout << "=====" << endl;
      cout << "Biodiesel " << biodiesel << endl;
      cout << "Glicerina " << glicerina << endl;
      cout << "Óleo " << oleo << endl;
      cout << "Soda Cáustica " << soda_caustica << endl;
      cout << "Etanol " << Etanol << endl;
      cout << "=====" << endl;
    }
    if (valorThread == 1) oleoResidual(); // Óleo residual
    if (valorThread == 2) tanqueOleo(); // Tanque de óleo
    if (valorThread == 3) tanqueReator(); // Tanque do Reator
    if (valorThread == 4) tanqueDecantador(); // Tanque do decantador
    if (valorThread == 5) tanque_soda_caustica_Etanol(); // Tanque de soda_caustica e Etanol
    if (valorThread == 6) tanqueLavagem1(); // Tanque Lavagem 1
    if (valorThread == 7) tanqueLavagem2(); // Tanque Lavagem 2
    if (valorThread == 8) tanqueLavagem3(); // Tanque Lavagem 3
    if (valorThread == 9) secadorBiodiesel(); // Secador biodiesel
    if (valorThread == 10) secadorDecantador(); // Secador decantador
  }
  return 0;
}

//reator[0] = oleo no reator // reator[1] = Naoh no reator // reator[2] = etoh
//a proporsao do reator para os 5 litros sera considerada 1.25 para o eleo e naoh e 2.5 para etoh

void oleoResidual(){
  //gera um tempo aleatorie entre 0 e 10s
  int tempo = (rand() % (1000 * 1000 * 10));
  usleep(tempo);
  //gera numero aleatorio para entrada no tanque de oleo (de 1 a 2 litros)
  float aux = rand() % 1000;
  float cargaOleo = 1.0 + (aux / 1000);
  //conta carga nova do oleo
  oleo = oleo + cargaOleo;
  usleep((1000 * 10000) - tempo);//controla o tempo aleatorio entre cada carga
}

void tanqueOleo(){
  //se o reator estiver com pouco oleo e o tanque estiver com o suficiente para transferencia 
  if (reator[0] < 1.25 && oleo >= 1.25){
    //float reatorOleo = reator[0];
    float oleoRestante = 1.25 - reator[0];
    //quantidade e oleo no reator depois da transferencia
    reator[0] = reator[0] + oleoRestante;
    //quantidade de oleo no tanque depois da transferencia
    oleo = oleo - oleoRestante;
  }
}

void tanqueReator(){
  //verifica se o reator satisfaz a condicao para processar 5 litros
  if (reator[0] >= 1.25 && reator[1] >= 1.25 && reator[2] >= 2.50){
    tanqueReatorEncheu = true;
    if (!reatorAtivo)
      cout << "O Reator lançou os produtos no Decantador." << endl;
      cout << "=====" << endl;
    reatorAtivo = true;
  }
  usleep(1000 * 1000);
}

void tanqueDecantador(){
  //verifica se o reator encheu pela flag 
  if (tanqueReatorEncheu){
    decantador = decantador + 5.0;
    //tira os produtos do reator 
    reator[0] = 0;
    reator[1] = 0;
    reator[2] = 0;

    usleep(1000 * 5000 * 1.66);

    tanqueReatorEncheu = false;
  }
  //calcula as proporsao do resultado do reator
  if (decantador > 0 && decantadorAtivo == true){
    glicerina = glicerina + decantador * 0.02;
    secadorEtanol = secadorEtanol + decantador * 0.09;
    solucaoLavagem = solucaoLavagem + decantador * 0.89;

    decantador = 0;
    lavagemAtiva[0] = true;
    secadorEtanolAtivo = true;
    decantadorAtivo = false;

    cout << "O Decantador lançou os produtos." << endl;
    cout << "=====" << endl;
  }
}
//condicao para 1 litro do Etanol
void secadorDecantador(){
  if (secadorEtanolAtivo == true){
    Etanol = Etanol + secadorEtanol - (secadorEtanol * 0.03);
    secadorEtanol = 0;
    secadorEtanolAtivo = false;
    usleep(1000 * 5000);
  }
}

void tanque_soda_caustica_Etanol(){
  //adiciona etoh e naoh a cada 1 segundo
  usleep(1000 * 1000);
  soda_caustica = soda_caustica + 0.25;
  Etanol = Etanol + 0.125;

//se o reator estiver com pouco naoh e etoh e o tanque estiver com o suficiente para transferencia 
  if (reator[1] < 1.25 && soda_caustica >= 1.25){
    float soda_causticaRestante = 1.25 - reator[1];
    reator[1] = reator[1] + soda_causticaRestante;
    soda_caustica = soda_caustica - soda_causticaRestante;
  }

  if (reator[2] < 2.50 && Etanol >= 2.50){
    float EtanolRestante = 2.50 - reator[2];
    reator[2] = reator[2] + EtanolRestante;
    Etanol = Etanol - EtanolRestante;
  }
}

void tanqueLavagem1(){
  if (lavagemAtiva[0] && solucaoLavagem > 0){
    decantadorAtivo = false;
    solucaoLavagem = solucaoLavagem - solucaoLavagem * 0.075;
    lavagemAtiva[0] = false;
    lavagemAtiva[1] = true;

    if (!lavagemAtivo){
      cout << "A lavagem foi ativada" << endl;
      cout << "=====" << endl;
      lavagemAtivo = true;
    }
  }
}

void tanqueLavagem2(){
  if (lavagemAtiva[1]){
    solucaoLavagem = solucaoLavagem - solucaoLavagem * 0.075;
    lavagemAtiva[1] = false;
    lavagemAtiva[2] = true;
  }
}

void tanqueLavagem3(){
  if (lavagemAtiva[2]){
    solucaoLavagem = solucaoLavagem - solucaoLavagem * 0.075;
    lavagemAtiva[2] = false;
    secadorBiodieselAtivo = true;
    secadorSolucao = solucaoLavagem;
    solucaoLavagem = 0;
    lavagemAtivo = false;
    }
}

void secadorBiodiesel(){
  if (secadorBiodieselAtivo){
    biodiesel = biodiesel + secadorSolucao - (secadorSolucao * 0.03);
    usleep(1000 * secadorSolucao * 5000);
    secadorBiodieselAtivo = false;
    decantadorAtivo = true;
  }
}