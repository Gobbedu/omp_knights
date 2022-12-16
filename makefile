CC = gcc -O1

all: cavalo_paralelo passeio_cavalo

cavalo_paralelo: cavalo_paralelo.c
	$(CC) -o cavalo_paralelo -fopenmp cavalo_paralelo.c 

passeio_cavalo: passeio_cavalo.c
	$(CC) -o passeio_cavalo passeio_cavalo.c 

run: passeio_cavalo cavalo_paralelo
	@echo "\nparalelo:"
	@export OMP_CANCELLATION=true
	@./cavalo_paralelo 2> /dev/null

	@echo "\nserial:"
	@./passeio_cavalo 2> /dev/null


runs: passeio_cavalo
	@./passeio_cavalo

runp: cavalo_paralelo
	@export OMP_CANCELLATION=true
	@./cavalo_paralelo

batch: run.sh
	@./run.sh

speed: eval.py
	@python3 eval.py

eval: all 
	@$(MAKE) batch --no-print-directory
	@$(MAKE) speed --no-print-directory

clean:
	@rm -rf passeio_cavalo cavalo_paralelo 
