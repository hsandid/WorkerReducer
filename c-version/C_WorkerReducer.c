#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/shm.h>
#include<sys/stat.h>
#include<sys/wait.h>
#include <string.h>
#include <time.h> 

//Should use wait/waitpid/exit commands

#define SHMSZ 4*sizeof(long)

int main(int argc, char **argv)
{
    //Checking if the correct number of arguments has been given
    if (argc>2)
    {
        printf("Error! This program only takes a single argument N\n");
        return 0;
    }
    else if(argc<2)
    {
        printf("Error! No argument given apart from program name\n");
        return 0;
    }

    //Start counting time
    clock_t start, end;
    double cpu_time_used;
    start = clock();

    //Obtaining argument N and calculating the correct amount of lines 320000/N
    int N;
    sscanf(argv[1], "%d", &N);
    int lines = 100/N;
    char linesBuffer[sizeof(char)*7];
    snprintf(linesBuffer, sizeof linesBuffer, "%d", lines);


    pid_t childpid;
    childpid = fork();

    if(childpid==0)
    {
          execlp("split", "split", "-l", linesBuffer ,"-d","input.txt","output",NULL);  
    }
    else if (childpid<0)
    {
        printf("Error in fork\n");
    }

    //Waiting for split process to end
    int status;
    waitpid(childpid ,&status, 0);

    //Creating N shared memories of size 4*sizeof(long)
    pid_t *shm_ids=(pid_t *)malloc(sizeof(pid_t)*N);

    for(int i=0;i<N;i++)
    {
        shm_ids[i]= shmget(IPC_PRIVATE, SHMSZ, S_IRUSR | S_IWUSR);
        // printf("Shared memory id for %d is %d\n",i,shm_ids[i]);
    }

    //Creating N worker processes (i.e load up new worker program)
    pid_t *worker_pids=(pid_t *)malloc(sizeof(pid_t)*N);
    char shmid_to_char[sizeof(char)*10];
    char id_worker[sizeof(char)*2];
    for(int i=0;i<N;i++)
    {
        worker_pids[i]=fork();
        if (worker_pids[i]==0)
        {
            //Load up new program
            snprintf(id_worker, sizeof id_worker, "%d", i);
            snprintf(shmid_to_char, sizeof shmid_to_char, "%d", shm_ids[i]);
            // printf("id worker %s\n",id_worker);
            // printf("shmid associated to worker %s\n",shmid_to_char);

            execl("worker","worker", id_worker, shmid_to_char,NULL);
            
        }
        else if (worker_pids[i]<0)
        {
            printf("Error in fork\n");
        }
    }

    //Creating 3 reducer processes + count amount of CMPS/CCE/ECE
    long cmpsAmount=0;
    long cceAmount=0;
    long eceAmount=0;

    long *message;
    pid_t reducer_pid[3];
    for(int i=0;i<3;i++)
    {
        reducer_pid[i]=fork();
        if (reducer_pid[i]==0)
        {
            //Load up new program
            if(i==0)
            {
                //Counts amount of CMPS words
                for(int j=0;j<N;j++)
                 {  
                    message = (long *) shmat(shm_ids[j],NULL,0);
                    while(message[0] != 999) 
                    {
                        sleep(1);
                    }
                    cmpsAmount+= message[1];
                    shmdt(message);
                 }

                 printf("Total amount of CMPS students is %ld\n",cmpsAmount);

                return 0;
            }
            else if (i==1)
            {
               //Counts amount of CCE words
                for(int j=0;j<N;j++)
                 {  
                    message = (long *) shmat(shm_ids[j],NULL,0);
                    while(message[0] != 999) 
                    {
                        sleep(1);
                    }
                    cceAmount+= message[2];
                    shmdt(message);
                 }

                 printf("Total amount of CCE students is %ld\n",cceAmount);

                return 0;
            }
            else if (i==2)
            {
                //Counts amount of ECE words
                for(int j=0;j<N;j++)
                 {  
                    message = (long *) shmat(shm_ids[j],NULL,0);
                    while(message[0] != 999) 
                    {
                        sleep(1);
                    }
                    eceAmount+= message[3];
                    shmdt(message);
                 }

                 printf("Total amount of ECE students is %ld\n",eceAmount);

                return 0;
            }
            else
            {
                printf("Error in fork\n");
            }
            
     
        }
        else if (reducer_pid[i]<0)
        {
            printf("Error in fork\n");
        }

    }

    //Wait for 3 reducer processes to end (did not redefine 'int status')
    for(int i=0;i<3;i++)
    {
        waitpid(reducer_pid[i] ,&status, 0);
    }
    // printf("All reducer processes done!\n");

    //Wait for N worker processes to end (did not redefine 'int status')
    for(int i=0;i<N;i++)
    {
        waitpid(worker_pids[i] ,&status, 0);
    }
    // printf("All worker processes done!\n");



    //Detaching from shared memory for parent & deleting
    for(int i=0;i<N;i++)
    {
        //Add shmat if any usage
        shmctl(shm_ids[i], IPC_RMID, NULL);
    }

    //Delete array of shm_ids
    free(shm_ids);

    //End counting time
    end = clock();
    cpu_time_used = (((double) (end - start)) / CLOCKS_PER_SEC)*10000;

    //Appending to file
    FILE *pFile;
    pFile = fopen("temp.txt","a");
    char arr[sizeof(cpu_time_used)];
    memcpy(arr,&cpu_time_used,sizeof(cpu_time_used));
    fprintf(pFile," %f",cpu_time_used);
    fclose(pFile);
    return 0;
}