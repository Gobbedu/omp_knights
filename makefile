all: serial paralelo

serial: passeio_cavalo.c
	gcc -o serial passeio_cavalo.c

paralelo: cavalo_paralelo.c
	gcc -o paralelo cavalo_paralelo.c -fopenmp


runs: serial
	@echo "serial:\n"
	./serial

runp: paralelo
# STACKSIZE limita numero de threads em um dado momento
	export OMP_STACKSIZE=6 
	@echo "paralelo:\n"
	./paralelo

run: runs runp


clean:
	@rm -rf paralelo serial