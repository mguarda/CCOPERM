#!/bin/bash

for((i = 1; i <= 5; i++ ))
do
	./EJECUTABLE_TSP_PERM "./Problems/TSP/berlin52.tsp"
#	./EJECUTABLE_QAP_PERM "./Problems/QAP/sko64.dat"	
done
