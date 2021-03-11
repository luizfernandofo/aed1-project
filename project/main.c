#include<stdio.h>
#include <stdlib.h>


#ifdef _WIN32 // Includes both 32 bit and 64 bit
    #define CLS "cls"
    	    
#elif __linux__
    #define CLS "clear"

#endif


int main(int argc, char const *argv[])
{
    FILE *f=NULL;

    f = fopen("teste.txt", "r");
    if(f == NULL)
    {
        f = fopen("teste.txt", "w");
        if(f == NULL)
        {
            printf("Falha ao abrir arquivo!\n");

            exit(EXIT_FAILURE);
        }
        printf("Arquivo criado.\n");
    }

    printf("Arquivo aberto.\n");

    fclose(f);

    return 0;
}
