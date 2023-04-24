#!/bin/bash

for j in 1 2 3 4 5 6 7 8 9 10 15 20 100 150 200 250 300 400 500 600
do
	./train_gate_param.sh $j > all_egs/train_gate_param_$j.txt
done

