cmake -S . -B build -GNinja -DCMAKE_BUILD_TYPE=Release -DUSE_REALSENSE=ON
cmake --build build --parallel 8
cmake --install build