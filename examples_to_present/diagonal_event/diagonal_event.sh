#!/bin/bash
function usage() {
    echo "Usage: $0 I J";
    echo "       I is the constant on guard (default:30)";
    echo "       J is the constant on guard (default:1)";
}

if [ $# -eq 2 ]; then
    I=$1
    J=$2
elif [ $# -eq 1 ]; then
    I=$1
    J=1
else    
    I=30
    J=1
    #usage
    #exit 1
fi

echo "system:diagonal_event_$I""_"$J"


event:a:1:0
event:b:0:1
event:c

process:P
location:P:q0{initial:}
location:P:q1{}
location:P:q2{}
location:P:q3{labels: green}


edge:P:q0:q1:a{{}}
edge:P:q1:q2:c{{provided:a_h-b_p==$I;}}

edge:P:q2:q2:a{{provided:a_h==$J;}}
edge:P:q2:q3:b{{provided:a_h==0;}}"

