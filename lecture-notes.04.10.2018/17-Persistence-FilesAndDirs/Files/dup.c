#include <assert.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    int fd = open("README", O_RDONLY);
    assert(fd >= 0);

    int fd2 = dup(fd);
    int rc = lseek(fd, 10, SEEK_SET);
    printf("fd: offset %d\n", rc);

    printf("fd2: offset %d\n", (int) lseek(fd2, 0, SEEK_CUR));

    return 0;
}

