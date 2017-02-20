#!/bin/bash
for j in {1..1000};
do
	for i in 25 50 75 100 125 150 175 200 225 250;
	do
		./a $i File_${i}.csv
		sleep 0.001
	done
done
