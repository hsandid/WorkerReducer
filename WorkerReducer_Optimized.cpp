#include<iostream>
#include<thread>
#include<string.h>
#include<sys/types.h>
#include<unistd.h>
#include <stdio.h> 
#include <sys/types.h> 
#include <fcntl.h> 
#include <time.h> 
#include <fstream>
#include <algorithm> 
#include <chrono> 
using namespace std;
using namespace std::chrono; 



bool *isDone;
long *cmpsAmount;
long *cceAmount;
long *eceAmount;
int N;
int lines;

void modMapper(int mult)
{
    //Initialize counters
    int cmpsCount=0;
    int cceCount=0;
    int eceCount=0;
    long pos=0;
    long totalSize = 20748044;
    int ret;
    char b[4];
    //Start at a specific byte
    int fd = open("input.txt", O_RDONLY); 

    pos = (totalSize/N)*mult;
    lseek(fd,pos,SEEK_CUR);

    //Read all instances and increment respective counters
    ret= read(fd ,&b , sizeof(char)*3);

  if(pos!=0)
  {
    int cmpsCheck=0;
    int cceCheck=0;
    int eceCheck=0;

    if(b[0]=='C' && b[1]=='M' && b[2]=='P')
    {
      cmpsCheck=1;
    }
    if (b[0]=='C'&& b[1]=='C' && b[2]=='E')
    {
      cceCheck=1;
    }
    if (b[0]=='E'&& b[1]=='C' && b[2]=='E')
    {
      eceCheck=1;
    }

    while (cmpsCheck==0 && cceCheck==0 && eceCheck==0)
    {
      pos--;
      lseek(fd,-4,SEEK_CUR);
      ret= read(fd ,&b , sizeof(char)*3);

      if(b[0]=='C' && b[1]=='M' && b[2]=='P')
    {
      cmpsCheck=1;
    }
    if (b[0]=='C'&& b[1]=='C' && b[2]=='E')
    {
      cceCheck=1;
    }
    if (b[0]=='E'&& b[1]=='C' && b[2]=='E')
    {
      eceCheck=1;
    }

    }
  }

    while(pos<(totalSize/N)*mult+(totalSize/N)-1)
    {
        if(b[0]=='C' && b[1]=='M' && b[2]=='P')
  {
    cmpsCount++;
    lseek(fd,1,SEEK_CUR);
    pos+=4;
    ret= read(fd ,&b , sizeof(char)*3);

  }
  else if (b[0]=='C'&& b[1]=='C' && b[2]=='E')
  {
    cceCount++;
    pos+=3;

    ret= read(fd ,&b , sizeof(char)*3);

    

      
  }
  else if (b[0]=='E'&& b[1]=='C' && b[2]=='E')
  {
    eceCount++;
    pos+=3;
 
    ret= read(fd ,&b , sizeof(char)*3);


      
  }
  else
  {
            
            lseek(fd,-2,SEEK_CUR);
            ret= read(fd ,&b , sizeof(char)*3);
            pos+=1;
            
  }

    }

    //End statement
    cmpsAmount[mult]=cmpsCount;
    cceAmount[mult]=cceCount;
    eceAmount[mult]=eceCount;
    isDone[mult]=true;
    close(fd);
}

void modReducer(int id)
{   
    string wordLookout;
    long counter=0;

    for (int z=0;z<N;z++)
    {
        while(isDone[z]==false)
        {
            sleep(1);
        }

        if (id==0)
        {
            counter+=cmpsAmount[z];
        }
        else if (id==1)
        {
            counter+=cceAmount[z];
        }
        else if (id==2)
        {
          counter+=eceAmount[z];
        } 
    }

    if (id==0)
        {
            cout<<"Total amount of CMPS students is "<<counter<<"\n";
        }
        else if (id==1)
        {
             cout<<"Total amount of CCE students is "<<counter<<"\n";
        }
        else if (id==2)
        {
           cout<<"Total amount of ECE students is "<<counter<<"\n";
        }

}

int main(int argc, char **argv)
{

    
    //Arguments
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
    auto start = high_resolution_clock::now(); 

    //Setting N and line per thread
    sscanf(argv[1], "%d", &N);
    lines = 320000/N;

    //Initialize count arrays (based on N)
    cmpsAmount = new long[N];
    cceAmount = new long[N];
    eceAmount = new long[N];

    //Initiliaze logic array (based on N)
    isDone = new bool[N];
    for(int z=0;z<N;z++)
    {
        isDone[z]=false;
    }

    //Initialize modMappers
    thread *modMapperThreads = new thread[N];
    for (int z=0;z<N;z++)
    {
        modMapperThreads[z]= std::thread(modMapper,z);
    }

    //Initialize modReducers
    thread red1(modReducer, 0);
    thread red2(modReducer, 1);
    thread red3(modReducer, 2);

    //Join modReducers
    red1.join();
    red2.join();
    red3.join();

    //Join modMappers
    for (int z=0;z<N;z++)
    {
        modMapperThreads[z].join();
    }
    

    //Destroy dynamic arrays
    delete cmpsAmount;
    delete cceAmount;
    delete eceAmount;
    delete isDone;

    //End counting time and store in file
    auto stop = high_resolution_clock::now(); 
    auto duration = duration_cast<seconds>(stop - start); 
    double cpu_time_used = duration.count();
    ofstream outfile;
    outfile.open("temp2.txt", std::ios_base::app); // append instead of overwrite
    outfile << " "<<cpu_time_used; 


    return 0;
}

