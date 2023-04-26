def get_clocks(edges):
	"""
	Returns a list of all clocks used in TA

	Parameters:
	list: A list of all edges in TA

	Returns:
	list: A list of all clocks in TA
	"""
	clocks = []
	edges_reset_no = 3
	for edge in edges:
		clocks+=edge[edges_reset_no]
		# print(edge)
	clocks = set(clocks)-set([""])
	
	clocks = list(clocks)
	for i in range(0,len(clocks)):
		clocks[i] = clocks[i].replace(" ","")
	return clocks

def write_op(system, events, process, states, orig_states, labels,edges):
	"""
	Output TA into stdout in TChecker format

	Parameter:
	system (str): system name
	events (list): list of all events
	process (str): process name
	states (list): list of all states in TA
	orig_states (list): list of all states in ECA
	labels (list): labels of all states in ECA
	edges (list): list of edges in TA
	
	Returns:
	None
	"""
	#states = [(state, bots_from_era, set_of_obligations_from_epa)]
	print("system",system, sep=":")
	print('\n',end="")

	clocks = get_clocks(edges)
	for i in clocks:
		print("clock:1",i, sep=":")
	print('\n',end="")
	
	for i in events:
		print("event",i,sep=":")
	print('\n',end="")
	
	print("process:", process, sep="")
	print('\n',end="")


	init_str_state = states[0][0] + str(states[0][1])[1:-1].replace("-1","0")
	init_str_state = init_str_state.replace(" ","").replace(",","")
	print("location",process,init_str_state,sep=":",end="")
	
	if labels[orig_states.index(states[0][0])]!="":
		print("{initial:,"+labels[orig_states.index(states[0][0])]+"}")
	else:
		print("{initial:}")

	output_states = [init_str_state]
	for i in states[1:]:
		str_state = i[0] + str(i[1])[1:-1].replace("-1","0") + str("_".join(list(i[2])))
		str_state = str_state.replace(" ","").replace(",","")
		output_states.append(str_state)
		op_label = labels[orig_states.index(i[0])]
		print("location",process, str_state, sep=":",end="")
		print("{"+op_label+"}")

	
	print('\n',end="")
	edge_pre_no=0
	edge_post_no=1
	edge_action_no=2
	edge_reset_no=3
	edge_guard_no=4
	for edge in edges:
		#(pre, post, action, reset, guard)
		print("edge:"+process+":",sep="",end="")
		
		str_pre = output_states[states.index(edge[edge_pre_no])]
		str_post = output_states[states.index(edge[edge_post_no])]
		ed_action = edge[edge_action_no]
		ed_reset = edge[edge_reset_no]
		ed_guard = edge[edge_guard_no]
		print(str_pre,str_post,ed_action,sep=":",end="")
		print("{",end="")
		if ed_guard!=[]:
			print("provided:"," && ".join(ed_guard),sep="",end="")
		if ed_reset!=[]:
			if ed_guard!=[]:
				print(" : do: ",end="")
			else:
				print("do: ",end="")
			
			str_ed_reset = ""
			for i in range(0,len(ed_reset)):
				# ed_reset[i]=ed_reset[i].replace(" ","")
				str_ed_reset = str_ed_reset + ed_reset[i] +"=0"+"; "
			
			str_ed_reset = str_ed_reset[:-2]
			print(str_ed_reset,end="")
		print("}")





