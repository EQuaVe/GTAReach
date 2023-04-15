<<comment
for k in 0 1 2 3 4 5
do
	for c in 10 100 1000 5000 10000 100000 1000000
	do
	   echo "_________________________________________"
	   echo "constant:"$c" extra_edges:"$k
	   echo ""
	   echo "Converting ECA to TA"
	   
	   ./generate_gsim_example.sh $c $k > toy_"$c"_"$k".txt
	   python ~/Desktop/eca_wrapper/eca_wrapper/main_prog.py toy_"$c"_"$k".txt > tck_toy_"$c"_"$k".txt
	   timeout -v 15s ../../install_dir/bin/tck-reach -a gsim tck_toy_"$c"_"$k".txt -C /dev/null | grep -Ee "COVERED_STATES|RUNNING_TIME_SECONDS|STORED_STATES|VISITED_STATES"
	   echo "-----------------------------------------"
	   echo "Zone graph exploration on ECA"
	   ./generate_gen_example.sh $c $k > eca_"$c"_"$k".txt
	   timeout -v 15s ../../install_dir/bin/tck-reach -a eca_gsim_gen eca_"$c"_"$k".txt -C /dev/null | grep -Ee "COVERED_STATES|RUNNING_TIME_SECONDS|STORED_STATES|VISITED_STATES"
	   
	   #echo "Normal event-clock exploration"
	   #cat tck_toy_"$c"_"$k".txt | sed s/clock:1:/clock:normal:/g > eca_normal_"$c"_"$k".txt 
	   #timeout -v 15s ../install_dir/bin/tck-reach -a eca_gsim_gen eca_normal_"$c"_"$k".txt -C /dev/null | grep -Ee "COVERED_STATES|RUNNING_TIME_SECONDS|STORED_STATES|VISITED_STATES"
	done
done
comment

for i in "100 1" "1000 1" "10000 1" "100000 1" "1000 2" "1000 3" "1000 4" "1000 5" "1000 6" "1000 7" "1000 100" "10000 4" "5000 5" "1000000 1" "50000 120"
do
    set -- $i # convert the "tuple" into the param args $1 $2...
    echo $1 and $2
    ./generate_gen_example.sh $1 $2 > all_egs/B3_"$1"_"$2".txt
    timeout -v 60s ../install_dir/bin/tck-reach -a eca_gsim_gen all_egs/B3_"$1"_"$2".txt -C /dev/null | grep -Ee "COVERED_STATES|RUNNING_TIME_SECONDS|STORED_STATES|VISITED_STATES"
    echo "-----------------------------------------"
    
done

<<comment
for i in "100 1" "1000 1" "10000 1" "100000 1" "1000 2" "1000 3" "1000 4" "1000 5" "1000 6" "1000 7" "1000000 1" "10000 4" "5000 5"
do
    set -- $i
    echo $1 and $2
	./generate_gsim_example.sh $1 $2 > tmp/toy_"$1"_"$2".txt
	python ./eca_wrapper/main_prog.py tmp/toy_"$1"_"$2".txt > tck_toy_"$1"_"$2".txt
	timeout -v 60s ../install_dir/bin/tck-reach -a gsim tck_toy_"$1"_"$2".txt -C /dev/null | grep -Ee "COVERED_STATES|RUNNING_TIME_SECONDS|STORED_STATES|VISITED_STATES"
	echo "-----------------------------------------"
done	
comment
	

