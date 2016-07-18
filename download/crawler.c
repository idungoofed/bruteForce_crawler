#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include "crawler.h"

/**
Hardcoded for http://puu.sh/FILENAME, where FILENAME is 6 characters long.
In essence, a more optimized version of my old puush crawler.
**/

void *writeControl(void *notNeeded) {
  while(run) {
    if (turn_to_write >= MAXTHREADS) {
      turn_to_write = turn_to_write%MAXTHREADS;
    }
  }
  printf("Program exiting\n");
  pthread_exit(NULL);
}

void *check(void *indexParam) {
  //initialize the string to be used with the system() call
  int startIndex = (int)(*((int *)indexParam));
  char* sysStr = (char *)malloc(sizeof(char) * ((int)cmdLen + 11));
  strncpy(sysStr, command, cmdLen);
  sysStr[cmdLen+6] = '.';
  sysStr[cmdLen+7] = 'j';
  sysStr[cmdLen+8] = 'p';
  sysStr[cmdLen+9] = 'g';
  sysStr[cmdLen+10] = '\0';
  
  //create the filename, store into correct position in sysStr
  int a, b, c, d, e, f;
  while (run) {
    for (a = 0; a < 62; a+=MAXTHREADS) {
      sysStr[cmdLen] = charSet[a];
      for (b = 0; b < 62; b+=MAXTHREADS) {
	sysStr[cmdLen+1] = charSet[b];
	for (c = 0; c < 62; c+=MAXTHREADS) {
	  sysStr[cmdLen+2] = charSet[c];
	  for (d = 0; d < 62; d+=MAXTHREADS) {
	    sysStr[cmdLen+3] = charSet[d];
	    for (e = 0; e < 62; e+=MAXTHREADS) {
	      sysStr[cmdLen+4] = charSet[e];
	      for (f = startIndex; f < 62; f+=MAXTHREADS) {
		sysStr[cmdLen+5] = charSet[f];
		//system(sysStr)
		//check
		//wait for turn, then write
		//remove
		while (turn_to_write != startIndex) {}
#ifdef DEBUG
		printf("String %d, command = %s\n", startIndex, sysStr);
#endif
		turn_to_write++;
	      }
	    }
	  }
	}
      }
    }
  }
  free(sysStr);
  free(indexParam);
  if (startIndex == MAXTHREADS - 1) {exit(0);}
  else {pthread_exit(NULL);}
}


int main(int argc, char* argv[]) {

  //init globals
  outFile = fopen("results.txt", "w");
  //leave space in command for the URL to be copied into by the thread
#ifdef QUIET
  command = (char *)malloc(sizeof(char) * 23);
  strcpy(command, "wget -q http://puu.sh/\0");
  cmdLen = 22;
#endif
#ifdef VERBOSE
  command = (char *)malloc(sizeof(char) * 20);
  strcpy(command, "wget http://puu.sh/\0");
  cmdLen = 19;
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
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
  //pthread_t *pthread = malloc(sizeof(pthread_t));
  pthread_t pthread;
  
#ifdef DEBUG
  printf("Command string: \"%s\"\n", command);
  //printf("Size = %d\n", (int)strlen(command));
#endif

  //start thread that controls writing to outFile
  turn_to_write = 0;
  pthread_create(&pthread, &attr, writeControl, (void *)(NULL));

  //start download threads
  //precondition: len(charSet) > MAXTHREADS
  for (i = 0; i < MAXTHREADS; i++) {
    int *arg = (int *)malloc(sizeof(*arg));
    *arg = i;
    pthread_create(&pthread, &attr, check, (void *)arg);
  }

  //while stopFIle doesn't exist, wait
  while(access(stopFile, F_OK) == -1) {}
  run = 0;
  pthread_attr_destroy(&attr);
  //free(pthread);
  free(command);
  fclose(outFile);
  pthread_exit(NULL);
}
  
