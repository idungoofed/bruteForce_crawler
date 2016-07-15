#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "crawler.h"

/**
Hardcoded for http://puu.sh/FILENAME, where FILENAME is 6 characters long.
In essence, a more optimized version of my old puush crawler.
**/
void *check(void *fileName) {
  fileName = (char *)fileName;
  char* sysStr = (char *)malloc(sizeof(char) * ((int)strlen(command) + 11));
  strncpy(sysStr, command, strlen(command) + 1);
  strncat(sysStr, fileName, 6);
  strncat(sysStr, ".jpg", 4);
  printf("Command string: %s\n", sysStr);
  counter--;
  //free(fileName);
  free(sysStr);
  return NULL;
}

int main(int argc, char* argv[]) {

  //init globals
  outFile = fopen("results.txt", "w");
  //leave space in command for the URL to be copied into by the thread
#ifdef QUIET
  command = (char *)malloc(sizeof(char) * 22);
  strncpy(command, "wget -q http://puu.sh/", 22);
#endif
#ifdef VERBOSE
  command = (char *)malloc(sizeof(char) * 19);
  strncpy(command, "wget http://puu.sh/", 19);
#endif
  
  
  //generate charSet
  char charSet[62];
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
  pthread_t *pthread = malloc(sizeof(pthread_t));
  pthread_attr_t attr;
  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
  
  
#ifdef DEBUG
  printf("Command string: \"%s\"\n", command);
  printf("Size = %d\n", (int)strlen(command));
    
#endif

  char *fn = (char *)malloc(sizeof(char) * 6);
  int a, b, c, d, e, f;
  for (a = 0; a < 62; a++) {
    fn[0] = charSet[a];
    for (b = 0; b < 62; b++) {
      fn[1] = charSet[b];
      for (c = 0; c < 62; c++) {
	fn[2] = charSet[c];
	for (d = 0; d < 62; d++) {
	  fn[3] = charSet[d];
	  for (e = 0; e < 62; e++) {
	    fn[4] = charSet[e];
	    for (f = 0; f < 62; f++) {
	      fn[5] = charSet[f];
#ifdef DEBUG
	      if (counter >= 8) printf("Waiting. Currently running: %d\n", counter);
#endif
	      while (counter >= 8) {} //blocking so at most 8 threads run at once
#ifdef DEBUG
	      printf("Resuming. Currently running: %d\n", counter+1);
#endif
	      counter++;
	      //char *tmpFN = (char *)malloc(sizeof(char) * 6);
	      //strncpy(tmpFN, fn, 6);
	      printf("Thread starting with string: %s\n", fn);
	      pthread_create(pthread, &attr, check, (void *)fn);
	    }
	  }
	}
      }
    }
  }		      
  free(pthread);
  free(command);
  fclose(outFile);
  free(fn);
  pthread_exit(NULL);
}
