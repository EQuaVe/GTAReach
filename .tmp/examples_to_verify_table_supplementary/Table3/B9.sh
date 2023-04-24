#!/bin/bash
function usage() {
    echo "Usage: $0 N";
    echo "       N number of bigloops";

}

if [ $# -eq 1 ]; then
    N=$1
else
    usage
    exit 1
fi

echo "system:big_loop_"$N

echo ""

echo "event:b"


for (( i=1; i<=$N; i++ ))
do
 echo "event:a$i:1:1"
done

echo ""
echo "clock:timer:t"

for (( i=1; i<=$N; i++ ))
do
 echo "clock:prophecy:p$i"
done

echo "process:P"
echo ""
echo "location:P:l0{initial:}"
echo ""
for (( i=1; i<=$N; i++ ))
do
 echo "location:P:l$i"
 echo "location:P:p$i""0"
 echo "location:P:p$i""1"
 echo "location:P:p$i""2"
 echo "location:P:p$i""3"
 echo ""
done


for (( i=1; i<=$N; i++ ))
do
 echo "edge:P:l0:p$i""0:b{{provided:a$i""_p==0 && t==0; do: t=-10;}}"
 echo "edge:P:p$i""0:p$i""1:b{{}}"
 echo "edge:P:p$i""1:p$i""2:b{{}}"
 echo "edge:P:p$i""2:l0:b{{provided:t==0;do:p$i,t;}}"
 echo "edge:P:p$i""1:p$i""3:a$i{{provided:a$i""_p==-1 && p$i==0; do:p$i;}}"
 echo "edge:P:p$i""3:p$i""1:a$i{{provided: p$i==0;}}"
 echo ""
done

for (( i=0; i<$N; i++ ))
do
 echo "edge:P:l$i:l$(($i+1)):b{{provided:a$(($i+1))""_h>0;}}"
done

