# sysfetch - system information fetcher written in C

![Screenshot](screenshot.png)

This tool is a simple (and poorly written) system information fetcher.
It has basic functionality and is (little to not) configurable.
Feel free to use/contribute if you like.

## Requirements

- `gcc`
- `make`

## Build and install

```bash
make all && make install
```

## Usage

```bash
sysfetch -h
```

## Features

`sysfetch` shows a variety of system information:

- OS name and version
- CPU model
- Architecture information
- Kernel version
- Shell information
- Number of packages installed (currently only `dpkg`)
- `libc` version (currently only `glibc`)
- System uptime

## To do's

- Add support for fetching GPU information.
- Add support for alternative package managers (e.g., pacman, rpm, etc.).
- Implement configuration file support for colors.
- Refactor print_colored() to allow customizable output formatting.
- Improve error handling by adding logging or debug mode.
- Add Windows/macOS support with platform-specific system calls.
