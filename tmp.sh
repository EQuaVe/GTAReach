
echo "
system:ad94_fig10

clock:1:x
clock:1:y
"

for (( c=0; c<100; c++ )) do

	echo "event:a$c"
done

echo "process:P
location:P:q0{initial:}"

for (( c=0; c<100; c++ )) do

	echo "edge:P:q0:q0:a$c{do:y=0}"
done

