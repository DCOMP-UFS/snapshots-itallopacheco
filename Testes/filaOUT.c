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
 * Compilação: mpicc -lpthread -o filaOUT filaOUT.c
 * Execução:   mpiexec -n 3 ./filaOUT
 * 
 * 
 * TODO: > função snapshot
 * 
 * 
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>  
#include <unistd.h>
#include <time.h>
#include <mpi.h>  


#define THREAD_NUM 2


typedef struct Clock { 
   int p[3];
} Clock;

int buffer[10];
int count = 0;

void Event(int pid, Clock *clock){
   clock->p[pid]++;   
   printf("[E] Process: %d, Clock: (%d, %d, %d)\n", pid, clock->p[0], clock->p[1], clock->p[2]);
}




void Send(int pidS, int pidR, Clock *clock){
   clock->p[pidS]++;
   
   int array[3] = {0, 0, 0};
   
   for(int i = 0; i < 3; i++){
      array[i] = clock->p[i];
   }
   
   MPI_Send(array ,3 , MPI_INT, pidR, 1, MPI_COMM_WORLD);
   printf("[S] Process: %d To: %d, Clock (%d, %d, %d)\n" ,pidS, pidR, clock->p[0], clock->p[1], clock->p[2]);
   // TO DO
   
}

void Receive(int pidS ,int pidR, Clock *clock){
   clock->p[pidR]++;
   
   int arrayR[3];
   MPI_Recv(arrayR, 3, MPI_INT, pidS, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
   
   for (int i = 0; i < 3; i++) 
	   if (arrayR[i] > clock->p[i])
		   clock->p[i] = arrayR[i];
   
   
   printf("[R] Process: %d From: %d, Clock (%d, %d, %d)\n" ,pidR, pidS, clock->p[0], clock->p[1], clock->p[2]);
   // TO DO

}



void *producer (void* args){
   
   int x = rand(); 
   
   buffer[count] = x;
   count ++;
   
}

void *consumer (void* args){
   
   int y = buffer[count - 1];
   count--;
   
   printf("Got %d\n", y);
}


void threads() {
   pthread_t th[THREAD_NUM];
   int i;
   for(i=0; i < THREAD_NUM ; i++){
      if(i % 2 == 0){
         if(pthread_create(&th[i], NULL, &producer, NULL) != 0){
            perror("Falha ao criar thread");
         }
      } else {
         if(pthread_create(&th[i],NULL, &consumer, NULL) != 0){
            perror("Falha ao criar thread");
         }
      }
   }
   for(i=0 ; i < THREAD_NUM; i++) {
      if(pthread_join(th[i],NULL) != 0){
         perror("Falha ao dar join na thread");
      }
   }
   
}




// Representa o processo de rank 0
void process0(){
   
   srand(time(NULL));
   
   
   threads(); // cria as 3 threads 
   
   Clock clock = {{0,0,0}};
  // printf("[ ] Process: %d, Clock: (%d, %d, %d)\n", 0, clock.p[0], clock.p[1], clock.p[2]);
   
   
   /*
   Event(0, &clock); // operacao a
   Send(0, 1, &clock); //operacao b
   Receive(1, 0, &clock);// operacao c
   Send(0, 2, &clock); // operacao d 
   Receive(2, 0, &clock); // operacao e
   Send(0, 1, &clock); // operacao f 
   Event(0, &clock); // operacao g
   */
}

// Representa o processo de rank 1
void process1(){
   Clock clock = {{0,0,0}};
 //  printf("[ ] Process: %d, Clock: (%d, %d, %d)\n", 1, clock.p[0], clock.p[1], clock.p[2]);
   
   /*
   Send(1, 0, &clock); // operacao h
   Receive(0, 1, &clock); // operacao i
   Receive(0, 1, &clock); // operacao j
   // TO DO
   */
}

// Representa o processo de rank 2
void process2(){
   Clock clock = {{0,0,0}};
  // printf("[ ] Process: %d, Clock: (%d, %d, %d)\n", 2, clock.p[0], clock.p[1], clock.p[2]);
   /*
   Event(2, &clock); // operacao k
   Send(2, 0, &clock); // operacao l
   Receive(0, 2, &clock); // operacao m

   // TO DO
   */
}

int main(void) {
   
   int my_rank;               

   MPI_Init(NULL, NULL); 
   MPI_Comm_rank(MPI_COMM_WORLD, &my_rank); 

   if (my_rank == 0) { 
      process0();
   } else if (my_rank == 1) {  
      process1();
   } else if (my_rank == 2) {  
      process2();
   }

   /* Finaliza MPI */
   MPI_Finalize(); 

   return 0;
}  /* main */


