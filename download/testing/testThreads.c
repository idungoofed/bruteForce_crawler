#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

FILE* f;


void *thingy(void *fn) {
  fn = (FILE *)fn;
  char *str = "abcdefghijklmnopqrstuvwxyz\n";
  fwrite((void *)str, sizeof(char), 27, fn);
  return NULL;
}

int main() {
  f = fopen("tempOut.txt", "w");
  pthread_t *pthread = malloc(sizeof(pthread_t));
  pthread_attr_t attr;
  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

  for(int i = 0; i < 10; i++) {
    pthread_create(pthread, &attr, thingy, (void *)(f));
  }

  fclose(f);
  free(pthread);
}
