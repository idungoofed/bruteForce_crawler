#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include "crawler.h"

/**
Hardcoded for http://puu.sh/FILENAME, where FILENAME is 6 characters long.
In essence, a more optimized version of my old puush crawler.
Currently having each thread write to a separate file, then combining them at the end.
**/

void *check(void *indexParam) {
  //initialize the string to be used with the system() call
  int startIndex = (int)(*((int *)indexParam));
  char* sysStr = (char *)malloc(sizeof(char) * ((int)cmdLen + 11)); //cmdLen + fnLen + extLen + 1
  strncpy(sysStr, command, cmdLen);
  sysStr[cmdLen+6] = '.'; 
  sysStr[cmdLen+7] = 'j'; 
  sysStr[cmdLen+8] = 'p'; 
  sysStr[cmdLen+9] = 'g'; 
  sysStr[cmdLen+10] = '\0';
  
  //open the results file for this thread
  //assumes MAXTHREADS < 100
  char *fname = (char *)malloc(sizeof(char) * 6);
  sprintf(fname, "tmp%d", startIndex);
  FILE *outF = fopen(fname, "w"); //overwrites file if it exists

  //result of system(sysStr)
  int sysRes;
  
  //create the filename, store into correct position in sysStr
  int a, b, c, d, e, f;
  for (a = 0; a < 62; a++) {
    sysStr[cmdLen] = charSet[a];
    for (b = 0; b < 62; b+=1) {
      sysStr[cmdLen+1] = charSet[b];
      for (c = 0; c < 62; c++) {
	sysStr[cmdLen+2] = charSet[c];
	for (d = 0; d < 62; d++) {
	  sysStr[cmdLen+3] = charSet[d];
	  for (e = 0; e < 62; e++) {
	    sysStr[cmdLen+4] = charSet[e];
	    for (f = startIndex; f < 62; f+=MAXTHREADS) {
	      if (!run) {
		/*
		  used to exit from all nested for-loops if threads are 
		  told to exit before testing all of their possibilities
		*/
		goto EXIT_EARLY;
	      }
	      sysStr[cmdLen+5] = charSet[f];
	      sysRes = system(sysStr);
	      //if there were no errors, save the link
	      if (sysRes == 0) {
		//compare?
		//yes -> download file, open comparison
		//need to implement ^
		fprintf(outF, "http://puu.sh/%.*s\n", 10, sysStr + cmdLen); //10 = fnLen + extLen
#ifdef DEBUG
		printf("Found file: %.6s\n", sysStr + cmdLen);
#endif
	      }
	    }
	  }
	}
      }
    }
  }
  //got here if the threads are told to exit (by run being set to 0)
 EXIT_EARLY:
#ifdef DEBUG  
  printf("String %d exiting\n", startIndex);
#endif
  //free resources
  numRunning--;
  fclose(outF);
  free(fname);
  free(sysStr);
  free(indexParam);
  pthread_exit(NULL);
}


int main(int argc, char* argv[]) {

  if (access(stopFile, F_OK) != -1) {
    printf("Stopfile exists. Remove it and rerun the program.\nExiting now.\n");
    exit(EXIT_FAILURE);
  }

  //init globals
  //leave space in command for the URL to be copied into by the thread
#ifdef QUIET
  command = (char *)malloc(sizeof(char) * 32);
  strcpy(command, "wget -q --spider http://puu.sh/\0");
  cmdLen = strlen(command);
#endif
#ifdef VERBOSE
  command = (char *)malloc(sizeof(char) * 29);
  strcpy(command, "wget --spider http://puu.sh/\0");
  cmdLen = strlen(command);
#endif

  
  //generate charSet
  int i;
  for (i = 48; i < 58; i++) {
    charSet[i-48] = i;
  }
  for (i = 65; i < 91; i++) {
    charSet[i-55] = i;
  }
  for (i = 97; i < 123; i++) {
    charSet[i-61] = i;
  }

  //initialize thread stuff
  pthread_attr_t attr;
  pthread_attr_init(&attr);
    
#ifdef DEBUG
  printf("Command string: \"%s\"\n", command);
  //printf("Size = %d\n", (int)strlen(command));
  printf("Size = %d\n", cmdLen);
#endif

  //create thread array for joining on later
  pthread_t *threadArr = (pthread_t *)malloc(sizeof(pthread_t) * MAXTHREADS);
  
  //start download threads
  //precondition: len(charSet) > MAXTHREADS
  numRunning = MAXTHREADS;
  for (i = 0; i < MAXTHREADS; i++) {
    int *arg = (int *)malloc(sizeof(*arg));
    *arg = i;
    pthread_create(&threadArr[i], &attr, check, (void *)arg);
  }

  //while stopFIle doesn't exist, wait. Recheck every 5 seconds
  //check if all threads are finished -> exit loop
  while(access(stopFile, F_OK) == -1) {
    if (!numRunning) {
      break;
    }
    sleep(5);
  }
  run = 0;
#ifdef DEBUG
  printf("run set\n");
#endif
  
  //wait for threads to finish
  for (i = 0; i < MAXTHREADS; i++) {
    pthread_join(threadArr[i], NULL);
  }

  //combine results from each thread
  system("cat tmp* > results.txt"); //this will overwrite anything in results.txt

  //free resources and remove stopfile and thread tmpfiles
  char *rmStop = (char *)malloc(sizeof(char) * (4 + strlen(stopFile)));
  sprintf(rmStop, "rm %s", stopFile);
  system(rmStop);
  free(rmStop);
  system("rm tmp*");
  free(threadArr);
  pthread_attr_destroy(&attr);
  free(command);
  printf("Program exiting\n");
  exit(EXIT_SUCCESS);
}
