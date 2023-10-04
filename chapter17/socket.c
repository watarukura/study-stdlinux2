//
// Created by 倉嶋渉 on 2023/10/04.
//
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#define MAX_BACKLOG 5
#define DEFAULT_PORT "80"

static int listen_socket(char *port) {
  struct addrinfo hints, *res, *ai;
  int err;

  memset(&hints, 0, sizeof(struct addrinfo));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flangs = AI_PASSIVE;
  if ((err = getaddrinfo(NULL, port, &hints, &res) != 0)) {
    log_exit(gai_strerror(err));
  }
  for (ai = res; ai; ai = ai->ai_next) {
    int sock;

    sock = socket(ai->ai_family, ai->ai_socktype, ai->ai_protocol);
    if (sock < 0) continue;
    if (bind(sock, ai->ai_addr, ai->ai_addrlen) < 0) {
      close(sock);
      continue;
    }
    if (listen(sock, MAX_BACKLOG) < 0) {
      close(sock);
      continue;
    }
    freeaddrinfo(res);
    return sock;
  }
  log_exit("failed to listen socket");
  return -1; /* NOT REACH */
}

static void server_main(int server_fd, char *docroot) {
  for (;;) {
    struct sockaddr_storage addr;
    socklen_t addrlen = sizeof addr;
    int sock;
    int pid;

    sock = accept(server_fd, (struct sockaddr*)&addr, &addrlen);
    if (sock < 0) {
      log_exit("accept(2) failed: %s", strerror(errno));
    }
    pid = fork();
    if (pid < 0) exit(3);
    if (pid == 0) {

      FILE *inf = fdopen(sock, "r");
      FILE *outf = fdopen(sock, "w");

      service(inf, outf, docroot);
      exit(0);
    }
    close(sock);
  }
}