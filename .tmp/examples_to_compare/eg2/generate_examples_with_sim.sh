#!/bin/bash


# Check parameters

C=10

function usage() {
    echo "Usage: $0 C";
    echo "       C Maximum Constant";
}

if [ $# -eq 1 ]; then
    C=$1
else
    usage
    exit 1
fi

echo "system:exp_example_$C

event:a
event:b
event:c

clock:history:h
clock:prophecy:p
"


echo "
process:P
location:P:l0{initial:}
location:P:l1{}
location:P:l2{}
location:P:l3{labels: green}
"

echo "
edge:P:l0:l1:a{{provided:;do:h;}}
edge:P:l1:l2:a{{provided:p-h>=-$C;}}

edge:P:l2:l2:b{{provided:h==1; do:h;}}
edge:P:l2:l3:b{{provided:h==0 && p==0;}}
"

