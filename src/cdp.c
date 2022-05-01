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

// get all partitions, check if partition passed is in system
// 1 -- Yes, it is.
// 0 -- No, it is not.
int get_partis_check(char* drive, char* partition){
  // Try opening file
  blkid_probe pr = blkid_new_probe_from_filename(drive);
  if(!pr) exit(EXIT_FAIL);

  // Get number of partitions
  blkid_partlist lp;
  int nparts, i;
  int sys_part = 0;

  lp = blkid_probe_get_partitions(pr);
  nparts = blkid_partlist_numof_partitions(lp);
  printf("[ INFO ] There are %d paritions on the system.\n", nparts);

  char* dev_name;
  for(i = 0; i < nparts; i++){
    pr = blkid_new_probe_from_filename(dev_name);
    blkid_do_probe(pr);

    if(strcmp(dev_name, partition) == 0){
      printf("[ ERR ] Partition %s (Partition #%d) is a system partition!\n", dev_name, i);
      sys_part = 1;
    }
  }
  return sys_part;
}
// write to partition
// in => file
// drive => /dev/X (e.g. /dev/sda)
// partition => /dev/XXXN (e.g. /dev/sda2)
void write_file_to_partition(char* in, char* drive, char* partition) {
  // check if partition is a system partition
  if(get_partis_check(drive, partition) == 1) exit(EXIT_FAIL); // system partition
  // now that it's taken care of, you can write to partition
  write_file(in, partition); // this is what dd does...
}

int main(int argc, char* argv[]){
  // variables
  char* input_file = 0;     // input file -- REQUIRED.
  char* output_file = 0;    // output file -- if left blank assume /dev/stdout
  int with_sudo;        // is sudo passed?
  // Parse arguments
  if(argc >= 2){
    // help
    if(strcmp(argv[1], "--help") == 0) show_help();


    if(strcmp(argv[1], "--input-file") == 0){
      // copy to input_file
      input_file = argv[2];

      // is output file detected?
      // output-file
      if(strcmp(argv[3], "--output-file") != 0) output_file = "stdout";

      // write to file
      if(argc > 6 && argc < 7){
        if(strcmp(argv[5], "--force-partition") == 0){
          with_sudo = check_for_sudo();
          if(with_sudo != 0) exit(EXIT_FAIL);

          // parti => argv[6]
          // drive => argv[8]
          write_file_to_partition(input_file, argv[8], argv[6]); // yes this requires different logic.
        }
      }
      if(*output_file == '\0') strcpy(output_file, "stdout");
    }
  }

  // do thing
  if(argc < 4) write_file(input_file, "stdout");
  else write_file(input_file, argv[4]);

  // quit
  return EXIT_PASS;
}
