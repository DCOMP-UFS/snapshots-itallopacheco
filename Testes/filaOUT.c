/**
 * Código base (incompleto) para implementação de relógios vetoriais.
 * Meta: implementar a interação entre três processos ilustrada na figura
 * da URL: 
 * 
 * https://people.cs.rutgers.edu/~pxk/417/notes/images/clocks-vector.png
 * 
 * Alunos: 
 * Itallo Pacheco Santana Santos
 * Luan Melo Guimarães
 * Lucas Carvalho Gomes Santiago 
 * 
 * Compilação: gcc -g -Wall -o filaOUT filaOUT.c -lpthread -lrt
 * Execução:   ./filaOUT
 * 
 * 
 * TODO: > função snapshot
 * 
 * testes de produtor-consumidor usando clocks 
 * 
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <semaphore.h>

#define THREAD_NUM 3
#define MAX 10

typedef struct Clock { 
   int p[3];
} Clock;


sem_t semEmpty;
sem_t semFull;

pthread_mutex_t mutexBuffer;

Clock buffer[10];
int count = 0;

void* producer(void* args) {

    while (1) {
        // Produz um clock com 1 mesmo numero aleatorio nas posições 0, 1 e 2.
        int x = rand() % 100;
        
        Clock clock = {{x, x, x}};
        sleep(1);

        // Adiciona esse clock ao buffer, 
        sem_wait(&semEmpty); // Verifica se o buffer tem slots.
        pthread_mutex_lock(&mutexBuffer); // O mutex se faz necessário uma vez que tanto a função
                                          // producer quanto a consumer vão alterar os valores de 
                                          // count e acessar o buffer simultaneamente.
        
        buffer[count] = clock;
        count++;
        
        pthread_mutex_unlock(&mutexBuffer);
        sem_post(&semFull); //diminui a quantidade de slots livres 
    }
}

void* consumer(void* args) {

    while (1) {
       
        int x;
        int y;
        int z;
        
        // remove os valores do primeiro elemento do buffer
        sem_wait(&semFull); // Verifica se o buffer está vazio 
        pthread_mutex_lock(&mutexBuffer); //Aqui o mutex também é necessario.
        
        x = buffer[0].p[0];
        y = buffer[0].p[1];
        z = buffer[0].p[2];
        count--;
        
        pthread_mutex_unlock(&mutexBuffer);
        sem_post(&semEmpty); // Libera um slot no buffer 

        // Printa o valor obtido 
        printf("Got (%d, %d, %d)\n", x, y, z);
        sleep(1);
    }
}

/*void thread(Fila fila){
   pthread_t th[THREAD_NUM];
   
   int i;
    for (i = 0; i < THREAD_NUM; i++) {
        if (i > 0) {
            if (pthread_create(&th[i], NULL, &producer, (void*) &fila) != 0) {
                perror("Failed to create thread");
            }
        } else {
            if (pthread_create(&th[i], NULL, &consumer, (void*) &fila) != 0) {
                perror("Failed to create thread");
            }
        }
    }
    for (i = 0; i < THREAD_NUM; i++) {
        if (pthread_join(th[i], NULL) != 0) {
            perror("Failed to join thread");
        }
    }
}
*/

int main(int argc, char* argv[]) {
    srand(time(NULL));
    
    pthread_mutex_init(&mutexBuffer, NULL);
    sem_init(&semEmpty, 0, 10); // indica que o semaforo tem 10 slots.
    sem_init(&semFull, 0, 0); // indica que dos 10 slots ele tem 0 sots ocupados.
    
    //thread(&fila1);
    pthread_t th1;
    pthread_t th2;
    
   
    pthread_create(&th1, NULL, &producer, NULL);
    pthread_create(&th2, NULL, &producer,  NULL);
    pthread_join(th1, NULL); 
    pthread_join(th2, NULL);
    
    
    sem_destroy(&semEmpty);
    sem_destroy(&semFull);
    pthread_mutex_destroy(&mutexBuffer);
    return 0;
}