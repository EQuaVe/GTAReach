python input_parser.py $1 > tmp_input.dot
./install_dir/bin/tck-reach -a eca_gsim_gen $1 -C tmp.dot
python output_parser.py tmp.dot $1 > tmp_output.dot

infol=$(echo $1 | cut -d '.' -f 1 )

cp tmp_input.dot $infol"_in.dot"
cp tmp_output.dot $infol"_out.dot"
