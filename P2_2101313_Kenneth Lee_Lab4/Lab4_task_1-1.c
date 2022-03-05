#include<stdlib.h>  
#include<stdio.h>
 
#define TOTAL_DISK_BLOCKS 32 
#define TOTAL_DISK_INODES 8  
#define BLOCK_SIZE_IN_KB 1
#define ALLOCATION_METHOD "SEQUENTIAL"
int blockStatus[TOTAL_DISK_BLOCKS]; // free = 0  
int blockStart; 
 
struct file_table {  
    char fileName[20];  
    int startBlock; 
    int fileSize; 
    int allotStatus; 
}; 
struct file_table fileTable[TOTAL_DISK_BLOCKS - TOTAL_DISK_INODES]; 
 
int AllocateBlocks(int Size) { 
    int i = 0, count = 0, inList = 0, nextBlock = 0; 
    int allocStartBlock = TOTAL_DISK_INODES; 
    int allocEndBlock = TOTAL_DISK_BLOCKS - 1; 
 
    // check whether sufficient free blocks are available 
    for (i = 0; i < (TOTAL_DISK_BLOCKS - TOTAL_DISK_INODES); i++) 
        if (blockStatus[i] == 0) 
            count++; 
    if (count < Size) 
        return 1;  // not enough free blocks     
 
    count = 0; 
    while (count < Size) { 
        nextBlock = (rand() % (allocEndBlock - allocStartBlock + 1)) + allocStartBlock;  
        for (i = nextBlock; i < (nextBlock + Size); i++) 
        { 
            if (blockStatus[i] == 0) 
                count = count + 1; 
            else { 
                count = 0; 
                break; 
            } 
        }  
    }             
    blockStart = nextBlock; 
 
    if (count == Size)  
        return 0; // success 
    else 
        return 1; // not successful 
} 
 
void main()  
{  
    int i =0, j = 0, numFiles = 0, nextBlock = 0, ret = 1;  
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
    //--- 
    //Seed the pseudo-random number generator used by rand() with the value seed 
    srand(1234); 
    //--- 
    for(i = 0; i < numFiles; i++) {  
        //--- 
        struct file_table file;
        printf("Enter the name of file #%d: ", i+1);
        scanf("%s", file.fileName);
        printf("Enter the size (kB) of file #%d: ", i+1);
        scanf("%d", &file.fileSize);
        printf("\n");
        
        fileTable[i] = file;
        ret = AllocateBlocks(fileTable[i].fileSize); 
        if(ret == 0){
            fileTable[i].startBlock = blockStart;
            for(int j = 0; j < file.fileSize;j++){
                blockStatus[blockStart - 1 + j] = 1;
            }
            fileTable[i].allotStatus = 1;
        }else{
            printf("Insufficient memory!");
            exit(0);
        }
        //--- 
    }
    
    printf("FILE_fileName\t FILE_SIZE \t BLOCKS_OCCUPIED\n");
    for(i = 0; i < numFiles; i++){
        printf("%s\t %d\t\t ",fileTable[i].fileName,fileTable[i].fileSize);
        for(int j = 0; j< fileTable[i].fileSize; j++){
            if(j < fileTable[i].fileSize -1){
                printf("%d-",fileTable[i].startBlock + j);
            }else{
                printf("%d",fileTable[i].startBlock + j);
            }
        }
        printf("\n");
    }
    printf("File allocation completed. Exiting.\n");
}