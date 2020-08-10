#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/shm.h>
#include<string.h>
#include<sys/stat.h>
#include<sys/wait.h>
#include<string.h>
#include <iostream>
#include <fstream>
using namespace std;
int main()
{
    
    double execNo[4];
    double execOp[4];
    int values[4]={1,2,4,8};
    pid_t worker_pids[4];
    char shmid_to_char[sizeof(char)*10];
    //Run 4 non-optimized thread and store their time
    for(int i=0;i<4;i++)
    {
        worker_pids[i]=fork();
        if(worker_pids[i]==0)
        {
            snprintf(shmid_to_char, sizeof shmid_to_char, "%d", values[i]);
            execl("run","run", shmid_to_char, NULL);
            return 0;
         }
    }
    
    int status;
     for(int i=0;i<4;i++)
    {
        waitpid(worker_pids[i] ,&status, 0);
    }

    

    //Run 4 optimized thread and store their time
    for(int i=0;i<4;i++)
    {
        worker_pids[i]=fork();
        if(worker_pids[i]==0)
        {
            snprintf(shmid_to_char, sizeof shmid_to_char, "%d", values[i]);
            execl("run_opt","run_opt", shmid_to_char, NULL);
            return 0;
         }
    }
    
     for(int i=0;i<4;i++)
    {
        waitpid(worker_pids[i] ,&status, 0);
    }

    //Get temp
    int i=0;
    double value;
    ifstream infile;
    infile.open("temp.txt"); 
    while(infile >> value && i<4)
    {
        
           
    		execNo[i] = value;    //assigns to the array
    		i++;
    	
       
          
    		
    	
    }    
	infile.close();
    i=0;
    infile.open("temp2.txt"); 
    while(infile >> value && i<4)
    {
        
           
    		execOp[i] = value;    //assigns to the array
    		i++;
    	
       
          
    		
    	
    }    
	infile.close();


    //Finalize bench.txt
    ofstream outfile;
    outfile.open("bench.txt", std::ios_base::binary); // append instead of overwrite
    outfile <<1<<" "<<execNo[0]<<" "<<execOp[0]<<endl;
    outfile <<2<<" "<<execNo[1]<<" "<<execOp[1]<<endl;
    outfile <<3<<" "<<execNo[2]<<" "<<execOp[2]<<endl;
    outfile <<4<<" "<<execNo[3]<<" "<<execOp[3]<<endl;
    outfile.close();
    //Exit
    return 0;
}