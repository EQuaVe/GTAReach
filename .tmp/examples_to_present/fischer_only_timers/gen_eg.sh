#!/bin/bash


	for i in 3 4 5 6 7 8 9
	do
		echo $i 
		fischer_param.sh $i > all_egs/"fischer_param_"$i".txt"		 
	done


