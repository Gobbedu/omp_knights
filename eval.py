import os
from ast import literal_eval
from statistics import median, stdev

def prettyprint(col1, col2, col3, col4, col5, col6):

    form = "{:^15} | {:^12} | {:^8.4f} | {:^8.4f} | {:^8.4f} | {:^8.4f} "
    if type(col3) == str:
        form = "{:^15} | {:^12} | {:^8} | {:^8} | {:^8} | {:^8} "

    print(form.format(
        col1, col2, col3, col4, col5, col6
    ))

    with open('result.txt', 'a') as f:
        print(form.format(
            col1, col2, col3, col4, col5, col6
            ), 
        file=f
        )


def evaluate(Nruns, Nthreads):
    '''
    - NThreads = [1, 2, 4, 8]
    - rodar N vezes:
        para cada nt in NThreads:
            roda p
            roda s
            salva speedup & stdev     
    '''
    os.system("make")    
    
    # aux files
    parallelf = "p.log"
    serialf = "s.log"

    prettyprint("Eficiencia", "", "1 CPU", "2 CPUs", "4 CPUs", "8 CPUs")

    for runs in Nruns:
        speedup = list()
        deviate = list()
        
        for Nthread in Nthreads:
            # salva em file tempo de N exec do parallel & serial
            os.system(f'export OMP_NUM_THREADS={Nthread}')
            os.system('export OMP_CANCELLATION=true')
            os.system(f'echo -n "[" > {parallelf}')
            os.system(f'echo -n "[" > {serialf}')

            for i in range(runs):
                os.system(f'./cavalo_paralelo {Nthread} 2>> {parallelf} > /dev/null')
                os.system(f'echo -n ", " >> {parallelf}')

                os.system(f'make runs 2>> {serialf} > /dev/null')
                os.system(f'echo -n ", " >> {serialf}')

            os.system(f'echo -n "]" >> {parallelf}')
            os.system(f'echo -n "]" >> {serialf}')

            with open('p.log', 'r') as p:
                paral_list = literal_eval(p.readline())

            with open('s.log', 'r') as s:
                serial_list = literal_eval(s.readline())

            speedup.append(median(serial_list)/median(paral_list))
            deviate.append(stdev(serial_list)/stdev(paral_list))
        
        prettyprint("", f'N={runs}', *speedup)
        prettyprint("std deviation", "", *deviate)

    prettyprint(*['-------']*6)

    os.system('make clean')
    os.system('rm -rf p.log s.log')

def main():
    Nruns = [20, 100]#, 200, 400]
    Nthreads = [1, 2, 4, 8] # nao mudar (quebra codigo)

    evaluate(Nruns, Nthreads)


def _main():
    with open('p.log', 'r') as p:
        paral = literal_eval(p.readline())

    with open('s.log', 'r') as s:
        seria = literal_eval(s.readline())

    # print(f'lens: {len(paral)} & {len(seria)}')


    Speed = list()
    for t1, tn in zip(seria, paral):
        Speed.append(t1/tn)

    S = sum(Speed)/len(Speed)
    print(f'Avg speedup: {S}')
    print(f'Speedup: {median(Speed)}\nDesvio: {stdev(Speed)}')


if __name__ == "__main__":
    main()