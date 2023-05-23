#!/bin/bash
if [ $# -lt 1 ]; then
	echo "ERROR: No input file given"
fi

if [ $# -eq 1 ]; then
	./install_dir/bin/tck-reach -a gta_gsim $1
fi

if [ $# -eq 2 ]; then

	if [ "$1" == "gta_gsim" ]; then
		./install_dir/bin/tck-reach -a gta_gsim $2
	fi

	if [ "$1" == "gsim" ]; then
		./install_dir/bin/tck-reach -a gsim $2
	fi
		
	if [ "$1" == "eca" ]; then
		python3 eca_wrapper/main_prog.py $2
	fi

fi

if [ $# -eq 3 ]; then
	if [[ "$1" == "eca" && "$3" != "-C" ]]; then
		python3 eca_wrapper/main_prog.py $2 > $3
	fi
	
	if [[ "$1" == "gsim" && "$3" == "-C" ]]; then
		./install_dir/bin/tck-reach -a gsim $2 -C tmp.txt
	fi
	
	if [[ "$1" == "gta_gsim" && "$3" == "-C" ]]; then
		./install_dir/bin/tck-reach -a gta_gsim $2 -C tmp.txt
	fi
	
fi

if [ $# -eq 4 ]; then
	
	if [ "$3" == "-C" ]; then
		if [ "$1" == "gta_gsim" ]; then
			./install_dir/bin/tck-reach -a gta_gsim $2 -C $4
		fi

		if [ "$1" == "gsim" ]; then
				./install_dir/bin/tck-reach -a gsim $2 -C $4
		fi	
	fi 

fi
