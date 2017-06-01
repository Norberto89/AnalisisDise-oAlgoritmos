#!/bin/bash




for i in 0.1 0.01 0.001 0.0001 0.00001;
do
	for j in 25 50 75 100 125;
	do
		echo Realizando Lectura $i $j;
		for k in "Problem_0_71.txt" "Problem_0_342.txt" "Problem_0_369.txt" "Problem_0_737.txt" "Problem_0_896.txt";
		do
			./resul Resultados/Resultados_${i}_${j}/res_$k $j $i;
			echo $k;
		done
		echo Terminado.;
	done
done
	

