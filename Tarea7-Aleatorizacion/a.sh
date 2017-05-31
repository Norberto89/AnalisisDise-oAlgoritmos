#!/bin/bash

sh cplexs.sh

rm -R Resultados/*

make clean
make all_cpp
sleep 5;

for i in 0 1 2 3;
do
	mkdir Resultados/Resultados_${i}
	for j in 1 2 3 4 5 6 7 8 9 10;
	do
		
		echo Realizando Experimentacion ${i} ${j};
		for k in "Problem_0_71.txt" "Problem_0_342.txt" "Problem_0_369.txt" "Problem_0_737.txt" "Problem_0_896.txt";
		do
			./main Instancias/$k $i >Resultados/Resultados_${i}/res_${j}_${k};
			echo ${j}_$k;
		done
		echo Experimentacion terminada.;
	done
done
	

