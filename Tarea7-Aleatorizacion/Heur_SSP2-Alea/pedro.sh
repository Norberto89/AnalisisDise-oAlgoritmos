#!/bin/bash



for i in "225_Pr101t.txt"  "225_Pr104t.txt"  "225_Pr107t.txt"  "225_Pr110t.txt"  "225_Pr113t.txt" "225_Pr101.txt"   "225_Pr104.txt"   "225_Pr107.txt"   "225_Pr110.txt"   "225_Pr113.txt" "225_Pr102t.txt" "225_Pr105t.txt"  "225_Pr108t.txt"  "225_Pr111t.txt"  "225_Pr114t.txt" "225_Pr102.txt" "225_Pr105.txt" "225_Pr108.txt"   "225_Pr111.txt"   "225_Pr114.txt" "225_Pr103t.txt"  "225_Pr106t.txt" "225_Pr109t.txt"  "225_Pr112t.txt"  "225_Pr115t.txt" "225_Pr103.txt"   "225_Pr106.txt"   "225_Pr109.txt"   "225_Pr112.txt"   "225_Pr115.txt";
do
   
     lpg -o domain3.pddl -f $i -n 1 >Terminal/res_$i;
     sleep 10;

done
