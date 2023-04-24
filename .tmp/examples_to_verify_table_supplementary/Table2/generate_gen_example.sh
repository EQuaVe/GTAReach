#!/bin/bash


# Check parameters

C=10
K=0
function usage() {
    echo "Usage: $0 C";
    echo "       $0 C K";
    echo "       C Maximum Constant";
    echo "       K Number of extra edges (default: 0)"
}

if [ $# -eq 1 ]; then
    C=$1
elif [ $# -eq 2 ]; then
    C=$1
    K=$2
else
    usage
    exit 1
fi

echo "system:exp_example_$C""_$K

event:a:1:1
event:b:1:1
"
for (( i=1; i<=$K; i++ ))
do
    echo "event:c$i:1:1"
done

echo "
process:P
location:P:l0{initial:}
location:P:l1{}"


echo "location:P:l2{}"

echo "
edge:P:l0:l1:a{{}}
edge:P:l1:l1:a{{provided:a_h==1&&b_p<=-$C;}}
"

for (( i=1; i<=$K; i++ ))
do
    echo "edge:P:l1:l1:c$i{{provided:a_h==1&&c$i""_p<=-$C;}}"
done

echo "edge:P:l1:l2:b{{}}"

