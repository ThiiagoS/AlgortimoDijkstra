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


typedef struct No{
    int info;
    No *proxDaFila;
}NO;

typedef struct fila{
    NO* Inicio;
}FILA;

typedef struct filass{
    FILA *cabecaa;
}FILASS;

typedef struct lista{
    FILASS *cabeca;
}LISTA;

FILE *abreArquivo(char *nameFile);
void imprimeDadosArquivo(FILE *pont_arq);
void *fechaAquivo(FILE *pont_arq);
void calculaNumeroColunasLinhas(FILE *pont_arq, int *linhas, int *colunas);

int **armazenaDadosArquivos(FILE *pont_arq, int Numlinhas, int Numcolunas);
int **alocaMemoriaMatriz(int Numlinhas, int Numcolunas);
void limpaMemoriaMatriz(int **Matriz);


GRAFO *criaGrafo(int Numlinhas);
ADJACENCIA *criaAdjacencia(int verticeDest, char pesoAresta);
bool criaAresta(GRAFO *graf, int verticeInicial, int verticeFinal, char peso);
void imprimeGrafo(GRAFO *graf);

void realocaElementosNoGrafo(GRAFO *grafo,int** matrizDados,int Numlinhas, int Numcolunas);
void encontraMenorRota(GRAFO* grafo);

void AdicionaNodo(FILA *fila, int info);
FILA *criaFila();
LISTA *criaLista(int numeroDeFilas);

void imprimeFilaElementos(FILA *fila);
main(void){
    FILE *arq;
    int **matrizDados;
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

    encontraMenorRota(grafo);

    limpaMemoriaMatriz(matrizDados);
    
    printf("\n\n");
    system("pause");
}

void encontraMenorRota(GRAFO* grafo){

    LISTA* ListaDeFilas = criaLista(grafo->numArestas);
    int contadorFilas = 0;

    int numeroDaFilaComMenorPeso = 0;
    
    int menorPesoAteagr = 0;
    bool primeiroPeso = true;

    int VerticeInical;
    int VerticeFinal;

    printf("Digite o vertice inicial: ");
    // scanf("%d",&VerticeInical);
    VerticeInical = 1;
    VerticeInical--;

    printf("Digite o vertice final : ");
    // scanf("%d",&VerticeFinal);
    VerticeFinal= 5;
    VerticeFinal--;
    
    if(grafo->ArranjoVertices[VerticeInical].cabeca != NULL){



        ADJACENCIA *adj = grafo->ArranjoVertices[VerticeInical].cabeca;
        while(adj != NULL){
        
        FILA* filaDeElementos = criaFila();
        ListaDeFilas->cabeca[contadorFilas].cabecaa = filaDeElementos;
        AdicionaNodo(filaDeElementos,VerticeInical);

        ADJACENCIA *adjTemp = adj;

        int somaPesos = 0;
        int novoVertice = adjTemp->vertice; //Vertice que ele esta indo

            while(adjTemp != NULL && novoVertice != VerticeFinal){

                novoVertice = adjTemp->vertice;
                AdicionaNodo(filaDeElementos,novoVertice);
                somaPesos = somaPesos + (adjTemp->pesoAresta -'0'); // peso aresta, (adjTemp->pesoAresta -'0')=  os valores ASCII dos caracteres são subtraídos um do outro
                adjTemp = grafo->ArranjoVertices[novoVertice].cabeca;
            }

            if(novoVertice ==  VerticeFinal){
                if(menorPesoAteagr > somaPesos || primeiroPeso){
                    primeiroPeso = false;
                    menorPesoAteagr = somaPesos;
                    numeroDaFilaComMenorPeso = contadorFilas;
                }
            }
            contadorFilas++;
            adj = adj->proxElementListaAdj;
        }

        printf("\n\nPeso = %d\n\n",menorPesoAteagr);
        imprimeFilaElementos(ListaDeFilas->cabeca[numeroDaFilaComMenorPeso].cabecaa);
    }else{
        printf("Vertice inicial nao possue arestas na matriz de custo.\n");
    }
}

FILA *criaFila(){
    FILA *filaElementos = (FILA*)malloc(sizeof(FILA)); 
    if(filaElementos != NULL){
        filaElementos->Inicio = NULL;
        return filaElementos;
    }else{
        printf("Memoria insuficiente");
        exit(1);
    }
}

LISTA *criaLista(int numeroDeFilas){

    LISTA *listaDeFilas = (LISTA*)malloc(sizeof(LISTA)); 

    if(listaDeFilas != NULL){
        
    listaDeFilas->cabeca = (FILASS*)malloc(sizeof(FILASS)*numeroDeFilas);
    for(int i=0;i<numeroDeFilas;i++){
        listaDeFilas->cabeca[i].cabecaa = NULL;
    }

    }else{
        printf("Memoria insuficiente");
        exit(1);
    }
    return listaDeFilas;
}

void imprimeFilaElementos(FILA *fila){
    NO *NoAux = fila->Inicio;
    printf("Caminho encontrado: ");
    while(NoAux !=NULL){
        if(NoAux->proxDaFila == NULL){
            printf("%d ",NoAux->info+1);
        }else{
            printf("%d -> ",NoAux->info+1);   
        }
        NoAux = NoAux->proxDaFila;
    }
}
void AdicionaNodo(FILA *fila, int info){
    NO* NovoElemento = (NO*)malloc(sizeof(NO));
    NO* NoAux = fila->Inicio;
    NO* NoAnterior;
    if(NovoElemento != NULL){

        if(NoAux == NULL){
            fila->Inicio = NovoElemento;
        }else{
            while(NoAux !=NULL){
                NoAnterior = NoAux;
                NoAux = NoAux->proxDaFila;
            }
            NoAnterior->proxDaFila = NovoElemento;
        }
        NovoElemento->info = info;
        NovoElemento->proxDaFila = NULL;

    }else{
        printf("Memoria insuficiente");
        exit(1);
    }
}
void realocaElementosNoGrafo(GRAFO *grafo,int** matrizDados,int Numlinhas, int Numcolunas){
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
        printf("Vertice %d : ",i+1);
        ADJACENCIA *adj = graf->ArranjoVertices[i].cabeca;
        
        while(adj != NULL){
            printf("Vertice %d (Peso = %c) ",adj->vertice+1,adj->pesoAresta);
            adj = adj->proxElementListaAdj;
        }
        printf("\n");
    }
}

int **armazenaDadosArquivos(FILE *pont_arq, int Numlinhas, int Numcolunas){
    int **matrizDadosArq = alocaMemoriaMatriz(Numlinhas,Numcolunas);
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


int **alocaMemoriaMatriz(int Numlinhas, int Numcolunas){
    int **Matriz;

    Matriz = (int**) malloc(sizeof(int)*Numlinhas);

    if(Matriz != NULL){

        for(int i=0;i<Numlinhas; i++){

            Matriz[i] = (int*) malloc(sizeof(int)*Numcolunas); 

            if(Matriz[i] == NULL){
                printf("Memoeria insuficiente!!!");
                exit(1);
            }

        }
    }else{
        printf("Memoeria insuficiente!!!");
        exit(1);
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

void limpaMemoriaMatriz(int **Matriz){
    for(int i=0;i<10;i++){
        free(Matriz[i]);
    }
    free(Matriz);
}