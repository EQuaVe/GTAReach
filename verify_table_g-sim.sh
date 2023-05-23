#!/bin/bash
echo ""
echo "VERIFYING TABLE G-SIM"
echo "-------------------------"
echo ""
echo ""

echo "Running Dining Phi. (6)"
./run.sh gsim ./examples_from_the_paper/table_verify_gsim/Dining_Phi_6.txt -C ./output/g-sim/Dining_Phi_6.txt | grep -Ee "(TIME)|(STORED)|(VISITED)"
echo ""

echo "Running FDDI (10)"
./run.sh gsim ./examples_from_the_paper/table_verify_gsim/FDDI_10.txt -C ./output/g-sim/FDDI_10.txt | grep -Ee "(TIME)|(STORED)|(VISITED)"
echo ""

echo "Running Fischer (10)"
./run.sh gsim ./examples_from_the_paper/table_verify_gsim/Fischer_10.txt -C ./output/g-sim/Fischer_10.txt | grep -Ee "(TIME)|(STORED)|(VISITED)"
echo ""

echo "Running ToyECA(10000,4)"
./run.sh eca ./examples_from_the_paper/toyECA_ECA/toyECA_ECA_10000_4.txt ./examples_from_the_paper/table_verify_gsim/toyECA_10000_4.txt
./run.sh gsim ./examples_from_the_paper/table_verify_gsim/toyECA_10000_4.txt -C ./output/g-sim/toyECA_10000_4.txt | grep -Ee "(TIME)|(STORED)|(VISITED)"
echo ""

echo "Running ToyECA(5000,6)"
./run.sh eca ./examples_from_the_paper/toyECA_ECA/toyECA_ECA_5000_6.txt ./examples_from_the_paper/table_verify_gsim/toyECA_5000_6.txt
./run.sh gsim ./examples_from_the_paper/table_verify_gsim/toyECA_5000_6.txt -C ./output/g-sim/toyECA_ECA_5000_6.txt | grep -Ee "(TIME)|(STORED)|(VISITED)"
echo ""

echo "Running parser for ToyECA(1000,100)"
timeout -v 60s ./run.sh eca ./examples_from_the_paper/toyECA_ECA/toyECA_ECA_1000_100.txt ./examples_from_the_paper/table_verify_gsim/toyECA_1000_100.txt
rm ./examples_from_the_paper/table_verify_gsim/toyECA_1000_100.txt
echo ""

echo "Running ToyECA(50000,120)"
timeout -v 60s ./run.sh eca ./examples_from_the_paper/toyECA_ECA/toyECA_ECA_50000_120.txt ./examples_from_the_paper/table_verify_gsim/toyECA_50000_120.txt
rm ./examples_from_the_paper/table_verify_gsim/toyECA_50000_120.txt
echo ""

