import re
import functools

def get_comp_op(guard):
	"""
	Returns the comparison operator in a guard
	
	Parameters:
	guard (str): A guard as string. Eg. "ya<-1"
	
	Returns:
	str: The comparison operator in the guard. Eg. "<"
	"""	
	if guard=="":
		return ""
	else:
		return re.search("(<=)|(>=)|(==)|(<)|(>)",guard).group()

def get_n_action(pre_bots, conds, events, event):
	"""
	Determine the ECA next state bots, edge reset and edge guards.

	Parameters:
	pre_bots (list): a list containing -1 and(or) 1.
	conds (list): a list containing guards
	events (list): a list containing all events
	event (str): current event

	Returns:
	list: post_bots containing -1 and(or) 1
	list:[reset] containing only one element "x{event}"
	list: n_guard containing all guards on the edge in TA

	"""

	
	#pre_bots = [-1,1]
	#conds = "c1&&c2"
	#event_index = 0...len(events)-1
	#action = "a"
	#post_bots = "[-1,1]"

	event_index = events.index(event)
	conditions=conds
	#print(conditions)
	#-1 is taken as bot
	
	ops = list(map(lambda cond: get_comp_op(cond),conditions))
	#print("OPERATIONS:",ops)
	
	is_next_state_npossible = False
	
	if conditions != [""]:
		for i in range(0,len(conditions)):
			#is_guard_on_action = (conditions[i].split(ops[i])[0][1:]==event)
			is_condition_bot = (conditions[i].split(ops[i])[1]=="-1")
			# print(conditions[i],ops[i],(conditions[i].split(ops[i])[0]))
			is_event_occured = (pre_bots[events.index(conditions[i].split(ops[i])[0][1:])]!=-1)

			is_next_state_npossible = (is_event_occured and is_condition_bot) or (not(is_event_occured) and not(is_condition_bot)) or is_next_state_npossible
			

	
	# print()
	# print("OUTPUT:")

	if is_next_state_npossible:
		return tuple()
	else:
		
		post_bots = pre_bots.copy()
		post_bots[event_index]=1
		
		guards=[]

		if conditions!= [""]:
			
			for i in range(0,len(conditions)):
				if conditions[i].split(ops[i])[1]!="-1":
					guards += [conditions[i]]
		
		reset = "x"+event
		# print("NEW RESET:",reset)
		
		#n_guard=" && ".join(guards)
		n_guard = guards
		# print("new guard:",n_guard)

		# print("==================================")
		return (post_bots, [reset], n_guard)


if __name__=="__main__":		
	conds = "xc==1"
	edge = ""
	events = ["a","b","c","d"]
	pre = ("l0",[-1,1,-1,-1])
	#pre[1][0]=1
	print(get_n_action(pre[1], conds, events,"c"))
