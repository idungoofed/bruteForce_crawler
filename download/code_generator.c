#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
  if (argc != 4) {
    printf("Usage: %s base_url numCharacters numThreads\n", argv[0]);
    return 1;
  }

  
  //generate character array
  char testChars[64];
  int i;
  for (i = 48; i < 58; i++) {
    testChars[i-48] = i;
  }
  for (i = 65; i < 91; i++) {
    testChars[i-55] = i;
  }
  for (i = 97; i < 123; i++) {
    testChars[i-61] = i;
  }  
  testChars[63] = '\0';

  
