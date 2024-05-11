#include "kernel/types.h"
#include "user/user.h"

void primes(int* nums, int count);

int main(int argc, char* argv[]) {
  int nums[100], i;

  for (i = 2; i <= 35; i++)
    nums[i - 2] = i;

  primes(nums, 34);
  return 0;
}

void primes(int* nums, int count) {
  int fd[2], pid, c, i, first;
  if (count <= 0) 
    return;
  first = nums[0];
  printf("prime %d\n", nums[0]);

  if (pipe(fd) < 0) {
    fprintf(2, "Something wrong with pipe\n");
    exit(1);
  }
  
  pid = fork();
  if (pid == 0) {
    close(fd[1]);
    count = read(fd[0], nums, 1024) / 4;
    close(fd[0]);
    primes(nums, count);
  } else if (pid > 0) {
    close(fd[0]);
    c = 0;
    for (i = 1; i < count; ++i) {
      if (nums[i] % first != 0) {
        nums[c++] = nums[i];
      }
    }
    write(fd[1], nums, sizeof(int) * c);
    close(fd[1]);
    wait(0);
  } else {
    fprintf(2, "Something wrong with fork\n");
    exit(1);
  }
  
}
