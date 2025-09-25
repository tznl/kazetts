# ttsfront
Build:
1. git clone --recurse-submodules https://github.com/tznl/kazetts.git
2. cd thirdparty 
3. provide thirdparty/onnxruntime/lib. if you dont want to compile, get it from https://github.com/microsoft/onnxruntime/releases/tag/v1.22.0
4. make && cd ..
5. cc -o mate mate.c
6. ./mate
7. cp -r "thirdparty/espeak-ng/build/espeak_ng/espeak-ng-data/" "build/data/"
8. ./run.sh
