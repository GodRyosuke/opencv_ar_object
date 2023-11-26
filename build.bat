cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug -G "Visual Studio 17 2022" --preset wander_ge_presets
@REM cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug -G "Visual Studio 17 2022"
cmake --build build --config Debug --parallel 3
@REM cmake --install build --config Debug