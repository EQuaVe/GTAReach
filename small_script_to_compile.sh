if [[ $1 == "h" ]]; then
    echo "<\$small_script_to_compile h> gives the help"
	echo "<\$small_script_to_compile c> compiles tchecker from start"
	echo "<\$small_script_to_compile> compiles the changed files in tchecker"    
	echo "<\$small_script_to_compile r file_name> run general clocks reachability algorithm"    
	exit
fi

if [[ $1 == "c" ]]; then
    reset
	rm -rf install_dir/
	mkdir install_dir
	
	rm -rf build_dir/
	mkdir build_dir
	cd build_dir
	cmake ../tchecker -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=../install_dir -DCMAKE_CXX_FLAGS=-pthread
	make -j2
	make doc
	make install
	cd ../
	#cd ../install_2/bin/
	echo "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"
	echo "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"
	echo "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"    
	exit
fi
if [ $# -lt 1 ]; then
	reset
	#rm -rf install_dir/
	#mkdir install_dir
	
	#rm -rf build_dir/
	#mkdir build_dir
	cd build_dir
	cmake ../tchecker -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=../install_dir -DCMAKE_CXX_FLAGS=-pthread
	make -j2
	make doc
	make install
	cd ../
	#cd ../install_2/bin/
	echo "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"
	echo "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"
	echo "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"    
fi

if [ $# -eq 2 ]; then
./install_dir/bin/tck-reach -a eca_gsim_gen $2 -C /dev/stdout
fi
