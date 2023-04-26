# Docker Image

## Downloading the Docker Image
<!-- DOI: `10.6084/m9.figshare.22698373` -->
DOI: `10.6084/m9.figshare.22701187`

<!-- Link for accessing the docker image: `https://doi.org/10.6084/m9.figshare.22698373.v1` -->
Link for accessing the docker image: `https://doi.org/10.6084/m9.figshare.22701187`

<!-- Download the docker image from [DOI LINK](https://doi.org/10.6084/m9.figshare.22698373.v1). -->

Download the docker image from [DOI LINK](https://doi.org/10.6084/m9.figshare.22701187).

## Requirements for running the Docker Image
* [Docker](https://docs.docker.com/engine/release-notes/23.0/) (version >=23.0.4)


## Instructions to load the docker image:
Run the following command to load the docker image:

    sudo docker load < gta_v3.tar


The terminal should then display:

    Loaded image: gta:3



---
## Testing Table 1 using the Docker Image
### Commands to verify the `GTA reach` column
  * `sudo docker run gta:3 gta_gsim examples_from_the_paper/table_verify_gta/Dining_Phi_6.txt`

  * `sudo docker run gta:3 gta_gsim examples_from_the_paper/table_verify_gta/FDDI_10.txt`
  
  * `sudo docker run gta:3 gta_gsim examples_from_the_paper/table_verify_gta/Fischer_10.txt`

  * `sudo docker run gta:3 gta_gsim examples_from_the_paper/table_verify_gta/toyECA_10000_4.txt`

  * `sudo docker run gta:3 gta_gsim examples_from_the_paper/table_verify_gta/toyECA_5000_6.txt`

  * `sudo docker run gta:3 gta_gsim examples_from_the_paper/table_verify_gta/toyECA_1000_100.txt`

  * `sudo docker run gta:3 gta_gsim examples_from_the_paper/table_verify_gta/toyECA_50000_120.txt`

  * `sudo docker run gta:3 gta_gsim examples_from_the_paper/table_verify_gta/Fire-alarm-pattern_5.txt`
  
  * `sudo docker run gta:3 gta_gsim examples_from_the_paper/table_verify_gta/CSMACD-bounded_1.txt`
  
  * `sudo docker run gta:3 gta_gsim examples_from_the_paper/table_verify_gta/CSMACD-bounded_4.txt`
  
  * `sudo docker run gta:3 gta_gsim examples_from_the_paper/table_verify_gta/ABP-prop1.txt`
  
  * `sudo docker run gta:3 gta_gsim examples_from_the_paper/table_verify_gta/ABP-prop2.txt`

### Commands to verify the `G-sim` column
  * `sudo docker run gta:3 gsim examples_from_the_paper/table_verify_gsim/Dining_Phi_6.txt`

  * `sudo docker run gta:3 gsim examples_from_the_paper/table_verify_gsim/FDDI_10.txt`
  
  * `sudo docker run gta:3 gsim examples_from_the_paper/table_verify_gsim/Fischer_10.txt`

  * `sudo docker run gta:3 gsim examples_from_the_paper/table_verify_gsim/toyECA_10000_4.txt`

  * `sudo docker run gta:3 gsim examples_from_the_paper/table_verify_gsim/toyECA_5000_6.txt`

  * `sudo docker run gta:3 eca examples_from_the_paper/toyECA_ECA/toyECA_ECA_1000_100.txt`

  * `sudo docker run gta:3 eca examples_from_the_paper/toyECA_ECA/toyECA_ECA_50000_120.txt`

  
---
## Running the image on a test file
---
We will show how to run our tool on `./examples_from_the_paper/table_verify_gta/ABP-prop1.txt` which is the 11<sup>th</sup> entry in Table 1 in the paper `A Unified Model for Real-Time Systems: Symbolic Techniques and Implementation`

To run the tool on the file `./examples_from_the_paper/table_verify_gta/ABP-prop1.txt` that contains a GTA model, run the command: 

    sudo docker run -v ./examples_from_the_paper/table_verify_gta/ABP-prop1.txt:/usr/src/ABP1.txt -t gta:3 gta_gsim /usr/src/ABP1.txt

In general, to run a file with name `file_name` (containing a GTA) that is at a location `file_location` (i.e. the file's path is `file_location/file_name`), run the command:

    `sudo docker run -v file_location/file_name:/usr/src/file_name -t gta:3 gta_gsim /usr/src/file_name`

---
We will show how to run our tool on `./examples_from_the_paper/table_verify_gsim/Dining_Phi_6.txt` which is the 1<sup>st</sup> entry in Table 1 in the paper `A Unified Model for Real-Time Systems: Symbolic Techniques and Implementation`

To run the tool on the file `./examples_from_the_paper/table_verify_gsim/Dining_Phi_6.txt` that contains a TA model, run the command: 

    sudo docker run -v ./examples_from_the_paper/table_verify_gsim/Dining_Phi_6.txt:/usr/src/Dining_Phi_6.txt -t gta:3 gsim /usr/src/Dining_Phi_6.txt

In general, to run a file with name `file_name` (containing a TA) that is at a location `file_location` (i.e. the file's path is `file_location/file_name`), run the command:

    `sudo docker run -v file_location/file_name:/usr/src/file_name -t gta:3 gsim /usr/src/file_name`
---
We will show how to run our tool on `./examples_from_the_paper/toyECA_ECA/toyECA_ECA_5000_6.txt` which is the 5<sup>th</sup> entry in Table 1 in the paper `A Unified Model for Real-Time Systems: Symbolic Techniques and Implementation`

To run the tool on the file `./examples_from_the_paper/toyECA_ECA/toyECA_ECA_5000_6.txt` that contains a ECA model, run the command: 

    sudo docker run -v ./examples_from_the_paper/toyECA_ECA/toyECA_ECA_5000_6.txt:/usr/src/toyECA_ECA_5000_6.txt -t gta:3 eca /usr/src/toyECA_ECA_5000_6.txt

This command prints a TA equivalent (according to alur and dill to the ECA)

In general, to run a file with name `file_name` (containing a ECA) that is at a location `file_location` (i.e. the file's path is `file_location/file_name`), run the command:

    `sudo docker run -v file_location/file_name:/usr/src/file_name -t gta:3 eca /usr/src/file_name`

<!-- sudo docker run -v ./examples_from_the_paper/table_verify_gta/ABP-prop1.txt:/usr/src/abp_prop1.txt -t gta:2 gta_gsim /usr/src/abp_prop1.txt -->

<!-- sudo docker run -v ./examples_from_the_paper/table_verify_gsim/FDDI_10.txt:/usr/src/fddi_10.txt -t gta:2 gsim /usr/src/fddi_10.txt -->

<!-- sudo docker run -v ./examples_from_the_paper/toyECA_ECA/toyECA_ECA_10000_4.txt:/usr/src/eca_10000_4.txt -t gta:2 eca /usr/src/eca_10000_4.txt > .tmp.txt -->

<!-- # Installation using Docker
To install using docker run the command:
`sudo docker build --progress=plain -t gta:1 .`

# Run using Docker
To run use the following command:
`sudo docker run gta:1`

To run with an input example use the command:
`sudo docker run gta:1 [local file path]` -->



# Installation without Docker
## Requirements for installing TChecker Tool
* g++ (version >=9.3.0)
* CMake (version >=2.8)
* flex (version >=2.6.4)
* bison (version >=3.5.1)
* boost library (version >=1.65.0) 
* Catch2 (version 2)

Catch2 version 2 can be obtained from [Catch2 github repository](https://github.com/catchorg/Catch2). Please download the `v2.x` branch, it can be downloaded using the command: 

    git clone https://github.com/catchorg/Catch2.git -b v2.x

Please, refer to Catch2 tutorial for [installation instructions](https://github.com/catchorg/Catch2/blob/master/docs/cmake-integration.md#installing-catch2-from-git-repository).

## Requirements for installing Parser Tool
* python (version >= 3.9)

To verify that python is installed: the command `python3 --version` should work and should output a version `>=3.9`.


## Compile
   
To compile the TChecker tool, run the following command 

      ./compile.sh

The command should create a `build_dir` directory and an `install_dir` directory.

---
# Run
* To run the tool on a `input_file` with path `file_path` execute the following command:
        
        `./run.sh file_path`

* To run the G-simulation tool on a `input_file` with path `file_path` containing a General Timed Automata (Note: this is also the default option with `./run.sh`), execute the following command:
        
        ./run.sh gta_gsim file_path

* To run the G-simulation tool on a `input_file` with path `file_path` containing a Timed Automata, execute the following command:
        
        ./run.sh gsim file_path

* To run the parser that takes input an ECA file with name `input_file` and path `input_file_path`, and outputs a Timed Automata (according to Alur & Dill) in `output_file` execute the following command:

        ./run.sh eca input_file_path/input_file output_file


---

# Testing Table 1 of the paper `A Unified Model for Real-Time Systems: Symbolic Techniques and Implementation` .
---

## File Structure
The directory `examples_from_the_paper` contains all the benchmarks used in the paper.

It contains three sub-directories as follows:
* `table_verify_gsim`: Files in this directory are TChecker Timed Automata Files. These are as follows:
  1. Dining_Phi_6.txt: This file corresponds to table entry 1.
  1. FDDI_10.txt: This file corresponds to table entry 2.
  1. Fischer_10.txt: This file corresponds to table entry 3.
  1. toyECA_5000_6.txt: This file corresponds to table entry 4.
  1. toyECA_10000_4.txt: This file corresponds to table entry 5.
* `table_verify_gta`: Files in this directory are TChecker General Model Files. These are as follows:
  1. ABP-prop1.txt: This file corresponds to table entry 11.
  1. ABP-prop2.txt: This file corresponds to table entry 12.
  1. CSMACD-bounded_1.txt: This file corresponds to table entry 9.
  1. CSMACD-bounded_4.txt: This file corresponds to table entry 10.
  1. Dining_Phi_6.txt: This file corresponds to table entry 1.
  1. FDDI_10.txt: This file corresponds to table entry 2.
  1. Fire-alarm-pattern_5.txt: This file corresponds to table entry 8.
  1. Fischer_10.txt: This file corresponds to table entry 3.
  1. toy_ECA_1000_100.txt: This file corresponds to table entry 6.
  1. toy_ECA_5000_6.txt: This file corresponds to table entry 5.
  1. toy_ECA_10000_4.txt: This file corresponds to table entry 4.
  1. toy_ECA_50000_120.txt: This file corresponds to table entry 7.
  
* `toyECA_ECA`: This folder contains file in ECA format.
  1. toyECA_ECA_1000_100.txt: This file corresponds to table entry 6.
  1. toyECA_ECA_5000_6.txt: This file corresponds to table entry 5.
  1. toyECA_ECA_10000_4.txt: This file corresponds to table entry 4.
  1. toyECA_ECA_50000_120.txt: This file corresponds to table entry 7.

## Commands to verify the table:
  ### Commands to verify the `GTA reach` column
  * `./run.sh gta_gsim examples_from_the_paper/table_verify_gta/Dining_Phi_6.txt`
  * `./run.sh gta_gsim examples_from_the_paper/table_verify_gta/FDDI_10.txt`
  * `./run.sh gta_gsim examples_from_the_paper/table_verify_gta/Fischer_10.txt`
  * `./run.sh gta_gsim examples_from_the_paper/table_verify_gta/toy_ECA_10000_4.txt`
  * `./run.sh gta_gsim examples_from_the_paper/table_verify_gta/toy_ECA_5000_6.txt`
  * `./run.sh gta_gsim examples_from_the_paper/table_verify_gta/toy_ECA_1000_100.txt`
  * `./run.sh gta_gsim examples_from_the_paper/table_verify_gta/toy_ECA_50000_120.txt`
  * `./run.sh gta_gsim examples_from_the_paper/table_verify_gta/Fire-alarm-pattern_5.txt`
  * `./run.sh gta_gsim examples_from_the_paper/table_verify_gta/CSMACD-bounded_1.txt`
  * `./run.sh gta_gsim examples_from_the_paper/table_verify_gta/CSMACD-bounded_4.txt`
  * `./run.sh gta_gsim examples_from_the_paper/table_verify_gta/Dining_Phi_6.txt`
  * `./run.sh gta_gsim examples_from_the_paper/table_verify_gta/ABP-prop1.txt`
  * `./run.sh gta_gsim examples_from_the_paper/table_verify_gta/ABP-prop2.txt`
  
### Commands to verify the `G-Sim` column
  * `./run.sh gsim examples_from_the_paper/table_verify_gsim/Dining_Phi_6.txt`
  * `./run.sh gsim examples_from_the_paper/table_verify_gsim/FDDI_10.txt`
  * `./run.sh gsim examples_from_the_paper/table_verify_gsim/Fische_10.txt`
  * * `./run.sh eca examples_from_the_paper/toyECA_ECA/toyECA_ECA_10000_4.txt > examples_from_the_paper/table_verify_gsim/toyECA_10000_4.txt`
    *  `./run.sh gsim examples_from_the_paper/table_verify_gsim/toyECA_10000_4.txt`
  * * `./run.sh eca examples_from_the_paper/toyECA_ECA/toyECA_ECA_5000_6.txt > examples_from_the_paper/table_verify_gsim/toyECA_5000_6.txt`
    *  `./run.sh gsim examples_from_the_paper/table_verify_gsim/toyECA_5000_6.txt`
  * `./run.sh eca examples_from_the_paper/toyECA_ECA/toyECA_ECA_1000_100.txt > examples_from_the_paper/table_verify_gsim/toyECA_1000_100.txt`

    The above command will not terminate in 15 minutes, so we report TIMEOUT.

  * `./run.sh eca examples_from_the_paper/toyECA_ECA/toyECA_ECA_50000_120.txt > examples_from_the_paper/table_verify_gsim/toyECA_50000_120.txt`

    The above command will not terminate in 15 minutes, so we report TIMEOUT.

---


# File Format
<!-- Please, refer to [Using TChecker](https://github.com/ticktac-project/tchecker/wiki/Using-TChecker) or to file `doc/file-format.md` in the repository. -->

The TChecker file format is used to describe timed automata. A file
consists in a sequence of declarations of processes, clocks,
locations, edges, etc, as described below. Declarations can appear in any order, as soon as
these two rules are respected:

- the `system` declaration must be the first declaration in the file

- every item (process, location, edge, event, variable, ...) must be
declared before it is used.

All declarations appear in the same global scope. Hence, all declared
variables (bounded integers, clocks, etc) are global. Local variables
can be simulated by adding a prefix to the name of the variable,
e.g. `P1_x` instead of `x` for clock `x` of process `P1`.

A comment starts with symbol `#` and runs till the end of the line.

Symbols `:`, `@` and `#` have a special meaning in TChecker files,
and are thus reserved. Keywords: `clock`, `edge`, `event`, `int`,
`location`, `process`, `sync` and `system` are reserved.

Identifiers are any string containing letters, numbers, underscore
(`_`) and dot (`.`) and starting with either a letter or underscore
(`_`).

1. [The `system` declaration](#the-system-declaration)
1. [The `process` declaration](#the-process-declaration)
1. [The `event` declaration](#the-event-declaration)
1. [The `clock` declaration](#the-clock-declaration)
1. [The `int` declaration](#the-int-declaration)
1. [The `location` declaration](#the-location-declaration)
1. [The `edge` declaration](#the-edge-declaration)
1. [The `sync` declaration](#the-sync-declaration)
1. [Attributes](#attributes)
   1. [Finite-state machines](#finite-state-machines)
   1. [Timed automata](#timed-automata)
1. [Expressions](#expressions)
1. [Statements](#statements)
1. [Semantics](#semantics)
   1. [Finite-state machines](#finite-state-machines-1)
   1. [Timed automata](#timed-automata-1)

# The `system` declaration

```
system:id
```

where `id` is the identifier of the system.

There shall be only one `system` declaration in a TChecker file. And it shall appear as the first declaration in the file.

# The `process` declaration

```
process:id
```

where `id` is the identifier of the process. No other process shall have
the same identifier.

A `process` declaration declares a process name. It does not declare a new scope. Hence all declarations following the process declaration are in the global scope.

There is no way to declare a type of process, and instantiate it in TChecker. In order to specify several instances of the same process type, the process declaration and all the related declarations (locations, edges, etc) shall be duplicated. This can be solved by writing a script that generates the TChecker model and that handles process instantiation smoothly.


# The `event` declaration

```
event:id
```

where `id` is the identifier of the event. No other event shall have the
same identifier.


# The `clock` declaration

There are two methods to declare clocks
1. [Standard TChecker Clocks](#standard-tchecker-clocks)
1. [Extended Clocks](#Extended-clocks)

## Standard TChecker Clocks
```
clock:size:id
```

declares an array of `size` clocks with identifier `id`. No other clock shall
have the same identifier.

For instance:

```
clock:1:x
```

declares a single clock `x`. And:

```
clock:10:y
```

declares an array `y` of 10 clocks. Then, `y[0]` is the first clock, and
`y[9]` is the last one.

Out-of-bounds access like `y[10]` are detected and raise a fatal error
when the model is analysed.

## Extended Clocks TODO:
<clock:clock_type:clock_name>
        clock_type must be one of the strings history, prophecy, normal, timer.
        clock_name can be any identifier except tmp

# TODO:
`Clocks have an implicit domain of values ranging from -infinity to +infinity,
and implicit initial value 0. Clocks only admit a restricted set of
operations (see [Expressions](#expressions) and [Statements](#statements) for
details). Some errors can be detected at compilation time, but some
others can only be detected when the model is analysed. In particular,
assigning a value to clock out of its domain raise a fatal error when
the model is analysed.`

NB: while arrays of clocks may be convenient for modeling purposes, they
make some analysis harder. In particular, the clock bounds that are used
for zone abstractions are harder to compute (and often much less precise)
when using clock arrays. This may result in exponentially bigger zone
graphs. We thus recommend to avoid using clock arrays when possible.


# The `int` declaration

```
int:size:min:max:init:id
```

declares the array of `size` bounded integer variables with identifier
`id`. Each variable in the array takes values between `min` and `max`
(both included) and initial value `init`. No other integer variable
shall have the same identifier.

For instance:

```
int:1:0:127:0:i
```

declares a single integer variable `i` with values between 0 and 127, and
initial value 0.

And:

```
int:5:-128:127:-1:j
```

declares an array `j` of 5 integer variables with values between -128 and 127,
and initial value -1. The first variable of the array is `j[0]` and the last
one is `j[4]`.

Out-of-bounds access like `j[10]` are detected and raise a fatal error
when the model is analysed.

NB: the semantics of bounded integers in TChecker does not allow to assign
an out-of-bound value to a variable. Consider variable `i` as declared
above. Assigning -1 to `i` is illegal. Similarly, if `i` has value 127, then
adding 1 to `i` is illegal. Illegal statements are simply deemed invalid and
not executable (see section [Semantics](#semantics) for details).


# The `location` declaration

```
location:p:id{attributes}
```

declares location with identifier `id` in process with identifier
`p`, and given `attributes`. The process identifier `p` shall have
been declared previously. No other location within process `p` shall
have the same identifier. It is perfectly valid that two locations in
different processes have the same identifier.

The `{attributes}` part of the declaration can be ommitted if no
attribute is associated to the location (or it can be empty:
`{}`). See section [Attributes](#attributes) for details.


# The `edge` declaration

```
edge:p:source:target:e{attributes}
```

declares an edge in process `p` from location `source` to location
`target` and labelled with event `e`.  The process `p` shall have been
declared previously. The two locations `source` and `target` shall
have been declared as well, and they shall both belong to process
`p`. The event `e` shall have been declared before the edge is
declared.

The `{attributes}` part of the declaration can be omitted if no
attribute is associated to the location (or it can be empty:
`{}`). See section [Attributes](#attributes) for details.


# The `sync` declaration

```
sync:sync_constraints
```

declares a synchronisation constraint. `sync_constraints` is a
column-separated list of synchronisation constraints of the form `p@e`
or `p@e?` where `p` is a process name, `e` is an event name, and the
option question mark `?` denotes a weak synchronisation. Process `p`
and event `e` shall have been declared before the synchronisation is
declared.

A `sync` declaration must contain at least 2 synchronisation
constraints, and at most one synchronisation constraint for each
process in the model.  A (strong) synchronization constraint `p@e`
means that process `p` *shall* synchronise one of its `e` labeled
edge. A weak synchronisation constraint `p@e?` means that process `p`
*may* synchronize one of its `e` labeled edge. Process `p` shall
synchronize if it has an enabled `e` labelled edge. It does not
prevent synchronization of the other processes otherwise.

If an event `e` appears in a `sync` declaration along with process `p`
(i.e.  in a constraint `p@e` or `p@e?`), then `e` labeled edges of
process `p` shall only be taken synchronously (according to `sync`
declarations). An event `e` that does not appear in any
synchronisation constraint with process `p` is asynchronous in process
`p`.

For instance, the following declarations (assuming processes and events have
been declared):

```
sync:P1@a:P2@a
sync:P1@a:P2@b:P3@c?:P4@d?
```

entails that event `a` is synchronous in processes `P1` and `P2`,
event `b` is synchronous in process `P2`, event `c` is synchronous in
process `P3`, and event `d` is synchronous in process `P4`. All other
events are asynchronous.

Thus assuming the following set of edges (and that locations and
events have been declared):

```
edge:P1:l0:l1:a
edge:P1:l0:l2:a

edge:P2:l0:l1:b

edge:P3:l0:l1:a

edge:P4:l0:l1:d
```

the set of global edges from the tuple of locations `<l0,l0,l0,l0>`
(i.e. all the processes are in location `l0`) is:

```
<l0,l0,l0,l0> - <P1@a,P2@b,P4@d> -> <l1,l1,l0,l1>  // 2nd `sync` declaration
<l0,l0,l0,l0> - <P1@a,P2@b,P4@d> -> <l2,l1,l0,l1>  // 2nd `sync` declaration
<l0,l0,l0,l0> - <P3@a>           -> <l0,l0,l1,l0>  // asynchronous
```

Observe that the first `sync` declaration cannot be instantiated as
process `P2` has no `a` labelled edge from `l0`.

The second `sync` declaration can be instantiated as the two strong
constraints `P1@a` and `P2@b` have corresponding edges. The weak
synchronisation constraint `P4@d?` has a corresponding edge in `P4`
from `l0`, so `P4` is involved in the global edge. `P3` however has no
`c` labelled edge from `l0`.  Thus, the weak constraint `P3@c?` cannot
be instantiated, and `P3` does not participate to the global
edge. However this does not prevent synchronisation of the other three
processes. The second `sync` declaration is instantiated twice since
process `P1` has two `a` labelled edges from location `l0`.

Event `a` is asynchronous in process `P3` as it does not appear in any
`sync` declaration along with process `P3`. Hence there is a global
asynchronous edge involving only process `P3` with event `a`.

A `sync` declaration consisting only of weak synchronisation
constraints like:

```
sync:P1@e?:P2@f?
```

is instantiated as soon as at least one of the constraint can be
instantiated. Hence there is no global edge when `P1` has no `e` labelled edge, and `P2` has no `f` edge.


# Attributes

Attributes allow to define properties of declarations. Currently, only
`edge` and `location` declarations support attributes.

A list of attributes `{attributes}` is a colon-separated list of
pairs `key:value`. Keys are identifier and values are any string not
containing reserved symbols `:`, `@` and spaces. The value may be
empty.

There is no fixed list of allowed attributes. New attributes can be
introduced at will as a way to provide information to
algorithms. Current algorithms may emit a warning when an unknown
attribute is encountered. But this shall not prevent the algorithm to
analyse the model (without taking the unknown attribute into account).

Current models support the attributes described below.


## Finite-state machines

Supported location attributes:

- `initial:` (no value) declares an initial location. Each process
  shall have at least one initial location, and it can have several of
  them

- `labels: list_of_labels` declares the labels of a location (later
  used for reachability checking for instance). The list of labels is
  a comma-separated list of strings (not containing any reserved
  characters or spaces).

- `invariant: expression` declares the invariant of the the
  location. See section [Expressions](#expressions) for details on expressions

Supported edge attributes:

- `provided: expression` declares the guard of the edge. See section
  [Expressions](#expressions) for details on expressions.

- `do: statement` declares the statement of the edge. See section
  [Statements](#statements) for details on statements.

NB: finite-state models shall have no `clock` declaration. As a
result, expression and statements shall not involve clocks.


## Timed automata

Extends attributes supported by finite-state machines with the
following ones, for locations:

- `committed:` (no value) declares the location committed (see
  [Semantics](#semantics) for details).

- `urgent:` (no value) declares the location urgent (see [Semantics](#semantics) for
  details).



# Expressions

Expressions are conjunctions of atomic expression or negation of
atomic expressions. An atomic expression is either a term or a binary
predicate applied to two terms. Terms consists in integer constants,
variables and arithmetic operators applied to terms. Expressions are
defined by the following grammar:

```
expr ::= atomic_expr
       | atomic_expr && expr

atomic_expr ::= int_term
              | predicate_expr
	      | ! atomic_expr
	      | clock_expr

predicate_expr ::= ( predicate_expr )
                 | int_term == int_term
		 | int_term != int_term
		 | int_term < int_term
		 | int_term <= int_term
		 | int_term >= int_term
		 | int_term > int_term

int_term ::= INTEGER
           | lvalue_int
           | - int_term
           | int_term + int_term
           | int_term - int_term
           | int_term * int_term
           | int_term / int_term
           | int_term % int_term
           | (if expr then int_term else int_term)

lvalue_int ::= INT_VAR_ID
             | INT_VAR_ID [ term ]
```

where `INT_VAR_ID` is a bounded integer variable identifier, and
`INTEGER` is a signed integer constant. Notice that integer terms
`int_term` are atomic expressions with the usual convention that the
expression is false iff the term has value 0.

There are restrictions on atomic expressions involving clock
variables. We only allow comparisons of clocks or difference of clocks
w.r.t. integer terms:

```
clock_expr ::= clock_term == int_term
             | clock_term < int_term
	     | clock_term <= int_term
	     | clock_term >= int_term
	     | clock_term > int_term

clock_term ::= clock_lvalue
             | clock_lvalue - clock_lvalue

clock_lvalue ::= CLOCK_ID
               | CLOCK_ID [ int_term ]
```

where `CLOCK_ID` is a clock identifier.


# Statements

Statements are sequences of assignements or `nop`:

```
stmt ::= statement [;]
       | statement ; stmt

statement ::= if_statement
           |  loop_statement
           |  simple_statement

statement ::= if expr then stmt end
           |  if expr then stmt else stmt end
           |  while expr do stmt end
           |  local_declaration
           |  int_assignment
           |  clock_assignment
    	   |  nop    	           

int_assignement ::= lvalue_int = int_term
```

Assignments to clock variables are restricted to assignment of a
constant to a clock `x = y`, or "diagonal assignments" of the form `x
= y + d` where d is any integer term:

```
clock_assignment ::= lvalue_clock = int_term
		   | lvalue_clock = lvalue_clock + int_term
```

It is possible to declare local variables using the keyword `local`. Even if 
local declarations may occur anywhere in an attribute, variables must not 
conflict with an existing variable (global or local). Size of 
local arrays must be computable at compilation time.
```
local_declaration ::= local var
                   |  local var = expr
                   |  local var [ expr ] 
```
# Semantics

## Finite-state machines

A *configuration* of the finite-state machine consists in a tuple of
locations L (one location per process) and a valuation V of the
bounded integer variables in the model.

The *initial configurations* are pairs (L,V) such that L is a tuple of
initial locations, V maps each variable to its initial value, and V
satisfies the `invariant` attribute of each location in L.

There is a *transition* (L,V) -- E -> (L',V') if there is a tuple E of
edges from L that instantiates a `sync` declaration, or E consists of
a single asynchronous edge, and:

- the `provided` attribute of each edge in E is satisfied by V

- V' is obtained from V by applying the `do` attribute of each edge
  in E consecutively

- for each variable i, V'(i) is in the domain of i

- L' is obtained from L according to the edges in E

- and V' satisfies the `invariant` of each location in in L'

This defines a transition system with configurations, initial
configurations and transitions as described above.


### Timed automata

The *configurations* of the timed automaton consists in a tuple of
locations L and a valuation V of the bounded integer variables (as for
finite-state machines) and a valuation X of the clocks in the model.

The *initial configurations* are triples (L,V,X) such that L and V are
initial as for the finite-state machine, X maps each clock to 0, and
the `invariant` attribute if each location in L is satisfied by V and
X.

There are two kinds of *transitions*. Discrete transitions (L,V,X) --
E -> (L',V',X') are defined as for finite-state machines, and
furthermore:

- clocks are taken into account for the evaluation of `provided`,
  `invariant` and `do` attributes of locations and edges in L and E

- and E involves (at least) a process with a `committed` location in L
  if any

Timed transitions (L,V,X) ---> (L,V,X')

- L has no `committed` and no `urgent` location

- there is a non-negative real number d such that X'(y)=X(y)+d for
  each clock y

- and the `invariant` in L is satisfied by V and X'.

This defines a transition system with configurations, initial
configurations and transitions as described above.

<!-- 
# Input Format
        TODO: Complete this section
## Examples
 For examples see example1_gen.txt, example2_gen.txt, ..., example9_gen.txt.

Declare system: same as standard tchecker
Declare clock: format: <clock:clock_type:clock_name>
        clock_type must be one of the strings history, prophecy, normal, timer.
        clock_name can be any identifier except tmp

Declare event: format: <event:event_name>
        event_name can be any identifier
Declare event clock: format: <event:event_name:bit_representing_history_clock:bit_representing_prophecy_clock>
         bit_representing_history_clock can be 0 or 1 representing whether event history clock corresponding to the event is there in the system or not
         bit_representing_prophecy_clock can be 0 or 1 representing whether event prophecy clock corresponding to the event is there in the system or not
         if history (or prophecy) clock is defined for the event then these clocks can be used in guards with event_name"_h" (or event_name"_p")
         
Write a program: A program is a sequence of provided and do operations. It must start with provided: operations.
                    every provided and do operation end with ";"
                    
                    See examples for more details.
 -->

# Benchmarks:
        TODO: Add file mapping corresponding to the benchmarks used in the paper.
