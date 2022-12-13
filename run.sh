#!/bin/bash

make

N_TIMES=100

echo -n "[" > parallel.log
echo -n "[" > serial.log

# salva no file tempo de N exec do parallel & serial
for i in {1..100}; do 
    make runp 2>>parallel.log > /dev/null; 
    echo -n ", " >> parallel.log; 

    make runs 2>>serial.log > /dev/null; 
    echo -n ", " >> serial.log;  
done

echo -n "]" >> parallel.log
echo -n "]" >> serial.log
