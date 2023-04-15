#!/bin/bash

# This file is a part of the TChecker project.
#
# See files AUTHORS and LICENSE for copyright details.

# Check parameters

K=10

function usage() {
    echo "Usage: $0 N";
    echo "       N number of processes"
}

if [ $# -eq 1 ]; then
    N=$1
else
    usage
    exit 1
fi

# Labels
labels="green,unreachable"
echo "#labels=${labels}"

# Model

echo "#clock:size:name
#int:size:min:max:init:name
#process:name
#event:name
#location:process:name{attributes}
#edge:process:source:target:event:{attributes}
#sync:events
#   where
#   attributes is a colon-separated list of key:value
#   events is a colon-separated list of process@event
"

echo "system:job_shop_${N}
"

# Events

echo "event:tau
"

# Global variables

echo "int:1:0:2:0:id
"

# Processes

for j in `seq 1 $N`;do
    echo "# Job $j
process:J${j}
clock:1:x${j}1
clock:1:y${j}1
clock:1:x${j}2
clock:1:y${j}2
clock:1:z${j}"

    echo "location:J${j}:i1{initial:}
location:J${j}:w1
location:J${j}:i2
location:J${j}:w2
location:J${j}:i3{committed:}
location:J${j}:done{labels:green}
location:J${j}:unreach{labels:unreachable}"

echo "edge:J${j}:i1:w1:tau{provided: id==0 : do: x${j}1=0;z${j}=0;id=2}
edge:J${j}:w1:i2:tau{provided: x${j}1>=1&&x${j}1<=8 : do: y${j}1=0;id=0}
edge:J${j}:i2:w2:tau{provided: id==0 : do: x${j}2=0;id=1}
edge:J${j}:w2:i3:tau{provided: x${j}2>=6&&x${j}2<=11 : do: y${j}2=0;id=0}
edge:J${j}:i3:done:tau{provided: x${j}1-y${j}1>=5&&x${j}2-y${j}2<=7&&z${j}<=20}
edge:J${j}:i3:done:tau{provided: x${j}1-y${j}1<=2&&x${j}2-y${j}2>=10&&z${j}<=20}
"
done