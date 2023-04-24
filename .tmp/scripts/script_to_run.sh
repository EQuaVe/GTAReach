python input_parser.py $1 > tmp_input.dot
xdot tmp_input.dot &
./install_dir/bin/tck-reach -a eca_gsim_gen $1 -C tmp.dot
python output_parser.py tmp.dot $1 > tmp_output.dot
#rm tmp1.dot
xdot tmp_output.dot &
#rm tmp.dot

