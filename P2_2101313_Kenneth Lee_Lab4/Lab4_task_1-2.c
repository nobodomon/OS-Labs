#include<stdio.h>  
#include<stdlib.h> 
 
#define TOTAL_DISK_BLOCKS 32 
#define TOTAL_DISK_INODES 8  
#define BLOCK_SIZE_IN_KB 1
#define ALLOCATION_METHOD "LINKED LIST"
int blockStatus[TOTAL_DISK_BLOCKS]; // free = 0 
int blockList[TOTAL_DISK_BLOCKS - TOTAL_DISK_INODES]; // list of blocks of a file 


struct file_table {  
    char fileName[20];  
    int fileSize;  
    struct block *sb;  
}; 
struct file_table fileTable[TOTAL_DISK_BLOCKS - TOTAL_DISK_INODES]; 
 
struct block {  
    int blockNumber;  
    struct block *next;  
};  

void appendNode(struct block **head, int nextNode);

struct block *startBlock = NULL;
 
int AllocateBlocks(int Size) { 
    int i = 0, count = 0, inList = 0, nextBlock = 0; 
    int allocStartBlock = TOTAL_DISK_INODES; 
    int allocEndBlock = TOTAL_DISK_BLOCKS - 1; 
    
    startBlock = NULL;

    for (i = 0; i < (TOTAL_DISK_BLOCKS - TOTAL_DISK_INODES); i++) 
        if (blockStatus[i] == 0) 
            count++; 
    if (count < Size) 
        return 1;  // not enough free blocks     
 
    count = 0;
    
    while (count < Size) { 
        //printf("%d",nextBlock);
        nextBlock = (rand() % (allocEndBlock - allocStartBlock + 1)) + allocStartBlock;  

        if (blockStatus[nextBlock] == 0) {
            blockStatus[nextBlock] = 1;
            count = count + 1;
            appendNode(&startBlock, nextBlock);
        }
    }

    if (count == Size)  
        return 0; // success 
    else 
        return 1; // not successful 
} 

void appendNode(struct block **head, int nextBlock){
    struct block *newBlock = malloc(sizeof(struct block));
    newBlock->blockNumber = nextBlock;
    newBlock->next = NULL;
    if(*head == NULL){
        *head = newBlock;
    }else{
        struct block *lastBlock = *head;

        //last node's next address will be NULL.
        while(lastBlock->next != NULL)
        {
            lastBlock = lastBlock->next;
        }

        //add the newNode at the end of the linked list
        lastBlock->next = newBlock;
    }
}


void main()  
{  
    int i = 0, j = 0, numFiles = 0, nextBlock= 0, ret = 1;  
    char s[20];

    printf("File allocation method: %s\n", ALLOCATION_METHOD);
    printf("Total blocks: %d\n", TOTAL_DISK_BLOCKS);
    printf("File allocation start at block: %d\n", TOTAL_DISK_INODES);
    printf("File allocation end at block: %d\n", TOTAL_DISK_BLOCKS - 1);
    printf("Size (kB) of each block: %d\n", BLOCK_SIZE_IN_KB);

    printf("\n");
    printf("Enter no of files: ");
    scanf("%d", &numFiles);
    printf("\n");

    //Seed the pseudo-random number generator used by rand() with the value seed 
    srand(1234); 
    for(i = 0; i < numFiles; i++) {  

        struct file_table file;

        printf("Enter the name of file #%d: ", i+1);
        scanf("%s", file.fileName);
        printf("Enter the size (kB) of file #%d: ", i+1);
        scanf("%d", &file.fileSize);
        printf("\n");
        fileTable[i] = file;
        startBlock = (struct block*) malloc(sizeof(struct block));
        ret = AllocateBlocks(fileTable[i].fileSize); 
        if(ret == 0){
            fileTable[i].sb = startBlock;
        }else{
            printf("Insufficient memory!");
            exit(0);
        }
    } 
    printf("FILE_fileName\t FILE_SIZE \t BLOCKS_OCCUPIED\n");
    for(i = 0; i < numFiles; i++){
        printf("%s\t %d\t\t ",fileTable[i].fileName,fileTable[i].fileSize);
        struct block *temp = fileTable[i].sb;

        //iterate the entire linked list and print the data
        int size = 1;
        while(temp != NULL)
        {
            if(size == fileTable[i].fileSize){
                printf("%d", temp->blockNumber);
            }else{
                printf("%d-", temp->blockNumber);
            }
            temp = temp->next;
            size++;
        }
        printf("\n");
    }
    printf("File allocation completed. Exiting.\n");
}