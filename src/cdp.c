#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <blkid/blkid.h>

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
  if(strcmp(out_file, "stdout") == 0){
    FILE* fp = fopen(in_file, "rb");    // rb   => read binary
    char* line = NULL;
    size_t len = 0;
    ssize_t read;

    if(!fp) exit(EXIT_FAIL);           // can't read file.
    while((read = getline(&line, &len, fp)) != -1){
      printf("%s", line);
    }
    exit(EXIT_PASS);    // Success, file read.
  } else {
    // Using else, just to make sure.
    FILE* fp_ = fopen(in_file, "rb");
    FILE* fp2 = fopen(out_file, "a");
    
    // Check if file is unreadable/unwriteable before we start.
    if(!fp_) {
      perror("cdp");
      exit(EXIT_FAIL);
    }

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

// returns 0 if sudo is given
// returns non-0 if no sudo
int check_for_sudo() {
  uid_t uid = getuid(), euid = geteuid();
  if(uid < 0 || uid != euid){
    printf("[ ERR ] You are not using sudo, you will certaintly run into trouble.\n");
    return 1;
  }
  // SUDO is given to us.
  return 0;
}

// write to partition
void write_file_to_partition(char* in, char* partition) {

}

int main(int argc, char* argv[]){
  // variables
  char* input_file;     // input file -- REQUIRED.
  char* output_file;    // output file -- if left blank assume /dev/stdout
  int with_sudo;        // is sudo passed?
  // Parse arguments
  if(argc >= 2){
    // help
    if(strcmp(argv[1], "--help") == 0) show_help();
    

    if(strcmp(argv[1], "--input-file") == 0){
      // copy to input_file
      input_file = argv[2];

      // is output file detected?
      if(argc >= 5) {
        // output-file
        if(strcmp(argv[3], "--output_file") != 0) output_file = "stdout";
        output_file = argv[4];

        // force sudo if using --force-partition
        if(strcmp(argv[5], "--force-partition") == 0){
          int with_sudo = check_for_sudo();
          if(with_sudo != 0) exit(EXIT_FAIL);
          
          // continue.
          write_file_to_partition(input_file, output_file);   // yes this requires different logic
        }
      } else {
        output_file = "stdout";
      }
    }
  }

  // do thing
  write_file(input_file, output_file);

  // quit
  return EXIT_PASS;
}
