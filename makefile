all: serial paralelo

serial: passeio_cavalo.c
	gcc -O3 -o serial passeio_cavalo.c

paralelo: cavalo_paralelo.c
	gcc -O3 -o paralelo cavalo_paralelo.c -fopenmp


runs: serial
	@echo "\nserial:"
	@./serial

runp: paralelo
# STACKSIZE limita numero de threads em um dado momento
# export OMP_NUM_THREADS=6
	export OMP_STACKSIZE=6 
	export OMP_NESTED=true
	export OMP_CANCELLATION=true
	@echo "\nparalelo:"
	@./paralelo

run: runs runp


clean:
	@rm -rf paralelo serial