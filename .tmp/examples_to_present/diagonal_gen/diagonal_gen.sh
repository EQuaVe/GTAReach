#!/bin/bash
function usage() {
    echo "Usage: $0 I J";
    echo "       C constant";
    echo "       K constant";

}

if [ $# -eq 2 ]; then
    I=$1
    J=$2
else
    usage
    exit 1
fi

echo "system:diagonal_$I""_$J


clock:prophecy:p1
clock:normal:n
clock:prophecy:p2

event:a


process:P
location:P:q0{initial:}
location:P:q1{}
location:P:q2{labels:green}

edge:P:q0:q1:a{{ provided:; do:p1,p2; provided:n-p2==$I && n-p1==$J; }}

edge:P:q1:q1:a{{provided:n==1; do:n;}}
edge:P:q1:q2:a{{}}
"

