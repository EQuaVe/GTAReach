#!/bin/bash
function usage() {
    echo "Usage: $0 N";
    echo "       K number of big loops";

}

if [ $# -eq 1 ]; then
    N=$1
else
    usage
    exit 1
fi

echo "system:diagonal_event_$N"

for (( i=1;i<=$N;i++ )) do
	echo "event:a$i:1:1"
done
echo "event:a$((N+1)):0:1"


echo "event:b:1:0"

echo "process:P
location:P:q0{initial:}
location:P:q$((2*$N+1))"

echo ""
for (( i=1;i<=$N;i=$i+1 )) do
	loc1=$((2*$i-1))
	loc2=$((2*$i))
	echo "location:P:q$loc1"
	echo "location:P:q$loc2"
	
	echo "edge:P:q$loc1:q$loc2:b{{}}"
	echo "edge:P:q$loc2:q$loc2:a$i{{provided: a$i""_h==1;}}"
	echo "edge:P:q$loc2:q$loc1:a$i{{provided: a$i""_h-b_h<=-10 && a$(($i+1))_p==0;}}"
	echo ""
done

for (( i=1;i<$N;i=$i+1 )) do
	echo ""
	loc1=$((2*$i-1))
	loc2=$((2*$i+1))
	echo "edge:P:q$loc1:q$loc2:a$(($i+1)){{provided: a$(($i+1))_p>=-1;}}"
done

echo "edge:P:q0:q1:a1{{provided: a1_p>=-1;}}"
echo "edge:P:q$((2*$N-1)):q$((2*$N+1)):a$(($i+1)){{}}"

exit 1
