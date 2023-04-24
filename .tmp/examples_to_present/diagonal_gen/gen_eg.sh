#!/bin/bash
<<comment
 ./diagonal_gen.sh 10 5 > all_egs/diagonal_gen_10_5.txt
 ./diagonal_gen.sh 20 5 > all_egs/diagonal_gen_20_5.txt
 ./diagonal_gen.sh 20 10 > all_egs/diagonal_gen_20_10.txt
 ./diagonal_gen.sh 30 10 > all_egs/diagonal_gen_30_10.txt
 ./diagonal_gen.sh 30 20 > all_egs/diagonal_gen_30_20.txt
comment

 ./diagonal_gen.sh 100 50 > all_egs/diagonal_gen_100-50.txt
 ./diagonal_gen.sh 400 500 > all_egs/diagonal_gen_400-500.txt
 ./diagonal_gen.sh 800 250 > all_egs/diagonal_gen_250-70.txt

