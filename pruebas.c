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

    int numordenes = 0; //ordenes-comandos separados por &
    char linecopy[strlen(line) + 1];
    strcpy(linecopy, line);
    char *plncpy = linecopy;
    char *orden = strsep(&plncpy, "&");
    while (orden != NULL)
    {
        if (*orden != '\0')
        {
            char *ordenvacia = strtok(orden, " ");
            if (ordenvacia != NULL)
            {
                numordenes++;
            }
        }
        orden = strsep(&plncpy, "&");
    }
    printf("el numero de ordenes es %d\n", numordenes);
}