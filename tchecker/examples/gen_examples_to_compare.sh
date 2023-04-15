#!/bin/bash  
  
#Array Declaration
#arr=( 10 100 1000 10000)  
#arr3=( 10 100 200)

job_shop(){

	arr=( 10 11 12 13 14 15 16 17 )  
	for i in "${arr[@]}"  
	do  
		job-shop.sh 2 $i 10 10 > "gen_model/old_model_comp/job_shop_10_$i""_10_10.txt"
	done
}

dining_philosophers(){
<<comment
	arr=( 4 5 6 7 10)  
	for i in "${arr[@]}"
	do
	dining-philosophers.sh 4 7 $i 2 > "gen_model/old_model_comp/dining_philosophers_4_7_$i""_2.txt"
	dining-philosophers.sh 5 7 $i 2 > "gen_model/old_model_comp/dining_philosophers_5_7_$i""_2.txt"
	dining-philosophers.sh 6 7 $i 2 > "gen_model/old_model_comp/dining_philosophers_6_7_$i""_2.txt"
	dining-philosophers.sh 7 7 $i 2 > "gen_model/old_model_comp/dining_philosophers_7_7_$i""_2.txt"
	done
comment
	arr=( 5 6 7 8)  
	for i in "${arr[@]}"
	do
	dining-philosophers.sh $i > "gen_model/old_model_comp/dining_philosophers_$i.txt"
	done

}

fischer(){
<<comment
	arr=( 4 5 6 7 10) 
	for i in "${arr[@]}"
	do 
	fischer.sh 4 $i > "gen_model/old_model_comp/fischer_4_$i"".txt"
	fischer.sh 5 $i > "gen_model/old_model_comp/fischer_5_$i"".txt"
	fischer.sh 6 $i > "gen_model/old_model_comp/fischer_6_$i"".txt"
	fischer.sh 7 $i > "gen_model/old_model_comp/fischer_7_$i"".txt"
	fischer.sh 8 $i > "gen_model/old_model_comp/fischer_8_$i"".txt"
	fischer.sh 9 $i > "gen_model/old_model_comp/fischer_9_$i"".txt"
	done
comment

	arr=( 5 7 9 10) 
	for i in "${arr[@]}"
	do 
	fischer.sh $i > "gen_model/old_model_comp/fischer_$i"".txt"
	done
}
fischer_async(){
<<comment
	arr=( 4 5 6 7 10) 
	for i in "${arr[@]}"
	do 
	fischer.sh 4 $i > "gen_model/old_model_comp/fischer_4_$i"".txt"
	fischer.sh 5 $i > "gen_model/old_model_comp/fischer_5_$i"".txt"
	fischer.sh 6 $i > "gen_model/old_model_comp/fischer_6_$i"".txt"
	fischer.sh 7 $i > "gen_model/old_model_comp/fischer_7_$i"".txt"
	fischer.sh 8 $i > "gen_model/old_model_comp/fischer_8_$i"".txt"
	fischer.sh 9 $i > "gen_model/old_model_comp/fischer_9_$i"".txt"
	done
comment

	arr=( 3 4 5 7 8 9 10) 
	for i in "${arr[@]}"
	do 
	fischer-async.sh $i > "gen_model/old_model_comp/fischer_async_$i"".txt"
	done
}
fddi(){
<<comment
	arr=( 10 100 200 300 350 400 450 500)  
	for i in "${arr[@]}"
	do
	fddi.sh 7 $i 7 2 > "gen_model/old_model_comp/fddi_7_$i""_7_2.txt"
	done
comment

	arr=( 5 7 10)  
	for i in "${arr[@]}"
	do
	fddi.sh $i > "gen_model/old_model_comp/bfddi_$i"".txt"
	done

}
fire_alarm(){
<<comment
	arr=( 10 100 1000 3000 5000)  
	for i in "${arr[@]}"
	do
	fire-alarm.sh 7 7 $i > "gen_model/old_model_comp/fire-alarm_7_7_$i"".txt"
	done
comment
	arr=( 3 4 5 6 7 8 9 10)  
	for i in "${arr[@]}"
	do
	fire-alarm.sh $i > "gen_model/old_model_comp/afire-alarm_$i"".txt"
	done

}
gps_mc(){
	arr=( 4 5 6 7 10 20 50)
	arr3=( 6 7 9)  
	for j in "${arr[@]}"
	do
	
	for i in "${arr[@]}"
	do
		for k in "${arr3[@]}"
		do
	
			gps-mc.sh 7 $k $i $j > "gen_model/old_model_comp/gps_mc_7_"$k"_"$i"_"$j".txt" 
		done
	done
	done
}
csmacd(){
<<comment
	arr=( 4 5 6 7 10 20 50)
	for i in "${arr[@]}"
	do
	csmacd.sh 5 5 $i > "gen_model/old_model_comp/csmacd_5_5_$i"".txt" ##contains int
	done
comment
	
	arr=( 5 8 10)
	for i in "${arr[@]}"
	do
	csmacd.sh $i > "gen_model/old_model_comp/csmacd_$i"".txt" ##contains int
	done
}

parallel(){
	arr=( 3 4 5 6 7 8)  

	for i in "${arr[@]}"  
	do  
		parallel-b.sh $i > "gen_model/old_model_comp/parallel_b_$i"".txt"
		parallel-c.sh $i > "gen_model/old_model_comp/parallel_c_$i"".txt"
		parallel.sh $i > "gen_model/old_model_comp/parallel_$i"".txt"

	done
}


fischer_async
fire_alarm

<<comment
job_shop
csmacd
parallel
gps_mc
fire_alarm
fischer
fddi
dining_philosophers
job_shop
comment

for f in gen_model/old_model_comp/*; do
  echo "File -> $f"
  x=$(echo $f | cut -d "/" -f 3)
  gawk -f ~/Desktop/tchecker_3_gsim/tchecker_gsim/parse.awk $f > gen_model/gen_model_comp/$x
done

