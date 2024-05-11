#include "kernel/types.h"
#include "kernel/param.h"
#include "user/user.h"

#define BUF_SIZE 512

int main(int argc, char* argv[]) {
	char* exec_args[MAXARG + 1], buf[BUF_SIZE];
	int i;
	if (argc + 1 >= MAXARG) {
		fprintf(2, "Number of arguments is too much!");
		exit(-1);
	} else if (argc == 1) {
		fprintf(2, "Number of arguments is too small!");
		exit(-1);
	}

	// Copy arguments
	for (i = 0; i + 1 < argc; ++i) {
		exec_args[i] = argv[i + 1];
	}
	for (; i < MAXARG; ++i) {
		int end = 0;
		int j = 0;
		while (j + 1 < BUF_SIZE) {
			int c = read(0, buf + j, 1);
			if (c == 0) {
				end = 1;
				break;
			} else if (buf[j] == '\n') {
				break;
			}
			++j;
		}
		if (end) {
			break;
		}
		if (j + 1 >= BUF_SIZE) {
			fprintf(2, "Very big argument bro");
			exit(-1);
		}
		buf[j] = '\0';
		exec_args[i] = malloc(strlen(buf) + 1);
		strcpy(exec_args[i], buf);
	}
	exec_args[i] = 0;

	int pid = fork();
	if (pid > 0) {
		exec(argv[1], exec_args);
	} else if (pid == 0) {
		wait(0);
	} else { 
		fprintf(2, "Couldn't do it");
		exit(-1);
	}
	exit(0);
}
