#include "sysfetch.h"
#include <features.h>
#include <libgen.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/utsname.h>
#include <unistd.h>

char *get_arch_info() {
  FILE *fp = popen("uname -m", "r");
  static char buffer[BUFFER_SIZE];
  if (fp == NULL) {
    perror("popen");
    return buffer;
  }
  char line[BUFFER_SIZE];
  if (fgets(line, sizeof(line), fp) != NULL) {
    size_t len = strlen(line);
    if (len > 0 && line[len - 1] == '\n') {
      line[len - 1] = '\0';
    }
    snprintf(buffer, BUFFER_SIZE, "%s", line);
  }
  pclose(fp);
  return buffer;
}

char *get_shell_info() {
  char *shell = getenv("SHELL");
  static char buffer[BUFFER_SIZE];
  if (!shell) {
    snprintf(buffer, BUFFER_SIZE, "%s", "N/A");
    return buffer;
  }

  snprintf(buffer, BUFFER_SIZE, "%s", basename(shell));
  return buffer;
}

char *get_installed_packages_info() {
  FILE *fp = popen("dpkg --get-selections | grep -c 'install'", "r");
  static char buffer[BUFFER_SIZE];
  if (fp == NULL) {
    perror("popen");
    return buffer;
  }

  int count;
  char line[BUFFER_SIZE];
  if (fgets(line, sizeof(line), fp) != NULL) {
    if (sscanf(line, "%d", &count) != 1) {
      count = 0;
    }
  }
  pclose(fp);
  snprintf(buffer, BUFFER_SIZE, "%d (dpkg)", count);
  return buffer;
}

char *get_libc_info() {
  static char buffer[BUFFER_SIZE];
#ifdef __GLIBC__
  snprintf(buffer, BUFFER_SIZE, "glibc %i.%i", __GLIBC__, __GLIBC_MINOR__);
#endif
  return buffer;
}

char *get_hostname() {
  char *filename = "/etc/hostname";
  FILE *file = fopen(filename, "r");
  static char buffer[BUFFER_SIZE];
  if (!file) {
    snprintf(buffer, BUFFER_SIZE, "%s", "N/A");
    return buffer;
  }

  if (fgets(buffer, BUFFER_SIZE, file) == NULL) {
    snprintf(buffer, BUFFER_SIZE, "%s", "N/A");
  }
  buffer[strcspn(buffer, "\n")] = 0;

  fclose(file);
  return buffer;
}

char *get_os_info() {
  char *filename = "/etc/os-release";
  static char buffer[BUFFER_SIZE];
  FILE *file = fopen(filename, "r");
  if (!file) {
    snprintf(buffer, BUFFER_SIZE, "N/A");
    return buffer;
  }

  char line[1024];
  while (fgets(line, sizeof(line), file)) {
    if (strncmp(line, "PRETTY_NAME=", 12) == 0) {
      char *p = line + 12;
      p++;
      p[strlen(p) - 2] = 0;
      snprintf(buffer, BUFFER_SIZE, "%s", p);
      buffer[strcspn(buffer, "\n")] = 0;
      break;
    }
  }

  fclose(file);
  return buffer;
}

char *get_kernel_info() {
  struct utsname buf_uname;
  static char buf_str[BUFFER_SIZE];
  uname(&buf_uname);
  snprintf(buf_str, BUFFER_SIZE, "%s", buf_uname.release);
  return buf_str;
}

char *get_uptime() {
  FILE *file = fopen("/proc/uptime", "r");
  static char buffer[BUFFER_SIZE];
  if (!file) {
    snprintf(buffer, BUFFER_SIZE, "N/A");
    return buffer;
  }

  char line[BUFFER_SIZE];
  float uptime_seconds = 0.0;
  if (fgets(line, sizeof(line), file) != NULL) {
    if (sscanf(line, "%f", &uptime_seconds) == 1) {
      int hours = (int)(uptime_seconds / 3600);
      int minutes = (int)((uptime_seconds - (hours * 3600)) / 60);
      snprintf(buffer, BUFFER_SIZE, "%d hours, %d minutes", hours, minutes);
    } else {
      snprintf(buffer, BUFFER_SIZE, "%s", "N/A");
    }
  } else {
    snprintf(buffer, BUFFER_SIZE, "%s", "N/A");
  }

  fclose(file);
  return buffer;
}

void print_colored(char *label, char *content) {
  printf("%s~%s %s%s%s: %s\n", YEL, NRM, MAG, label, NRM, content);
}

void print_header() {
  printf("\n%s-=[%s %ssysfetch%s %sv%s%s (%s@%s%s) %s]=-%s\n\n", BLU, NRM, CYN,
         NRM, YEL, VERSION, NRM, MAG, AUTHOR, NRM, BLU, NRM);
}

int main(int argc, char *argv[]) {
  print_header();

  printf("%s%s%s@%s%s%s\n-----------------\n", BLU, "m0", NRM, GRN,
         get_hostname(), NRM);

  print_colored(LABEL_OS, get_os_info());
  print_colored(LABEL_ARCH, get_arch_info());
  print_colored(LABEL_KERNEL, get_kernel_info());
  print_colored(LABEL_SHELL, get_shell_info());
  print_colored(LABEL_PKGS, get_installed_packages_info());
  print_colored(LABEL_LIBC, get_libc_info());
  print_colored(LABEL_UPTIME, get_uptime());

  // print color palette
  printf("\n\e[30m \e[31m \e[32m \e[33m \e[34m \e[35m "
         "\e[36m \e[37m \e[0m\n");

  return EXIT_SUCCESS;
}
