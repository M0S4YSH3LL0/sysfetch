// New features, improvements
// TODO: Add support for fetching GPU and CPU information.
// TODO: Add support for alternative package managers (e.g., pacman, rpm, etc.).
// TODO: Add Windows/macOS support with platform-specific system calls.
// TODO: Implement configuration file support for user-defined labels and
// colors.
// TODO: Refactor print_colored() to allow customizable output formatting.
// TODO: Optimize file reading by using statically allocated buffers where
// possible.
// TODO: Improve error handling by adding logging or debug mode.
// TODO: Implement multithreading to speed up data retrieval.
// TODO: Add unit tests for each system info retrieval function.

#include <features.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/utsname.h>
#include <unistd.h>

#define VERSION "0.36"
#define AUTHOR "m0s4ysh3ll0"

// might add custom config support, like: sysfetch -c custom_config.conf
#define CONFIG_PATH "~/.config/sysfetch/config.conf"
#define BUFFER_SIZE 256

// color codes
#define NRM "\x1B[0m"
#define RED "\x1B[31m"
#define GRN "\x1B[32m"
#define YEL "\x1B[33m"
#define BLU "\x1B[34m"
#define MAG "\x1B[35m"
#define CYN "\x1B[36m"
#define WHT "\x1B[37m"

// Default labels
char hostname_label[BUFFER_SIZE] = "host";
char kernel_label[BUFFER_SIZE] = "kernel";
char uptime_label[BUFFER_SIZE] = "uptime";
char os_label[BUFFER_SIZE] = "os";
char shell_label[BUFFER_SIZE] = "shell";
char libc_label[BUFFER_SIZE] = "libc";
char pkgs_label[BUFFER_SIZE] = "packages";

void get_cpu_name(char *buffer, size_t s) { printf("hello world"); }

void get_shell_info(char *buffer, size_t size) {
  char *shell = getenv("SHELL");

  if (!shell) {
    snprintf(buffer, size, "%s", "N/A");
    return;
  }

  snprintf(buffer, size, "%s", shell);
}

// NOTE: only for dpkg packages for now
void get_installed_packages_info(char *buffer, size_t size) {
  FILE *fp = popen("dpkg --get-selections | grep -c 'install'", "r");
  if (fp == NULL) {
    perror("popen");
    return;
  }

  int count;
  char line[BUFFER_SIZE];
  if (fgets(line, sizeof(line), fp) != NULL) {
    if (sscanf(line, "%d", &count) != 1) {
      count = 0;
    }
  }
  pclose(fp);
  snprintf(buffer, size, "%d (dpkg)", count);
}

// currently only for glibc
void get_libc_info(char *buffer, size_t size) {
#ifdef __GLIBC__
  snprintf(buffer, size, "glibc %i.%i", __GLIBC__, __GLIBC_MINOR__);
#elif
  snprintf(buffer, size, "%s", "N/A");
#endif
  return;
}

void get_hostname(char *buffer, size_t size) {
  char *filename = "/etc/hostname";
  FILE *file = fopen(filename, "r");
  if (!file) {
    snprintf(buffer, size, "N/A");
    return;
  }

  if (fgets(buffer, size, file) == NULL) {
    snprintf(buffer, size, "%s", "N/A");
  }

  fclose(file);
  return;
}

void get_os_info(char *buffer, size_t size) {
  char *filename = "/etc/os-release";
  FILE *file = fopen(filename, "r");
  if (!file) {
    snprintf(buffer, size, "N/A");
    return;
  }

  char line[1024];
  while (fgets(line, sizeof(line), file)) {
    if (strncmp(line, "PRETTY_NAME=", 12) == 0) {
      // hacky stuff to remove first and last character and trailing newline
      char *p = line + 12;
      p++;
      p[strlen(p) - 2] = 0;
      snprintf(buffer, size, "%s", p);
      buffer[strcspn(buffer, "\n")] = 0;
      break;
    }
  }

  fclose(file);
}

void get_kernel_info(char *buffer, size_t size) {
  struct utsname buf;
  uname(&buf);
  snprintf(buffer, size, "%s", buf.release);
}

void get_uptime(char *buffer, size_t size) {
  FILE *file = fopen("/proc/uptime", "r");
  if (!file) {
    snprintf(buffer, size, "N/A");
    return;
  }

  char line[BUFFER_SIZE];
  float uptime_seconds = 0.0;
  if (fgets(line, sizeof(line), file) != NULL) {
    if (sscanf(line, "%f", &uptime_seconds) == 1) {
      int hours = (int)(uptime_seconds / 3600);
      int minutes = (int)((uptime_seconds - (hours * 3600)) / 60);
      snprintf(buffer, size, "%d hours, %d minutes", hours, minutes);
    } else {
      snprintf(buffer, size, "%s", "N/A");
    }
  } else {
    snprintf(buffer, size, "%s", "N/A");
  }

  fclose(file);
}

void print_colored(char *label, char *content) {
  printf("%s~%s %s%s%s: %s\n", YEL, NRM, GRN, label, NRM, content);
}

void print_header() {
  printf("\n%s-=[%s %ssysfetch%s %sv%s%s (%s@%s%s) %s]=-%s\n\n", BLU, NRM, CYN,
         NRM, YEL, VERSION, NRM, MAG, AUTHOR, NRM, BLU, NRM);
}

int main(int argc, char *argv[]) {
  char uptime[BUFFER_SIZE];
  char kernel[BUFFER_SIZE];
  char os[BUFFER_SIZE];
  char hostname[BUFFER_SIZE];
  char libc[BUFFER_SIZE];
  char installed_pkgs[BUFFER_SIZE];
  char shell[BUFFER_SIZE];

  get_hostname(hostname, BUFFER_SIZE);
  get_os_info(os, BUFFER_SIZE);
  get_kernel_info(kernel, BUFFER_SIZE);
  get_libc_info(libc, BUFFER_SIZE);
  get_uptime(uptime, BUFFER_SIZE);
  get_installed_packages_info(installed_pkgs, BUFFER_SIZE);
  get_shell_info(shell, BUFFER_SIZE);

  print_header();

  print_colored(os_label, os);
  print_colored(kernel_label, kernel);
  print_colored(shell_label, shell);
  print_colored(pkgs_label, installed_pkgs);
  print_colored(libc_label, libc);
  print_colored(uptime_label, uptime);

  return EXIT_SUCCESS;
}
