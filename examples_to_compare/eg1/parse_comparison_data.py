
import sys
import re


file_name = sys.argv[1]
file_inp=open(file_name)
all_lines = file_inp.readlines()
file_inp.close()

input_name = []
gsim = []
gen = []

for i in range(0,len(all_lines)):
	all_lines[i]=all_lines[i].strip("\n").lstrip()
	
#removing empty strings from python
while "" in all_lines:
	all_lines.remove("")


for i in range(0,len(all_lines)):
	print(i, all_lines[i])

print()
print()
print()

i = 0
while i<len(all_lines):
	#print(i, all_lines[i])
	if "__" in all_lines[i]:
		#start gsim
		i+=1
		line = all_lines[i]
		max_const = line.split(" ")[0].split(":")[1]
		edges = line.split(" ")[1].split(":")[1]
		input_name.append([max_const, edges])
		
		i+=2
		line = all_lines[i]
		if "-" in line:
			gsim.append(["TIMEOUT","TIMEOUT","TIMEOUT","TIMEOUT"])
		else:
			#print("ani:46", line)
			covered_states = line.split(" ")[1]
			
			i+=1
			line = all_lines[i]
			running_time = line.split(" ")[1]
			
			i+=1
			line = all_lines[i]
			stored_states = line.split(" ")[1]
			
			i+=1
			line = all_lines[i]
			visited_states = line.split(" ")[1]
			
			gsim.append([covered_states, running_time, stored_states, visited_states])
			i+=1
			
			#print("GSIM", covered_states, running_time, stored_states, visited_states, sep=":")
			#print("-" in all_lines[i])
	if "-" in all_lines[i]:
		#start gen
		
		i+=2
		line = all_lines[i]
		#print("ani:71",line)
		if "__" in line:
			gen.append(["TIMEOUT","TIMEOUT","TIMEOUT","TIMEOUT"])
			
		else:
			#print("ani:75", line)
			covered_states = line.split(" ")[1]
			
			i+=1
			line = all_lines[i]
			running_time = line.split(" ")[1]
			
			i+=1
			line = all_lines[i]
			stored_states = line.split(" ")[1]
			
			i+=1
			line = all_lines[i]
			visited_states = line.split(" ")[1]
			
			gen.append([covered_states, running_time, stored_states, visited_states])
			i+=1
	
for i in range(0,len(gsim)):
	#print(gsim[i], gen[i], input_name[i])
	print("K="+input_name[i][0]+",", "N="+input_name[i][1], end="")
	
	for j in range(0, len(gsim[i])):
		print(" & ", gsim[i][j], " & ", gen[i][j], end=" ")
	print("\\\\")

