# sysfetch - system information fetcher

**sysfetch** is a lightweight system information fetcher written in C.
It provides basic functionality and is (little to not) configurable.
Feel free to use/contribute if you like.

![Screenshot](screenshot.png)

## Requirements

- `gcc`
- `make`

## Build & Install

Build and install to your local prefix (`~/.local` by default, specify with `PREFIX=`):

```bash
make all && make install
```

## Usage

```bash
sysfetch -h
```

## Configuration

`$HOME/.config/sysfetch/sysfetch.conf` (after installation)

You can customize the following:

- label names
- prefix
- separator

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

## To-Do List / Planned improvements

- Add support for fetching GPU information.
- Add support for alternative package managers (e.g., pacman, rpm, etc.).
- Implement configuration file support for labels and colors. (WIP)
- Refactor print_colored() to allow customizable output formatting.
- Improve error handling by adding logging or debug mode.
- Add Windows/macOS support with platform-specific system calls.
