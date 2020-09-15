#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <assert.h>
#include <unistd.h>

// off_t lseek(int fildes, off_t offset, int whence);

int
main(int argc, char *argv[])
{
    int fd = open("file1", O_WRONLY);
    assert(fd >= 0);

    // close(fd);
    return 0;
}

