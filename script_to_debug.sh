#!/bin/bash

for f in tchecker/examples/gen_model/gen_model_comp/*; do
  echo "============================+++++=============================="
  echo "$x"
  timeout -v 2s ./install_dir/bin/tck-reach -a eca_gsim_gen $f | tail
done

echo "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"

for f in $(ls|grep -Ee "example[0-9]+.*.txt")
do
	echo "============================+++++=============================="
	echo $f
	echo "y" | timeout -v 2s ./install_dir/bin/tck-reach -a eca_gsim_gen $f  | tail
done

