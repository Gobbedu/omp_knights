#!/bin/bash

make

PARALLEL="p.log"
SERIAL="s.log"

echo -n "[" > $PARALLEL
echo -n "[" > $SERIAL

# salva no file tempo de N exec do parallel & serial
for i in {1..100}; do 
    make runp 2>> $PARALLEL > /dev/null; 
    echo -n ", " >> $PARALLEL; 

    make runs 2>> $SERIAL > /dev/null; 
    echo -n ", " >> $SERIAL;  
done

echo -n "]" >> $PARALLEL
echo -n "]" >> $SERIAL
