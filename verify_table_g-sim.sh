#!/bin/bash
echo "VERIFYING TABLE G-SIM"
echo "-------------------------"
echo ""
echo ""

echo "Running Dining Phi. (6)"
./run.sh gsim ./examples_from_the_paper/table_verify_gsim/Dining_Phi_6.txt
echo ""

echo "Running FDDI (10)"
./run.sh gsim ./examples_from_the_paper/table_verify_gsim/FDDI_10.txt
echo ""

echo "Running Fischer (10)"
./run.sh gsim ./examples_from_the_paper/table_verify_gsim/Fischer_10.txt
echo ""

echo "Running ToyECA(10000,4)"
./run.sh gsim ./examples_from_the_paper/table_verify_gsim/toyECA_10000_4.txt
echo ""

echo "Running ToyECA(5000,6)"
./run.sh gsim ./examples_from_the_paper/table_verify_gsim/toyECA_5000_6.txt
echo ""

echo "Running parser for ToyECA(1000,100)"
timeout 300s ./run.sh eca ./examples_from_the_paper/toyECA_ECA/toyECA_ECA_1000_100.txt
echo ""

echo "Running ToyECA(50000,120)"
timeout 300s ./run.sh eca ./examples_from_the_paper/toyECA_ECA/toyECA_ECA_50000_120.txt
echo ""

