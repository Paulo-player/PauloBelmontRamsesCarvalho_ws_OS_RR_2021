#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<pthread.h>
#include<unistd.h>
#define max 3

int b = 0, e = 0, size = 0, fila[max+1];
pthread_mutex_t travaFila;

int vazio(){
	return !size;
}

void enqueue(int cliente){
	if(size < max+1){
		if(vazio())
			printf("O cliente %d acorda o barbeiro e senta em sua cadeira\n", cliente);
		else
			printf("O cliente %d senta em uma das cadeiras vagas da sala de espera\n", cliente);
		fila[e] = cliente;
		e = (e % max) + 1;
		size++;
	}else
		printf("Todas as cadeiras estavam ocupadas, o cliente %d foi embora\n", cliente);
}

int dequeue(){
	if(!vazio()){
        int cliente = fila[b];
		srand(time(NULL));
        printf("O barbeiro começa a cortar o cabelo do cliente %d.\n", cliente);
		sleep(rand() % 4);
		printf("O barbeiro termina de cortar o cabelo do cliente %d, que vai embora\n", fila[b]);
		
		b = (b % max) + 1;
		size--;

		if(vazio())
			printf("Não há clientes para serem atendidos, o barbeiro baianor vai dormikkkk\n");
		return cliente;
	}else return -1;
}

void* cliente(void* arg){
	unsigned long idc = *((unsigned long*) arg);
	
	pthread_mutex_trylock(&travaFila);
	enqueue(idc);
	pthread_mutex_unlock(&travaFila);
}

void* barbeiro(void* arg){
	for(;;){
		pthread_mutex_trylock(&travaFila);
		dequeue();
		pthread_mutex_unlock(&travaFila);
	}
}

int main(){
	pthread_mutex_init(&travaFila, NULL);
	
	int i;
	pthread_t threadB, threadC;
	
	pthread_create(&threadB, NULL, barbeiro, NULL);
	srand(time(NULL));
	for(i = 0;; i++){
		sleep(rand() % 4 + 1);
		pthread_create(&threadC, NULL, cliente, &i);
	}
	return 0;
}