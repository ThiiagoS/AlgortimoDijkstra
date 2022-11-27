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
typedef struct adjacencia{
    int vertice;
    char pesoAresta;
    struct adjacencia *proxElementListaAdj;
}ADJACENCIA;

typedef struct vertice{
    ADJACENCIA *cabeca; 
}VERTICE;

typedef struct grafo{
    int numVertices;
    int numArestas;
    VERTICE *ArranjoVertices;
}GRAFO;

FILE *abreArquivo(char *nameFile);
void imprimeDadosArquivo(FILE *pont_arq);
void *fechaAquivo(FILE *pont_arq);
void calculaNumeroColunasLinhas(FILE *pont_arq, int *linhas, int *colunas);

char **armazenaDadosArquivos(FILE *pont_arq, int Numlinhas, int Numcolunas);
char **alocaMemoriaMatriz(int Numlinhas, int Numcolunas);
void limpaMemoriaMatriz(char **Matriz);


GRAFO *criaGrafo(int Numlinhas);
ADJACENCIA *criaAdjacencia(int verticeDest, char pesoAresta);
bool criaAresta(GRAFO *graf, int verticeInicial, int verticeFinal, char peso);
void imprimeGrafo(GRAFO *graf);

void realocaElementosNoGrafo(GRAFO *grafo,char** matrizDados,int Numlinhas, int Numcolunas);

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

    GRAFO* grafo = criaGrafo(Numlinhas);

    realocaElementosNoGrafo(grafo,matrizDados,Numlinhas, Numcolunas);

    imprimeGrafo(grafo);

    limpaMemoriaMatriz(matrizDados);
}

void realocaElementosNoGrafo(GRAFO *grafo,char** matrizDados,int Numlinhas, int Numcolunas){
    for(int i=0;i<Numlinhas;i++){
        for(int j=0;j<Numcolunas;j++){
            if(matrizDados[j][i] != '0' && matrizDados[j][i] != '\n'){
                criaAresta(grafo,i,j,matrizDados[j][i]);
            }
        }
    }
}
GRAFO *criaGrafo(int Numlinhas){
    int i;

    GRAFO *grafo = (GRAFO*)malloc(sizeof(GRAFO));
    grafo->numVertices = Numlinhas;
    grafo->numArestas = 0;
    grafo->ArranjoVertices = (VERTICE *)malloc(sizeof(VERTICE)*Numlinhas);
    for(i=0;i<Numlinhas;i++){
        grafo->ArranjoVertices[i].cabeca = NULL;
    }
    return(grafo);
}

ADJACENCIA *criaAdjacencia(int verticeDest, char pesoAresta){
    ADJACENCIA *adj = (ADJACENCIA *)malloc(sizeof(ADJACENCIA));
    adj->vertice = verticeDest;
    adj->pesoAresta = pesoAresta;
    adj->proxElementListaAdj = NULL;

    return adj;
}

bool criaAresta(GRAFO *graf, int verticeInicial, int verticeFinal, char peso){
    
    ADJACENCIA *novo = criaAdjacencia(verticeFinal,peso);
    novo->proxElementListaAdj = graf->ArranjoVertices[verticeInicial].cabeca;
    graf->ArranjoVertices[verticeInicial].cabeca = novo;
    graf->numArestas++;
    return true;
}

void imprimeGrafo(GRAFO *graf){
    int i;
    for(i=0;i < graf->numVertices; i++){
        printf("v%d: ",i+1);
        ADJACENCIA *adj = graf->ArranjoVertices[i].cabeca;
        
        while(adj != NULL){
            printf("v%d(%c)",adj->vertice+1,adj->pesoAresta);
            adj = adj->proxElementListaAdj;
        }
        printf("\n");
    }
}

char **armazenaDadosArquivos(FILE *pont_arq, int Numlinhas, int Numcolunas){
    char **matrizDadosArq = alocaMemoriaMatriz(Numlinhas,Numcolunas);
    int i=0, j=0;
    char c;
    do{ 
        c = fgetc(pont_arq);
        if(c != EOF){
            if (c != ' ' && c != '\n'){
                matrizDadosArq[i][j] = c;
                i++;
                if(i == 10){
                    i=0;
                    j++;
                }
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