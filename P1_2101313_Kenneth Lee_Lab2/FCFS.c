#include <stdio.h> 
 
int main () 
{ 
    int i = 0, j = 0, numProcesses= 0, timeSlice = 0, maxBurstTime = 0, temp = 0; 
    int  burstTime[10], backupBurstTime[10], waitTime[10], turnAroundTime[10], 
responseTime[10]; 
    float avgWaitTime = 0.0, avgTurnAroundTime = 0.0, avgResponseTime = 0.0; 
 
    printf("\t CPU Scheduling Method: First Come First Serve \n"); 
     
    printf("\n\t Enter the no of processes: "); 
    scanf("%d", &numProcesses); 
    for (i = 0; i < numProcesses; i++) 
    { 
        printf("\n\t Enter Burst Time for process %d: ", i + 1); 
        scanf("%d", &burstTime[i]); 
        backupBurstTime[i] = burstTime[i]; 
    } 
 
    for (i = 0; i < numProcesses; i++){ 
        turnAroundTime[i] = temp + burstTime[i]; 
        waitTime[i] = turnAroundTime[i] - backupBurstTime[i];
        responseTime[i] = waitTime[i];
        temp = temp + burstTime[i]; 
        burstTime[i] = 0; 

        // add wait respective times to total avg
        avgWaitTime += waitTime[i];
        avgResponseTime += responseTime[i];
        avgTurnAroundTime += turnAroundTime[i];
    } 
 
    /*Calculate the ‘waiting time’, ‘turn-around-time’ and  
      'response time' for each process. */ 
    
 
 
    printf("\n\t PROCESS\t BURST TIME\t WAITING TIME\t TURNAROUND TIME\t RESPONSE TIME\n"); 
    for (i = 0; i < numProcesses; i++) 
        printf("\t P%d \t %d \t\t %d \t\t %d \t\t\t %d \n",  
            i + 1, backupBurstTime[i], waitTime[i], turnAroundTime[i], responseTime[i]); 
    
    

    /*Calculate and print the ‘average waiting time’,  
    ‘average turn-around-time’, and 'average response time' */ 
    printf("\n\t The Average Waiting time: %.2f\n", avgWaitTime / numProcesses); 
    printf("\n\t The Average Turnaround time: %.2f\n", avgTurnAroundTime / numProcesses); 
    printf("\n\t The Average Response time: %.2f\n", avgResponseTime / numProcesses); 
 
    return 0; 
}