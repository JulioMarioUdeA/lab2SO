#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/time.h>

char **mypath;

int main(int argc, char *argv[])
{
    mypath = malloc(sizeof(char *) * 2);
    mypath[0] = malloc(sizeof(char) * 6);
    mypath[0] = "/bin/";
    mypath[1] = NULL;

    printf("%p\n", &mypath);
    int pos = 0;
    for (int i = 1; i < argc; i++)
    {
        printf("--para %s--\n", argv[i]);
        pos = 0;
        int itis = 0;
        while (mypath[pos] != NULL)
        {
            printf("                while %d\n", pos);
            if (strcmp(argv[i], mypath[pos]) == 0)
            {
                printf("no va a entrar\n");
                itis = 1;
                break;
            }
            pos++;
        }
        if (itis == 0)
        {
            printf("                si entro\n");
            int cont = pos + 2;                                      //numero de posiciones del newmypath
            char **tmp_ptr = realloc(mypath, sizeof(char *) * cont); ///////////
            if (tmp_ptr == NULL)
            {
                printf("fallo en realocacion\n");
                exit(1);
            }
            mypath = tmp_ptr;
            mypath[cont - 1] = NULL;
            mypath[cont - 2] = malloc(sizeof(char) * (strlen(argv[i]) + 1));
            strcpy(mypath[cont - 2], argv[i]);
            /*
            char argcopy[strlen(argv[i]) + 1];
            strcpy(argcopy, argv[i]);
            printf("                argcopy = {%d-%ld-%s-%p}", cont, strlen(argcopy), argcopy, &argcopy);
            printf("\n");
            mypath[cont - 2] = argcopy;*/
        }
    }

    pos = 0;
    while (mypath[pos] != NULL)
    {
        printf("{%d-%ld-%s-%p}", pos, strlen(mypath[pos]), mypath[pos], &mypath[pos]);
        printf("\n");
        pos++;
    }
    printf("%p\n", &mypath);
}
