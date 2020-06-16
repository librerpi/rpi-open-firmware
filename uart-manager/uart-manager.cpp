//#include <stropts.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <sys/epoll.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/signalfd.h>

#define MAX_EVENTS 10

void monitor_fd(int epollfd, int fd, const char *errmsg) {
  struct epoll_event ev;
  ev.events = EPOLLIN;
  ev.data.fd = fd;
  if (epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &ev)) {
    perror(errmsg);
    _exit(3);
  }
}

int main(int argc, char **argv) {
  struct epoll_event events[MAX_EVENTS];
  char buffer[512];
  ssize_t size;
  int fd, epollfd, signals;
  const int flags = TIOCM_DTR;
  sigset_t sigmask;
  struct termios old_tio, new_tio;

  sigemptyset(&sigmask);
  sigaddset(&sigmask, SIGINT);
  signals = signalfd(-1, &sigmask, 0);

  sigprocmask(SIG_BLOCK, &sigmask, 0);

  fd = open("/dev/ttyUSB0", O_RDWR | O_NOCTTY);
  if (fd < 0) {
    perror("unable to open uart");
    return 1;
  }
  struct termios options;
  tcgetattr(fd, &options);
  printf("before c_cflag: %x\n", options.c_cflag);
  options.c_cflag &= ~CRTSCTS;
  options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
  options.c_iflag &= ~(IXON | IXOFF | IXANY);
  options.c_iflag |= IGNCR;

  options.c_oflag &= ~ONOCR;
  options.c_oflag &= ~OPOST;

  options.c_cflag &= ~HUPCL;
  tcsetattr(fd, TCSANOW, &options);


  tcgetattr(STDIN_FILENO,&old_tio);
  new_tio=old_tio;
  new_tio.c_lflag &=(~ICANON & ~ECHO);
  tcsetattr(STDIN_FILENO,TCSANOW,&new_tio);

  epollfd = epoll_create1(0);
  if (epollfd < 0) {
    perror("unable to epoll");
    return 2;
  }

  monitor_fd(epollfd, fd, "epoll add uart failed");
  monitor_fd(epollfd, STDIN_FILENO, "epoll add stdin failed");
  monitor_fd(epollfd, signals, "epoll add signalfd failed");

  ioctl(fd, TIOCMBIC, &flags);
  bool run = true;
  while (run) {
    int nfds = epoll_wait(epollfd, events, MAX_EVENTS, -1);
    if (nfds < 0) {
      printf("%d ", nfds);
      perror("epoll_wait failed");
      return 5;
    }
    for (int i=0; i < nfds; i++) {
      if (events[i].data.fd == fd) {
        size = read(fd, buffer, 512);
        if (size < 0) {
          perror("cant read uart");
          return 6;
        }
        if (write(STDOUT_FILENO, buffer, size) < 0) {
          perror("cant write to stdout");
          return 7;
        }
      } else if (events[i].data.fd == STDIN_FILENO) {
        size = read(STDIN_FILENO, buffer, 512);
        if (size < 0) {
          perror("cant read stdin");
          return 8;
        }
        if (write(fd, buffer, size) < 0) {
          perror("cant write to uart");
          return 9;
        }
      } else if (events[i].data.fd == signals) {
        ioctl(fd, TIOCMBIS, &flags);
        run = false;
      }
    }
  }
  puts("closing");
  close(fd);
  tcsetattr(STDIN_FILENO,TCSANOW,&old_tio);
  return 0;
}
