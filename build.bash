cmake -S . -B build -GNinja -DCMAKE_BUILD_TYPE=Debug
cmake --build build --parallel 8
cmake --install build