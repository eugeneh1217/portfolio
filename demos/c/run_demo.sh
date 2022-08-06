rm -rf build
cmake $1/CMakeLists.txt -B build
cd build 
make
cd ..
./build/demo "${@:2}"