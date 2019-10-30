#!/bin/bash

echo "Testador automático das integrais"

gcc conc.c -o concorrente.out -Wall -lm -lpthread
gcc seq.c -o sequencial.out -Wall -lm 

RANDOM=$$

# Teste de espaçamento
for ((i = 0; i < 10; i++));do
	echo "Concorrente"
	echo $RANDOM
	./concorrente.out -$RANDOM $RANDOM 0.1 4
	echo "-------------"
	echo "Sequencial"
	./sequencial.out -$RANDOM $RANDOM 0.1 4
	echo "------------"
done

