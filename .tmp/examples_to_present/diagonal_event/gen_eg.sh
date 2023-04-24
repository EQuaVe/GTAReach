#!/bin/bash

for j in 100 2000 3000 4000 5000 8000
do
        for i in 20 21 37 40 60
        do 
	./diagonal_event.sh $j $i > all_egs/diagonal_event_$j"_"$i.txt
	echo "./diagonal_event.sh $j $i > all_egs/diagonal_event_$j""_""$i.txt"
        done
done

