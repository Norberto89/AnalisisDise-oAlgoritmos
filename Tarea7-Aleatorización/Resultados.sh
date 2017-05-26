#!/bin/bash



for k in "Problem_0_71.txt" "Problem_0_342.txt" "Problem_0_369.txt" "Problem_0_737.txt" "Problem_0_896.txt";
do
	for i in 0 1 2 3;
	do
		for j in 1 2 3 4 5 6 7 8 9 10;
		do
		
			echo Realizando Lectura ${i} ${j};
		
			./resul Resultados/Resultados_${i}/res_${j}_${k}  $i $j ${i}_${k};
			echo ${j}_$k;
		done
		echo Terminado.;
	done
done
	
