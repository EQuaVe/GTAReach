#!/bin/bash

# This file is a part of the TChecker project.
#
# See files AUTHORS and LICENSE for copyright details.

# Generates a TChecker model for preemptive scheduling with Earliest Deadline First schedule.

# Checks command line arguments
if [ $# -le 1 ];
then
	echo "Usage: $0 <n> <c1> <d1> ... <cn> <dn>";
	echo " ";
	echo "where:    <n>            is the number of task types";
	echo "          <ci>           is the computation time of task i";
	echo "          <di>           is the deadline for task i";
	echo " ";
	exit 0;
else
    REQUIREDARGUMENTS=$((($1 * 2) + 1))
    if [ $# -ne ${REQUIREDARGUMENTS} ];
    then
        echo "Usage: $0 <n> <c1> <d1> ... <cn> <dn>";
        echo " ";
        echo "where:    <n>            is the number of task types";
        echo "          <ci>           is the computation time of task i";
        echo "          <di>           is the deadline for task i";
        echo " ";
        exit 0;
    fi
fi

ARGUMENTS=("$@")
NTASKS=${ARGUMENTS[0]}

for i in `seq 1 $NTASKS`; do
    COMPUTETIME[i]=${ARGUMENTS[$((2*$i - 1))]}
    DEADLINE[i]=${ARGUMENTS[$((2*$i))]}
done
max=${DEADLINE[1]}
for i in `seq 1 $NTASKS`; do
    if [ ${max} -le ${DEADLINE[${i}]} ] 
    then
        max=${DEADLINE[${i}]}
    fi
done
MAXDEADLINE=$((${max}+1))
DEADLINE[0]=${MAXDEADLINE}

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

echo "system:schedulability
"

SYSTEMSTATEMENT="# ${NTASKS} tasks:"
for i in `seq 1 ${NTASKS}`; do
    SYSTEMSTATEMENT="${SYSTEMSTATEMENT} (${COMPUTETIME[${i}]},${DEADLINE[${i}]})"
done
echo "${SYSTEMSTATEMENT}
"


# Events

echo "# Events

event:tau"

for tid in `seq 1 $NTASKS`; do
echo "event:release${tid}
event:run${tid}
event:done${tid}
event:error${tid}"
done
echo ""

# Scheduler

echo "
# EDF Scheduler"

# process
echo "
process:scheduler"

# clocks
for i in `seq 1 ${NTASKS}`; do
    echo "clock:1:dpr${i}"
done
echo "clock:1:dpr0"

# integer variables
echo "int:1:0:${NTASKS}:0:queuedtasks"
for i in `seq 1 ${NTASKS}`; do
    echo "int:1:0:1:0:queued${i}"
done

# locations
echo "location:scheduler:qempty{initial:}
location:scheduler:qnonempty{committed:}
location:scheduler:taskrunning{}"

for i in `seq 1 $NTASKS`; do
    for j in `seq 0 ${i}`; do
        echo "location:scheduler:temp${i}${j}{committed:}"
    done
done

# Transitions
for i in `seq 1 $NTASKS`; do
    echo "edge:scheduler:qempty:temp${NTASKS}${i}:release${i}{do: dpr${i}=0;queuedtasks=1}"
done

echo "edge:scheduler:qnonempty:temp10:tau{provided: queued1==0}"
echo "edge:scheduler:qnonempty:temp11:tau{provided: queued1==1}"

for i in `seq 2 ${NTASKS}`; do
    echo "edge:scheduler:temp$((${i}-1))0:temp${i}0:tau{provided: queued${i}==0}"
    echo "edge:scheduler:temp$((${i}-1))0:temp${i}${i}:tau{provided: queued${i}==1}"
    for j in `seq 1 ${i}`; do
        if [ $((${i}-1)) -ge ${j} ]; then
            echo "edge:scheduler:temp$((${i}-1))${j}:temp${i}${j}:tau{provided: queued${i}==0}"
            echo "edge:scheduler:temp$((${i}-1))${j}:temp${i}${j}:tau{provided: queued${i}==1&&dpr${i}-dpr${j}<$((${DEADLINE[${i}]}-${DEADLINE[${j}]}))}"
        fi
    done
    for j in `seq 1 $((${i}-1))`; do
        echo "edge:scheduler:temp$((${i}-1))${j}:temp${i}${i}:tau{provided: queued${i}==1&&dpr${j}-dpr${i}<=$((${DEADLINE[${j}]}-${DEADLINE[${i}]}))}"
    done
done
for i in `seq 1 ${NTASKS}`; do
    echo "edge:scheduler:temp${NTASKS}${i}:taskrunning:run${i}{}"
done
for i in `seq 1 ${NTASKS}`; do
    echo "edge:scheduler:taskrunning:qnonempty:release${i}{do: dpr${i}=0;queuedtasks=1+queuedtasks}"
    echo "edge:scheduler:taskrunning:qnonempty:done${i}{provided: queuedtasks>1 : do: queuedtasks=-1+queuedtasks}"
    echo "edge:scheduler:taskrunning:qempty:done${i}{provided: queuedtasks==1 : do: queuedtasks=0}"
done

# Automata for tasks

echo "
# Automata for Tasks"

for tid in `seq 1 $NTASKS`; do
echo "
# Task ${tid}

process:task${tid}
clock:1:c${tid}
clock:1:d${tid}
location:task${tid}:free{initial:}
location:task${tid}:queued{}
location:task${tid}:running{invariant: c${tid}<=${COMPUTETIME[${tid}]}&&d${tid}<=${DEADLINE[${tid}]}}
location:task${tid}:preempted{invariant: d${tid}<=${DEADLINE[${tid}]}}
location:task${tid}:errorstate{}
edge:task${tid}:free:queued:release${tid}{do: d${tid}=0;queued${tid}=1 }
edge:task${tid}:queued:running:run${tid}{do: c${tid}=0}"
for i in `seq 1 ${NTASKS}`; do
    if [ ${i} != ${tid} ]
    then
        echo "edge:task${tid}:running:preempted:release${i}{provided: c${tid}<${COMPUTETIME[${tid}]}}"
        echo "edge:task${tid}:preempted:preempted:run${i}{}"
        echo "edge:task${tid}:preempted:preempted:done${i}{provided: c${tid}<=${DEADLINE[${tid}]}&&c${tid}>=${COMPUTETIME[${i}]} : do: c${tid}=-${COMPUTETIME[${i}]}+c${tid}}"
    fi
done
echo "edge:task${tid}:preempted:running:run${tid}{}"
echo "edge:task${tid}:running:errorstate:error${tid}{provided: d${tid}>=${DEADLINE[${tid}]}&&c${tid}<${COMPUTETIME[${tid}]}}"
echo "edge:task${tid}:preempted:errorstate:error${tid}{provided: d${tid}>=${DEADLINE[${tid}]}}"
echo "edge:task${tid}:running:free:done${tid}{provided: c${tid}>=${COMPUTETIME[${tid}]} : do: queued${tid}=0}"
for i in `seq 1 ${NTASKS}`; do
    if [ ${i} != ${tid} ]
    then
        echo "edge:task${tid}:free:free:run${i}{}"
        echo "edge:task${tid}:free:free:done${i}{}"
        echo "edge:task${tid}:free:free:release${i}{}"
        echo "edge:task${tid}:queued:queued:run${i}{}"
        echo "edge:task${tid}:queued:queued:done${i}{}"
        echo "edge:task${tid}:queued:queued:release${i}{}"
        echo "edge:task${tid}:preempted:preempted:run${i}{}"
        echo "edge:task${tid}:preempted:preempted:release${i}{}"
    fi
done

done

# Automaton triggering tasks (Flower automaton)

echo "
# Task Automaton

process:taskautomaton
location:taskautomaton:init{initial:}"
for i in `seq 1 ${NTASKS}`; do
    echo "edge:taskautomaton:init:init:release${i}{provided: queued${i}==0}"
done

# Automaton triggering tasks (Sequential releases)

# echo "# Task Automaton

# process:taskautomaton
# location:taskautomaton:init{initial:}"
# for i in `seq 1 $NTASKS`; do
#     echo "location:taskautomaton:released${i}{}"
# done
# echo "edge:taskautomaton:init:released1:release1{}"
# if [ ${NTASKS} -gt 1 ]; then
#     for i in `seq 1 $(($NTASKS - 1))`; do
#         echo "edge:taskautomaton:released${i}:released$((${i}+1)):release$((${i}+1)){}"
#     done
# fi

# ERROR automaton

echo "
# ERROR automaton

process:errorautomaton
location:errorautomaton:init{initial:}
location:errorautomaton:final{labels: error}"
for i in `seq 1 ${NTASKS}`; do
    echo "edge:errorautomaton:init:final:error${i}{}"
    echo "edge:errorautomaton:final:final:error${i}{}"
done

# Automaton to track the timings of synchronizations

echo "
# Timing automaton

process:timingautomaton
location:timingautomaton:timeinit{initial:}
clock:1:t"

# Synchronizations

echo "
# Synchronizations
"

for i in `seq 1 ${NTASKS}`; do
    SYNCEXPR="sync:taskautomaton@release${i}:scheduler@release${i}"
    for j in `seq 1 ${NTASKS}`; do
        SYNCEXPR="${SYNCEXPR}:task${j}@release${i}"
    done
    echo "${SYNCEXPR}"
done

for i in `seq 1 $NTASKS`; do
    SYNCEXPR="sync:scheduler@run${i}:task${i}@run${i}"
    for j in `seq 1 $NTASKS`; do
        if [ $i != $j ]; then
            SYNCEXPR="${SYNCEXPR}:task${j}@run${i}"
        fi
    done
    echo "${SYNCEXPR}"
done

for i in `seq 1 $NTASKS`; do
    SYNCEXPR="sync:task${i}@done${i}:scheduler@done${i}"
    for j in `seq 1 $NTASKS`; do
        if [ $i != $j ]; then
            SYNCEXPR="${SYNCEXPR}:task${j}@done${i}"
        fi
    done
    echo "${SYNCEXPR}"
done

for i in `seq 1 $NTASKS`; do
    echo "sync:task${i}@error${i}:errorautomaton@error${i}"
done

echo ""
