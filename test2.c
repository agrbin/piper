#include <stdio.h>

int main(int argc, char **argv) {
  int it;
  fprintf(stderr, "%s: I have %d args. Quotes for clarity.\n",
          argv[0], argc - 1);
  for (it = 1; it < argc; ++it)
    fprintf(stderr, "%s:   '%s'\n", argv[0], argv[it]);
  return 0;
}
