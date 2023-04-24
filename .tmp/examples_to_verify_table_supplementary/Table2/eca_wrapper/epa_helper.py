import re
import functools

def get_comp_guard(guard):
	"""
	Returns the comparison operator in a guard

	Parameters:
	guard (str): A string representing a guard

	Return:
	str: A string from "<","<=",">",">=" and "=="
	"""
	comp_op = re.search("(<)|(>)|(=>)|(=<)|(==)",guard[::-1]).group()
	return comp_op[::-1]

def get_guard_constant(guard):
	"""
	Returns the constant appearing in a guard
	
	Parameters:
	guard (str): A string representing a guard

	Return:
	int: An integer atleast -1 which is the constant in the guard
	"""
	guard_num = re.search("^[0-9]*-?",guard[::-1]).group()[::-1]
	return int(guard_num)

def convert_const_to_clock(constraint):
	"""
	Returns clock(string) corresponding to a constraint/guard
	
	Parameters:
	constraint (str): constraint of the form y{event} {compare_operator} {integer}
						compare_operator is in {<,==.<=,>,>=}


	Returns:
	str: zy{event}{compare}{integer}
						compare is in {ll,ee,le,gg,ge}
						for a negative integer '-' is replaced by '_' 

	"""

	#constraint = zyevent<integer
	#obligation = zyeventconstraintinteger
	#note: This does prepend z

	compare_obj = None
	compare_op = ""
	compare_orig = ""
	rev_matched_str = ""

	is_ll = re.search("^[0-9]*-?<",constraint[::-1])
	compare_op,compare_orig,rev_matched_str = (("ll","<",is_ll.group()) if is_ll!=None else (compare_op,compare_orig,rev_matched_str))

	is_le = re.search("^[0-9]*-?=<",constraint[::-1])
	compare_op,compare_orig,rev_matched_str = (("el","=<",is_le.group()) if is_le!=None else (compare_op,compare_orig,rev_matched_str))

	is_gg = re.search("^[0-9]*-?>",constraint[::-1])
	compare_op,compare_orig,rev_matched_str = (("gg",">",is_gg.group()) if is_gg!=None else (compare_op,compare_orig,rev_matched_str))

	is_ge = re.search("^[0-9]*-?=>",constraint[::-1])
	compare_op,compare_orig,rev_matched_str = (("eg","=>",is_ge.group()) if is_ge!=None else (compare_op,compare_orig,rev_matched_str))

	is_ee = re.search("^[0-9]*-?==",constraint[::-1])
	compare_op,compare_orig,rev_matched_str = (("ee","==",is_ee.group()) if is_ee!=None else (compare_op,compare_orig,rev_matched_str))

	obligation = ""
	obligation = constraint[:-len(rev_matched_str)]+re.sub("-","_",re.sub(compare_orig,compare_op,rev_matched_str,1),1)[::-1]

	return "z"+obligation

def get_guard_from_obligation(obligation):
	"""
	Get the guard constraint in an obligation clock encoding

	Parameters:
	obligation (str): An obligation clock

	Returns:
	str: A string giving the guard in an obligation clock encoding
	"""

	#obligation = zyeventconstraintinteger; constraint = ll,le...
	#constraint = yevent<integer
	
	compare_obj = None
	compare_op = ""

	is_ll = re.search("^[0-9]*_?ll",obligation[::-1])
	compare_obj,compare_op = ((is_ll,"<") if is_ll!=None else (compare_obj,compare_op))

	is_le = re.search("^[0-9]*_?el",obligation[::-1])
	compare_obj,compare_op = ((is_le,"<=") if is_le!=None else (compare_obj,compare_op))
	
	is_gg = re.search("^[0-9]*_?gg",obligation[::-1])
	compare_obj,compare_op = ((is_gg,">") if is_gg!=None else (compare_obj,compare_op))

	is_ge = re.search("^[0-9]*_?eg",obligation[::-1])
	compare_obj,compare_op = ((is_ge,">=") if is_ge!=None else (compare_obj,compare_op))

	is_ee = re.search("^[0-9]*_?ee",obligation[::-1])
	compare_obj,compare_op = ((is_ee,"==") if is_ee!=None else (compare_obj,compare_op))

	compare_num = re.sub("_","-",(compare_obj.group()[::-1][2:]),1)

	yevent = obligation[1:-len(compare_obj.group())]
	
	const = yevent+compare_op+compare_num

	return const


def convert_obligation_clock_to_guard(obligation):
	"""
	Returns a guard corresponding to an obligation
	
	Parameters:
	obligation (str): obligation of the form zy{event}{compare}{integer}
						compare is in {ll,ee,le,gg,ge}
						for a negative integer '-' is replaced by '_' 

	Returns:
	str: zy{event}{compare}{integer} {compare_operator} {integer}
						compare_operator is in {<,==.<=,>,>=}
	"""
	
	#obligation = zyeventconstraintinteger; constraint = ll,le...
	#constraint = zyeventconstraintinteger<integer
	#this does not remove appended/prepended z
	obli_guard = get_guard_from_obligation(obligation)
	obli_comp = get_comp_guard(obli_guard)
	obli_const = get_guard_constant(obli_guard)
	
	guard = obligation+obli_comp+str(obli_const)

	return guard


def get_obligations_event(state_obligations,event):
	"""
	Return the set of obligations correspoinding to an event

	Parameters:
	state_oblligations (list): List of clocks(which are strings)
	event (str): a string corresponding to an event

	Returns:
	list: A list of clocks(which are strings) which have obligation corresponding to the 
			event passed as parameter
	"""

	#given a state in A', get all obligations in A' corresponding to an event
	matched_obligations = []

	for obligation in state_obligations:

		guard_obli  = get_guard_from_obligation(obligation)
		guard_comp = get_comp_guard(guard_obli)
		guard_num = str(get_guard_constant(guard_obli))
		# print("y"+event,guard_obli[:-len(guard_comp+guard_num)],"y"+event == guard_obli[:-len(guard_comp+guard_num)])
		# print()
		if "y"+event == guard_obli[:-len(guard_comp+guard_num)]:
			matched_obligations.append(obligation)
	
	#print("state_obligations:",state_obligations,"\tevent:", event,"\tevent_obligation:",matched_obligations)
	return matched_obligations


def is_obligation_bot(obligations):
	"""
	Returns whether an obligation in a list of obligations is compared with bot

	Parameters:
	obligations (list): A list of obligation clocks(which are strings).

	Returns:
	True: if an obligation is compared to an event
	False: Otherwise
	"""

	for obligation in obligations:
		
		guard_obli = get_guard_from_obligation(obligation)
		
		if get_guard_constant(guard_obli)==-1:
			return True
		
	return False


def compare_guard_obligation(guard, obli):
	"""
	Compares a guard and an obligation and tells us whether guard entails
	obligation or obligation entails guard or none.

	Parameters:
	guard (str): A guard string
	obli (str): An obligation string

	Return:
	0: if guard entails obligation
	1: if obligation entails guard
	-1: otherwise
	"""
	#obli = zyeventllinteger
	#guard = yevent<integer
	#return 0 for guard and not obligation
	#return 1 for obligation and not guard
	#return -1 otherwise

	if guard=="" or obli=="":
		return -1
	
	obli_eq_guard = get_guard_from_obligation(obli)
	# print(obli_eq_guard)

	guard_comp = get_comp_guard(guard)
	obli_comp = get_comp_guard(obli_eq_guard)

	guard_const = get_guard_constant(guard)
	obli_const = get_guard_constant(obli_eq_guard)

	guard_event_comp = guard[:-len(str(guard_const))]
	obli_event_comp = obli_eq_guard[:-len(str(obli_const))]
	
	# guard_event_comp = re.search("^y.*"+"((<=)|(==)|(>=)|(>)|(<))",guard).group()[1:]
	# obli_event_comp = re.search("^y.*"+"((<=)|(==)|(>=)|(>)|(<))",obli_eq_guard).group()[1:]
	
	# print("FUN:g,o",guard_event_comp,guard_const,"________",obli_event_comp,obli_const,guard_event_comp==obli_event_comp)
	if guard_event_comp==obli_event_comp:
		
		guard_match_is_lt_eq = ("<" in guard_comp)
		guard_match_is_gt_eq = (">" in guard_comp)
		
		if guard_match_is_lt_eq==True and (guard_const)>=(obli_const) and obli_const>=0:
			return 1
		
		elif guard_match_is_gt_eq==True and (guard_const)>=(obli_const) and obli_const>=0:
			return 0

	return -1


def get_new_obligations(guard,current_obligations):
	"""
	Returns the obligations induced by a guard and
	the obligations to entailed by a guard

	Parameters:
	guard (list): a list of guards Eg: ["ya<10, yb>100"]
	current_obligations (list): a list of obligations(string)

	Return:
	(list,list): first list contains clocks(string) to add (which are induced by the guard).
					second list contains clocks(string) to remove (which are entailed by the guard)
	"""

	##returns obligations_to_add, obligations_to_rem
	##guard = g1 && g2 && g3 ... && gn
	##current_obligations = ["o1","o2",...]
	
	constraints = guard
	# print("lllll",guard, constraints)
	
	if constraints !=[""]:
	#	print("guards_before_removal:",constraints,"\tconstraints_before_removal:",current_obligations)
		
		remove_obligations = []
		remove_guards = []
		for constraint in constraints:
			for obligation in current_obligations:
				if compare_guard_obligation(constraint,obligation)==0:
					remove_obligations.append(obligation)
				if compare_guard_obligation(constraint,obligation)==1:
					remove_guards.append(constraint)
		
		constraints = list(set(constraints)-set(remove_guards))
		constraints_in_obli_form = []
		
		for const in constraints:
			constraints_in_obli_form.append(convert_const_to_clock(const))
	
		return (constraints_in_obli_form, remove_obligations)
	
	else:
		return ([],[])

if __name__=="__main__":
	# obligation = "zypqee_1"
	# print(convert_obligation_to_guard(obligation))
	# print(get_guard_from_obligation(obligation))
	
	obligation2 = ["zypqee_16","zypqll234","zypqle234","zypqee234","zypqge234","zypqgg234","zypqgg2","zypqll_1","zypqle_1","zypqge_1","zypqgg_1"]
	obligation2 += ["zyrdee_16","zyrdll234","zyrdle234","zyrdee234","zyrdge234","zyrdgg234","zyrdgg2","zyrdll_1","zyrdle_1","zyrdge_1","zyrdgg_1"]
	obligation2 += ["zyrlee_16","zyrlll234","zyrlle234","zyrlee234","zyrlge234","zyrlgg234","zyrlgg2","zyrlll_1","zyrlle_1","zyrlge_1","zyrlgg_1"]
	obligation2 += ["zyree_16","zyrll234","zyrle234","zyree234","zyrge234","zyrgg234","zyrgg2","zyrll_1","zyrle_1","zyrge_1","zyrgg_1"]
	
	print(get_obligations_event(obligation2,"r"))
	for obli in obligation2:
		print(obli, get_guard_from_obligation(obli), sep=": ")
		print(obli, convert_obligation_clock_to_guard(obli))
	# print(is_obligation_bot(obligation2))

	# obligations = ["zyrlll2","zyrlll2","zyrlll2","zyrlgg2","zyrlgg2","zyrlgg2","zyrlgg2","zyrlle2","zyrlle2","zyrlge2","zyrlee2"]
	# guards =      ["yrl<1"  ,"yrl<3"  ,"yr<2"   ,"yrl>1"  ,"yrl>3"  ,"yr>2"   , "yrl<2" ,"yrl<=3"  ,"yr<=3" , "yrl>=3","yrl==2"]
	# for i in range(0,len(obligations)):
	# 	print(obligations[i],guards[i],compare_guard_obligation(guards[i], obligations[i]))
	# 	print()

	# exit()
	guards2 = ["ybb<1","ybb==1","ybb<=1","ybb>=1","ybb>1"]
	# for guard in guards2:
	# 	print(get_comp_guard(guard))

	guards3 = guards2 + ["ybb<-1","ybb==-1","ybb<=-15","ybb>=-134","ybb>-123"]
	for guard in guards3:
		# print(get_guard_constant(guard))
		print(guard, convert_const_to_clock(guard))
	
	robust_guard = ["yb-b<-1","-ybb==-1","yb-b<=-15","ybb->=-134","y-bb>-123"]
	for guard in robust_guard:
		# print(get_guard_constant(guard))
		print(guard, convert_const_to_clock(guard))
	# is_obligation_bot_alternate(obligations)
	# print(get_only_event_of_obligation(obligation))
	# print(get_only_constraint_of_obligation(obligation))
	# print(get_only_integer_of_obligation(obligation))