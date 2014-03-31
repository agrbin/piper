#include <stdio.h>

int main(int argc, char **argv) {
  char tmp[100];
  printf("%s.\n", argv[0]);
  fflush(stdout);
  fprintf(stderr, "%s: ispisujem svoje ime\n", argv[0]);
  if (scanf("%100s", tmp) == 1) {
    fprintf(stderr, "%s: primio sam %s\n", argv[0], tmp);
  }
  return 0;
}
