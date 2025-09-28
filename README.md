# ttsfront
Build:
1. git clone --recurse-submodules https://github.com/tznl/kazetts.git
2. provide thirdparty/onnxruntime/lib. if you dont want to compile, get it from https://github.com/microsoft/onnxruntime/releases/tag/v1.23.0
3. cd thirdparty 
4. make
5. cd ..
6. make
7. cd build && ./run

For cross compilation:
CC="x86_64-w64-mingw32-gcc" CXX="x86_64-w64-mingw32-g++"
or
CC="clang --target=x86_64-w64-mingw32" CXX="clang --target=x86_64-w64-mingw32"
