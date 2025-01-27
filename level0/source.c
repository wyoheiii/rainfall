#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int param_1, int param_2){
    int convertedValue;
    char *shellPath;
    uid_t euid;
    gid_t egid;

    convertedValue = atoi(*(char **)(param_2 + 4));
    if (convertedValue == 423) {  // 0x1a7 を 10進数 423 に置換
        shellPath = strdup("/bin/sh");
        placeholder = 0;

        egid = getegid();
        euid = geteuid();

        setresgid(egid, egid, egid);
        setresuid(euid, euid, euid);

        execv("/bin/sh", &shellPath);
    } else {
        fwrite("No !\n", 1, 5, stderr);
    }
    return 0;
}
