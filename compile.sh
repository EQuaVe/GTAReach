reset

touch ./install_dir
rm -rf ./install_dir
mkdir ./install_dir

touch ./build_dir
rm -rf ./build_dir
mkdir ./build_dir

cd build_dir
cmake ../tchecker -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=../install_dir -DCMAKE_CXX_FLAGS=-pthread
make -j2
make doc
make install
cd ../

echo "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"
echo "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"
echo "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"
