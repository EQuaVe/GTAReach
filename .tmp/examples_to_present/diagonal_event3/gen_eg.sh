#!/bin/bash

for j in 3 5 6 7 10 12 15 17 20 25 30
do
	./diagonal_event.sh $j > all_egs/diagonal_event_$j.txt
done

