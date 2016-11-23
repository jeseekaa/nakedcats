/*Jessica Guo (ಠ益ಠщ)
Christine Do

Assignment02 Threads and Processes */

#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <ctype.h>
/*read in arguments in the form of:
compressT_LOLS(<file to compress>, <number of parts>)
compressR_LOLS(<file to compress>, <number of parts>)
*/

//compression function, serves as a thread, that takes in the beginning and end of when it should compress w/ variables 
//outputs result into new file with a propended # if >1 file created 


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
        if(isalpha(curr) || isalpha(s[i])){
            if (curr == s[i]) {
                count++;
            } else if (isspace(curr) ||isspace(s[i])){ //i think something here is not one correctly
                curr=s[i];
                continue;
            }
            else {
                buffer_i = processEndOfCharSequence(buffer, buffer_i, count, curr);

                count = 1;
                curr = s[i];
            }
        }
        
       else {
            if(isdigit(s[i])){
                printf("Warning: non-alphabetical detected: %c\n", s[i]);
            }
            curr=s[i]; 
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

int main(int argc, char *argv[])
{
    char* fileName = argv[1];
    char fileNameToken[100];
    int numWorkers = atoi(argv[2]);
    char existenceCheck[100];
    char existenceCheck0[100];

    pthread_t *tid = malloc(sizeof(pthread_t*) * numWorkers);

    int i = 0;
    int err;

    struct stat st; 
    int fileSize;

    /*chunk size logic...
    merrrh
    */


    if (stat(fileName, &st) == 0) {
        fileSize = st.st_size;
    } else {
        printf("file opening error\n");
        return -1;
    }

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
   
    int chunkSize = fileSize / numWorkers;
    int remainderDistribution = fileSize % numWorkers;
    printf("chunk size: %d\n", chunkSize);

    int startIndex=0;

    while(i < numWorkers)
    {
        int actualChunkSize = chunkSize;

        /*if(remainderDistribution > 0) {
            actualChunkSize++;
            remainderDistribution--;
        }*/
	    
	/*I suck so i just appended extra bytes to the first thread/file
	if i have time after processes i can try to distribute */
        if(remainderDistribution>0){
            actualChunkSize= actualChunkSize+remainderDistribution;
            remainderDistribution =0;
        }

	//naive way of calculating the start position of each thread.
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
    

        err = pthread_create(&(tid[i]), NULL, &compressFileChunk, (void *) payload);
        if (err != 0)
            printf("\ncan't create thread :[%s]", strerror(err));
        else
            printf("\n Thread created successfully\n");

        i++;
    }

    sleep(5);

    return 0;
}
