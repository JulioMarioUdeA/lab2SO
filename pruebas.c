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

int main(int argc, char *argv[])
{
    char *pal = "hola";
    char *pol = "paola";
    char *sum = concatenar(pal, pol);
    printf("%p%s", &sum,sum);
}
