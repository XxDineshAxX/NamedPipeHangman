/* make up unique pipename
 * send it to the server
 * get another unique pipename from the server
 * repeat
 * 	get the starred word from the new pipe
 *  display to the user 
 *  get the user's guess letter
 *  send to the server 
 */
//Usage: quoteclient <server-fifo-filename>
//
//create own named pipe (fifo) and set permissions
//send this fifo name to server
//open own named pipe
//read the quote and display

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>

#define MAXLEN 1000

int main(int argc, char *argv[]) {
        
        char clientfifo[MAXLEN];
	char serverfifo[MAXLEN];
        sprintf(clientfifo, "/tmp/%s-%d", getenv("USER"), getpid());
        mkfifo(clientfifo, 0600);
        chmod(clientfifo, 0622);

	sprintf(serverfifo, "/tmp/%s", getenv("USER"));        


        FILE *fp = fopen(serverfifo, "w");
        fprintf(fp, "%s", clientfifo);
        fclose(fp);
	int status = 0;

	char uniqPipe[MAXLEN];
	FILE *fp2 = fopen(clientfifo, "r");
                if (!fp2) {
                        printf("FIFO %s cannot be opened for reading.\n", clientfifo);
                        exit(2);
                }
               
                fgets(uniqPipe, MAXLEN, fp2);
                char *cptr = strchr(uniqPipe, '\n');
                if (cptr){
                   *cptr = '\0';
                }

         fclose(fp2);






        while (status == 0){
        FILE *wordfp = fopen(clientfifo, "r");
	char line[MAXLEN];
        
        while (fgets(line, MAXLEN, wordfp))
                printf("%s",line);
	
	fclose(wordfp);	  
	
	if (line[0] == 'T'){
		status = 1;
	}
	
	if (status == 0){
	fp = fopen(uniqPipe, "w");
	char guess[MAXLEN];
	scanf("%s", &guess);
	fprintf(fp, "%s", guess);
        fclose(fp);
        }
	}

        unlink(clientfifo);
}

