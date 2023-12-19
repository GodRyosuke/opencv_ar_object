cmake -S . -B build -GNinja -DCMAKE_BUILD_TYPE=Release -DUSE_REALSENSE=OFF
cmake --build build --parallel 8
cmake --install build