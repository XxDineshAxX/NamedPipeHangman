/*
 * read dictionary file to array of words & get ready to play the hangman!
 * wait for a request to come in (unique pipename)
 * respond with another unique pipename 
 * send a bunch of stars (indicating the word length)
 * fork() to enable games to proceed in parallel.
 * for each guess the client sends in, respond with a message 
 * and send updated display word.
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>

#define MAXWORDS 100000
#define MAXLEN 100000

char *words[MAXWORDS];
int numWords=0;

int main() {
        char line[MAXLEN];

        FILE *fp = fopen("dictionary.txt", "r");
        if (!fp) {
                puts("dictionary.txt cannot be opened for reading.");
                exit(1);
        }
        int i=0;

        //read one line at a time, allocate memory, then copy the line into array
        while (fgets(line, MAXLEN, fp)) {
                words[i] = (char *) malloc (strlen(line)+1);
                strcpy(words[i], line);
                i++;
        }
        numWords = i;
        printf("%d words were read.\n", numWords);

        srand(getpid() + time(NULL) + getuid());
        

        //create a named pipes to read client's requests
        char filename[MAXLEN];
        sprintf(filename, "/tmp/%s", getenv("USER"));
        mkfifo(filename, 0600);
        chmod(filename, 0622);
        
	


        while (1) {

								
		
                FILE *fp = fopen(filename, "r");
                if (!fp) {
                        printf("FIFO %s cannot be opened for reading.\n", filename);
                        exit(2);
                }
                printf("Opened %s to read...\n", filename);
                fgets(line, MAXLEN, fp);
                char *cptr = strchr(line, '\n');
                if (cptr){
                   *cptr = '\0';
		}

		fclose(fp);

		int wordNumber = rand() % numWords;
	
            
		if (fork() == 0){
		
		char filename2[MAXLEN];
	        sprintf(filename2, "/tmp/%s-%d", getenv("USER"), getpid());
	        mkfifo(filename2, 0600);
	        chmod(filename2, 0622);
	

		FILE *fp2 = fopen(line, "w");
	        fprintf(fp2, "%s", filename2);
	        fclose(fp2);
	

		char hangWord[MAXLEN];
                char display[MAXLEN];
               //int wordNumber = rand() % numWords;
                printf("%s", words[wordNumber]);

                char *cptr2 = strchr(words[wordNumber], '\n');
                if (cptr2){
                   *cptr2 = '\0';
                }

                strcpy(hangWord, words[wordNumber]);
                strcpy(display, words[wordNumber]);
                int n = strlen(hangWord);

                for(int i = 0; i < n; i++){
                        display[i] = '*';
                }

                int unexposed = n;
                char guess[MAXLEN];
                int wrongGuesses = 0;
		


		while (unexposed > 0){
		
                FILE *clientfp = fopen(line, "w");
                //printf("%s: ", line);
		
		int found = 0;
		if (guess[0] != '\0'){
                for(int i = 0; i < n; i++){
                        if (guess[0] == hangWord[i]){
                                found = 1;
                                if (guess[0] == display[i]){
					fprintf(clientfp, "%c is already in the word\n", guess[0]);
					break;
                                }
				else {
					display[i] = guess[0];
					unexposed--;
				}
                        }

                }
		if (found == 0){
			fprintf(clientfp, "%c is not in the word\n", guess[0]);
			wrongGuesses++;

		}
		}
	
  		              
                if (unexposed > 0){
                fprintf(clientfp, "\n(Guess) Enter a letter in word %s > ", display);
		}
                fclose(clientfp);
                
                
		
		if (unexposed > 0){
		fp = fopen(filename2, "r");
		if (!fp) {
                        printf("FIFO %s cannot be opened for reading.\n", filename2);
                        exit(2);
                }
//                printf("Opened %s to read...\n", filename2);
                fgets(guess, MAXLEN, fp);
          

                fclose(fp);
		}
		}
		FILE *endfp = fopen(line, "w");
		fprintf(endfp, "The word is %s. You missed %d times.\n", hangWord, wrongGuesses);
                fclose(endfp);
		for (int y = 0; y < MAXLEN; y++){
			guess[y] = '\0';
		}
		
		exit(0);
		}

	}
             
        
}

