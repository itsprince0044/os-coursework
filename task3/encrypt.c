/* Task 3.4: File encryption and decryption using XOR cipher */
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#define BUF_SIZE 1024

/* XOR each byte of the buffer with the repeating key.
   Because XOR is its own inverse, the SAME function both
   encrypts and decrypts. */
void xor_cipher(char *data, int len, const char *key) {
    int keylen = strlen(key);
    for (int i = 0; i < len; i++)
        data[i] = data[i] ^ key[i % keylen];   /* wrap around the key */
}

/* read infile, transform it, write to outfile */
int process_file(const char *infile, const char *outfile, const char *key) {
    int in = open(infile, O_RDONLY);
    if (in < 0) { perror("open input"); return -1; }

    int out = open(outfile, O_CREAT | O_WRONLY | O_TRUNC, 0600);  /* owner-only */
    if (out < 0) { perror("open output"); close(in); return -1; }

    char buffer[BUF_SIZE];
    ssize_t n;
    while ((n = read(in, buffer, BUF_SIZE)) > 0) {
        xor_cipher(buffer, n, key);
        write(out, buffer, n);
    }
    close(in);
    close(out);
    return 0;
}

int main(void) {
    const char *key = "MySecretKey123";

    /* create a sensitive plaintext file */
    const char *plain = "task3/secret.txt";
    int fd = open(plain, O_CREAT | O_WRONLY | O_TRUNC, 0600);
    const char *msg = "CONFIDENTIAL: student ID and exam answers.\n";
    write(fd, msg, strlen(msg));
    close(fd);

    printf("=== File encryption demonstration ===\n\n");
    printf("Original file (%s):\n%s\n", plain, msg);

    /* encrypt it */
    process_file(plain, "task3/secret.enc", key);
    printf("Encrypted file (secret.enc) - raw bytes:\n");
    system("cat task3/secret.enc; echo");   /* shows scrambled, unreadable content */

    /* decrypt it back */
    process_file("task3/secret.enc", "task3/secret.dec", key);
    printf("\nDecrypted file (secret.dec):\n");
    system("cat task3/secret.dec");

    printf("\nEncryption and decryption completed successfully.\n");
    return 0;
}
