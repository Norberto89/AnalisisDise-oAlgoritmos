#!/bin/bash
for i in {10..20};
do
	./gen_seg $i segmentos_${i}.txt
done
