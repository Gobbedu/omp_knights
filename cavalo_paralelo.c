#include <stdio.h>
#include <time.h>
#include <omp.h>


#define NUM_THREADS 6
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

/* Possiveis movimentos
#1
x2 = x + 1;
y2 = y + 2;

#2
x2 = x + 1;
y2 = y - 2;

#3
x2 = x - 1;
y2 = y + 2;

#4
x2 = x - 1;
y2 = y - 2;

#5
x2 = x + 2;
y2 = y + 1;

#6
x2 = x + 2;
y2 = y - 1;

#7
x2 = x - 2;
y2 = y + 1;

#8
x2 = x - 2;
y2 = y - 1;
*/
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

int _passeio_cavalo(int tabuleiro[N][M], int x, int y, int jogada){
    int x2, y2, i, res = 0;
    if (jogada == N*M)
        return 1;

    int evalx[9], evaly[9], valido = 0;
    #pragma omp parallel for private(x2, y2) shared(evalx, evaly, valido)
    for (i=1;i<9;i++){
        x2 = proximo_movimento_x(x,i);
        y2 = proximo_movimento_y(y,i);
        if (jogada_valida(x2,y2, tabuleiro)){
            #pragma omp critical 
            {
                // printf("%d, %d\n", x2, y2);
                evalx[valido] = x2;
                evaly[valido] = y2;
                valido++;
            }
        }
    }
    #pragma barrier

    for(i = 0; i<valido; i++){
        tabuleiro[evalx[i]][evaly[i]] = jogada+1;
        // printf("%d, %d\n", evalx[i], evaly[i]);
        if (passeio_cavalo(tabuleiro, evalx[i],evaly[i], jogada+1))
            return 1;
        tabuleiro[evalx[i]][evaly[i]] = 0;
    }

    return 0;
}

int passeio_cavalo(int tabuleiro[N][M], int x, int y, int jogada){
    int x2, y2, i;
    if (jogada == N*M)
        return 1;

    for (i=1;i<9;i++){
        x2 = proximo_movimento_x(x,i);
        y2 = proximo_movimento_y(y,i);
        if (jogada_valida(x2,y2, tabuleiro)){
            tabuleiro[x2][y2] = jogada+1;
            if (passeio_cavalo(tabuleiro, x2,y2, jogada+1))
                return 1;
            tabuleiro[x2][y2] = 0;
        }
    }

    return 0;
}

int main(){
    int i, j;
    int tabuleiro[N][M];
    int x_inicio = 0, y_inicio = 0;
    clock_t start, end;
    double cpu_time_used;
    start = clock();
    
    printf("Resolvendo para N=%d e M=%d\n",N,M);

// NAO VALE A PENA SERIALIZAR: OVERHEAD > GANHO
    for (i=0; i < N; i++)
        for (j=0; j < M; j++)
            tabuleiro[i][j] = 0;

    tabuleiro[x_inicio][y_inicio] = 1;

    // omp_set_num_threads(NUM_THREADS); 
    // printf("NUM THREADS: %d\n", NUM_THREADS);

    if (passeio_cavalo(tabuleiro, x_inicio, y_inicio, 1))
        print_tabuleiro(tabuleiro);
    else
        printf("Nao existe solucao\n");
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("%f seconds\n",cpu_time_used);
    fprintf(stderr, "%f", cpu_time_used);
}
