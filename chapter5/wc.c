#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

static int do_wc(const char *path);
static void die(const char *s);

int main(int argc, char *argv[]) {
  int i;
  int c;
  if (argc > 1) {
    for (i = 1; i < argc; i++) {
      c = do_wc(argv[i]);
    }
  } else {
    exit(1);
  }
  printf("%d\n", c);
  exit(0);
}

#define BUFFER_SIZE 2048

static int do_wc(const char *path) {
  int fd;
  unsigned char buf[BUFFER_SIZE];
  unsigned char lf = '\n';
  unsigned char nul = '\0';
  int n;
  int i;
  int k = 0;

  fd = open(path, O_RDONLY);
  if (fd < 0) die(path);
  for (;;) {
    n = read(fd, buf, sizeof buf);
    if (n < 0) die(path);
    if (n == 0) break;
    for (i = 0; i < sizeof buf; i++) {
      if (buf[i] == nul) {
        break;
      }
      if (buf[i] == lf) {
        k++;
      }
    }
  }
  if (close(fd) < 0) die(path);
  return k;
}

static void die(const char *s) {
  perror(s);
  exit(1);
}
