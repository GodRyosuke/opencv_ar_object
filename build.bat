@REM cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug -G "Visual Studio 17 2022"
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release -G "Visual Studio 17 2022" -DUSE_REALSENSE=OFF --preset wander_ge_presets
cmake --build build --config Release --parallel 3
cmake --install build --config Release