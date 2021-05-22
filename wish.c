#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/time.h>

char error_message[30] = "An error has occurred\n";
char **mypath;

char *concatenar(char *str1, char *str2)//no sirve
{
    char sum[strlen(str1) + strlen(str2) + 1];
    strcpy(sum, str1);
    return strcat(sum, str2);
}

void concatenar1(char *sum, char *str1, char *str2)//pone en sum la concatenacion de str1 y str2 que no son modificados
{
    //char sum[strlen(str1) + strlen(str2) + 1];
    strcpy(sum, str1);
    strcat(sum, str2);
}

int limpiarSaltoln(char *line)//retorna 0 si la linea está vacía, 1 si no lo está; a la vez quita el caracter de salto de line
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

void liberarmd()//libera la memoria dinamica ocupada por la variable mypath
{
    int pos = 0;
    while (mypath[pos] != NULL)
    {
        free(mypath[pos]);
        pos++;
    }
    free(mypath);
}

int lanzarProceso(int redir, char *argcompleto, char *argumentos[], char *pedazo)//crea un proceso hijo y lo transforma en argcompleto; retorna 1 si el comando existe, 0 si no;
{                                                                                //redir es 1 si hay redireccion, 0 si no; pedazo especifica el nombre del archivo en el que es escribirá la redireccion.
    int loencontro = 0;                                                          //argumentos son todos los argumentos de un comando.
    if (access(argcompleto, X_OK) == 0)
    {
        loencontro = 1;
        //##################################################### creacion del proceso
        int proc = fork();
        if (proc < 0)
        {
            write(STDERR_FILENO, error_message, strlen(error_message));
        }
        else if (proc == 0)
        {
            if (redir == 1)
            {
                close(fileno(stdout));
                close(fileno(stderr));
                int out = open(pedazo, O_TRUNC | O_RDWR | O_CREAT | O_APPEND, 0600);
                //int save_out = dup(fileno(stdout));
                if (-1 == dup2(out, fileno(stdout)) || -1 == dup2(out, fileno(stderr)))
                {
                    write(STDERR_FILENO, error_message, strlen(error_message));
                }
            }
            execv(argcompleto, argumentos);
            write(STDERR_FILENO, error_message, strlen(error_message));
            liberarmd();
            exit(1);
        }
    }
    return loencontro;
}

void pillarComandos(char *line)// hace todo el proceso de obtener comandos, mirar redireccion, comandos build-in
{
    //########################################################### solo obteniendo numero de ordenes-comandos
    int pos = 0;
    int numordenes = 0; //ordenes-comandos separados por &
    char linecopy[strlen(line) + 1];
    strcpy(linecopy, line);
    char *plncpy = linecopy;
    char *ordenfull = strsep(&plncpy, "&");
    while (ordenfull != NULL)
    {
        if (*ordenfull != '\0')
        {
            char *ordenvacia = strtok(ordenfull, " ");
            if (ordenvacia != NULL)
            {
                numordenes++;
            }
        }
        ordenfull = strsep(&plncpy, "&");
    }

    //########################################################## obteniendo ordenes-comandos, redireccion, comandos buid-in de aqui en adelante

    ordenfull = strsep(&line, "&");//segmentos separados por &
    while (ordenfull != NULL)
    {
        if (*ordenfull != '\0')
        {
            //################################################## pillando redir y error en redir
            pos = 0;
            int erredir = 0;
            int redir = 0;
            char *pedazos[3];
            pedazos[0] = NULL;
            pedazos[1] = NULL;
            pedazos[2] = NULL;
            char *pedazo = strsep(&ordenfull, ">");//segmentos separados por >
            while (pedazo != NULL)
            {
                if (pos == 2 || *pedazo == '\0')
                {
                    erredir = 1;
                    break;
                }
                pedazos[pos] = pedazo;
                if (pos == 1)
                {
                    char pedazocopia[strlen(pedazo) + 1];
                    strcpy(pedazocopia, pedazo);
                    char *pedazito = strtok(pedazocopia, " ");
                    int cont = 0;
                    while (pedazito != NULL)
                    {
                        cont++;
                        pedazito = strtok(NULL, " ");
                    }
                    if (cont != 1)
                    {
                        erredir = 1;
                        break;
                    }
                    else
                    {
                        pedazos[pos] = strtok(pedazo, " ");
                    }
                }
                pos++;
                pedazo = strsep(&ordenfull, ">");
            }

            if (erredir == 0 && pedazos[0] != NULL) //########################## Todo bien con la redireccion
            {
                if (pedazos[1] != NULL && *pedazos[1] != '\0')
                {
                    redir = 1;
                }
                //########################################## obtener ordenes-comandos
                char *orden = pedazos[0];
                char ordencopy[strlen(orden) + 1];
                strcpy(ordencopy, orden);
                //################################ obteniendo numero de argumentos
                int numargs = 0;
                char *arg = strtok(orden, " ");
                while (arg != NULL)
                {
                    numargs++;
                    arg = strtok(NULL, " ");
                }
                if (numargs != 0)
                {
                    //################################# extraccion de argumentos
                    char *argumentos[numargs + 1]; //char **argumentos = malloc((numargs + 1) * sizeof(char *));
                    argumentos[numargs] = NULL;
                    numargs = 0;
                    arg = strtok(ordencopy, " ");
                    while (arg != NULL)
                    {
                        argumentos[numargs] = arg;
                        numargs++;
                        arg = strtok(NULL, " ");
                    }

                    //################################ comparando comandos BUILD IN
                    
                    if (strcmp(argumentos[0], "exit") == 0) //----------------------------------EXIT!!
                    {
                        if (numargs != 1)
                        {
                            write(STDERR_FILENO, error_message, strlen(error_message));
                        }
                        else
                        {
                            liberarmd();
                            exit(0);
                        }
                    }
                    else if (strcmp(argumentos[0], "path") == 0) //----------------------------PATH!!
                    {
                        liberarmd();
                        mypath = NULL;

                        char **tmp_ptr = realloc(mypath, sizeof(char *) * numargs);
                        if (tmp_ptr == NULL)
                        {
                            write(STDERR_FILENO, error_message, strlen(error_message));
                            liberarmd();
                            exit(1);
                        }
                        mypath = tmp_ptr;
                        for (int i = 1; i < numargs; i++)
                        {
                            char sum[strlen(argumentos[i]) + 2];
                            concatenar1(sum, argumentos[i], "/");
                            mypath[i - 1] = malloc(sizeof(char) * (strlen(sum) + 1));
                            strcpy(mypath[i - 1], sum);
                        }
                        mypath[numargs - 1] = NULL;
                    }
                    else if (strcmp(argumentos[0], "cd") == 0) //-----------------------------------CD!!
                    {
                        if (2 != numargs || -1 == chdir(argumentos[1]))
                        {
                            write(STDERR_FILENO, error_message, strlen(error_message));
                        }
                    }
                    else //------------------------------------------------------------------------ELSE!!
                    {
                        pos = 0;
                        int loencontro = lanzarProceso(redir, argumentos[0], argumentos, pedazos[1]); // busca el comando en local
                        if (loencontro == 0)
                        {
                            while (mypath[pos] != NULL) // busca el comando en los paths
                            {
                                char argcompleto[strlen(mypath[pos]) + strlen(argumentos[0]) + 1];
                                concatenar1(argcompleto, mypath[pos], argumentos[0]);
                                loencontro = lanzarProceso(redir, argcompleto, argumentos, pedazos[1]);
                                if (loencontro == 1)
                                {
                                    break;
                                }
                                pos++;
                            }
                        }
                        if (loencontro == 0)
                        {
                            write(STDERR_FILENO, error_message, strlen(error_message));
                        }
                    }
                }
            }
            else //####################################### Todo mal con la redireccion
            {
                write(STDERR_FILENO, error_message, strlen(error_message));
            }
        }

        //################################## actualizacion
        ordenfull = strsep(&line, "&");
    }
    for (int i = 0; i < numordenes; i++)
    {
        wait(NULL);
    }
}

int main(int argc, char *argv[])
{
    mypath = malloc(sizeof(char *) * 2);
    mypath[0] = malloc(sizeof(char) * 6);
    strcpy(mypath[0], "/bin/");
    mypath[1] = NULL;

    size_t len = 0;
    char *line = NULL;

    if (argc == 1)//modo interactivo
    {
        FILE *archivo = stdin;

        while (1)
        {
            printf("wish> ");
            if (getline(&line, &len, archivo) != -1)
            {
                if (1 == limpiarSaltoln(line))
                {
                    pillarComandos(line);
                }
            }
        }
    }
    else if (argc == 2)//modo batch
    {
        FILE *archivo = fopen(argv[1], "r");
        if (archivo == NULL)
        {
            write(STDERR_FILENO, error_message, strlen(error_message));
            exit(1);
        }
        while (getline(&line, &len, archivo) != -1)
        {
            if (limpiarSaltoln(line) == 1)
            {
                pillarComandos(line);
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