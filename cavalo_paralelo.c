#include <stdio.h>
#include <time.h>
#include <omp.h>
#include <string.h>
#include <stdlib.h>


// # threads eh limitado a ser <= N*M (tam tabuleiro)
#define NUM_THREADS 8
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

void zera_tabuleiro(int tabuleiro[N][M]){
    for(int i=0;i<N;i++)
        for(int j=0;j<M;j++)
            tabuleiro[i][j] = 0;
}

int tabuleiro_valido(int tabuleiro[N][M]){
    for(int i = 0; i < N; i++)
    for(int j = 0; j < M; j++)
        if(tabuleiro[i][j] == 0)
            return 0;
    return 1;
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

// Percorre a arvore de dfs 'escolhendo' um nodo filho aleatorio 
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
        if (_passeio_cavalo(tabuleiro, evalx[i],evaly[i], jogada+1))
            return 1;
        tabuleiro[evalx[i]][evaly[i]] = 0;
    }

    return 0;
}

// Percorre a arvore de dfs escolhendo sempre o 1° filho do nodo
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

// Começa uma busca dfs para cada posicao no tabuleiro
int threaded_walk(clock_t start)
{
        double cpu_time_used;
        clock_t end;
        
    // limita # threads, sendo menor que o tabuleiro
    int T = NUM_THREADS;
    T = (T > N*M) ? N*M : T; 
    omp_set_num_threads(T);

    printf("using %d threads\n", T);

    #pragma omp set_dynamic(0) 
    #pragma omp schedule(static, 1)
    #pragma omp parallel for
    for(int i = 0; i < T; i++){
        int x2 = i % N;
        int y2 = i / N;

        // absolutamente privado >:(
        int tabuleiro[N][M];
        zera_tabuleiro(tabuleiro);

        if(jogada_valida(x2, y2, tabuleiro)){
            tabuleiro[x2][y2] = 1;

            if (passeio_cavalo(tabuleiro, x2, y2, 1)){
                print_tabuleiro(tabuleiro);

                end = clock();
                cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
                printf("%f seconds\n",cpu_time_used);
                fprintf(stderr, "%f", cpu_time_used);
                exit(0);     // nao precisa lidar com cancelar omp threads           
            }
            tabuleiro[x2][y2] = 0;
        }
    }
    
    return 0;
}

int main(){
    int i, j;
    int x_inicio = 0, y_inicio = 0;
    clock_t start, end;
    double cpu_time_used;
    start = clock();
    
    printf("Resolvendo para N=%d e M=%d\n",N,M);

    if(!threaded_walk(start))
        printf("Nao existe solucao\n");

    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("%f seconds\n",cpu_time_used);
    fprintf(stderr, "%f", cpu_time_used);
    exit(0);
}
