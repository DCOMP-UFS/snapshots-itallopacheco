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
        // Produce
        int x = rand() % 100;
        
        Clock clock = {{x, x, x}};
        sleep(1);

        // Add to the buffer
        sem_wait(&semEmpty);
        pthread_mutex_lock(&mutexBuffer);
        
        buffer[count] = clock;
        
        count++;
        pthread_mutex_unlock(&mutexBuffer);
        sem_post(&semFull);
    }
}

void* consumer(void* args) {

    while (1) {
        int x;
        int y;
        int z;
        
        // Remove from the buffer
        sem_wait(&semFull);
        pthread_mutex_lock(&mutexBuffer);
        
        x = buffer[0].p[0];
        y = buffer[0].p[1];
        z = buffer[0].p[2];
        count--;
        
        pthread_mutex_unlock(&mutexBuffer);
        sem_post(&semEmpty);

        // Consume
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
    sem_init(&semEmpty, 0, 10);
    sem_init(&semFull, 0, 0);
    
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