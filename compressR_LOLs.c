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
char fileNameToken[100];
char existenceCheck[100];
char existenceCheck0[100];

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
    strcpy(fileNameToken, fileName);

    int cutOffIndex1 = strlen(fileNameToken) - 4;
    fileNameToken[cutOffIndex1] = '\0';

    sprintf(existenceCheck, "%s_LOLS.txt", fileNameToken);

    sprintf(existenceCheck0, "%s_LOLS0.txt",fileNameToken);

    if(access(existenceCheck, F_OK) != -1 || access(existenceCheck0, F_OK) != -1) {
        printf("file already compressed, try new file\n");
        exit(1);
    }

//error for if asking to create more workers than bytes in the files
//ask for smaller input , exit(1) the program
if(numWorkers>fileSize){
  printf("ERROR: Number of output files is greater than size of file.\n");
  printf("Please enter a number less than or equal to %d\n", fileSize);
  exit(1);
}

char* parameters[] = {"./worker", argv[1], argv[2], NULL, NULL};
execvp(parameters[0], parameters);
printf("ERROR: execvp failed\n");

//all the inputs good
//forking everything and execute the worker file
return 0;

}
