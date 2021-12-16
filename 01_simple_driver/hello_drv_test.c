#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define LEN 1024
#define ERROR -1 // return a error for Usage
/*
 * ./hello_drv_test -w abc
 * ./hello_drv_test -r
 */

int main(int argc, char **argv)
{
    int fd;
    char buf[LEN];
    int len;
    int ret;

    if(argc < 2) {
        printf("Usage: %s -w<string>\n", argv[0]);
        printf("       %s -r\n", argv[0]);
        return ERROR;
    }

    fd = open("/dev/hello",O_RDWR);
    if (fd == -1) {
        printf("cannot open file /dev/hello\n");
        return ERROR;
    }
    else {
        printf("open file /dev/hello ok\n");
    }

    if((0 == strcmp(argv[1], "-w")) && (argc == 3)) {
        len = strlen(argv[2]) + 1;
        len = len < 1024 ? len : 1024;
        ret = write(fd, argv[2], len);
        printf("write driver： %d\n", ret);
    } 
    else {
        ret = read(fd, buf, 1024);
        printf("read driver： %d\n", ret);
        buf[1023] = '\0';
        printf("APP read : %s\n", buf);
    }

    close(fd);

    return 0;
}