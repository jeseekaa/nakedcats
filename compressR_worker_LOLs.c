/*Jessica Guo (ಠ益ಠщ)
Christine Do

Assignment02 Threads and Processes */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h> //for fork()
#include <sys/stat.h>
#include <ctype.h> //for isdig, etc

//worker : parent calls fork() then exec() this process

typedef struct PreProcessPayload {
    char* fileName;
    int startIndex;
    int processingBlockSize;
    int partitionNumber;
} PreProcessPayload;

PreProcessPayload *createPreProcessPayload(char* fileName, int startIndex, int processingBlockSize, int partitionNumber) {
    PreProcessPayload *payload = (PreProcessPayload *) malloc(sizeof(PreProcessPayload));
    payload->fileName = (char*) malloc((strlen(fileName) + 1)* sizeof(char));
    strcpy(payload->fileName, fileName);
    payload->startIndex = startIndex;
    payload->processingBlockSize = processingBlockSize;
    payload->partitionNumber = partitionNumber;

    return payload;
}

int processEndOfCharSequence(char *buffer, int buffer_i, int count, char curr) {
    if (count > 2) {
        buffer_i += sprintf(&buffer[buffer_i], "%d%c", count, curr);
    } else {
        int i;
        for (i = 0; i < count; i++) {
            buffer[buffer_i] = curr;
            buffer_i++;
        }
    }

    return buffer_i;
}

char *compressString(char *s) {
    printf("compressing: %s\n", s);
    char* buffer = (char *) malloc((strlen(s) + 1) * sizeof(char));
    int buffer_i = 0;

    int count = 0;
    char curr = *s;

    int i;
    for (i = 0; i <= strlen(s); i++) {
        if(isalpha(curr)){
            if (curr == s[i]) {
                count++;
            }
            else {
                buffer_i = processEndOfCharSequence(buffer, buffer_i, count, curr);

                count = 1;
                curr = s[i];
            }
        }
      else if (isspace(curr)){
               curr=s[i];
         }
        
      else {
            if(isdigit(curr)){
                printf("Warning: non-alphabetical detected: %c\n", curr);
            }

            curr = s[i]; 
        }

    } //end of for loop, parsing input string
    buffer_i = processEndOfCharSequence(buffer, buffer_i, count, curr);

    // null terminate buffer
    buffer[buffer_i] = '\0';

    // create new string that uses only as much memory needed;
    char *return_str = (char *) malloc(buffer_i * sizeof(char));
    strcpy(return_str, buffer);
    free(buffer);

    printf("compressed: %s\n", return_str);
    return return_str;
}


void* compressFileChunk(void *arg)
{
    PreProcessPayload *payload = (PreProcessPayload *) arg;

    FILE *fp;
    fp = fopen(payload->fileName, "r");
    fseek(fp, payload->startIndex, SEEK_SET);


    char *buffer = (char *) malloc(payload->processingBlockSize * sizeof(char));
    int i;
    for (i = 0; i < payload->processingBlockSize && !feof(fp); i++) {
        int c = fgetc(fp);

        if (c == EOF) {
            break;
        }

        buffer[i] = c;
    }
    printf("buffer: %s\n", buffer);

    // needs a better memory allocation in case an insanely big partition number is provided
    char *outfileName = (char *) malloc(strlen(payload->fileName) + 6);
    char *testing = payload->fileName;
    int length = strlen(testing)-4;
    char *fileNameToken = (char *) malloc(length);
    char outfileNameBuffer[length+100];
    //pls dont make many threads ty u_u


    int cutOffIndex = strlen(payload->fileName) - 4;
    payload->fileName[cutOffIndex] = '\0';
    //sprintf(outfileNameBuffer, "%s_LOLS%d.txt", payload->fileName, payload->partitionNumber);

    if(payload -> partitionNumber == -1) {
        sprintf(outfileNameBuffer, "%s_LOLS.txt", payload->fileName);
    } else {
        sprintf(outfileNameBuffer, "%s_LOLS%d.txt", payload->fileName, payload->partitionNumber);
    }
    printf("text file name: %s, length: %d, new file name : %s\n", testing, length, outfileNameBuffer);

    FILE *outfile;
    outfile = fopen(outfileNameBuffer, "w");

    int ret = fputs(compressString(buffer), outfile);
    if (ret == EOF) {
        // do some shit here
    }

    return NULL;
}

int main(int argc, char * argv[]){

//checking iput
if(argc != 3) {
		printf("Error: Invalid number of arguments.\n");
		exit(1);
	}

printf("Hi, we are in the worker code now!!\n");

char* fileName = argv[1];
int numWorkers = atoi(argv[2]);

int i = 0;
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

    //exist check

    char fileNameToken[100];
    char existenceCheck[100];
    char existenceCheck0[100];
    strcpy(fileNameToken, fileName);

    int cutOffIndex1 = strlen(fileNameToken) - 4;
    fileNameToken[cutOffIndex1] = '\0';

    sprintf(existenceCheck, "%s_LOLS.txt", fileNameToken);

    sprintf(existenceCheck0, "%s_LOLS0.txt",fileNameToken);

    printf("%s %s testing fo these \n", existenceCheck, existenceCheck0);


    if(access(existenceCheck, F_OK) != -1 || access(existenceCheck0, F_OK) != -1) {
        printf("file already compressed, try new file\n");
        return -1;
    }


//size per worker

int chunkSize = fileSize/numWorkers;
int remainderDistribution = fileSize % numWorkers;

printf("chunk size: %d\n", chunkSize);



//start the forking 
pid_t myid = getpid();
printf("Initial PID (in the worker file): %d\n", myid);

int startIndex =0;

pid_t pids[numWorkers];

//send out the spawns of satan
for(i=0; i<numWorkers; i++){

	int actualChunkSize = chunkSize;

   //append extra bytes to the first chunk
    if(remainderDistribution>0){
        actualChunkSize= actualChunkSize+remainderDistribution;
        remainderDistribution =0;
    }
    if(i==0){
        startIndex =0 ;
    }
    else if(i==1){
        startIndex = startIndex + actualChunkSize + (fileSize%numWorkers);
    }

    else {
        startIndex= startIndex + actualChunkSize;
    }

    int partitionNumber;

    if (numWorkers == 1){
        partitionNumber = -1;
    } else {
        partitionNumber = i;
    }

    PreProcessPayload *payload = createPreProcessPayload(fileName, startIndex, actualChunkSize, partitionNumber);
    
	if((pids[i] =fork())<0){
		printf("ERROR: fork() failed.\n");
		exit(1);
	}
	else if(pids[i]==0){
		printf("in the child process #%d\n", i);
		compressFileChunk(payload);
		exit(0);
	}
}//end of for loop

int status;
pid_t pid;

//waiting for each process to end
while(numWorkers>0){
	pid=wait(&status);
	printf("child wiht PID %ld exited with status 0x%x.\n", (long)pid, status);
	numWorkers--;
}

sleep(3);
return 0;

}
