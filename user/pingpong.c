#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
  int fd[2], pid;
  char byte;

  if (pipe(fd) < 0) {
    fprintf(2, "Something wrong with pipe\n");
    exit(1);
  }
  
  pid = fork();
  
  if (pid == 0) {
    read(fd[0], &byte, 1);
    close(fd[0]);
    printf("%d: received ping\n", getpid());
    write(fd[1], &byte, 1);
    close(fd[1]);
  } else if (pid > 0) {
    write(fd[1], &byte, 1);
    close(fd[1]);
    read(fd[0], &byte, 1);
    close(fd[0]);
    printf("%d: received pong\n", getpid());
  } else {
    fprintf(2, "Something wrong with fork\n");
    exit(1);
  }

  exit(0);
}
