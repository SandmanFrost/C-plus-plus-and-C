#include "cat.h"

int main(int argc, char **argv) {
  if (argc > 1)
    cat(argc, argv);
  else
    printf("cat: flags empty and file...\n");
  return 0;
}