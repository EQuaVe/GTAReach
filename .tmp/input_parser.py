import sys
import re
# print("file_name:", sys.argv[1])
file1 = open(sys.argv[1], 'r')
lines = file1.readlines()
file1.close()

for i in lines:
	if i.split(":")[0]=="system":
		system_name = i.split(":")[1].strip("\n")
nodes = []
nodes_names = []
nodes_labels = []
edges = []
edges_new = []
clocks = []
initial_nodes = []
for i in lines:
	i = i.strip("\n")
	i = i.replace(" ","")
	if i.split(":")[0]=="location":
		nodes.append(i.split(":")[2].split("{")[0])
		nodes_names.append(i.split(":")[1]+":"+i.split(":")[2].split("{")[0])
		nodes_labels.append(":".join(i.split(":")[2:]).split("{")[1][:-1])
		# print("ani:", i.split("{")[-1], "initial:" in i.split("{")[-1][:-1])
		if "initial:" in i.split("{")[-1][:-1]:
			initial_nodes.append(len(nodes)-1)
	
	elif i.split(":")[0]=="edge":
		# print(":".join(i.split(":")[4:]))
		edges.append([i.split(":")[2],i.split(":")[3],":".join(i.split(":")[4:]).split("{{")[0],":".join(i.split(":")[4:])])
		edges_new.append([i.split(":")[1]])

	elif i.split(":")[0]=="clock":
		clocks.append(":".join(i.split(":")[1:]))
	elif i.split(":")[0]=="event" and len(i.split(":"))>2:
		if i.split(":")[-2] == "1":
			event_history = "event history:"+i.split(":")[1]+"_h"
			clocks.append(event_history)

		if i.split(":")[-1] == "1":
			event_prophecy = "event prophecy:"+i.split(":")[1]+"_p"
			clocks.append(event_prophecy)
# print(clocks)
# for i in edges:
# 	print(i)


print("digraph", system_name, "{")
print("node_legend [shape=record, label=\"{ ", " | ".join(clocks), "}\"]")
print("init [label=\"\", shape=point]")


for i in range(0,len(nodes)):
	print(i, "[", "label=\"", nodes_names[i], "\n", nodes_labels[i], "\"]")

for i in initial_nodes:
	print("init ->", i, " [style=dashed, color=grey]")

for i in range(0,len(edges)):
	for j in range(0,len(nodes)):
		if edges_new[i][-1]+":"+edges[i][0]==nodes_names[j]:
			print(j,"->",end=" ")
			break
	for j in range(0,len(nodes)):
		if edges_new[i][-1]+":"+edges[i][1]==nodes_names[j]:
			print(j,end = "[label=\" ")
			break
	# print("\nani:432", edges[i][-1].split(";"),"::::::")
	tmp_edges = []
	for sub_edge in edges[i][-1].split(";"):
		
		if re.sub(" ","",sub_edge)[:2]=="do" and sub_edge!="do:":
			# print(sub_edge)#, [k for k in sub_edge.split[","]])
			# for k in sub_edge[3:].split(","):
				# if "=" not in k:
			tmp_edges.append("do:"+",".join(["["+i+"]" if "=" not in i else i for i in sub_edge[3:].split(",")]))
		else:
			tmp_edges.append(sub_edge)
	# print("ani:dfsd:",";".join(tmp_edges))
	full_edge = ";".join(tmp_edges)[:-2]
	full_edge = "{{".join(full_edge.split("{{")[1:])
	print(edges[i][-1].split("{{")[0],", ⟨", full_edge,sep="",end = "⟩\"]")
	print()
	# print("ani:2",edges[i][-1].split("{{")[0],", ⟨","".join(edges[i][-1].split("{{")[1:])[:-2],sep="",end = "⟩\"]")
	# print()
print()
print("}")


