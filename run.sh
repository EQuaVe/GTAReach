if [ $# -lt 1 ]; then
 ./install_dir/bin/tck-reach -a eca_gsim_gen example1_gen.txt
fi

if [ $# -eq 1 ]; then
 ./install_dir/bin/tck-reach -a eca_gsim_gen $1
fi
 
if [ $# -gt 1 ]; then
 echo 'ERROR:'"'$@'"' command is unsupported'
fi

