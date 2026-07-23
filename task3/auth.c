/* Task 3.2: User authentication with salted password hashing */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define MAX_USERS 10
#define CRED_FILE "task3/users.dat"

/* A simple, readable hash function (djb2 by Dan Bernstein).
   NOTE: chosen for clarity. A real system must use bcrypt/argon2 -
   this is discussed in the security analysis. */
unsigned long hash_password(const char *salt, const char *password) {
    char combined[256];
    snprintf(combined, sizeof(combined), "%s%s", salt, password);  /* salt + password */

    unsigned long hash = 5381;
    for (int i = 0; combined[i] != '\0'; i++)
        hash = ((hash << 5) + hash) + combined[i];   /* hash * 33 + c */
    return hash;
}

/* Register a new user: generate a salt, hash the password, store both */
void register_user(const char *username, const char *password) {
    srand(time(NULL) + rand());
    int salt_num = rand();
    char salt[32];
    snprintf(salt, sizeof(salt), "%d", salt_num);

    unsigned long hash = hash_password(salt, password);

    FILE *fp = fopen(CRED_FILE, "a");         /* append new record */
    if (!fp) { perror("cannot open credentials file"); return; }
    fprintf(fp, "%s %s %lu\n", username, salt, hash);   /* username salt hash */
    fclose(fp);

    printf("Registered user '%s' (password stored as salted hash)\n", username);
}

/* Verify a login attempt against the stored salt and hash */
int login(const char *username, const char *password) {
    FILE *fp = fopen(CRED_FILE, "r");
    if (!fp) { perror("cannot open credentials file"); return 0; }

    char u[64], salt[32];
    unsigned long stored_hash;
    int found = 0;

    while (fscanf(fp, "%63s %31s %lu", u, salt, &stored_hash) == 3) {
        if (strcmp(u, username) == 0) {       /* username matches - check password */
            found = 1;
            unsigned long attempt = hash_password(salt, password);
            fclose(fp);
            return attempt == stored_hash;    /* 1 = success, 0 = wrong password */
        }
    }
    fclose(fp);
    if (!found) printf("(no such user)\n");
    return 0;
}

int main(void) {
    printf("=== User authentication demonstration ===\n\n");

    /* set up two accounts */
    register_user("alice", "SecretPass123");
    register_user("bob",   "hunter2");
    printf("\n");

    /* test cases */
    printf("alice + correct password : %s\n",
           login("alice", "SecretPass123") ? "ACCESS GRANTED" : "ACCESS DENIED");
    printf("alice + wrong password   : %s\n",
           login("alice", "wrongpass") ? "ACCESS GRANTED" : "ACCESS DENIED");
    printf("bob   + correct password : %s\n",
           login("bob", "hunter2") ? "ACCESS GRANTED" : "ACCESS DENIED");
    printf("carol + any password     : %s\n",
           login("carol", "whatever") ? "ACCESS GRANTED" : "ACCESS DENIED");

    return 0;
}
