/* Task 3.3: File permission system (rwx for owner/group/others) */
#include <stdio.h>
#include <string.h>

/* permission bits, like Linux */
#define P_READ  4
#define P_WRITE 2
#define P_EXEC  1

/* a file with an owner, a group, and a 3-digit octal permission set */
typedef struct {
    char name[64];
    char owner[32];
    char group[32];
    int  perms;    /* e.g. 0754 stored as octal */
} SecureFile;

/* which class is this user: owner, group member, or other? */
const char *user_class(SecureFile *f, const char *user, const char *user_group) {
    if (strcmp(user, f->owner) == 0)       return "owner";
    if (strcmp(user_group, f->group) == 0) return "group";
    return "others";
}

/* extract the permission digit (0-7) for the user's class */
int perms_for_class(SecureFile *f, const char *cls) {
    if (strcmp(cls, "owner") == 0) return (f->perms >> 6) & 7;  /* first digit */
    if (strcmp(cls, "group") == 0) return (f->perms >> 3) & 7;  /* second digit */
    return f->perms & 7;                                        /* third digit */
}

/* check one operation (read/write/exec) for a given user */
int check_access(SecureFile *f, const char *user, const char *user_group, int op, const char *op_name) {
    const char *cls = user_class(f, user, user_group);
    int allowed = perms_for_class(f, cls) & op;

    printf("%-6s (%-6s) wants to %-5s '%s' : %s\n",
           user, cls, op_name, f->name, allowed ? "ALLOWED" : "DENIED");
    return allowed != 0;
}

/* print permissions in rwx form, like ls -l */
void print_perms(SecureFile *f) {
    int p = f->perms;
    char s[10] = "---------";
    const char *bits = "rwxrwxrwx";
    for (int i = 0; i < 9; i++)
        if (p & (1 << (8 - i))) s[i] = bits[i];
    printf("File '%s'  owner=%s group=%s  perms=%s (%o)\n",
           f->name, f->owner, f->group, s, f->perms);
}

int main(void) {
    /* file owned by alice, group "staff", perms rwxr-x--- (0750) */
    SecureFile file = {"report.txt", "alice", "staff", 0750};

    printf("=== File permission system ===\n\n");
    print_perms(&file);
    printf("\n");

    /* test different users against the permissions */
    check_access(&file, "alice", "staff",   P_READ,  "read");   /* owner: allowed */
    check_access(&file, "alice", "staff",   P_WRITE, "write");  /* owner: allowed */
    check_access(&file, "bob",   "staff",   P_READ,  "read");   /* group: allowed */
    check_access(&file, "bob",   "staff",   P_WRITE, "write");  /* group: denied  */
    check_access(&file, "carol", "guests",  P_READ,  "read");   /* others: denied */

    return 0;
}
