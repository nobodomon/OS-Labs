#include <stdio.h>
#include <stdlib.h>

#define TOTAL_DISK_BLOCKS 32
#define TOTAL_DISK_INODES 8
#define BLOCK_SIZE_IN_KB 1
#define ALLOCATION_METHOD "INDEXED"
int blockStatus[TOTAL_DISK_BLOCKS]; // free = 0
int blockList[TOTAL_DISK_BLOCKS - TOTAL_DISK_INODES];

struct file_table {
    char fileName[20];
    int fileSize;
    int blocks[TOTAL_DISK_BLOCKS - TOTAL_DISK_INODES];
};

struct file_table fileTable[TOTAL_DISK_BLOCKS - TOTAL_DISK_INODES];

int AllocateBlocks(int Size) {
    int i = 0, count = 0, inList = 0, nextBlock = 0;
    int allocStartBlock = TOTAL_DISK_INODES;
    int allocEndBlock = TOTAL_DISK_BLOCKS - 1;

    // Check whether sufficient free blocks are available
    for (i = 0; i < (TOTAL_DISK_BLOCKS - TOTAL_DISK_INODES); i++) {
        if (blockStatus[i] == 0)
            count++;
    }

    if (count < Size)
        return 1; // not enough free blocks

    count = 0;
    while (count < Size) {
        nextBlock = (rand() % (allocEndBlock - allocStartBlock + 1)) + allocStartBlock;
        if (blockStatus[nextBlock] == 0) {
            blockStatus[nextBlock] = 1;
            blockList[count] = nextBlock;
            count += 1;
        }
    
    }    
    if (count == Size)
        return 0; // Success
    else
        return 1; // 
}


void main() {
    int i = 0, j = 0, numFiles = 0, nextBlock = 0, ret = 1;
    char s[20]; struct block *temp;
    printf("File allocation method: %s\n", ALLOCATION_METHOD);
    printf("Total blocks: %d\n", TOTAL_DISK_BLOCKS);
    printf("File allocation start at block: %d\n", TOTAL_DISK_INODES);
    printf("File allocation end at block: %d\n", TOTAL_DISK_BLOCKS -1);
    printf("Size (kB) of each block: %d\n", BLOCK_SIZE_IN_KB);

    printf("Enter no of files: ");
    scanf("%d", &numFiles);
    printf("\n");

    //Seed the psuedo-random number generator used by rand() with the value seed
    srand(1234);
    for (i = 0; i < numFiles; i++) {
        struct file_table file;
        printf("Enter the name of file #%d: ", i + 1);
        scanf("%s", file.fileName);
        printf("Enter the size (kB) of the file #%d: ", i + 1);
        scanf("%d", &file.fileSize);
        fileTable[i] = file;
        ret = AllocateBlocks(fileTable[i].fileSize);
        // If successful
        if (ret == 0) {
            for (j = 0; j < fileTable[i].fileSize; j++) {
                fileTable[i].blocks[j] = blockList[j];
            }
        }else{
            printf("Insufficient memory!");
            exit(0);
        }
            
    } 


    printf("\nFILE_fileName \t\tFILE_SIZE \t\tBlOCKS_OCCUPIED\n");
    for (j = 0; j < numFiles; j++) {
        printf("%s \t\t%d \t\t\t", fileTable[j].fileName, fileTable[j].fileSize);
        for (nextBlock = 0; nextBlock < fileTable[j].fileSize; nextBlock++) {
            if (nextBlock)
                printf("-");
            printf("%d", fileTable[j].blocks[nextBlock]);
        }
        printf("\n");
    }
    printf("File allocation completed. Exiting.\n");
}