/*
 * Feito por 
 * Erick Eckermann Cardoso - GRR20186075
 *
 * 20/dez/2022
*/

#include <stdio.h>
#include <time.h>
#include <omp.h>
#include <stdlib.h>
#include <string.h>

#define N 6
#define M 6
void print_tabuleiro(int tabuleiro[N][M]){
    int i, j;
    for (i=0; i < N; i++){
        for (j=0; j < M; j++)
            printf("%3d ",tabuleiro[i][j]);
        printf("\n");
    }
}

int jogada_valida(int x, int y, int tabuleiro[N][M]){
    if (x < 0  || x >= N || y < 0 || y >= M)
        return 0;
    if(tabuleiro[x][y] != 0)
        return 0;
    return 1;
}

int proximo_movimento_y(int y, int movimento){
    int valor = 1;
    if( movimento < 5 )
        valor = 2;
    if (movimento % 2 == 0) // se par, eh uma subtracao
        return y - valor;
    else
        return y + valor;
    
}
int proximo_movimento_x(int x, int movimento){
    if (movimento < 3)
        return x + 1;
    else if (movimento < 5)
        return x - 1;
    else if (movimento < 7)
        return x + 2;
    else 
        return x - 2;
}

//Kernel do código ==================PARALELIZADA================
int busca_passeio_cavalo(int tabuleiro[N][M], int x, int y, int jogada){
    int x2, y2, i;

    if (jogada == N*M)
        return 1;

    for (i=1;i<9;i++){
        x2 = proximo_movimento_x(x,i);
        y2 = proximo_movimento_y(y,i);
        if (jogada_valida(x2,y2, tabuleiro)){
            tabuleiro[x2][y2] = jogada+1;
            if(busca_passeio_cavalo(tabuleiro, x2,y2, jogada+1))
                return  1;
            tabuleiro[x2][y2] = 0;
        }
    }

    return 0;
}

void passeio_cavalo(int tabuleiro[N][M], int x, int y, unsigned short *result, clock_t start){
    int x2, y2, i;

    omp_set_dynamic(0);
    #pragma omp parallel for schedule(static,1)
    for (i=1;i<9;i++){
        int tab_aux[N][M];
        memcpy(tab_aux, tabuleiro, (sizeof(int)*N*M));
        x2 = proximo_movimento_x(x,i);
        y2 = proximo_movimento_y(y,i);
        if (jogada_valida(x2,y2, tab_aux)){
            tab_aux[x2][y2] = 2;
            if(busca_passeio_cavalo(tab_aux, x2,y2, 2)){
                memcpy(tabuleiro, tab_aux, (sizeof(int)*N*M));
                *result = 1;
                // print_tabuleiro(tabuleiro);
                clock_t end = clock();
                double cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
                printf("%f paralelo\n",cpu_time_used);
                fprintf(stderr, "%f", cpu_time_used);
                exit(0);
            }
            tab_aux[x2][y2] = 0;
        }
    }
}
//==================================PARALELIZADA================

int main(){
    int i, j;
    int tabuleiro[N][M];
    int x_inicio = N/2, y_inicio = M/2;
    clock_t start, end;     //Variáveis para contar o tempo de execução
    double cpu_time_used;
    start = clock();
    
    //printf("Resolvendo para N=%d e M=%d com implementacao PARALELA\n",N,M);
    //Zera o Tabuleiro
    for (i=0; i < N; i++)
        for (j=0; j < M; j++)
            tabuleiro[i][j] = 0;

    //Seta a posição inicial do cavalo
    tabuleiro[x_inicio][y_inicio] = 1;

    //Chama parte principal do código
    unsigned short result = 0;
    passeio_cavalo(tabuleiro, x_inicio, y_inicio, &result, start);  
    
    /*if(result)
        print_tabuleiro(tabuleiro);
    else
        printf("Nenhuma solução possível\n");*/

    //Calcula o tempo total de execução
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

    printf("%f paralelo\n",cpu_time_used);
    fprintf(stderr, "%f\n", cpu_time_used);
    exit(-1);
}
