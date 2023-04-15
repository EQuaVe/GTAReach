#!/bin/bash

for f in tchecker/examples/gen_model/old_model_comp/*; do

  x=$(echo $f | cut -d "/" -f 5)
  echo "============================+++++=============================="
  echo "$x"
  y=$(timeout -v 60s ./install_dir/bin/tck-reach -a gsim $f | grep -Ee "STORED_STATES|RUNNING_TIME_SECONDS|VISITED_STATES|COVERED_STATES")
  z=$(timeout -v 60s ./install_dir/bin/tck-reach -a eca_gsim_gen tchecker/examples/gen_model/gen_model_comp/$x|grep -Ee "STORED_STATES|RUNNING_TIME_SECONDS|VISITED_STATES|COVERED_STATES")
  echo "G-Sim"
  echo "$y"
  echo "---------------------------"
  echo "GEN-MODEL"
  echo "$z"
  
done
  echo "============================+++++=============================="

