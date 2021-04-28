#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    if (argc == 1)
    {
        printf("wunzip: file1 [file2 ...]\n");
        exit(1);
    }
    else
    {
        int cont;
        char c;

        for (int i = 1; i < argc; i++)
        {
            FILE *archivo;
            archivo = fopen(argv[i], "r");
            if (archivo == NULL)
            {
                printf("wunzip: cannot open file\n");
                exit(1);
            }

            while (fread(&cont, 4, 1, archivo) != 0)
            {
                fread(&c, 1, 1, archivo);
                for(int i = 0; i < cont; i++)
                {
                    putc(c,stdout);
                }
            }
            fclose(archivo);
        }
    }
    exit(0);
}