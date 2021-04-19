#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/time.h>

char error_message[30] = "An error has occurred\n";
char **mypath;

int main(int argc, char *argv[])
{
    mypath = malloc(sizeof(char *) * 2);
    mypath[0] = malloc(sizeof(char) * 6);
    mypath[0] = "/bin/";
    mypath[1] = NULL;

    if (argc == 1)
    {
        size_t len = 0;
        char *line = NULL;
        FILE *archivo = stdin;

        while (1)
        {
            printf("%dwish> ", getpid());
            if (getline(&line, &len, archivo) != -1)
            {
                char *p = line;
                if (*line == '\n')
                {
                    continue;
                }
                while (*p != '\0')
                {
                    if (*p == '\n')
                    {
                        *p = '\0';
                    }
                    p++;
                }

                printf("        -usted ingresó %s-\n", line);

                int numordenes = 0;
                char *orden = strtok(line, "&");
                while (orden != NULL)
                {
                    numordenes++;
                    printf("        {%d-%ld-%s-%p}", numordenes, strlen(orden), orden, &orden);
                    printf("\n");

                    char ordencopy[strlen(orden) + 1];
                    char *pocpy = ordencopy;
                    strcpy(ordencopy, orden);

                    printf("        [%d-%ld-%s-%p]", numordenes, strlen(ordencopy), ordencopy, &ordencopy);
                    printf("\n");
                    //################################ obteniendo numero de argumentos
                    int numargs = 0;
                    char *arg = strsep(&orden, " ");
                    while (arg != NULL)
                    {
                        if (*arg != '\0')
                        {
                            numargs++;
                            printf("        <%d-%ld-%s-%p>", numargs, strlen(arg), arg, &arg);
                            printf("\n");
                        }

                        arg = strsep(&orden, " ");
                    }

                    //################################# extraccion de argumentos
                    char *argumentos[numargs + 1];
                    argumentos[numargs] = NULL;
                    numargs = 0;
                    arg = strsep(&pocpy, " ");
                    while (arg != NULL)
                    {
                        if (*arg != '\0')
                        {
                            argumentos[numargs] = arg;
                            printf("        ¬¬%d-%ld-%s-%p¬¬", numargs, strlen(argumentos[numargs]), argumentos[numargs], &argumentos[numargs]);
                            printf("\n");
                            numargs++;
                        }

                        arg = strsep(&pocpy, " ");
                    }

                    //################################ comparando comandos directos
                    if (strcmp(argumentos[0], "exit") == 0)
                    {
                        exit(0);
                    }
                    else if (strcmp(argumentos[0], "path") == 0)
                    {
                        int pos = 0;
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
                                    exit(1);
                                }
                                mypath = tmp_ptr;
                                mypath[cont - 1] = NULL;
                                mypath[cont - 2] = malloc(sizeof(char) * (strlen(argumentos[i]) + 1));
                                strcpy(mypath[cont - 2], argumentos[i]);
                            }
                        }
                        pos = 0;
                        while (mypath[pos] != NULL)
                        {
                            printf("path---->   %d-%ld-%s-%p", pos, strlen(mypath[pos]), mypath[pos], &mypath[pos]);
                            printf("\n");
                            pos++;
                        }
                    }
                    else if (strcmp(argumentos[0], "cd") == 0)
                    {
                        /* code */
                    }
                    else
                    {
                        //################################ creacion del proceso
                        int proc = fork();
                        if (proc < 0)
                        {
                            write(STDERR_FILENO, error_message, strlen(error_message));
                            exit(1);
                        }
                        else if (proc == 0)
                        {
                            execvp(argumentos[0], argumentos);
                            write(STDERR_FILENO, error_message, strlen(error_message));
                            exit(1);
                        }
                        else
                        {
                            wait(NULL);
                        }
                    }

                    //################################## impresiones

                    if (orden == NULL)
                    {
                        printf("        orden null\n");
                    }
                    else
                    {
                        printf("        ~%s~\n", orden);
                    }
                    orden = strtok(NULL, "&");
                    if (orden == NULL)
                    {
                        printf("        orden null\n");
                    }
                    else
                    {
                        printf("        ~%s~\n", orden);
                    }
                }

                if (numordenes == 0)
                {
                    continue;
                }
            }
        }
    }
    else if (argc == 2)
    {
    }
    else
    {
    }
}