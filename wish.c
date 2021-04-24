#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/time.h>

char error_message[30] = "An error has occurred\n";
char **mypath;

char *concatenar(char *str1, char *str2)
{
    char sum[strlen(str1) + strlen(str2) + 1];
    strcpy(sum, str1);
    return strcat(sum, str2);
}

int limpiarSaltoln(char *line)
{
    char *p = line;
    if (*line == '\n' || *line == 13)
    {
        return 0; //no limpia, linea vacía
    }
    while (*p != '\0')
    {
        if (*p == '\n' || *p == 13)
        {
            *p = '\0';
            break;
        }
        p++; //limpia
    }
    return 1;
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

void pillarComandos(char *line)
{
    int numordenes = 0;
    char *orden = strsep(&line, "&");
    while (orden != NULL)
    {
        if (*orden != '\0')
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
            char *arg = strtok(orden, " ");
            while (arg != NULL)
            {
                numargs++;
                printf("        <%d-%ld-%s-%p>", numargs, strlen(arg), arg, &arg);
                printf("\n");

                arg = strtok(NULL, " ");
            }
            if (numargs != 0)
            {
                //################################# extraccion de argumentos
                char *argumentos[numargs + 1];
                argumentos[numargs] = NULL;
                numargs = 0;
                arg = strtok(ordencopy, " ");
                while (arg != NULL)
                {
                    argumentos[numargs] = arg;
                    printf("        ¬¬%d-%ld-%s-%p¬¬", numargs, strlen(argumentos[numargs]), argumentos[numargs], &argumentos[numargs]);
                    printf("\n");
                    numargs++;

                    arg = strtok(NULL, " ");
                }

                //################################ comparando comandos BUILD IN
                if (strcmp(argumentos[0], "exit") == 0) //----   EXIT!!
                {
                    liberarmd();
                    exit(0);
                }
                else if (strcmp(argumentos[0], "path") == 0) //----     PATH!!
                {
                    liberarmd();
                    mypath = NULL;

                    char **tmp_ptr = realloc(mypath, sizeof(char *) * numargs);
                    if (tmp_ptr == NULL)
                    {
                        printf("fallo en realocacion\n");
                        write(STDERR_FILENO, error_message, strlen(error_message));
                        liberarmd();
                        exit(1);
                    }
                    mypath = tmp_ptr;
                    for (int i = 1; i < numargs; i++)
                    {
                        mypath[i - 1] = malloc(sizeof(char) * (strlen(argumentos[i]) + 1));
                        strcpy(mypath[i - 1], argumentos[i]);
                    }
                    mypath[numargs - 1] = NULL;

                    int pos = 0;
                    while (mypath[pos] != NULL)
                    {
                        printf("path---->   %d-%ld-%s-%p", pos, strlen(mypath[pos]), mypath[pos], &mypath[pos]);
                        printf("\n");
                        pos++;
                    }
                }
                else if (strcmp(argumentos[0], "cd") == 0) //----    CD!!
                {
                    if (2 != numargs || -1 == chdir(argumentos[1]))
                    {
                        write(STDERR_FILENO, error_message, strlen(error_message));
                    }
                }
                else
                {
                    //################################ creacion del proceso
                    int proc = fork();
                    printf("padre %d creando a %d\n", getpid(), proc);
                    if (proc < 0)
                    {
                        printf("fallo al crear proceso hijo\n");
                        write(STDERR_FILENO, error_message, strlen(error_message));
                        //liberarmd();
                        //exit(1);
                    }
                    else if (proc == 0)
                    {
                        int pos = 0;
                        while (mypath[pos] != NULL)
                        {
                            char *argcompleto = concatenar(mypath[pos], argumentos[0]);
                            if (access(argcompleto, X_OK) == 0)
                            {
                                execv(argcompleto, argumentos);
                                printf("fallo al transformar proceso hijo\n");
                                write(STDERR_FILENO, error_message, strlen(error_message));
                                //liberarmd();
                                exit(1);
                            }
                            pos++;
                        }
                        printf("ese comando lo tendras que buscar en pasto por alla\n");
                        write(STDERR_FILENO, error_message, strlen(error_message));
                        //liberarmd();
                        exit(0);
                    }
                    else
                    {
                        wait(NULL);
                    }
                }
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
        orden = strsep(&line, "&");
        if (orden == NULL)
        {
            printf("        orden null\n");
        }
        else
        {
            printf("        ~%s~\n", orden);
        }
    }
}

int main(int argc, char *argv[])
{
    mypath = malloc(sizeof(char *) * 2);
    mypath[0] = malloc(sizeof(char) * 6);
    strcpy(mypath[0], "/bin/");
    mypath[1] = NULL;

    size_t len = 0;
    char line[200];

    if (argc == 1)
    {
        FILE *archivo = stdin;

        while (1)
        {
            printf("%dwish> ", getpid());
            if (getline(&line, &len, archivo) != -1)
            {
                if (1 == limpiarSaltoln(line))
                {
                    printf("     -usted ingresó %s-\n", line);
                    pillarComandos(line);
                }
            }
        }
    }
    else if (argc == 2)
    {
        FILE *archivo = fopen(argv[1], "r");
        if (archivo == NULL)
        {
            printf("error al leer el archivo\n");
            write(STDERR_FILENO, error_message, strlen(error_message));
            exit(1);
        }
        int pos = 0;
        while (fgets(line, 200, archivo) != NULL)
        {
            if (limpiarSaltoln(line) == 1)
            {
                printf("%d   -se ingresó %s-\n", pos, line);
                pillarComandos(line);
                printf("%d   -se ingresó %s-\n", pos, line);
                pos++;
            }
        }
        fclose(archivo);
    }
    else
    {
        write(STDERR_FILENO, error_message, strlen(error_message));
        liberarmd();
        exit(1);
    }
    exit(0);
}