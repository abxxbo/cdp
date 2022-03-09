#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#define EXIT_FAIL 2
#define EXIT_PASS 0

// Shows help command to stderr then exits with code 2
void show_help(){
  fprintf(stderr, "cdp help\n");
  fprintf(stderr, "--input-file [file]      => file cdp will copy from\n");
  fprintf(stderr, "--output-file [file]     => file cdp will copy to. If no file specified, it will output contents to stdout.\n");
  fprintf(stderr, "--force-partition        => Only use when writing to a partition or drive. Prevents accidental data loss.\n");
  exit(EXIT_FAIL);
}

// Copy files contents to file 2. If file 2 is stdout,
// just print to screen (technically copying to stdout?)
void write_file(char* in_file, char* out_file){
  // Check if out_file is stdout
  // if so, write to stdout.
  if(out_file == "stdout"){
    FILE* fp = fopen(in_file, "rb");    // rb   => read binary
    char* line = NULL;
    size_t len = 0;
    ssize_t read;

    if(!fp) exit(EXIT_FAIL);           // can't read file.
    while((read = getline(&line, &len, fp)) != -1){
      printf("%s\n", line);
    }
    exit(EXIT_PASS);    // Success, file read.
  } else {
    // Using else, just to make sure.
    FILE* fp_ = fopen(in_file, "rb");
    FILE* fp2 = fopen(out_file, "w");
    
    // Check if file is unreadable/unwriteable before we start.
    if(!fp || !fp2) exit(EXIT_FAIL);

    // start.
    char* line = NULL;
    size_t len = 0;
    ssize_t read;
    while((read = getline(&line, &len, fp_)) != -1){
      // write to fp2
      fprintf(fp2, "%s", line);
    }
    exit(EXIT_PASS);    // Success, file written
  }
}


int main(int argc, char** argv){
  // variables
  char* input_file, output_file;
  // Parse arguments
  if(argc >= 2){
    if(strcmp(argv[1], "--help") == 0) show_help();
    if(strcmp(argv[1], "--input-file") == 0){
      // No --force-partition
      input_file = argv[2];
      if(strcmp(argv[3], "--output-file") == 0){
        output_file = argv[4];
        write_file(input_file, output_file);
      }

      // No --output-file
      if(output_file == "\0" || output_file == NULL){
        // --output_file was not passed
        write_file(input_file, "stdout");
      }
    }
  }

  // quit
  return EXIT_PASS;
}
