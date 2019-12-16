#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/sysmacros.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

int main(int argc, char **argv) {
  int status;
  //status = mknod("/dev/ttyAMA0", S_IFCHR | 0666, makedev(204,64));
  status = mknod("/dev/kmsg", S_IFCHR | 0666, makedev(1,11));
  if (status) return 1;
  int fd = open("/dev/kmsg", O_RDWR);
  if (fd < 0) {
    printf("failed to open kmsg: %s\n", strerror(errno));
  }
  status = dup2(fd, 0);
  if (status < 0) return 3;
  status = dup2(fd, 1);
  if (status < 0) return 40;
  status = dup2(fd, 2);
  if (status < 0) return 50;
  close(fd);
  status = write(1, "hello world\n", 12);
  if (status != 12) return 60;
  status = printf("and now with printf\n");
  if (status < 0) return 70;
  return 43;
}
