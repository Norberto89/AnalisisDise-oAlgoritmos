#!/bin/bash

sh cplexs.sh

rm Resultados/*

make clean
make all_cpp
sleep 5;

for i in "Problem_3_530.txt" "Problem_3_1024.txt" "Problem_3_2596.txt" "Problem_3_6384.txt" "Problem_3_7862.txt";
do
   
    ./main Instancias/$i >Resultados/res_$i;
     sleep 10;
	
done
