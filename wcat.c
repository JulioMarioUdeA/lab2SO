#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    FILE *archivo;
    char line[201];

    for(int i = 1; i < argc; i++){
        archivo = fopen(argv[i], "r");
        if (archivo == NULL){
            printf("wcat: cannot open file\n");
            exit(1);
        }
        while(fgets(line, 201, archivo)!=NULL){
            printf("%s",line);
        }
        fclose(archivo);
    }
    exit(0);
}
