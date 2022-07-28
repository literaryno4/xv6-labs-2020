
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int p[2];
int isDelete[36];

void sieve(int p[2]) {
    close(p[1]);
    int n;
    int c;
    int nread;
    if ((nread = read(p[0], &n, 4)) <= 0) {
        exit(0);
    }
    printf("prime %d\n", n);
    
    int newP[2];
    if (pipe(newP) < 0) {
        write(2, "pipe error!\n", 12);
        exit(1);
    }

    int pid = fork();
    if (pid > 0) {
        close(newP[0]);
        while ((nread = read(p[0], &c, 4)) > 0) {
            if (c % n != 0) {
                write(newP[1], &c, nread);
            }
        }
        close(p[0]);
        close(newP[1]);
        wait((int*) 0);
        exit(0);
    } else {
        sieve(newP);
        exit(0);
    }
}

int main (int argc, char* argv[]) {
    if (argc > 1) {
        write(2, "Usage: primes\n", 14);
    }

    for (int i = 0; i < 36; ++i) {
        isDelete[i] = 0;
    }

    if (pipe(p) < 0) {
        write(2, "pipe error!\n", 12);
        exit(1);
    }
    int pid = fork();
    if (pid > 0) {
        close(p[0]);
        for (int c = 2; c <=35; ++c) {
            write(p[1], &c, 4);
        }
        close(p[1]);
        wait((int*) 0);
        exit(0);
    } else {
        sieve(p);
        exit(0);
    }
}
