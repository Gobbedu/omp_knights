CC = gcc -O3

all: cavalo_paralelo passeio_cavalo

cavalo_paralelo: cavalo_paralelo.c
	$(CC) -o cavalo_paralelo -fopenmp cavalo_paralelo.c 

passeio_cavalo: passeio_cavalo.c
	$(CC) -o passeio_cavalo passeio_cavalo.c 

run: passeio_cavalo cavalo_paralelo
	@./cavalo_paralelo 2> /dev/null
	@./passeio_cavalo  2> /dev/null
	@$(MAKE) clean --no-print-directory

speed: speedup.py all
# sudo para controlar prioridade de subprocesso dentro do codigo em python
	@sudo nice -n -20 python3 speedup.py ./cavalo_paralelo ./passeio_cavalo
	@$(MAKE) clean --no-print-directory

clean:
	@rm -rf passeio_cavalo cavalo_paralelo 
