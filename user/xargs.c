#include "kernel/param.h"
#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char* argv[]) {
    int nread;
    int i = 1;
    char buf[512];
    char* p = buf;
    char* excargv[MAXARG];
    if (argc < 2) {
        excargv[0] = "echo";
        ++i;
    } else {
        for (; i < argc && i <= MAXARG; ++i) {
            excargv[i - 1] = argv[i];
        }
    }
    while((nread = read(0, p, 1)) == 1) {
        if (*p == '\n') {
            *p = 0;
            excargv[i - 1] = buf;
            int pid = fork();
            if (pid == 0) {
                printf("\n");
                if (exec(excargv[0], excargv) < 0) {
                    printf("exec error!\n");
                    exit(-1);
                }
                exit(0);
            } else {
                wait((int*)0);
            }
            p = buf;
        }
        ++p;
    }
    exit(0);
}
