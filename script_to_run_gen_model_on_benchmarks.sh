#!/bin/bash

function usage() {
    echo "Usage: $0 BENCHMARKS_FOLDER";
}
for f in $1/*; do
  
  x=$(echo $f | rev | cut -d "/" -f 1 | rev)
  echo "============================+++++=============================="
  echo "$x"
  y=$(echo "")
  z=$(echo "y" | timeout -v 60s ./install_dir/bin/tck-reach -a eca_gsim_gen $f |grep -Ee "STORED_STATES|RUNNING_TIME_SECONDS|VISITED_STATES|COVERED_STATES")
  echo "G-Sim"
  echo "$y"
  echo "---------------------------"
  echo "GEN-MODEL"
  echo "$z"
  
done
  echo "============================+++++=============================="

