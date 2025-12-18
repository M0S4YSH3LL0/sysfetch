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

typedef struct {
  char CONFIG_PREFIX[8];
  char CONFIG_SEPERATOR[8];
  char CONFIG_LABEL_OS[128];
  char CONFIG_LABEL_ARCH[128];
  char CONFIG_LABEL_CPU[128];
  char CONFIG_LABEL_PKGS[128];
  char CONFIG_LABEL_KERNEL[128];
  char CONFIG_LABEL_SHELL[128];
  char CONFIG_LABEL_LIBC[128];
  char CONFIG_LABEL_UPTIME[128];
} Config;

Config *parse_config() {
  char path[512];
  char line[512];
  snprintf(path, sizeof(path), "%s/.config/sysfetch/sysfetch.conf",
           getenv("HOME"));
  Config *cfg = calloc(1, sizeof(Config));
  FILE *file = fopen(path, "r");
  if (!file) {
    free(cfg);
    return NULL;
  }
  while (fgets(line, sizeof(line), file)) {
    char key[64];
    char value[128];

    if (sscanf(line, " %63[^=]=\"%127[^\"]\"", key, value) != 2)
      continue;

    if (strcmp(key, "LABEL_OS") == 0) {
      strncpy(cfg->CONFIG_LABEL_OS, value, sizeof(cfg->CONFIG_LABEL_OS) - 1);
    } else if (strcmp(key, "LABEL_ARCH") == 0) {
      strncpy(cfg->CONFIG_LABEL_ARCH, value,
              sizeof(cfg->CONFIG_LABEL_ARCH) - 1);
    } else if (strcmp(key, "LABEL_CPU") == 0) {
      strncpy(cfg->CONFIG_LABEL_CPU, value, sizeof(cfg->CONFIG_LABEL_CPU) - 1);
    } else if (strcmp(key, "LABEL_KERNEL") == 0) {
      strncpy(cfg->CONFIG_LABEL_KERNEL, value,
              sizeof(cfg->CONFIG_LABEL_KERNEL) - 1);
    } else if (strcmp(key, "LABEL_UPTIME") == 0) {
      strncpy(cfg->CONFIG_LABEL_UPTIME, value,
              sizeof(cfg->CONFIG_LABEL_UPTIME) - 1);
    } else if (strcmp(key, "LABEL_PKGS") == 0) {
      strncpy(cfg->CONFIG_LABEL_PKGS, value,
              sizeof(cfg->CONFIG_LABEL_PKGS) - 1);
    } else if (strcmp(key, "LABEL_LIBC") == 0) {
      strncpy(cfg->CONFIG_LABEL_LIBC, value,
              sizeof(cfg->CONFIG_LABEL_LIBC) - 1);
    } else if (strcmp(key, "LABEL_SHELL") == 0) {
      strncpy(cfg->CONFIG_LABEL_SHELL, value,
              sizeof(cfg->CONFIG_LABEL_SHELL) - 1);
    } else if (strcmp(key, "PREFIX") == 0) {
      strncpy(cfg->CONFIG_PREFIX, value, sizeof(cfg->CONFIG_PREFIX) - 1);
    } else if (strcmp(key, "SEPERATOR") == 0) {
      strncpy(cfg->CONFIG_SEPERATOR, value, sizeof(cfg->CONFIG_SEPERATOR) - 1);
    }
  }
  fclose(file);
  return cfg;
}

char *get_arch_info() {
  static struct utsname arch_info;
  static char buffer[BUFFER_SIZE];
  uname(&arch_info);
  snprintf(buffer, BUFFER_SIZE, "%s", arch_info.machine);
  return buffer;
}

char *get_cpu_info() {
  static char buffer[BUFFER_SIZE];

  FILE *fp = fopen("/proc/cpuinfo", "r");
  if (!fp) {
    perror("fopen");
    return buffer;
  }

  char line[1024];

  while (fgets(line, sizeof(line), fp)) {
    if (strncmp(line, "model name", 10) == 0 && buffer[0] == '\0') {
      char *p = strchr(line, ':');
      if (p) {
        while (*(++p) == ' ')
          ;
        strncpy(buffer, p, sizeof(buffer) - 1);
        buffer[strcspn(buffer, "\n")] = 0;
      }
    }
  }
  fclose(fp);
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

void print_colored(char *label, char *content, Config *conf) {
  printf("%s%s%s %s%s%s%s %s\n", YEL, conf->CONFIG_PREFIX, MAG, label, NRM,
         conf->CONFIG_SEPERATOR, NRM, content);
}

void print_header() {
  printf("\n%s-=[%s %ssysfetch%s %sv%s%s (%s@%s%s) %s]=-%s\n\n", BLU, NRM, CYN,
         NRM, YEL, VERSION, NRM, MAG, AUTHOR, NRM, BLU, NRM);
}

int main(int argc, char *argv[]) {
  Config *conf = parse_config();
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

  print_colored(conf->CONFIG_LABEL_OS, get_os_info(), conf);
  print_colored(conf->CONFIG_LABEL_ARCH, get_arch_info(), conf);
  print_colored(conf->CONFIG_LABEL_KERNEL, get_kernel_info(), conf);
  print_colored(conf->CONFIG_LABEL_SHELL, get_shell_info(), conf);
  print_colored(conf->CONFIG_LABEL_PKGS, get_installed_packages_info(), conf);
  print_colored(conf->CONFIG_LABEL_LIBC, get_libc_info(), conf);
  print_colored(conf->CONFIG_LABEL_CPU, get_cpu_info(), conf);
  print_colored(conf->CONFIG_LABEL_UPTIME, get_uptime(), conf);
  // print color palette
  printf("\n\e[30m \e[31m \e[32m \e[33m \e[34m \e[35m "
         "\e[36m \e[37m \e[0m\n");

  return EXIT_SUCCESS;
}
