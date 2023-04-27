#A is eca, A' is TA

import sys
import re
import functools
import epa_helper as ph
import era_helper as rh
import read_input_file as rif
import write_output_file as wof

def get_all_out_edges_ECA(edges, state, ed_pre_state_no=0):
	"""
	With a state in ECA, return all edges going out of it in that ECA

	Parameters:
	edges (list): List of all edges in ECA
	state (str): A state label
	ed_pre_state_no (default=0) (int): Pre state number in edges
	
	Returns:
	list: List of all edges having pre-state as state
	"""

	#given a state in A, return all edges going out of it in A
	
	all_out_edges = list( filter(lambda edge:edge[ed_pre_state_no]==state, edges) )
	
	return all_out_edges
	

if __name__=="__main__":
	
	file_name = sys.argv[1]
	
	all_content = rif.get_all_contents_input_file(file_name)
	#process, initial_state, states, state_labels, event, edge, system
	####edge = [(pre, post, letter, erc_guard, epa_guard)] #bot is given by -1

	process = all_content[0]
	initial_state = all_content[1]
	states = all_content[2]
	state_labels = all_content[3]
	all_event = all_content[4]
	edges = all_content[5]
	###(pre, post, letter, erc_guard, epa_guard) #bot is given by -1
	system = all_content[6]

	ed_pre_state = 0
	ed_post_state = 1
	ed_event_no = 2
	ed_erc_guard_no = 3
	ed_epa_guard_no = 4
		
	

	new_states = [(initial_state,[-1]*len(all_event),set())]#contains all explored or need to explore new states
	#A' has states of the form (state, bots_from_era, set_of_obligations_from_epa)

	st_label_no = 0
	st_bot_no = 1
	st_obli_no = 2
	
	new_edges = []
	#A' (pre, post, action, reset, guard)
	
	explore_stack= new_states.copy()
	#initial_state_label, empty set

	while explore_stack!=[]:
		
		explore_state = explore_stack.pop()
		
		out_edges = get_all_out_edges_ECA(edges, explore_state[st_label_no])
		
		for edge in out_edges:
			event = edge[ed_event_no]
			
			#epa action:
			pre_obligations = list(explore_state[st_obli_no])
			obligation_event = ph.get_obligations_event(list(explore_state[st_obli_no]),event)
			
			allow_next_state = not(ph.is_obligation_bot(obligation_event))

			
			obligations_to_advance = list(set(pre_obligations)-set(obligation_event))
			
			new_obligations = ph.get_new_obligations(edge[ed_epa_guard_no],obligations_to_advance)
			obligations_to_add = new_obligations[0]
			obligations_to_rem = new_obligations[1]
			
			post_obligations = list(list(set(obligations_to_advance)-set(obligations_to_rem))+list(obligations_to_add))
			
			p_guard = []
			p_reset = []

			if allow_next_state:
				obligation_event_to_guards = list(map(lambda event_oblig:ph.convert_obligation_clock_to_guard(event_oblig),obligation_event)) 
				p_guard = obligation_event_to_guards
				p_reset = obligations_to_add
				

			#era action
			pre_bots = explore_state[st_bot_no]
			erc_guard = edge[ed_erc_guard_no]
			events = all_event
			n_erc_action = rh.get_n_action(pre_bots, erc_guard, events, event)
			
			
			#new state
			if allow_next_state and n_erc_action!=tuple():
				
				(post_bots, r_reset, r_guard) = n_erc_action
				
				next_state_label = edge[ed_post_state]
				next_state_bots = post_bots
				next_state_obli = set(post_obligations)
				
				next_state = (next_state_label,next_state_bots,next_state_obli)
				
				
				if next_state not in new_states:
					explore_stack.append(next_state)
					new_states.append(next_state)


				final_guard = r_guard + p_guard
				final_reset = r_reset + p_reset
				
				
				#pre(str), post(str), action(str), reset(list), guard(list)
				new_edges.append((explore_state, next_state, event, final_reset, final_guard))
	

	wof.write_op(system, all_event, process, new_states, states, state_labels, new_edges)
				
				
				
				
				
				
