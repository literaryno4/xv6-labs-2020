#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main (int argc, char* argv[]) {
    if (argc > 1) {
        write(2, "Usage: pingpong\n", 16);
    }
    int pid = fork();
    int p[2];
    char buffer[1];
    if (pipe(p) < 0) {
        write(2, "pipe error!\n", 12);
        exit(1);
    }

    switch (pid) {
    case -1:
        write(2, "fork error!\n", 12);
        exit(1);
    case 0:
        // child
        close(p[0]);
        write(p[1], "h", 1);
        close(p[1]);
        printf("%d: received ping\n", getpid());
        exit(0);
    default:
        break;
    }
    pid = wait((int *) 0);
    close(p[1]);
    read(p[0], buffer, 1);
    close(p[0]);
    printf("%d: received pong\n", getpid());

    exit(0);
}
