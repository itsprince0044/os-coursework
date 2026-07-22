/* Task 3.1: File operations using POSIX system calls */
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#define BUF_SIZE 1024

/* create a new file, or truncate it if it exists */
int file_create(const char *name) {
    int fd = open(name, O_CREAT | O_WRONLY | O_TRUNC, 0644);  /* rw-r--r-- */
    if (fd < 0) { perror("create failed"); return -1; }
    close(fd);
    printf("Created file '%s'\n", name);
    return 0;
}

/* write text into a file (overwrites existing contents) */
int file_write(const char *name, const char *text) {
    int fd = open(name, O_WRONLY | O_TRUNC);
    if (fd < 0) { perror("write failed"); return -1; }

    ssize_t written = write(fd, text, strlen(text));
    close(fd);

    if (written < 0) { perror("write failed"); return -1; }
    printf("Wrote %ld bytes to '%s'\n", (long)written, name);
    return 0;
}

/* read a file and print its contents */
int file_read(const char *name) {
    int fd = open(name, O_RDONLY);
    if (fd < 0) { perror("read failed"); return -1; }

    char buffer[BUF_SIZE];
    ssize_t bytes = read(fd, buffer, BUF_SIZE - 1);
    close(fd);

    if (bytes < 0) { perror("read failed"); return -1; }
    buffer[bytes] = '\0';
    printf("Contents of '%s' (%ld bytes):\n%s\n", name, (long)bytes, buffer);
    return 0;
}

/* delete a file */
int file_delete(const char *name) {
    if (unlink(name) < 0) { perror("delete failed"); return -1; }
    printf("Deleted file '%s'\n", name);
    return 0;
}

int main(void) {
    const char *filename = "task3/testfile.txt";

    printf("=== File operations demonstration ===\n\n");

    file_create(filename);
    file_write(filename, "Hello from ST5004CEM.\nThis file was written using POSIX write().\n");
    file_read(filename);

    /* error handling demo: try reading a file that does not exist */
    printf("\n-- error handling test --\n");
    file_read("task3/does_not_exist.txt");

    printf("\n");
    file_delete(filename);

    return 0;
}
