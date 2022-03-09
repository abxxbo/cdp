#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#define EXIT_FAIL 2
#define EXIT_PASS 0


void show_help(){
  fprintf(stderr, "cdp help\n");
  fprintf(stderr, "--input-file [file]      => file cdp will copy from\n");
  fprintf(stderr, "--output-file [file]     => file cdp will copy to. If no file specified, it will output contents to stdout.\n");
  fprintf(stderr, "--force-partition        => Only use when writing to a partition or drive. Prevents accidental data loss.\n");
  exit(EXIT_FAIL);
}


int main(int argc, char** argv){
  // Parse arguments
  if(argc >= 2){
    if(strcmp(argv[1], "--help") == 0) show_help();
  }

  // quit
  return EXIT_PASS;
}
