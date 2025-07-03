# sysfetch - system information fetcher written in C

![Screenshot](screenshot.png)

This tool is a simple (and poorly written) system information fetcher.
It has basic functionality and is (little to not) configurable.
Feel free to use/contribute if you like.

## Requirements

- `gcc`
- `make`

## Build

```bash
make all
```

## Usage

```bash
sfetch
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
- Implement configuration file support for colors.
- Refactor print_colored() to allow customizable output formatting.
- Improve error handling by adding logging or debug mode.
- Add Windows/macOS support with platform-specific system calls.
- Implement multithreading to speed up data retrieval.
