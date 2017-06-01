#!/bin/bash

sh cplexs.sh

rm -R Resultados/*

make clean
make all_cpp
sleep 5;

for i in 0.1 0.01 0.001 0.0001 0.00001;
do
	for j in 25 50 75 100 125;
	do
		mkdir Resultados/Resultados_${i}_${j};
		echo Realizando Experimentacion ${i} ${j};
		for k in "Problem_0_71.txt" "Problem_0_342.txt" "Problem_0_369.txt" "Problem_0_737.txt" "Problem_0_896.txt";
		do
			./main Instancias/$k $j $i >Resultados/Resultados_${i}_${j}/res_$k;
			echo $k;
		done
		echo Experimentacion terminada.;
	done
done
	

