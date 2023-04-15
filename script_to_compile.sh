
if [[ $1 == "c" ]]; then
    reset
    	touch install_dir
	rm -rf install_dir
	mkdir install_dir
	
	touch build_dir
	rm -rf build_dir
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
if [[ $1 == "d" ]]; then
    reset
	rm -rf install_dir/
	mkdir install_dir
	
	rm -rf build_dir/
	mkdir build_dir
	cd build_dir
	cmake ../tchecker -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX=../install_dir -DCMAKE_CXX_FLAGS=-pthread
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

if [ $# -eq 1 ]; then
./install_dir/bin/tck-reach -a eca_gsim ./tchecker/examples/ad94_eca2.txt -l green -C /dev/stdout
fi

if [ $# -eq 2 ]; then
./install_dir/bin/tck-reach -a eca_gsim_gen $2 -C /dev/stdout
fi

if [ $# -eq 3 ]; then
./install_dir/bin/tck-reach -a eca_gsim $2 -C $3
python parse_dot.py $3 > tmp.dot
dot -Tpdf tmp.dot -o tmp.pdf
evince tmp.pdf
fi

if [ $# -eq 4 ]; then
./install_dir/bin/tck-reach -a eca_gsim $2 -l $4 -C $3
python parse_dot.py $3 > tmp.dot
dot -Tpdf tmp.dot -o tmp.pdf
evince tmp.pdf
fi
