#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>
    
pthread_mutex_t mutex;  //protect service from the same CPU from other threads
sem_t cpu, pro;         //CPU n Processes ready or not
int n, x, *p, wait=0, done=0;

void *CPU() {   
    for (int i = 0; i < n; i++) {
        //try to get a process or sleep if there is none
        sem_wait(&pro);
        //wake up CPU to process
        sem_post(&cpu); 

        pthread_mutex_lock(&mutex);     //Enters CS
        usleep((rand() % 51)*1000);
        wait--; done++;
        pthread_mutex_unlock(&mutex);   //Exits CS

        printf("[pid = %d] was processed.\n", x);
        printf("Waiting : %d\tDone : %d\n", wait, done);
    }
}

void *process(void *idp) {
    printf("[pid = %d] is waiting.\n", *((int *)idp));
    //set the process ready to be processed
    sem_post(&pro);
    wait++;
    //wait for the CPU to get ready 
    sem_wait(&cpu);
    printf("[pid = %d] is being processed.\n", *((int *)idp)); 
    x = *((int *)idp);
}
void *make_process(){
    for (int i = 0; i < n; i++) {
        pthread_t pid;
        pthread_create(&pid, NULL, process, &p[i]);  
    }
}

int main(){
    printf("Enter the number of processes : ");
    input:
    scanf("%d",&n);
    if(n>20 || n<1){
        printf("Enter between 1 & 20 : ");
        goto input;
    }
    pthread_t pro_id;
    pthread_t cpu_id;
    pthread_mutex_init(&mutex, NULL);
    sem_init(&pro, 0, 0);
    sem_init(&cpu, 0, 0);
    p = malloc(n*sizeof(int));
    for (int i = 0; i < n; i++) p[i] = i + 1;

    pthread_create(&cpu_id, NULL, CPU, NULL);
    pthread_create(&pro_id, NULL, make_process, NULL);
    
    pthread_join(cpu_id, NULL);
    pthread_join(pro_id, NULL);
 
    pthread_mutex_destroy(&mutex);
    sem_destroy(&cpu);
    sem_destroy(&pro);
    return 0;  	
}