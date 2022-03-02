#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

pthread_mutex_t mutex;  //to make sure 1 player at a time
sem_t *S; int n;        //n semaphores for n players

void *player() {
    int pid = 0;
	do {
        pthread_mutex_lock(&mutex);     //Enters CS
        six:
        int val = (rand() % 6) + 1;
        printf("Player %d rolled and got %d\n", pid + 1, val);
        if(val == 6) goto six;
        sem_post(&S[pid]);              //signals player has rolled the die
        pthread_mutex_unlock(&mutex);   //exits CS
        sem_wait(&S[pid]);              //player waits for the next turn
        pthread_mutex_lock(&mutex);     //Enters CS
        int next = (pid + 1) % n;
        printf("Player %d passed the die to %d\n", pid + 1, next + 1);
        sem_post(&S[next]);             //wakes up next member
        pthread_mutex_unlock(&mutex);   //exits CS
        pid = next;
	} while(1);
}

int main() {
    printf("Enter No. of players : ");
    input:
    scanf("%d",&n);
    if(n>4 || n<1){
        printf("Enter between 1 & 4 : ");
        goto input;
    }
    pthread_t pid;
    pthread_mutex_init(&mutex, NULL);

    S = malloc(n*sizeof(sem_t));
    for(int i = 0; i < n; i++)
        sem_init(&S[i], 0, 0);

	pthread_create(&pid, NULL, player, NULL);
    pthread_join(pid, NULL);

    pthread_mutex_destroy(&mutex);
    for (int i = 0; i < n; i++)
        sem_destroy(&S[i]);
    return 0;
}