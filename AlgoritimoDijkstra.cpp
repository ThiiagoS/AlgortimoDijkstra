#include <stdio.h>
#include <stdlib.h>


// typedef struct{
//     Vertice* VerticeInicial; 
// }Grafo;

// typedef struct{
//     Aresta* Aresta;
//     Lista* proximaAresta;
// }Lista;

// typedef struct{
//     Lista* ElementosFilhos;
// }Vertice;

// typedef struct{
//     float peso;
//     Vertice* VerticeInicial;
//     Vertice* VerticeFinal;
// }Aresta;

typedef struct vertice;


typedef struct aresta
{
    float peso;
    vertice* VerticeInterligado;
    aresta* proximaAresta;
}Aresta;

typedef struct vertice
{
    char* NomeVertice = " ";
    Aresta* ListaDeArestas;
}Vertice;

FILE *abreArquivo(char *nameFile);
void imprimeDadosArquivo(FILE *pont_arq);
void *fechaAquivo(FILE *pont_arq);
void calculaNumeroColunasLinhas(FILE *pont_arq, int *linhas, int *colunas);

char **armazenaDadosArquivos(FILE *pont_arq, int Numlinhas, int Numcolunas);
char **alocaMemoriaMatriz(int Numlinhas, int Numcolunas);
void limpaMemoriaMatriz(char **Matriz);

void calculaMenorCaminho(char **matrizDados,int Numlinhas, int Numcolunas);

main(void){
    FILE *arq;
    char **matrizDados;
    int Numlinhas = 0;
    int Numcolunas = 0;

    arq = abreArquivo("Tabela de Custo.txt");
    calculaNumeroColunasLinhas(arq, &Numlinhas, &Numcolunas);
    fechaAquivo(arq);

    arq = abreArquivo("Tabela de Custo.txt");
    matrizDados = armazenaDadosArquivos(arq, Numlinhas, Numcolunas);
    fechaAquivo(arq);

    limpaMemoriaMatriz(matrizDados);
}


Vertice* CriaNovoElemento(){
    Vertice * NovoVertice;

    NovoVertice = (Vertice*)malloc(sizeof(Vertice));
    if(NovoVertice != NULL){
        return NovoVertice;
    }else{
        printf("Memoria insuficiente para alocar.");
        exit(1);
    }
}

Aresta* CriaNovaAresta(){
    Aresta * NovoAresta;

    NovoAresta = (Aresta*)malloc(sizeof(Aresta));
    if(NovoAresta != NULL){
        return NovoAresta;
    }else{
        printf("Memoria insuficiente para alocar.");
        exit(1);
    }
}
char **armazenaDadosArquivos(FILE *pont_arq, int Numlinhas, int Numcolunas){
    char **matrizDadosArq = alocaMemoriaMatriz(Numlinhas,Numcolunas);
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

    return matrizDadosArq;
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

void calculaNumeroColunasLinhas(FILE *pont_arq, int *linhas, int *colunas){
    char c;

    bool controlaContagemColunas = true;
    bool verificarQuebraDeLinhas = false;
    int contadorCol = 0;
    do{
        c = fgetc(pont_arq);

        if(c != ' ' && controlaContagemColunas){

            if(c == '\n'){
                controlaContagemColunas = false;
            }else{
                *colunas = *colunas + 1;
            }
        }

        if(c == '\n'){
            *linhas = *linhas + 1;
            contadorCol = 0;
        }else{
            contadorCol++;
        }

    }while(c != EOF);

    if(contadorCol>0){
        *linhas = *linhas + 1;
    }
}

void *fechaAquivo(FILE *pont_arq){
    fclose(pont_arq);
}


char **alocaMemoriaMatriz(int Numlinhas, int Numcolunas){
    char **Matriz;

    Matriz = (char**) malloc(sizeof(char)*Numlinhas);

    for(int i=0;i<Numlinhas; i++){
        Matriz[i] = (char*) malloc(sizeof(char)*Numcolunas); 
    }

    return Matriz;
}

void imprimeDadosArquivo(FILE *pont_arq){
    char c;
    do{ 
        c = fgetc(pont_arq);

        printf("%c",c);
    }while(c != EOF);
}

void limpaMemoriaMatriz(char **Matriz){
    for(int i=0;i<10;i++){
        free(Matriz[i]);
    }
    free(Matriz);
}