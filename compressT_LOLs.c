/*Jessica Guo (ಠ益ಠщ)
Christine Do

Assignment02 Threads and Processes */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/stat.h>

/*read in arguments in the form of:
compressT_LOLS(<file to compress>, <number of parts>)
compressR_LOLS(<file to compress>, <number of parts>)
*/

//compression function, serves as a thread, that takes in the beginning and end of when it should compress w/ variables 
//outputs result into new file with a propended # if >1 file created 

int main(int argc, char * argv[]){
	//check argument input
	struct stat st;
	int fileSize;
	if (argc != 3) {
		printf("Invalid number of arguments\n");
		return 1;
	}
	if (stat(argv[1], &st) == -1) {
		printf("file error - does not exist or cannot be opened\n");
		return 1;
	} else {
		fileSize = st.st_size;
		printf("the string is %d chars long\n", fileSize);
		//check the permissions and if this has already been called and stuff.. will look into manpages later lols
	}

}

