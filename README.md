# sysfetch - system information fetcher written in C

## Build

```bash
make all
```

This will:

1. first `clean` the output binary if it already exists
2. then `build` the binary
3. finally `link` it to `~/.local/bin/sfetch`

## Usage

Run with default configuration

```bash
sfetch
```

Run with custom configuration file (coming soon...)

```bash
sfetch -c /path/to/your/sysfetch.conf
```

## Features

- show OS information
- show kernel version
- show shell information
- show # of packages installed (currently only `dpkg`)
- show `libc` version (currently only `glibc`)
- show uptime information

## To do's

See comments in `sysfetch.c`
