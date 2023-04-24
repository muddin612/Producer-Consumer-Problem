/*

Mohammed Uddin
CSIT 345
HW 4

*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include "buffer.h"

buffer_item buffer[BUFFER_SIZE];
int count = 0;

sem_t empty;
sem_t full;
pthread_mutex_t mutex;

int insert_item(buffer_item item){

    if( count != BUFFER_SIZE){ // checks if the buffer is empty

        buffer[count] = item; // adds to buffer
        count++;
        return 0;
    }
    else{
        return -1;
    }
}

int remove_item(buffer_item *item){

    if( count > 0){ // checks of the buffer is full
        *item = buffer[count - 1]; // removes from buffer
        count--;
        return 0;
    }
    else{
        return -1;
    }
    

}

void *producer(void *parm){
    buffer_item item;

    while(1){
        sleep(rand() % 20 + 1); // sleep for a random amount of time between 1 and 20 seconds
        item = rand() % 1000 + 1; // creates a random number from 1 to 1000

        sem_wait(&empty);
        pthread_mutex_lock(&mutex);

        if(insert_item(item)){
            fprintf(stderr,"Error: buffer is full\n");
        }
        else{
            printf("Producer produced %d\n", item);
        }

        pthread_mutex_unlock(&mutex);
        sem_post(&full);        
    }
}

void *consumer(void *param){
    buffer_item item;

    while(1){
        sleep(rand() % 20 + 1); // sleep for a random amount of time between 1 and 20 seconds

        sem_wait(&full);
        pthread_mutex_lock(&mutex);

        if(remove_item(&item)){
            fprintf(stderr,"Error buffer is empty\n");
        }
        else{
            printf("Consumer consumed %d\n", item);
        }

        pthread_mutex_unlock(&mutex);
        sem_post(&empty);

    }
}


int main(int argc, char *argv[]){

    // Checks if the user entered the required info
    if(argc < 4){
        fprintf(stderr,"Usage: %s <sleep time> <producer threads> <consumer threads>\n",argv[0]);
    }

    int sleepTime = atoi(argv[1]);
    int numProducer = atoi(argv[2]);
    int numConsumer = atoi(argv[3]);


    // Initializes mutex lock and semaphore
    pthread_mutex_init(&mutex, NULL);
    sem_init(&empty,0,BUFFER_SIZE);
    sem_init(&full,0,0);
    



    //Create producer threads
    for(int x = 0; x < numProducer; x++){
        pthread_t tid;
        pthread_create(&tid, NULL, producer, NULL);
    }

    //Create Consumer threads
    for(int x = 0; x < numConsumer; x++){
        pthread_t tid;
        pthread_create(&tid, NULL, consumer, NULL);
    }

    pthread_mutex_destroy(&mutex);
    sem_destroy(&empty);
    sem_destroy(&full);
    sleep(sleepTime);
    return 0;




}