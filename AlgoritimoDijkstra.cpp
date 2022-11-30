#include <stdio.h>
#include <stdlib.h>

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
    int peso;
    bool chegouNoFinal;
}FILA;

typedef struct controlaFila{
    FILA *header;
}CONTROLAFILA;

typedef struct lista{
    CONTROLAFILA *cabeca;
}LISTA;

//Nomes: Thiago F. Santos,  Eduarda Cavalheiro, Guilherme Knak

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
void imprimeFilaElementos(FILA *fila);

void AdicionaNodo(FILA *fila, int info);
FILA *criaFila();
LISTA *criaLista(int numeroDeFilas);

int iniciaCaminho(GRAFO* grafo,ADJACENCIA *adj,LISTA *ListaDeFilas,int contadorFilas, int VerticeInical, int VerticeFinal,FILA *HistoricoFilaAnt);
void insereHistoricoDeElementoFila(FILA *FilaAnterior, FILA* NovaFila);
void verificaMenorTrajeto(LISTA* ListaDeFilas, int contadorFilas);

void limpaMemoriaListaFila(LISTA* ListaDeFilas,int contadorFilas);
void limpaMemoriaGrafo(GRAFO *grafo, int Numlinhas);

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
    limpaMemoriaGrafo(grafo,Numlinhas);

    printf("\n\n");
    system("pause");
}

void encontraMenorRota(GRAFO* grafo){

    LISTA* ListaDeFilas = criaLista(grafo->numArestas);
    int contadorFilas = 0;

    int numeroDaFilaComMenorPeso = 0;
    
    bool encontrouVerticeFinal = false;
    
    int menorPesoAteagr = 0;
    bool primeiroPeso = true;

    int VerticeInical;
    int VerticeFinal;

    printf("Digite o vertice inicial: ");
    scanf("%d",&VerticeInical);
    VerticeInical--;

    printf("Digite o vertice final : ");
    scanf("%d",&VerticeFinal);
    VerticeFinal--;
    
    if(grafo->ArranjoVertices[VerticeInical].cabeca != NULL){

        ADJACENCIA *adj = grafo->ArranjoVertices[VerticeInical].cabeca;
        FILA* filaDeElementos = criaFila();
        AdicionaNodo(filaDeElementos,VerticeInical);

        contadorFilas = iniciaCaminho(grafo,adj,ListaDeFilas,contadorFilas,VerticeInical,VerticeFinal,filaDeElementos);

        verificaMenorTrajeto(ListaDeFilas,contadorFilas);

        limpaMemoriaListaFila(ListaDeFilas,contadorFilas);

    }else{
        printf("Vertice inicial nao possue arestas na matriz de custo.\n");
        system("pause");
        exit(1);
    }
}

void verificaMenorTrajeto(LISTA* ListaDeFilas, int contadorFilas){
    FILA* FilaAux;

    bool primeiroPeso = true;
    int MenorPesoAtual;
    int numeroDaFilaComMenorPeso;

    for(int i=0;i<=contadorFilas;i++){
        FilaAux = ListaDeFilas->cabeca[i].header;

        if(FilaAux->chegouNoFinal == true){
            if(MenorPesoAtual> FilaAux->peso || primeiroPeso){
                primeiroPeso = false;
                MenorPesoAtual = FilaAux->peso;
                numeroDaFilaComMenorPeso = i;
            }
        }
    }

    if(!primeiroPeso){
        printf("\nPeso = %d\n\n",MenorPesoAtual);
        imprimeFilaElementos(ListaDeFilas->cabeca[numeroDaFilaComMenorPeso].header);
    }else{
        printf("Nao existe um caminho para estes vertices inseridos.\n");
        system("pause");
        exit(1);
    }
}
int iniciaCaminho(GRAFO* grafo,ADJACENCIA *adj,LISTA *ListaDeFilas,int contadorFilas, int VerticeInical, int VerticeFinal, FILA *HistoricoFilaAnt){

    if(adj != NULL){

        FILA* filaDeElementos = criaFila();
        ListaDeFilas->cabeca[contadorFilas].header = filaDeElementos;
        
        insereHistoricoDeElementoFila(HistoricoFilaAnt,filaDeElementos);

        ADJACENCIA *adjTemp = adj;
        int somaPesos = 0;
        int novoVertice = adjTemp->vertice; //Vertice que ele esta indo

        while(adjTemp != NULL && novoVertice != VerticeFinal){
            
            if(adjTemp->proxElementListaAdj != NULL){
                contadorFilas = iniciaCaminho(grafo,adjTemp->proxElementListaAdj,ListaDeFilas,contadorFilas=contadorFilas+1,VerticeInical,VerticeFinal,filaDeElementos);
            }
            novoVertice = adjTemp->vertice;
            AdicionaNodo(filaDeElementos,novoVertice);
            filaDeElementos->peso = filaDeElementos->peso + (adjTemp->pesoAresta -'0');// (adjTemp->pesoAresta -'0') =  os valores ASCII dos caracteres são subtraídos um do outro
            adjTemp = grafo->ArranjoVertices[novoVertice].cabeca;
        }

        if(novoVertice ==  VerticeFinal){
            filaDeElementos->chegouNoFinal = true;
        }   
    }
    return contadorFilas;
}

void insereHistoricoDeElementoFila(FILA *FilaAnterior, FILA* NovaFila){
    NO *NoAux = FilaAnterior->Inicio;
    NovaFila->peso = FilaAnterior->peso;

    while(NoAux != NULL){
        AdicionaNodo(NovaFila,NoAux->info);
        NoAux = NoAux->proxDaFila;
    }
}

FILA *criaFila(){
    FILA *filaElementos = (FILA*)malloc(sizeof(FILA)); 
    if(filaElementos != NULL){
        filaElementos->Inicio = NULL;
        filaElementos->peso = 0;
        filaElementos->chegouNoFinal = false;
        return filaElementos;
    }else{
        printf("Memoria insuficiente\n");
        system("pause");
        exit(1);
    }
}

LISTA *criaLista(int numeroDeFilas){

    LISTA *listaDeFilas = (LISTA*)malloc(sizeof(LISTA)); 

    if(listaDeFilas != NULL){
        
        listaDeFilas->cabeca = (CONTROLAFILA*)malloc(sizeof(CONTROLAFILA)*numeroDeFilas);
        for(int i=0;i<numeroDeFilas;i++){
            listaDeFilas->cabeca[i].header = NULL;
        }
    
    }else{
        printf("Memoria insuficiente\n");
        system("pause");
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
        printf("Memoria insuficiente\n");
        system("pause");
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
        printf("Falha na abertura do arquivo\n");
        system("pause");
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
                printf("Memoeria insuficiente!!!\n");
                system("pause");
                exit(1);
            }

        }
    }else{
        printf("Memoeria insuficiente!!!\n");
        system("pause");
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

void limpaMemoriaGrafo(GRAFO *grafo, int Numlinhas){
    for(int i=0;i<Numlinhas;i++){
        free(grafo->ArranjoVertices[i].cabeca);
    }
    free(grafo->ArranjoVertices);
    free(grafo);
}

void limpaMemoriaListaFila(LISTA* ListaDeFilas,int contadorFilas){
    FILA* FilaAux;

    for(int i=0;i<=contadorFilas;i++){
        FilaAux = ListaDeFilas->cabeca[i].header;
        free(FilaAux);
    }
    free(ListaDeFilas);
}
