# cdp
cdp is a utility that copies files from one file to another. Its intention is to be as quick as possible, and faster (more reliable) than `dd`.

# Features
- cdp is "fool proof" in the sense that you cannot overwrite a partition with nonsensical data.

# Flags
- Input file (file to be copied from) can be defined with `--input-file`
- Output file (file to copy to) can be defined with `--output-file`
  - If `--output-file` is not specified, the file defined with ``-input-file` will be written to stdout.
- If you are writing to a partition/drive, you **MUST** specify `--force-partition`. This is to avoid accidental data loss.
  - **NOTE:** When `cdp` is done writing data to a partition/drive, it will automatically execute `sync(1)` for you.

## Example
This is an example usage of all features presented in cdp.

### Partition Blocking
```sh
# /dev/sda is a system partition -- we are running our machine on it.
$ cdp --input-file example.iso --output-file /dev/sda
[ INFO ] You are not using --force-partition, this will certainly lead to trouble.
[ INFO ] /dev/sda is a system partition. I am not writing data to this partition.
```

### Writing to stdout
```sh
$ cat << EOF > foo
Example file to be printed to stdout
EOF
$ cdp --input-file foo
Example file to be printed to stdout.
```

### Writing to a file
```sh
$ cat << EOF > foo2
This file will be copied to foo3
EOF
$ cdp --input-file foo2 --output-file foo3
$ cat foo3
This will be copied to foo3
```

# Compiling
You may compile CDP using this command. It is recommended that you do not compile cdp, rather, use one of the releases.
```sh
$ make
```

To install CDP, run
```sh
$ sudo make install
```
This will install cdp to `/usr/local/bin/cdp`
