import sys
import re


file_name = sys.argv[1]
file_inp=open(file_name)
all_lines = file_inp.readlines()
file_inp.close()

history_clocks = []
prophecy_clocks = []

for i in range(0,len(all_lines)):
	all_lines[i]=all_lines[i].strip("\n").lstrip()
	all_lines[i]=all_lines[i].split(" ")

# print(all_lines)
for i in range(0,len(all_lines)):
	# print(all_lines[i][1])
	if i==1:
		
		if len(sys.argv)>2:
			print("node_legend [shape=record, label=\"{",end="")
			file_name2 = sys.argv[2]
			file_inp2=open(file_name2)
			all_lines2 = file_inp2.readlines()
			file_inp2.close()
			all_clocks = []
			for j in range(0,len(all_lines2)):
				all_lines2[j] = all_lines2[j].strip("\n").lstrip()
				all_lines2[j] = re.sub(" ","",all_lines2[j])
				
				if all_lines2[j][:5] == "clock":
					all_clocks.append(all_lines2[j][6:])
				
				if all_lines2[j][:5] == "clock" and (all_lines2[j].split(":")[1]=="timer" or all_lines2[j].split(":")[1]=="prophecy"):
					prophecy_clocks.append(all_lines2[j].split(":")[2])
				
				if all_lines2[j][:5] == "clock" and (all_lines2[j].split(":")[1]=="normal" or all_lines2[j].split(":")[1]=="history"):
					history_clocks.append(all_lines2[j].split(":")[2])
				
				elif all_lines2[j][:5] == "event":
					if len(all_lines2[j].split(":"))>2:
						if all_lines2[j].split(":")[2]=="1":
							all_clocks.append("event history:"+all_lines2[j].split(":")[1]+"_h")
							history_clocks.append(all_lines2[j].split(":")[1]+"_h")
						if all_lines2[j].split(":")[3]=="1":
							all_clocks.append("event prophecy:"+all_lines2[j].split(":")[1]+"_p")
							prophecy_clocks.append(all_lines2[j].split(":")[1]+"_p")				
			print(" | ".join(all_clocks),end="}\"]\n")

	if len(all_lines[i])>1:
		if all_lines[i][1][0]=="[":
			###this is a node/state in zone graph

			label = all_lines[i][2].split("=")[1][1:-2]
			# print(label)

			zone = " ".join(all_lines[i][3:]).split("\"")[1]
			rem_tmp_zone = zone.split("&")
			
			rem_tmp_zone = [re.sub(" ","", z) for z in rem_tmp_zone]
			rem_tmp_zone[0] = rem_tmp_zone[0][1:]
			rem_tmp_zone[-1] = rem_tmp_zone[-1][:-1]
			
			# print(rem_tmp_zone)
			# print()
			rem_zone = []
			for j in rem_tmp_zone:
				if "tmp" not in re.split("<=|>|>=|=|<|-",j):
					rem_zone+=[j]
			
			# print("ani:", l_z,i, len(all_lines),all_lines,sep="\n \n")
			# print("ANI:rem_tmp:",rem_tmp_zone)
			# print("ANI:rem:",rem_zone)
			rem_zone2 = []
			for j in rem_zone:
				# print("ANI:",j,re.search("<=",j))
				if re.search("<=",j)!=None:
					if len(j.split("<="))==3 and j.split("<=")[0]=="-inf" and j.split("<=")[2]=="inf":
						# print("ANI:",j)
						pass
					elif len(j.split("<="))==3 and j.split("<=")[0]=="-inf" and (j.split("<=")[1] in prophecy_clocks) and j.split("<=")[2]=="0":
						# print("ANI:",j)
						pass
					elif len(j.split("<="))==3 and j.split("<=")[0]=="0" and (j.split("<=")[1] in history_clocks) and j.split("<=")[2]=="inf":
						# print("ANI:",j)
						pass
					elif len(j.split("<="))==3 and j.split("<=")[0]=="0" and len(j.split("<=")[1].split("-"))>1 and (j.split("<=")[1].split("-")[0] in history_clocks) and (j.split("<=")[1].split("-")[1] in prophecy_clocks) and j.split("<=")[2]=="inf":
						pass
					elif len(j.split("<="))==3 and j.split("<=")[0]=="-inf" and len(j.split("<=")[1].split("-"))>1 and (j.split("<=")[1].split("-")[0] in prophecy_clocks) and (j.split("<=")[1].split("-")[1] in history_clocks) and j.split("<=")[2]=="0":
						pass
					else:
						rem_zone2.append(j)
				else:
					rem_zone2.append(j)
			# print("ANI:", rem_zone2)
			# print("ANI:")
			zone = " &\n ".join(rem_zone2)
			l_z = label+" \\n"+zone
			
			print(all_lines[i][0]+ " " + "[ label=\""+ l_z + "\"]")
		else:
			###this is an edge
			# print(all_lines[i])
			all_lines[i][-1] = re.sub("vedge=\"", "label=\"", all_lines[i][-1],1)
			if len(all_lines[i])>4 and "subsumption" in all_lines[i][3]:
				# print(all_lines[i])
				# print(all_lines[i][:-1])
				all_lines[i] = all_lines[i][:-1] + ['color=blue,'] + [all_lines[i][-1]]
			# print("ani:", all_lines[i])
			
			print(" ".join(all_lines[i]))

	else:
		#do blue edges for simulation relation
		# print("ani: ", all_lines[i])
		print(" ".join(all_lines[i]))
# print(history_clocks)
# print(prophecy_clocks)
