#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    if (argc == 1)
    {
        printf("wzip: file1 [file2 ...]\n");
        exit(1);
    }
    else
    {

        int cont = 0; //contador
        char cant;    //caracter anterior
        char c;
        int paso = 0; //1 cuando pasa a otro archivo,
                      //0 cuando pasa entre letras

        for (int i = 1; i < argc; i++)
        {
            FILE *archivo;
            archivo = fopen(argv[i], "r");
            if (archivo == NULL)
            {
                printf("wzip: cannot open file\n");
                exit(1);
            }

            if (paso == 0)
            {
                cant = getc(archivo);
            }
            while (cant != EOF)
            {
                c = getc(archivo);
                if (c == EOF)
                {
                    paso = 1;
                    if (i != argc - 1) //si no es el ultimo archivo
                    {
                        break;
                    }
                }
                cont++;
                if (cant != c) //cambio de caracter
                {
                    fwrite(&cont, 4, 1, stdout);
                    fwrite(&cant, 1, 1, stdout);
                    cont = 0;
                    cant = c;
                }
                paso = 0;
            }

            fclose(archivo);
        }
    }
    exit(0);
}