BEGIN  {
	FS=":"
}
END 	{
	
	 #print "---------------------"
	 #print "FILENAME (name of ip file):" FILENAME
	 #print "OFS (output field separator):" OFS
	 #print "ORS (output record separator):" ORS
	 #print "FS (input field separator):" FS
	 #print "RS (input record separator):" RS
}

$1 ~ /^ *clock/ {print "clock:normal:"$3}


$1 !~ /^ *clock/ && $1 ~ /^ *edge/ {
	tmp = 0
	OFS = ""
	ORS = ""
	for(i=1;i<5;i++)
		print $i":"
	s = split($5, a, "{")
	#print "ANI:"a[2]"\n"
	#print $5"_:split:_"$s"_"a[1]"_"a[2]"_______"
	if (length(a)>1 && !(a[2] ~ /^ *} *$/)){
		if (a[2]=="do"){
			print a[1]"{{provided:;"a[2]":"
			tmp = 1
		}
		else
			print a[1]"{{"a[2]":"
	}
	else{
			print a[1]"{{}}"
	}
	if (length(a)>1 && !(a[2] ~ /^ *} *$/)){
		for(i=6;i<=NF-1;i++){
			if ($i ~ /^ *provided *$/){
				tmp = 0
			}
			if ($i ~ /^ *do *$/){
				print $i":"
			#	print "ANI:before tmp="tmp"\n"
				tmp = 1
			#	print "ANI:changed tmp="tmp"\n"
			}
			#print "ANI:JERE:" tmp","$i"\n"
			if (!($i ~ /^ *do *$/)){
	#			print "ANI:tmp=" tmp"\n"
				if (tmp==1){
					print gensub(/=[0]+/, "", "g", $i)";"
				}
				else{
					print $i";"
				}
			}
		}
		if (tmp==1){
			newstr = gensub(/=[0]+/, "", "g", $NF)
			newstr = gensub(/;/, ",", "g", newstr)"\n"
		}
		else{
			newstr = $NF
		}
		#print "HHHHHHHHH"$NF
		s1 = split(newstr, b, "}")
		print b[1]";}}"
	}
	
	ORS = "\n"
	OFS = " "
	print ""
	#print NF
	#print $5
}

$1 !~ /^ *clock/ && $1 !~ /^ *edge/ {
	print $0
}
