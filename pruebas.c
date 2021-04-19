#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
char **mypath;
int main(int argc, char *argv[])
{
    mypath = malloc(sizeof(char *) * 2);
    mypath[0] = "/bin/";
    mypath[1] = NULL;


    printf("%p\n", &mypath);
    int pos = 0;
    for (int i = 1; i < argc; i++)
    {
        pos = 0;
        int itis = 0;
        while (mypath[pos] != NULL)
        {
            if (strcmp(argv[i], mypath[pos]) == 1)
            {
                itis = 1;
                break;
            }
            pos++;
        }
        if (itis == 0)
        {
            int cont = pos + 2;
            char *newmypath[cont];
            newmypath[cont - 1] = NULL; //nesmypath[3] = null
            char argcopy[strlen(argv[i]) + 1];
            strcpy(argcopy, argv[i]);
            newmypath[cont - 2] = argcopy;
            for (int i = 0; i < pos; i++)
            {
                newmypath[i] = mypath[i];
            }

            pos = 0;
            while (newmypath[pos] != NULL)
            {
                printf("¬¬%d-%ld-%s-%p¬¬", pos, strlen(newmypath[pos]), newmypath[pos], &newmypath[pos]);
                printf("\n");
                pos++;
            }
        }
    }
}