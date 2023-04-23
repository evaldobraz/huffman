#include <stdio.h>
#include <stdlib.h>

typedef struct
{
    void *frequency;
    void *
}huffman_tree;


int main()
{
    FILE *open_file;
    char file_name[256]; //variável que irá armazenar o nome do arquivo a ser aberto
    
    printf("DIGITE O NOME DO ARQUIVO QUE DESEJA ABRIR: "); 
    scanf("%s", file_name); //lê o nome do arquivo que deve estar na pasta /output o nome do arquivo deve ser digitado com a extensão
    open_file = fopen(file_name, "r+b"); //abre o arquivo para leitura binária

    if(open_file == NULL)
    {
        printf("[ERRO] NAO FOI POSSiVEL ABRIR O ARQUIVO\n"); 
        exit(1);
    } 

    int frequency[256] = {0};
    int byte;

    while ((byte = fgetc(open_file)) != EOF) {
        frequency[byte]++;
    }

    for(int i=0; i<256; i++)
    {
        printf("Byte 0x%02X = %d\n", i, frequency[i]);
    }

    fclose(open_file);
}