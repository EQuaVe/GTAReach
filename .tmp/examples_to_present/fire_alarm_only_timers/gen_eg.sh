#!/bin/bash


	for i in 3 4 5 6 7 8 9
	do
		echo $i
		fire_alarm.sh $i > all_egs/"fire_alarm_"$i".txt"		 
	done

