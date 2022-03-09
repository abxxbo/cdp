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
  
# Compiling
You may compile CDP using
```sh
$ make
```

To install CDP, run
```sh
$ sudo make install
```
This will install cdp to `/usr/local/bin/cdp`
