/*Jessica Guo (ಠ益ಠщ)
Christine Do

Assignment02 Threads and Processes */

//Processe manager and Parent Process

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> //fork() for processes
#include <pthread.h> //for threads
#include <string.h>
#include <dirent.h> //for readdir()
#include <ctype.h>
#include <time.h>

#define MAX 5000 //500 compressions 

//isspace() for space: spaces, tabs, newline
//isalpha() for alphabetical
//isdigit() 

/*read in arguments in the form of:
compressT_LOLS(<file to compress>, <number of parts>)
compressR_LOLS(<file to compress>, <number of parts>)
*/

//fork() creates processes
//pthread_create() threads


//compress function takes in pointer to beginning and end of string of file
//char *compress(DIR *dirp, char *start, char *end) are the parameters....

char *compress(char *input)
{     
  int letter_count;
  char count[MAX];
  int len = strlen(input);
   
  char *output = (char *)malloc(sizeof(char)*(len)+ 1);
  
  int i=0; //original string iterator
  int j = 0; //ouput iterator
  int k=0; //count iterator
 
  //traverse the input 
  for(i = 0; i < len; i++)
  {
    if(isalpha(input[i])){
 
    char temp = input[i];
     
    // Count the number of occurrences 
    letter_count = 1;     
    while(i + 1 < len && ((temp == input[i+1])|| isdigit(input[i+1])|| isspace(input[i+1])))
    {
      if(temp == input[i+1]){
        letter_count++;
      }
      if(isdigit(input[i])){
        printf("Warning digit found: %c\n",input[i]);
      }

      i++;
    }  
    // Store letter_count in a character array count[] 
    sprintf(count, "%d", letter_count);
    //printf("%d\n",letter_count);
 
    //Copy the count[] to outputination 
   for(k = 0; *(count+k); k++, j++)
   { 
      if(count[k]== '2'){
        output[j] = temp;

      } 
      //i can't figure it out atm
      /*else if (count[k] =='1'){
        continue;
      }*/
      else{
        output[j]=count[k];
      }
    } 

     output[j++] = input[i];

  }//end of if(isalpha())

    if(isspace(input[i])){
      continue;
    }
    if(isdigit(input[i])){
      printf("warning digit found: %c\n", input[i] );
      continue;
    }

 }

  output[j] = '\0';
  return output;
}   

int main(/*int argc, char * argv[]*/){

/*	if(argc != 2){
		printf("Error: Invalid number of arguments.\n");
		exit(1);
	}*/

/* I can't figure this out rn but its suppose to return a struct with the file size that 
we can use to divide the work up
	DIR *dirp;
	struct dirent *dp;

	if(dirp =opendir(argv[1] ==NULL)){
		perror("ERROR: file not valid.\n");
		exit(1);
	}
*/

/*testing;*/
	char str[] = "bb  bjk4kkll";
	char *comp = compress(str);
	printf("%s\n", comp);

}