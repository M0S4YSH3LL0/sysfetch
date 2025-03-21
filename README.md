# sysfetch - system information fetcher written in C

## Requirements

- `gcc` to compile

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
- show architecture information

## To do's

- Add support for fetching GPU and CPU information.
- Add support for alternative package managers (e.g., pacman, rpm, etc.).
- Add Windows/macOS support with platform-specific system calls.
- Implement configuration file support for user-defined labels and colors.
- Refactor print_colored() to allow customizable output formatting.
- Optimize file reading by using statically allocated buffers where possible.
- Improve error handling by adding logging or debug mode.
- Implement multithreading to speed up data retrieval.
- Add unit tests for each system info retrieval function.
