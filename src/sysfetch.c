#include "sysfetch.h"
#include "config.h"
#include <features.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/utsname.h>
#include <unistd.h>

void get_arch_info(char *buffer, size_t size) {
  FILE *fp = popen("uname -m", "r");
  if (fp == NULL) {
    perror("popen");
    return;
  }
  char line[BUFFER_SIZE];
  if (fgets(line, sizeof(line), fp) != NULL) {
    size_t len = strlen(line);
    if (len > 0 && line[len - 1] == '\n') {
      line[len - 1] = '\0';
    }
    snprintf(buffer, size, "%s", line);
  }
}

void get_shell_info(char *buffer, size_t size) {
  char *shell = getenv("SHELL");

  if (!shell) {
    snprintf(buffer, size, "%s", "N/A");
    return;
  }

  snprintf(buffer, size, "%s", shell);
}

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
  buffer[strcspn(buffer, "\n")] = 0;

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
  printf("%s~%s %s%s%s: %s\n", YEL, NRM, MAG, label, NRM, content);
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
  char arch[BUFFER_SIZE];

  get_arch_info(arch, BUFFER_SIZE);
  get_hostname(hostname, BUFFER_SIZE);
  get_os_info(os, BUFFER_SIZE);
  get_kernel_info(kernel, BUFFER_SIZE);
  get_libc_info(libc, BUFFER_SIZE);
  get_uptime(uptime, BUFFER_SIZE);
  get_installed_packages_info(installed_pkgs, BUFFER_SIZE);
  get_shell_info(shell, BUFFER_SIZE);

  print_header();

  printf("%s%s%s@%s%s%s\n-----------------\n", BLU, "m0", NRM, GRN, hostname,
         NRM);
  print_colored(LABEL_OS, os);
  print_colored(LABEL_ARCH, arch);
  print_colored(LABEL_KERNEL, kernel);
  print_colored(LABEL_SHELL, shell);
  print_colored(LABEL_PKGS, installed_pkgs);
  print_colored(LABEL_LIBC, libc);
  print_colored(LABEL_UPTIME, uptime);

  return EXIT_SUCCESS;
}
