#include <stdio.h> 
 
int main () 
{ 
    int i = 0, j = 0, numProcesses= 0, timeSlice = 0, maxBurstTime = 0, temp = 0, pos = 0; 
    int processes[10], burstTime[10], backupBurstTime[10], waitTime[10], turnAroundTime[10], 
responseTime[10]; 
    float avgWaitTime = 0.0, avgTurnAroundTime = 0.0, avgResponseTime = 0.0; 
 
    printf("\t CPU Scheduling Method: Shortest Job First \n"); 
     
    printf("\n\t Enter the no of processes: "); 
    scanf("%d", &numProcesses); 
    for (i = 0; i < numProcesses; i++) 
    { 
        printf("\n\t Enter Burst Time for process %d: ", i + 1); 
        scanf("%d", &burstTime[i]); 
        backupBurstTime[i] = burstTime[i]; 
        processes[i] = i + 1;
    } 
    // Sort the processes
    for (i = 0; i < numProcesses; i++){ 
        pos = i;

        for(j = i + 1; j< numProcesses; j++){
            if(burstTime[j] < burstTime[pos]){
                pos = j;
            }
        }

        //Swap the longest with the i
        temp = burstTime[i];
        burstTime[i] = burstTime[pos];
        burstTime[pos] = temp;

        temp = processes[i];
        processes[i] = processes[pos];
        processes[pos] = temp;

    } 

    waitTime[0] = 0;
    // Calculate wait time for each process
    for(int i = 1; i< numProcesses; i++){
        waitTime[i] = 0;
        for(j = 0; j< i; j++){
            waitTime[i] += burstTime[j];
        }
        avgWaitTime += waitTime[i];
    }

    for(int i = 0; i < numProcesses; i++){
        turnAroundTime[i] = burstTime[i] + waitTime[i];
        responseTime[i] = waitTime[i];

        // add wait respective times to total avg
        avgResponseTime += waitTime[i];
        avgTurnAroundTime += turnAroundTime[i];
    }
 
    printf("\n\t PROCESS\t BURST TIME\t WAITING TIME\t TURNAROUND TIME\t RESPONSE TIME\n"); 
    for (i = 0; i < numProcesses; i++) 
        printf("\t P%d \t\t %d \t\t %d \t\t %d \t\t\t %d \n",  
            processes[i], burstTime[i], waitTime[i], turnAroundTime[i], responseTime[i]); 
    
    

    /*Calculate and print the ‘average waiting time’,  
    ‘average turn-around-time’, and 'average response time' */ 
    printf("\n\t The Average Waiting time: %.2f\n", avgWaitTime / numProcesses); 
    printf("\n\t The Average Turnaround time: %.2f\n", avgTurnAroundTime / numProcesses); 
    printf("\n\t The Average Response time: %.2f\n", avgResponseTime / numProcesses); 
 
    return 0; 
}