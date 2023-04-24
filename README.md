# Installation using Docker
To install using docker run the command:
`sudo docker build --progress=plain -t gta:1 .`

# Run using Docker
To run use the following command:
`sudo docker run gta:1`

To run with an input example use the command:
`sudo docker run gta:1 [local file path]`



# Installation without Docker
## Requirements
* g++ (version >=9.3.0)
* CMake (version >=2.8)
* flex (version >=2.6.4)
* bison (version >=3.5.1)
* boost library (version >=1.65.0) 
* Catch2 (version 2)

## Compile
   
To compile run the following command 
`./compile.sh`

The command should create a `build_dir` directory and an `install_dir` directory.

---
# Run
To run the tool on a `file` exectute the following command:
        `./run.sh file_path`

Simply executing `./run.sh` will run the tool on `example1_gen.txt` file.


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


# Benchmarks:
        TODO: Add file mapping corresponding to the benchmarks used in the paper.
