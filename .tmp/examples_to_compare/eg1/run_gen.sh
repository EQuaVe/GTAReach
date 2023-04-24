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

./generate_gsim_example.sh 1000 100 > toy_1000_100.txt
	   python ~/Desktop/eca_wrapper/eca_wrapper/main_prog.py toy_1000_100.txt > tck_toy_1000_100.txt

./generate_gen_example.sh 1000 100 > eca_1000_100.txt
	   timeout -v 15s ../../install_dir/bin/tck-reach -a eca_gsim_gen eca_1000_100.txt -C /dev/null
