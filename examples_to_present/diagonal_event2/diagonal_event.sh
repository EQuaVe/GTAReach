#!/bin/bash
function usage() {
    echo "Usage: $0 K";
    echo "       K maximum constant";

}

if [ $# -eq 1 ]; then
    K=$1
else
    usage
    exit 1
fi

echo "system:diagonal_event_$K


event:a:1:0
event:b:0:1
event:c

process:P
location:P:l0{initial:}
location:P:l1{}
location:P:l2{}
location:P:l3{labels: green}


edge:P:l0:l1:a{{}}
edge:P:l1:l2:c{{provided:a_h-b_p==$K;}}

edge:P:l2:l2:a{{provided:a_h==1;}}
edge:P:l2:l3:b{{}}"

