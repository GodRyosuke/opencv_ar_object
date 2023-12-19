# ChARUcoを使ったARサンプル
マーカー上に3Dモデルを表示する
使用したマーカーはこちらです。

デフォルトでは、Realsenseを使用することになっているので、PCカメラを使う場合はcmake configuration時で、USE_REALSENSE=OFFとしておいてください。

# Windows
## Installation
### すでに準備されたものを用いる場合
下記のリンクからdependenciesフォルダをダウンロードできます。
.gitやsrcがある同じ階層においてください。

https://drive.google.com/file/d/1uhEPOEQJAjW4ur1irwECur0qcAWmRlZQ/view?usp=sharing

### 自分で外部ライブラリを準備する場合
```bash
git clone https://github.com/GodRyosuke/opencv_ar_object.git
cd opencv_ar_object
mkdir dependencies
cd dependencies
#install vcpkg
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
vcpkg/bootstrap-vcpkg.bat
# install assimp
vcpkg install assimp:x64-windows
```
glfwをサイトからダウンロードし、buildしてください。
opencvとopencv_conrib ver4.5.4をダウンロードし、buildしてください。
cmake --installされたものはdependenciesフォルダ内にコピーしてください。

## build project
```bash
mkdir build
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release -G "Visual Studio 17 2022" --preset wander_ge_presets
cmake --build build --config Release --parallel 3
cmake --install build --config Release
```

# Ubuntu
## Installation
```bash
sudo apt-get install assimp glfw opencv
git clone https://github.com/GodRyosuke/opencv_ar_object.git
```
opencvをaptでインストールしていますが、**バージョンが4.5.4**であることを確認しておいてください。
異なる場合は、sourceからinstallしてください。

## build project
```bash
mkdir build && cd build
cmake ..
make
```