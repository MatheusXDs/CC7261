#include "stdio.h"
#include "unistd.h"
#include "stdlib.h"
#include "pthread.h"
#include "semaphore.h"
#include <random>
#include <iostream>

#define CHAIRS 5   /* número de cadeiras para os clientes à espera */
#define TRUE 1

sem_t customers;   /* número de cliente à espera de atendimento */
sem_t barbers;     /* número de barbeiros à espera de clientes */
sem_t mutex;       /* para exclusão mútua */
int waiting = 0;   /* clientes que estão esperando (não estão cortando) */

std::uniform_int_distribution<> distr(2, 3); // define o range 20 a 30 minutos = 2 a 3 segundos
float dia = 144; //6 segundos = 1 hora //144 segundos = 24 horas
int barbeiroDormiu = 0;
int numClientesAtendidos = 0;

/* protótipos */
void* barber(void *arg);
void* customer(void *arg);
void cut_hair();
void customer_arrived();
void get_haircut();
void giveup_haircut();
void barber_sleep();

int main() {
	sem_init(&customers, TRUE, 0);
	sem_init(&barbers, TRUE, 0);
	sem_init(&mutex, TRUE, 1);

	pthread_t b, c;

	/* criando único barbeiro */
	pthread_create(&b, NULL, barber, NULL);

	/* criação indefinida de clientes */
	while(dia >= 0) { 
		
		printf("Horas restantes do dia: %.2f \n", dia/6);
		customer_arrived();
		pthread_create(&c, NULL, customer, NULL);

		sleep(3);//dois clientes chegam em uma hora (6 seg), um cliente chega a cada 
	}

	printf("O barbeiro teve %d clientes \n", numClientesAtendidos);
	printf("O barbeiro dormiu %d minutos\n", barbeiroDormiu);
	return 0;
}

void* barber(void *arg) {
	while(TRUE) {
		sem_wait(&customers);   /* vai dormir se o número de clientes for 0 */
		sem_wait(&mutex);       /* obtém acesso a 'waiting' */
		waiting = waiting - 1;  /*descresce de um o contador de clientes à espera */
		sem_post(&barbers);     /* um barbeiro está agora pronto para cortar cabelo */
		sem_post(&mutex);       /* libera 'waiting' */
		cut_hair();             /* corta o cabelo (fora da região crítica) */
		if(waiting == 0){
			barber_sleep();
		}
	}
	pthread_exit(NULL);
}

void* customer(void *arg) {
	sem_wait(&mutex);           /* entra na região crítica */

	if(waiting < CHAIRS) {      /* se não houver cadeiras vazias, saia */
		waiting = waiting + 1;  /* incrementa o contador de clientes à espera */
		get_haircut();          /* sentado e sendo servido */
		sem_post(&customers);   /* acorda o barbeiro se necessário */
		sem_post(&mutex);       /* libera o acesso a 'waiting' */
		sem_wait(&barbers);     /* vai dormir se o número de barbeiros livres for 0 */
	} else {
		sem_post(&mutex);       /* a barbearia está cheia; não espera */
		giveup_haircut();
	}
	pthread_exit(NULL);
}

void cut_hair() {	
  float tempoCorte = 2.5 + ( 5 *((rand() % 2) -1))/10;
	printf("Barbeiro esta cortando o cabelo do Cliente\n");
	dia = dia - tempoCorte;
	numClientesAtendidos++;
	printf("Horas restantes do dia: %.2f \n", dia/6);
	sleep(tempoCorte);	
}

void customer_arrived() {
	printf("Cliente chegou para cortar cabelo!\n");
}
void get_haircut() {
	printf("Cliente terminou de cortar o cabelo!\n");
}

void giveup_haircut() {
	printf("Cliente desistiu! (O salao esta muito cheio!)\n");
}

void barber_sleep(){
	printf("O barbeiro esta dormindo!\n");
	dia--; //barbeiro dorme por 10 min
	barbeiroDormiu++;
	printf("Horas restantes do dia: %.2f \n", dia/6);
}