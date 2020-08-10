#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/shm.h>
#include<string.h>

//Let the worker take as argument
//the index of the split and shm_id

int main(int argc, char **argv)
{
     //Checking if the correct number of arguments has been given
    if (argc>3)
    {
        printf("Error! This program takes 2 arguments N (worker)\n");
        return 0;
    }
    else if(argc<3)
    {
        printf("Error! No argument given apart from program name (worker)\n");
        return 0;
    }

    int splitIndex;
    sscanf(argv[1], "%d", &splitIndex);

    pid_t shmid;
    sscanf(argv[2], "%d", &shmid);

    

    //Open specific file
    char* outputName = "output0";
    char* outputValue = argv[1];

    char* fileName;
    fileName = malloc(strlen(outputName)+2); 
    strcpy(fileName, outputName); 
    strcat(fileName, outputValue); 

    // printf("Worker Handles output%d and has shmid of %d and opens file %s\n",splitIndex,shmid,fileName);

    FILE *fp;

    fp = fopen(fileName, "r");

    
    //Read instances of CMPS,CCE,ECE and store them into variables
    long cmpsAmount=0;
    long cceAmount=0;
    long eceAmount=0;
    char temp[10];
    while (fscanf(fp, " %10s", temp) == 1) {
        if(strcmp(temp,"CMPS")==0)
        {
            cmpsAmount++;
        }
        else if (strcmp(temp,"CCE")==0)
        {
            cceAmount++;
        }
        else if (strcmp(temp,"ECE")==0)
        {
            eceAmount++;
        }
    }
   
    
    //Open shared memory 
    long *message;
    message = (long *) shmat(shmid,NULL,0);
    
    //Add variables
    message[0] = 999;
    message[1] = cmpsAmount;
    message[2] = cceAmount;
    message[3] = eceAmount;

    
    //Detach shared memory & close file
    shmdt(message);
    fclose(fp);

    //Exit
    // printf("Amount of CMPS is %ld, amount of CCE is %ld, amount of ECE is %ld\n", cmpsAmount,cceAmount,eceAmount);
    return 0;
}