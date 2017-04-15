#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <linux/if.h>
#include <linux/if_tun.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <sys/time.h>
#include <errno.h>
#include <stdarg.h>

#include "tun.h"
#define MTU 1500
#define STDOUT 1
#define LINE_COUNT 16

void print_hex(int fd_out, const char *buf, int n) {
  int line = 0;
  int col = 0;
  char tmp[100];
  while (line * LINE_COUNT + col < n) {

    int cnt = sprintf(tmp, "%02x ", (unsigned char)buf[line * LINE_COUNT + col]);
    if (write(fd_out, tmp, (unsigned int)cnt) < 0) {
      perror("write(fd_out)");
      exit(1);
    }

    if (col == LINE_COUNT - 1) {
      line++;
      col = 0;
      printf("\n");
    }
    else {
      col++;
    }
  }
  if (col != 0) {
    printf("\n");
  }
}

int max_fd(int *fds, int count) {
  int max = 0;
  for (int i = 0; i < count; ++i) {
    if (fds[i] > max) {
      max = fds[i];
    }
  }
  return max;
}

int set_fds(int *fds, int count, fd_set *rd_set) {
  for (int i = 0; i < count; ++i) {
    FD_SET(fds[i], rd_set);
  }
}

#define IDX_STDIN 0
#define IDX_TUN_DEV 1
#define IDX_FIRST_IF 2

int main() {
  char name[16] = "tun5";
  int fd = tun_alloc(name, IFF_TUN | IFF_NO_PI);
  if (fd < 0)
    exit(1);

  while (1) {
    char buf[MTU];
    int n = (int) read(fd, buf, MTU);
    if (n < 0)
      break;

    print_hex(STDOUT, buf, n);
  }

  int fds[10];
  int fd_count = 2;
  int net_if_count = 0;
  fds[0] = 0;
  fds[1] = fd;
  int maxfd = max_fd(fds, fd_count);

  while(1) {
    int ret;
    fd_set rd_set;

    FD_ZERO(&rd_set);
    set_fds(fds, fd_count, &rd_set);

    ret = select(maxfd + 1, &rd_set, NULL, NULL, NULL);
    if (ret < 0 && errno == EINTR) {
      continue;
    }

    if (ret < 0) {
      perror("select()");
      exit(1);
    }

    // tun device RX Packets
    if (FD_ISSET(fds[IDX_STDIN], &rd_set)) {

    }
    if (FD_ISSET(fds[IDX_TUN_DEV], &rd_set)) {

    }
    for (int i = 0; i < net_if_count; ++i) {
      if ((FD_ISSET(fds[IDX_FIRST_IF + i], &rd_set))) {
        // network if #i

      }
    }
  }
}
