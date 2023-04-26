import re
import sys

def get_er_ep_guard(guard):
	"""
	Get era and epa guard from a string of guards

	Parameters:
	guard (str): A string of guards. Eg. "xa<2 && yb>10 && xc<=100"

	Returns:
	(str, str): The first string is er guards joined by &&
				The second string is ep guards joined by &&
	"""
	guard = guard.replace(" ","")
	guard_list = guard.split("&&")
	erc_guard = []
	epa_guard = []
	for guards in guard_list:
		if guards!="":
			if guards[0]=="x":
				erc_guard.append(guards)
			elif guards[0]=="y":
				###converting == to <= and >=
				if "==" in guards:
					guard1 = re.sub("==","=<",guards[::-1],1)[::-1]
					guard2 = re.sub("==","=>",guards[::-1],1)[::-1]
					epa_guard.append(guard1)
					epa_guard.append(guard2)
				else:
					epa_guard.append(guards)
	return erc_guard, epa_guard
	
	
def get_all_contents_input_file(file_name):
	"""
	Read input ECA file.

	Parameters:
	file_name (str): Path to the file

	Returns:
	str: process name in the file
	str: the initial_state name of ECA 
	list: all states names in ECA
	list: all state labels in ECA
	list: all events in ECA
	list: all edges in ECA
	str: the system name
	"""

	# print("Input File Name:",file_name)
	
	file_inp=open(file_name)
	all_lines = file_inp.readlines()
	for i in range(0,len(all_lines)):
		all_lines[i]=all_lines[i].strip("\n")
		all_lines[i]=all_lines[i].replace(" ","")

	i=0
	while i!=len(all_lines):
		if all_lines[i]=="" or all_lines[i][0]=='#':
			del all_lines[i]
		else:
			i=i+1 

	# print("All important input file contents", all_lines)
	# print("------Input File Details------")

	system = all_lines[0].split(":")[1]
	# print("SYSTEM:",system) #system name

	event = []
	process = ""
	states = []
	edge = [] ###(pre, post, letter, erc_guard, epa_guard) #bot is given by -1
	states_labels = []
	initial_state = ""
	
	for i in all_lines:
		if "event:" in i:
			event.append(i.split(":")[1])
		if "process:" in i:
			process = i.split(":")[1]
		if "location:" in i:
			curr_state = ":".join(i.split(":")[2:]).split("{")[0]
			curr_label = i.split("{")[1][:-1]
			
			states.append(curr_state)#without labels
			states_labels.append(curr_label)
			
			if "initial" in curr_label:
				initial_state = curr_state
				# TO DO: FIX INITIAL STATE EDITS
				states_labels[-1]=re.sub("initial: *,?","",states_labels[-1])
			
		if "edge:" in i:
			#edge:P:l3:l1:a{b<1}
			pre = i.split(":")[2]
			post = i.split(":")[3]
			letter = i.split(":")[4].split("{")[0]
			
			#should be no provided keyword
			guard = i.split(":")[4].split("{")[1].strip("}")
			erc_guard, epa_guard = get_er_ep_guard(guard)
			edge.append((pre, post, letter, erc_guard, epa_guard))	

	# print("SYSTEM:",system)
	# print("PROCESS:",process)
	# print("EDGES:",edge)
	# print("EVENTS:",event)
	# print("STATES:",states)
	# print("STATE LABLES:",states_labels)
	# print("INITIAL STATE",initial_state)
	
	
	return process, initial_state, states, states_labels, event, edge, system

if __name__=="__main__":
	process, initial_state, states, states_labels, event, edge, system = get_all_contents_input_file(sys.argv[1])
	
	print("SYSTEM:",system)
	print("PROCESS:",process)
	print("EVENTS:",event)
	print("STATES:",states)
	print("STATE LABLES:",states_labels)
	print("INITIAL STATE",initial_state)
	print("EDGES:",edge)
	# print("++++++++++++++++++++++++")