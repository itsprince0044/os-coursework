/* Task 3.5: Audit logging - records all file access and actions */
#include <stdio.h>
#include <time.h>
#include <string.h>

#define LOG_FILE "task3/audit.log"

/* append one entry: timestamp, user, action, target, result */
void log_action(const char *user, const char *action,
                const char *target, const char *result) {
    FILE *fp = fopen(LOG_FILE, "a");   /* append-only: never overwrites history */
    if (!fp) { perror("cannot open audit log"); return; }

    time_t now = time(NULL);
    char timestamp[64];
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", localtime(&now));

    fprintf(fp, "[%s] user=%-6s action=%-8s target=%-14s result=%s\n",
            timestamp, user, action, target, result);
    fclose(fp);

    printf("Logged: %s %s %s -> %s\n", user, action, target, result);
}

int main(void) {
    printf("=== Audit logging demonstration ===\n\n");

    /* simulate a sequence of user actions - both successful and failed */
    log_action("alice", "LOGIN",   "-",           "SUCCESS");
    log_action("alice", "READ",    "report.txt",  "ALLOWED");
    log_action("bob",   "LOGIN",   "-",           "FAIL");
    log_action("bob",   "WRITE",   "report.txt",  "DENIED");
    log_action("alice", "ENCRYPT", "secret.txt",  "SUCCESS");
    log_action("carol", "DELETE",  "report.txt",  "DENIED");

    printf("\n--- Full audit log contents ---\n");
    char line[256];
    FILE *fp = fopen(LOG_FILE, "r");
    while (fp && fgets(line, sizeof(line), fp)) printf("%s", line);
    if (fp) fclose(fp);

    return 0;
}
