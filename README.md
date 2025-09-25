# ttsfront
Build:
1. git clone --recurse-submodules https://github.com/tznl/kazetts.git
2. cd thirdparty && make && cd ..
3. cc -o mate mate.c
4. ./mate
5. cp -r "thirdparty/espeak-ng/build/espeak_ng/espeak-ng-data/" "build/data/"
6. ./run.sh
