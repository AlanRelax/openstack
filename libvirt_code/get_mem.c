#include "mylib.h"

int main(int argc, char **argv) {
  char *hostname;

  hostname = *(++argv);

  printf("%s\n", hostname);
}
