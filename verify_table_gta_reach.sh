#!/bin/bash
echo ""
echo "VERIFYING TABLE GTA REACH"
echo "-------------------------"
echo ""
echo ""

echo "Running Dining Phi. (6)"
./run.sh gta_gsim ./examples_from_the_paper/table_verify_gta/Dining_Phi_6.txt | grep -Ee "(TIME)|(STORED)|(VISITED)"
echo ""

echo "Running FDDI (10)"
./run.sh gta_gsim ./examples_from_the_paper/table_verify_gta/FDDI_10.txt | grep -Ee "(TIME)|(STORED)|(VISITED)"
echo ""

echo "Running Fischer (10)"
./run.sh gta_gsim ./examples_from_the_paper/table_verify_gta/Fischer_10.txt | grep -Ee "(TIME)|(STORED)|(VISITED)"
echo ""

echo "Running ToyECA(10000,4)"
./run.sh gta_gsim ./examples_from_the_paper/table_verify_gta/toyECA_10000_4.txt | grep -Ee "(TIME)|(STORED)|(VISITED)"
echo ""

echo "Running ToyECA(5000,6)"
./run.sh gta_gsim ./examples_from_the_paper/table_verify_gta/toyECA_5000_6.txt | grep -Ee "(TIME)|(STORED)|(VISITED)"
echo ""

echo "Running ToyECA(1000,100)"
./run.sh gta_gsim ./examples_from_the_paper/table_verify_gta/toyECA_1000_100.txt | grep -Ee "(TIME)|(STORED)|(VISITED)"
echo ""

echo "Running ToyECA(50000,120)"
./run.sh gta_gsim ./examples_from_the_paper/table_verify_gta/toyECA_50000_120.txt | grep -Ee "(TIME)|(STORED)|(VISITED)"
echo ""

echo "Running Fire-alarm-pattern(5)"
./run.sh gta_gsim ./examples_from_the_paper/table_verify_gta/Fire-alarm-pattern_5.txt | grep -Ee "(TIME)|(STORED)|(VISITED)"
echo ""

echo "Running CSMACD-bounded(1)"
./run.sh gta_gsim ./examples_from_the_paper/table_verify_gta/CSMACD-bounded_1.txt | grep -Ee "(TIME)|(STORED)|(VISITED)"
echo ""

echo "Running CSMACD-bounded(4)"
./run.sh gta_gsim ./examples_from_the_paper/table_verify_gta/CSMACD-bounded_4.txt | grep -Ee "(TIME)|(STORED)|(VISITED)"
echo ""

echo "Running ABP-prop1(1)"
./run.sh gta_gsim ./examples_from_the_paper/table_verify_gta/ABP-prop1.txt | grep -Ee "(TIME)|(STORED)|(VISITED)"
echo ""

echo "Running ABP-prop2(1)"
./run.sh gta_gsim ./examples_from_the_paper/table_verify_gta/ABP-prop2.txt | grep -Ee "(TIME)|(STORED)|(VISITED)"
echo ""

