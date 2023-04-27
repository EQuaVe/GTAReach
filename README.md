# Contents
1. [Installation with Docker](#installation-with-Docker)
1. [Installation without Docker](#installation-without-Docker)
1. [Running the tool(s)](#running-the-tool(s))
1. [Testing Table 1 of the paper `A Unified Model for Real-Time Systems: Symbolic Techniques and Implementation`](#testing-table-1-of-the-paper-`A-Unified-Model-For-Real-Time-Systems:-Symbolic Techniques-And-Implementation`)
1. [File Format](#File-Format)
---
# Installation with Docker

## Downloading the Docker Image

DOI: `10.6084/m9.figshare.22701187`

Link for accessing the docker image: `https://doi.org/10.6084/m9.figshare.22701187`

Download the docker image from [DOI LINK](https://doi.org/10.6084/m9.figshare.22701187).

## Requirements for running the Docker Image
* [Docker](https://docs.docker.com/engine/release-notes/23.0/) (version >=23.0.4)


## Instructions to load the docker image:
Run the following command to load the docker image:

    sudo docker load < gta_v3.tar


The terminal should then display:

    Loaded image: gta:3


<!-- sudo docker cp 92cd2758ba7a:/usr/src/tchecker3_gta/tmp.sh ./tmp2.sh -->
---
## Running Docker Image:

Run the following command to start the interactive terminal:

    sudo docker run -it gta:3

For running the tool go to (Running the tool(s))[#Running-the-tool(s)]




<!-- ## Testing Table 1 using the Docker Image
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

### Running GTA
In general, to run a file with name `file_name` (containing a GTA) that is at a location `file_location` (i.e. the file's path is `file_location/file_name`), run the command:

    `sudo docker run -v file_location/file_name:/usr/src/file_name -t gta:3 gta_gsim /usr/src/file_name`

We will show how to run our tool on `./examples_from_the_paper/table_verify_gta/ABP-prop1.txt` which is the 11<sup>th</sup> entry in Table 1 in the paper `A Unified Model for Real-Time Systems: Symbolic Techniques and Implementation`

To run the tool on the file `./examples_from_the_paper/table_verify_gta/ABP-prop1.txt` that contains a GTA model, run the command: 

    sudo docker run -v ./examples_from_the_paper/table_verify_gta/ABP-prop1.txt:/usr/src/ABP1.txt -t gta:3 gta_gsim /usr/src/ABP1.txt



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
 -->
---
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

# Running the tool(s)
* To run the tool on a `input_file` with path `file_path` execute the following command:
        
        `./run.sh file_path`

* To run the G-simulation tool on a `input_file` with path `file_path` containing a General Timed Automata (Note: this is also the default option with `./run.sh`), execute the following command:
        
        ./run.sh gta_gsim file_path

* To run the G-simulation tool on a `input_file` with path `file_path` containing a Timed Automata, execute the following command:
        
        ./run.sh gsim file_path

* To run the parser that takes input an ECA file with name `input_file` and path `input_file_path`, and outputs a Timed Automata (according to Alur & Dill) in `output_file` execute the following command:

        ./run.sh eca input_file_path/input_file output_file
---

# Testing Table 1 of the paper `A Unified Model for Real-Time Systems: Symbolic Techniques and Implementation`


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
  
  ### Script to verify the `GTA reach` column
  * `./verify_table_gta_reach.sh`

  ### Commands to verify the `G-Sim` column
  * `./run.sh gsim examples_from_the_paper/table_verify_gsim/Dining_Phi_6.txt`
  * `./run.sh gsim examples_from_the_paper/table_verify_gsim/FDDI_10.txt`
  * `./run.sh gsim examples_from_the_paper/table_verify_gsim/Fischer_10.txt`
  * * `./run.sh eca examples_from_the_paper/toyECA_ECA/toyECA_ECA_10000_4.txt > examples_from_the_paper/table_verify_gsim/toyECA_10000_4.txt`
    *  `./run.sh gsim examples_from_the_paper/table_verify_gsim/toyECA_10000_4.txt`
  * * `./run.sh eca examples_from_the_paper/toyECA_ECA/toyECA_ECA_5000_6.txt > examples_from_the_paper/table_verify_gsim/toyECA_5000_6.txt`
    *  `./run.sh gsim examples_from_the_paper/table_verify_gsim/toyECA_5000_6.txt`
  * `./run.sh eca examples_from_the_paper/toyECA_ECA/toyECA_ECA_1000_100.txt > examples_from_the_paper/table_verify_gsim/toyECA_1000_100.txt`

    The above command will not terminate in 15 minutes, so we report TIMEOUT.

  * `./run.sh eca examples_from_the_paper/toyECA_ECA/toyECA_ECA_50000_120.txt > examples_from_the_paper/table_verify_gsim/toyECA_50000_120.txt`

    The above command will not terminate in 15 minutes, so we report TIMEOUT.

  ### Script to verify the `G-Sim` column
  * `./verify_table_g-sim.sh`
---

--
# File Format
Please, refer to [Using TChecker](https://github.com/anirjoshi/TIMED_AUTOMATA_GTA_MODEL_LATEST/blob/general_model/tchecker/doc/file-format-combined.md) or to file `tchecker/doc/file-format-combined.md` in the repository.


<!-- # Benchmarks:
        TODO: Add file mapping corresponding to the benchmarks used in the paper. -->
