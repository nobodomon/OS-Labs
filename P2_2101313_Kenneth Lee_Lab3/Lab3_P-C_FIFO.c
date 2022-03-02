#include<stdio.h>
#include <time.h>
#include<stdlib.h>
#include<pthread.h>
#include <unistd.h>

typedef struct _queueNode
{
    int item;
    struct _queueNode *next;
}QueueNode;

typedef struct _queue{
    int size;
    pthread_mutex_t mutex;
    QueueNode *head;
    QueueNode *tail;

}Queue;


void *createProducerThread(void* vargp);
void *createConsumerThread(void* vargp);
void *createMainProcessSleep(void* vargp);
void enqueue (Queue *qptr, int item);
int dequeue(Queue *qPtr);

QueueNode* getFront(Queue q);
int isEmptyQueue(Queue q);
void printExpQ(Queue* qPtr);

Queue buffer;
int mainProcessSleepTime = 0;
int running = 0;

pthread_mutex_t CritSec;

int main(int argc, char* argv[]){

    srand(time(NULL));

    int numOfArgs = argc -1;
    mainProcessSleepTime = atoi(argv[1]);
    int numOfProducers = atoi(argv[2]);
    int numOfConsumer = atoi(argv[3]);
    pthread_mutexattr_t mutexattr;
    pthread_mutexattr_settype(&mutexattr, PTHREAD_MUTEX_RECURSIVE_NP);
    pthread_mutex_init(&CritSec, &mutexattr);
    buffer.head = NULL;
    buffer.tail = NULL;
    buffer.size = 0;
    buffer.mutex = CritSec;

    if(numOfArgs == 3){
        printf("Number of arguements = %d\n", numOfArgs);
        printf("Main Process sleep time = %d\n", mainProcessSleepTime);
        printf("Number of producer(s) = %d\n", numOfProducers);
        printf("Nubmer of consumer(s) = %d\n", numOfConsumer);
        pthread_t producerThreads[numOfProducers];
        pthread_attr_t producerAttr;
        pthread_attr_init(&producerAttr);
        pthread_attr_setdetachstate(&producerAttr, PTHREAD_CREATE_JOINABLE);
        pthread_t consumerThreads[numOfConsumer];
        pthread_attr_t consumerAttr;
        pthread_attr_init(&consumerAttr);
        pthread_attr_setdetachstate(&consumerAttr, PTHREAD_CREATE_JOINABLE);
        pthread_t mainProcessSleepThread;
        pthread_attr_t mainProcessAttr;
        pthread_attr_init(&mainProcessAttr);
        pthread_attr_setdetachstate(&mainProcessAttr, PTHREAD_CREATE_JOINABLE); 
        long i;
        //pthread_join(mainProcessSleepThread,NULL);

        pthread_mutex_lock(&buffer.mutex);
        for(i = 0; i< numOfProducers; i++){
            //pthread_t thread = producerThreads[i];
            long prodID = i+1;
            pthread_create(&producerThreads[i], &producerAttr,createProducerThread, (void*) prodID);
            //pthread_join(producerThreads[i],NULL);
        }

        for(i = 0; i< numOfConsumer; i++){
            //pthread_t thread = consumerThreads[i];
            long consID = i+1;
            pthread_create(&consumerThreads[i], &consumerAttr,createConsumerThread, (void*) consID);
            //pthread_join(consumerThreads[i],NULL);
        }
        pthread_mutex_unlock(&buffer.mutex);
        

        pthread_create(&mainProcessSleepThread,&mainProcessAttr,createMainProcessSleep,NULL);

        pthread_attr_destroy(&producerAttr);
        pthread_attr_destroy(&consumerAttr);
    }
    
    
    pthread_exit(NULL);
}

void *createProducerThread(void* vargp){
    long prodID;
    prodID = (long)vargp;
    printf("Producer%ld thread created successfully \n", prodID);
    while(mainProcessSleepTime> 0){
        if(buffer.size < 5){
            int randomInt = rand() % 999;
            pthread_mutex_lock(&buffer.mutex);
            enqueue(&buffer,randomInt);
            pthread_mutex_unlock(&buffer.mutex);
            printf("Producer%ld inserted item\t%d\n",prodID,randomInt);
            sleep((rand() %
            (3 - 1 + 1)) + 1);
        }else{
            sleep(1);
        }
    }
    pthread_exit(NULL);
}

void *createConsumerThread(void* vargp){
    long consID;
    consID = (long)vargp;
    printf("Consumer%ld thread created successfully \n", consID);
    while(mainProcessSleepTime> 0){
        if(buffer.size != 0){
            pthread_mutex_lock(&buffer.mutex);
            printf("Consumer%ld removed item\t%d\n",consID,dequeue(&buffer));
            pthread_mutex_unlock(&buffer.mutex);
            sleep((rand() %
                (3 - 1 + 1)) + 1);
        }else{
            sleep(1);
        }
    }
    pthread_exit(NULL);
}

void *createMainProcessSleep(void* vargp){

    running = 1;
    sleep(mainProcessSleepTime);
    mainProcessSleepTime = 0;
    running = 0;
    printf("Sleep time elapsed. Producers and Consumers terminated\n");
    printf("Number of remaining items on buffer = %d\n", buffer.size);
    //exit(-1);
    pthread_exit(NULL);
}

// Add item into buffer
void enqueue (Queue *qPtr, int item)
{
    QueueNode *newNode;
    newNode = (QueueNode*) malloc(sizeof(QueueNode));
    newNode->item = item;
    newNode->next = NULL;

    if(isEmptyQueue(*qPtr))
        qPtr->head = newNode;
    else
        qPtr->tail->next = newNode;

    qPtr->tail =newNode;
    qPtr->size++;
}

// Remove item from buffer
int dequeue(Queue *qPtr)
{
    pthread_mutex_trylock(&qPtr->mutex);
    int dequedData;
    if(qPtr == NULL || qPtr->head ==NULL)
        return 0;
    else{
        QueueNode *temp = qPtr->head;
        qPtr->head = qPtr->head->next;

        if(qPtr->head == NULL)
            qPtr->tail = NULL;

        dequedData = temp->item;
        free(temp);
        qPtr->size--;


    }
    pthread_mutex_unlock(&qPtr->mutex);
    return dequedData;

}

QueueNode* getFront(Queue q)
{
    return q.head;
}

int isEmptyQueue(Queue q){
   if(q.size==0) return 1;
    else return 0;
}