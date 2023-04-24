#!/bin/bash

for j in 3 5 8 15 20 
do
	for i in 3 4 5 6 7 8 9 10
	do
		echo $i $j
		fischer_param.sh $i $j > "fischer_param_"$i"_"$j".txt"		 
	done
done

