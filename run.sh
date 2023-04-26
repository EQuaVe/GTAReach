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
fi
