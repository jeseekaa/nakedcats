/*Jessica Guo (ಠ益ಠщ)
Christine Do

Assignment02 Threads and Processes */

//Processe manager and Parent Process

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> //fork() for processes
//#include <pthread.h> //for threads
#include <string.h>
#include <sys/stat.h>
#include <ctype.h>
#include <time.h>


int main(int argc, char * argv[]){

//checking input
if(argc != 3) {
    printf("Error: Invalid number of arguments.\n");
    exit(1);
  }

char* fileName = argv[1];
int numWorkers = atoi(argv[2]);

int i = 0;
pid_t id;

struct stat st;
int fileSize;

if(stat(fileName, &st)==0){
  fileSize = st.st_size;

}else{
  printf("file opening error\n");
  exit(1);
}

//error for if asking to create more workers than bytes in the files
//ask for smaller input , exit(1) the program
if(numWorkers>fileSize){
  printf("ERROR: Number of output files is greater than size of file.\n");
  printf("Please enter a number less than or equal to %d\n", fileSize);
  exit(1);
}

//the inputs are acceptanble, exec() the worker file 

char* parameters[] = {"./compressR_worker_LOLs", argv[1], argv[2], NULL, NULL};
execvp(parameters[0], parameters);
printf("ERROR: execvp failed\n");


return 0;

}
