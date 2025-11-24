#include "sysfetch.h"
#include <features.h>
#include <libgen.h>
#include <linux/sysinfo.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/sysinfo.h>
#include <sys/types.h>
#include <sys/utsname.h>
#include <unistd.h>

char *get_arch_info() {
  static struct utsname arch_info;
  static char buffer[BUFFER_SIZE];
  uname(&arch_info);
  snprintf(buffer, BUFFER_SIZE, "%s", arch_info.machine);
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
  FILE *fp = fopen("/var/lib/dpkg/status", "r");
  static char buffer[BUFFER_SIZE];
  if (fp == NULL) {
    perror("popen");
    return buffer;
  }

  int count = 0;
  char line[BUFFER_SIZE];
  while (fgets(line, sizeof(line), fp)) {
    if (strncmp(line, "Status: install ok installed", 27) == 0) {
      count++;
    }
  }

  fclose(fp);
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
  static char buffer[BUFFER_SIZE];

  if (gethostname(buffer, BUFFER_SIZE) == -1) {
    perror("gethostname");
    exit(1);
  }
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
  static struct sysinfo info;
  static char buffer[BUFFER_SIZE];
  if (sysinfo(&info) == -1) {
    perror("sysinfo");
    exit(1);
  }
  int uptime_seconds = info.uptime;
  int hours = (int)(uptime_seconds / 3600);
  int minutes = (int)((uptime_seconds - (hours * 3600)) / 60);
  snprintf(buffer, BUFFER_SIZE, "%d hours, %d minutes", hours, minutes);
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
  bool show_header = 0;
  for (int i = 1; i < argc; ++i) {
    if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
      printf("-h Shows this help message.\n-s Print header\n");
      exit(0);
    } else if (strcmp(argv[i], "-v") == 0 ||
               strcmp(argv[i], "--version") == 0) {
      printf("sysfetch %s (Build: %s %s)\n", VERSION, __DATE__, __TIME__);
      exit(0);
    } else if (strcmp(argv[i], "-s") == 0 || strcmp(argv[i], "--header") == 0) {
      show_header = 1;
    } else {
      printf("Unknown option: %s\n", argv[i]);
    }
  }

  if (show_header) {
    print_header();
  } else {
    printf("\n");
  }

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
