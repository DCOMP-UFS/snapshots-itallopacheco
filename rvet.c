#include <stdio.h>
#include <string.h>  
#include <mpi.h>     


typedef struct Clock { 
   int p[3];
} Clock;


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

// Representa o processo de rank 0
void process0(){
   Clock clock = {{0,0,0}};
   printf("[ ] Process: %d, Clock: (%d, %d, %d)\n", 0, clock.p[0], clock.p[1], clock.p[2]);
   
   Event(0, &clock); // operacao a
   Send(0, 1, &clock); //operacao b
   Receive(1, 0, &clock);// operacao c
   Send(0, 2, &clock); // operacao d 
   Receive(2, 0, &clock); // operacao e
   Send(0, 1, &clock); // operacao f 
   Event(0, &clock); // operacao g
   
}

// Representa o processo de rank 1
void process1(){
   Clock clock = {{0,0,0}};
   printf("[ ] Process: %d, Clock: (%d, %d, %d)\n", 1, clock.p[0], clock.p[1], clock.p[2]);
   
   Send(1, 0, &clock); // operacao h
   Receive(0, 1, &clock); // operacao i
   Receive(0, 1, &clock); // operacao j
   // TO DO
   
}

// Representa o processo de rank 2
void process2(){
   Clock clock = {{0,0,0}};
   printf("[ ] Process: %d, Clock: (%d, %d, %d)\n", 2, clock.p[0], clock.p[1], clock.p[2]);
   
   Event(2, &clock); // operacao k
   Send(2, 0, &clock); // operacao l
   Receive(0, 2, &clock); // operacao m

   // TO DO
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