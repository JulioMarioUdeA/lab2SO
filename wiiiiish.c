#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/time.h>

void crearprocesos(char *line);
const char *mypath[] = {"/bin/", NULL};
char error_message[30] = "An error has occurred\n";

int main(int argc, char *argv[])
{
    if (argc == 1)
    {
        size_t len = 0;
        char *line = NULL;
        FILE *archivo = stdin;

        while (1)
        {
            int i = 0;
            if (mypath == NULL)
            {
                printf("null");
            }
            if (mypath[i] == NULL)
            {
                printf("null[0]");
            }
            while (mypath[i] != NULL)
            {
                printf("-%s", mypath[i]);
                i++;
            }

            printf("%dwish> ", getpid());
            if (getline(&line, &len, archivo) != -1 && *line != '\n')
            {
                crearprocesos(line);
            }
            printf("\n");
            fflush(archivo);
        }
        fclose(archivo);
    }
    else if (argc == 2)
    {
        size_t len = 0;
        char *line = NULL;
        FILE *archivo = fopen(argv[1], "r");
        if (archivo == NULL)
        {
            write(STDERR_FILENO, error_message, strlen(error_message));
            exit(1);
        }
        while (getline(&line, &len, archivo) != -1)
        {
            printf("\nla linea es: %s", line);
            crearprocesos(line);
        }
        fclose(archivo);
    }
    else
    {
        write(STDERR_FILENO, error_message, strlen(error_message));
        exit(1);
        free(mypath);
    }
}

void crearprocesos(char *line)
{
    char *comando = strsep(&line, "&\n"); //el comando está separádo con &
    if (comando != NULL && *comando != '\0' && *comando != '\n')
    {
        printf("\n----------#####____________");
        ///////copiando la linea
        int tam = (int)strlen(comando);
        char *linecopy = (char *)malloc(sizeof(char) * tam + 1);
        for (int i = 0; i <= tam; i++)
        {
            linecopy[i] = comando[i];
        }

        ////////obteniendo numero de argumentos
        char *pedazo = NULL;
        pedazo = strsep(&linecopy, " \n");
        int cont = 0;
        while (pedazo != NULL)
        {
            if (*pedazo != '\0' && *pedazo != '\n')
            {
                cont++;
            }
            pedazo = strsep(&linecopy, " \n");
        }
        free(linecopy);

        //obteniendo argumentos en un array
        pedazo = strsep(&comando, " \n");
        char *argumentos[cont + 1];
        //char **argumentos = (char **)malloc(sizeof(char) * (cont + 1));
        argumentos[cont] = NULL;
        int n = 0;
        while (pedazo != NULL)
        {
            if (*pedazo != '\0' && *pedazo != '\n')
            {
                argumentos[n] = (char *)malloc(sizeof(char) * ((int)strlen(pedazo) + 1));

                for (int i = 0; i <= (int)strlen(pedazo); i++)
                {
                    argumentos[n][i] = pedazo[i];
                }
                printf("\n~%s~%s~", argumentos[n], pedazo);
                n++;
            }
            pedazo = strsep(&comando, " \n");
        }
        char *orden = argumentos[0];

        if (strcmp(orden, "exit") == 0)
        {
            if (1 != cont)
            {
                write(STDERR_FILENO, error_message, strlen(error_message));
            }
            else
            {
                exit(0);
            }
        }
        else if (strcmp(orden, "cd") == 0)
        {
            if (2 != cont || -1 == chdir(argumentos[1]))
            {
                write(STDERR_FILENO, error_message, strlen(error_message));
            }
        }
        else if (strcmp(orden, "path") == 0)
        {
            int i = 0;
            while (argumentos[i] != NULL)
            {
                //strcpy(mypath[i - 1], argumentos[i]);
                mypath[i] = argumentos[i + 1];
                i++;
            }
        }
        else
        {
            int rc = fork();
            if (rc < 0)
            {
                write(STDERR_FILENO, error_message, strlen(error_message));
            }
            else if (rc == 0)
            {
                printf("\nentrando en el proceso hijo %s", argumentos[0]);
                //execvp(argumentos[0], argumentos);

                int i = 0;
                while (mypath[i] != NULL)
                {
                    int tam = (int)strlen(mypath[i]);
                    //char *copy = (char *)malloc(sizeof(char) * tam + 1 + (int)strlen(argumentos[0]));
                    char copy[tam + 1 + (int)strlen(argumentos[0])];
                    for (int j = 0; j < tam; j++)
                    {
                        copy[j] = mypath[i][j];
                    }
                    int tam2 = (int)strlen(argumentos[0]);
                    for (int j = 0; j <= tam2; j++)
                    {
                        copy[tam + j] = argumentos[0][j];
                    }

                    if (access(copy, X_OK) == 0)
                    {
                        execv(copy, argumentos);
                    }
                    i++;
                }
                write(STDERR_FILENO, error_message, strlen(error_message));
                exit(1);
            }
            else
            {
                wait(NULL);
                if (line != NULL && *line != '\0')
                {
                    printf("\ncreando otro proceso: ");
                    crearprocesos(line);
                }
            }
        }
    }
}