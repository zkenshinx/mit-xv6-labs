#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"

void search(char* path, char* dest_file);

int main(int argc, char* argv[]) {
  char path[512], dest_file[512];
  if (argc != 3) {
    fprintf(2, "Usage: find <dir> <dest>\n");
    exit(1);
  }

  strcpy(path, argv[1]);
  strcpy(dest_file, argv[2]);

  search(path, dest_file);
}

char* get_file_name(char* path) {
  char *p;
  for (p = path + strlen(path); p >= path && *p != '/'; --p)
    ;
  p++;
  return p;
}

void search(char* path, char* dest_file) {
  char buf[512], *p;
  int fd;
  struct stat st;
  struct dirent de;

  if ((fd = open(path, O_RDONLY)) < 0){
    fprintf(2, "ls: cannot open %s\n", path);
    return;
  }
  
  if (fstat(fd, &st) < 0){
    fprintf(2, "ls: cannot stat %s\n", path);
    close(fd);
    return;
  }

  // printf("hi\n);
  if (st.type == T_FILE) {
    char* file_name = get_file_name(path);
    if (strcmp(file_name, dest_file) == 0) {
      printf("%s\n", path);
    }
  } else if (st.type == T_DIR) {
    if (strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
      printf("ls: path too long\n");
      goto end;
    }
    strcpy(buf, path);
    p = buf + strlen(buf);
    *p++ = '/';
    while (read(fd, &de, sizeof(de)) == sizeof(de)) {
      if(de.inum == 0)
        continue;
      memmove(p, de.name, DIRSIZ);
      p[DIRSIZ] = 0;
      if (p[0] == '.') {
        continue;
      }
      search(buf, dest_file);
    }
  }

end:
  close(fd);
}
