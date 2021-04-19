#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/time.h>

char **mypath;
char *concatenar(char *str1, char *str2)
{
    char sum[strlen(str1) + strlen(str2) + 1];
    strcpy(sum, str1);
    return strcat(sum, str2);
}

void liberarmd()
{
    int pos = 0;
    while (mypath[pos] != NULL)
    {
        free(mypath[pos]);
        pos++;
    }
    free(mypath);
}

int main(int argc, char *argv[])
{
    mypath = malloc(sizeof(char *) * 2);
    mypath[0] = malloc(sizeof(char) * 6);
    strcpy(mypath[0], "/bin/");
    mypath[1] = NULL;

    liberarmd();
    mypath = NULL;

    char **tmp_ptr = realloc(mypath, sizeof(char *) * argc);
    if (tmp_ptr == NULL)
    {
        printf("fallo en realocacion\n");
        liberarmd();
        exit(1);
    }
    mypath = tmp_ptr;
    for (int i = 1; i < argc; i++)
    {
        mypath[i - 1] = malloc(sizeof(char) * (strlen(argv[i]) + 1));
        strcpy(mypath[i - 1], argv[i]);
    }
    mypath[argc - 1] = NULL;

    //strcpy(mypath[0], "fl");

    int pos = 0;
    while (mypath[pos] != NULL)
    {
        printf("path---->   %d-%ld-%s-%p", pos, strlen(mypath[pos]), mypath[pos], &mypath[pos]);
        printf("\n");
        pos++;
    }
    /*
    for (int i = 1; i < numargs; i++)
                        {
                            pos = 0;
                            int itis = 0;
                            while (mypath[pos] != NULL)
                            {
                                if (strcmp(argumentos[i], mypath[pos]) == 0)
                                {
                                    itis = 1;
                                    break;
                                }
                                pos++;
                            }
                            if (itis == 0)
                            {
                                int cont = pos + 2; //numero de posiciones del newmypath
                                char **tmp_ptr = realloc(mypath, sizeof(char *) * cont);
                                if (tmp_ptr == NULL)
                                {
                                    printf("fallo en realocacion\n");
                                    write(STDERR_FILENO, error_message, strlen(error_message));
                                    liberarmd();
                                    exit(1);
                                }
                                mypath = tmp_ptr;
                                mypath[cont - 1] = NULL;
                                mypath[cont - 2] = malloc(sizeof(char) * (strlen(argumentos[i]) + 1));
                                strcpy(mypath[cont - 2], argumentos[i]);
                            }
                        }
    }*/
}
