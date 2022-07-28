#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

// KMP implementation of string matching
int strStr(char* s, char* p) { 
    int ssize = strlen(s);
    int psize = strlen(p);
    if (psize == 0) {
        return 0;
    }
    if (psize > ssize) {
        return -1;
    }

    int presuffixLen[psize];
    presuffixLen[0] = 0;

    for (int i = 0, j = 1; j < psize; ++j) {
        while (i > 0 && p[i] != p[j]) {
            i = presuffixLen[i - 1];
        }
        if (p[i] == p[j]) {
            ++i;
        }
        presuffixLen[j] = i;
    }

    for (int i = 0, j = 0; j < ssize; ++j) {
        while (i > 0 && p[i] != s[j]) {
            i = presuffixLen[i - 1];
        }
        if (p[i] == s[j]) {
            ++i;
        }
        if (i == psize) {
            return j - i + 1;
        }
    }
    return -1;
}

void find(char* path, char* target) {
    int fd, idx;
    char buf[512], *p;
    struct dirent de;
    struct stat st;

    if ((fd = open(path, 0)) < 0) {
        fprintf(2, "ls: cannot open %s\n", path);
        return;
    }

    if (fstat(fd, &st) < 0) {
        fprintf(2, "ls: cannot stat %s\n", path);
        close(fd);
        return;
    }
    switch (st.type) {
        case T_DEVICE:
        case T_FILE:
            idx = strStr(path, target);
            if (idx > 0 && path[idx - 1] == '/') {
                printf("%s\n", path);
            }
            break;
        case T_DIR:
            strcpy(buf, path);
            p = buf + strlen(buf);
            *p++ = '/';
            while (read(fd, &de, sizeof(de)) == sizeof(de)) {
                if (de.inum == 0 || strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0) {
                    continue;
                }
                memmove(p, de.name, DIRSIZ);
                p[DIRSIZ] = 0;
                find(buf, target);
            }
            break;
    }
    close(fd);
}

int main(int argc, char*argv[]) {
    if (argc < 3) {
        fprintf(2, "Usage: %s path target\n", argv[0]);
        exit(1);
    }
    find(argv[1], argv[2]);
    exit(0);
}
