#include <stdio.h>
#include <stdlib.h>

FILE *abreArquivo(char *nameFile);
void imprimeDadosArquivo(FILE *pont_arq);
void *fechaAquivo(FILE *pont_arq);
char **armazenaDadosArquivos(FILE *pont_arq);
char **alocaMemoriaMatriz();
void limpaMemoriaMatriz(char **Matriz);

main(void){
    FILE *arq;
    char **matrizDados;
    arq = abreArquivo("Tabela de Custo.txt");
    // imprimeDadosArquivo(arq);
    matrizDados = armazenaDadosArquivos(arq);
    fechaAquivo(arq);
    limpaMemoriaMatriz(matrizDados);
}

char **armazenaDadosArquivos(FILE *pont_arq){
    char **matrizDadosArq = alocaMemoriaMatriz();
    int i=0, j=0;
    char c;
    do{ 
        c = fgetc(pont_arq);
        if (c != ' '){
            matrizDadosArq[i][j] = c;
            i++;
            if(i == 10){
                i=0;
                j++;
            }
        }
    }while(c != EOF);

}

char **alocaMemoriaMatriz(){
    char **Matriz;

    Matriz = (char**) malloc(sizeof(char)*10);

    for(int i=0;i < 10; i++){
        Matriz[i] = (char*) malloc(sizeof(char)*10); 
    }

    return Matriz;
}

void limpaMemoriaMatriz(char **Matriz){
    for(int i=0;i<10;i++){
        free(Matriz[i]);
    }
    free(Matriz);
}
void imprimeDadosArquivo(FILE *pont_arq){
    char c;
    do{ 
        c = fgetc(pont_arq);

        printf("%c",c);
    }while(c != EOF);
}

FILE *abreArquivo(char *nameFile){
    FILE *pont_arq;

    pont_arq = fopen(nameFile,"r");
    if (pont_arq != NULL){
        return pont_arq;
    }else{
        printf("Falha na abertura do arquivo");
        exit(1);
    }
}

void *fechaAquivo(FILE *pont_arq){
    fclose(pont_arq);
}