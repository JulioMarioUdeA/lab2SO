#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/time.h>

int main(int argc, char *argv[])
{
    char *line = argv[1];

    char *p = line;
    while (*p != '\0')
    {
        if (*p == '\n' || *p == 13)
        {
            *p = '\0';
            break;
        }
        p++;
    }

    char *pedazo = strtok(line, ">");
    while (pedazo != NULL)
    {
        if (*pedazo == '\0')
        {
            printf("/0\n");
        }
        else if (pedazo == NULL)
        {
            printf("NULL\n");
        }
        else
        {
            printf("-%s\n", pedazo);
        }
        pedazo = strtok(NULL, ">");
    }
    if (pedazo == NULL)
    {
        printf("NULL\n");
    }
}