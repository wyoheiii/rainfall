#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


void run(void)
{
    fwrite("Good... Wait what?\n", 1, 19, stdout);
    system("/bin/sh");
    return;
}

void main(void)
{
    char buffer[64];

    gets(buffer);
    return;
}