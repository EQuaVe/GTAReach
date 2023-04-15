#!/bin/bash
function usage() {
    echo "Usage: $0 N";
    echo "       N number of trains";

}

if [ $# -eq 1 ]; then
    N=$1
else
    usage
    exit 1
fi

echo "system:train_gate_"$N


echo "
event:lower:1:0
event:raise:1:0
event:down:0:0
event:up:0:0
event:approach:1:0
event:exit:1:0
"

for (( i=1; i<=$N; i++ ))
do
 echo "event:approach$i:1:0
event:in$i:0:0
event:out$i:0:0
event:exit$i:1:0
"
done




for (( i=1; i<=$N; i++ ))
do
 echo "process:T$i
location:T$i:t0{initial:}
location:T$i:t1{invariant: approach$i""_h<=5}
location:T$i:t2{invariant: approach$i""_h<=5}
location:T$i:t3{invariant: approach$i""_h<=5}
"

 echo "edge:T$i:t0:t1:approach$i""{{}}
edge:T$i:t1:t2:in$i""{{ provided: approach$i""_h>2;}}
edge:T$i:t2:t3:out$i""{{}}
edge:T$i:t3:t0:exit$i""{{}}
"
done

echo "process:G
location:G:g0{initial:}
location:G:g1{invariant: lower_h<=5}
location:G:g2{}
location:G:g3{invariant: raise_h<=2}

process:C
location:C:c0{initial:}
location:C:c1{invariant: approach_h<=1}
location:C:c2{}
location:C:c3{invariant: exit_h<=2}

edge:G:g0:g1:lower{{}}
edge:G:g1:g2:down{{}}
edge:G:g2:g3:raise{{}}
edge:G:g3:g0:up{{provided: raise_h>=1; }}

edge:C:c0:c1:approach{{}}
edge:C:c1:c2:lower{{ provided:approach_h==1; }}
edge:C:c2:c3:exit{{}}
edge:C:c3:c0:raise{{}}

sync:G@lower:C@lower
sync:G@raise:C@raise
"

for (( i=1; i<=$N; i++ ))
do
 echo "sync:T$i@approach$i:C@approach
sync:T$i@exit$i:C@exit"
done
