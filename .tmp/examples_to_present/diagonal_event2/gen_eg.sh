#!/bin/bash

for j in 100 2000 3000 4000 5000 8000
do
	./diagonal_event.sh $j > all_egs/diagonal_event_$j.txt
done

