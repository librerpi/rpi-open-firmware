//#include <stropts.h>
#include <cstring>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sys/epoll.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/signalfd.h>
#include <sys/stat.h>
#include <termios.h>
#include <thread>
#include <unistd.h>

#define MAX_EVENTS 10

using namespace std;

void monitor_fd(int epollfd, int fd, const char *errmsg) {
  struct epoll_event ev;
  ev.events = EPOLLIN;
  ev.data.fd = fd;
  if (epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &ev)) {
    perror(errmsg);
    _exit(3);
  }
}

struct xmodem_packet {
  uint8_t magic;
  uint8_t block_num;
  uint8_t block_num_invert;
  uint8_t payload[128];
  uint8_t checksum;
} __attribute__((packed));

struct send_args {
  int fd;
  struct xmodem_packet packet;
};

static ssize_t write_capped(int fd, void *buf, size_t len) {
#define CAP 256
  if (len > CAP) len = CAP;
  ssize_t ret = write(fd, buf, len);
  struct timespec t = { 0, 1000 * 1000 * 100 };
  nanosleep(&t, NULL);
  return ret;
}

static void send_packet(struct send_args arg) {
  struct timespec t;
  uint8_t *raw_packet = reinterpret_cast<uint8_t*>(&arg.packet);
  int i = 0;
  while (i < 132) {
    //printf("L %d/132", i);
    //fflush(stdout);
    int ret = write(arg.fd, raw_packet + i, 132 - i);
    i += ret;
  }
  //printf("sent %d %d %d %d\n", arg.packet.magic, arg.packet.block_num, arg.packet.block_num_invert, arg.packet.checksum);
}

static void xmodem_send_block(int fd, uint32_t blockNr, void *buffer, int elf_size) {
  struct xmodem_packet packet;

  packet.magic = 1;
  packet.block_num = blockNr & 0xff;
  packet.block_num_invert = 255 - (blockNr & 0xff);
  memcpy(packet.payload, buffer + (128* (blockNr-1)), 128);
  uint8_t crc8 = 0;
  for (int i=0; i<128; i++) {
    crc8 += packet.payload[i];
  }
  packet.checksum = crc8;
  struct send_args arg = { fd, packet };
  thread t1(send_packet, arg);
  t1.detach();
}

int main(int argc, char **argv) {
  struct epoll_event events[MAX_EVENTS];
  char buffer[512];
  ssize_t size;
  int fd, epollfd, signals;
  const int flags = TIOCM_DTR;
  sigset_t sigmask;
  struct termios old_tio, new_tio;
  void *elf_buffer = NULL;
  int elf_size = 0;
  bool xmodem = false;
  int blockNr = 0;
  int ret;

  string ring_buffer = "";

  if (argc == 2) {
    int elf_fd = open(argv[1], O_RDONLY);
    if (elf_fd < 0) {
      perror("cant open elf file");
      return 10;
    }
    struct stat stats;
    fstat(elf_fd, &stats);
    elf_size = stats.st_size;
    elf_buffer = mmap(NULL, stats.st_size, PROT_READ, MAP_SHARED, elf_fd, 0);
    if (elf_buffer == MAP_FAILED) {
      perror("cant mmap elf file");
      return 11;
    }
    close(elf_fd);
  }

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
  ret = tcsetattr(fd, TCSANOW, &options);
  if (ret) {
    perror("cant do 1st tcsetattr");
    return 12;
  }

  tcgetattr(STDIN_FILENO,&old_tio);
  new_tio=old_tio;
  new_tio.c_lflag &=(~ICANON & ~ECHO);
  ret = tcsetattr(STDIN_FILENO,TCSANOW,&new_tio);
  if (ret) {
    perror("cant do 2nd tcsetattr");
    return 13;
  }

  epollfd = epoll_create1(0);
  if (epollfd < 0) {
    perror("unable to epoll");
    return 2;
  }

  monitor_fd(epollfd, fd, "epoll add uart failed");
  monitor_fd(epollfd, STDIN_FILENO, "epoll add stdin failed");
  monitor_fd(epollfd, signals, "epoll add signalfd failed");

  ret = ioctl(fd, TIOCMBIC, &flags);
  if (ret) {
    perror("cant do 1st ioctl");
    return 14;
  }
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
        string chunk(buffer, size);
        ring_buffer += chunk;
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
        ret = ioctl(fd, TIOCMBIS, &flags);
        if (ret) {
          perror("cant do 2nd ioctl");
          return 15;
        }
        run = false;
      }

      if (ring_buffer.length() > 1024) {
        ring_buffer = ring_buffer.substr(ring_buffer.length() - 1024, 1024);
      }
      if (ring_buffer.find("press X to stop autoboot") != string::npos) {
        ring_buffer = "";
        write(fd, "X", 1);
        xmodem = true;
        blockNr = 1;
      }
      if (xmodem) {
        if (ring_buffer.find("\x15", 0, 1) != string::npos) {
          puts("NAK");
          xmodem_send_block(fd, blockNr, elf_buffer, elf_size);
          ring_buffer = "";
        } else if (ring_buffer.find("\6", 0, 1) != string::npos) {
          printf("ACK %d / %d\r", blockNr, elf_size/128);
          fflush(stdout);
          blockNr++;
          if ((blockNr*128) > elf_size) {
            write(fd, "\4", 1);
            puts("EOF!");
          } else {
            xmodem_send_block(fd, blockNr, elf_buffer, elf_size);
          }
          ring_buffer = "";
        }
      }
    }
  }
  puts("closing");
  close(fd);
  ret = tcsetattr(STDIN_FILENO,TCSANOW,&old_tio);
  if (ret) {
    perror("cant do 3rd tcsetattr");
    return 16;
  }
  return 0;
}
