#!/bin/sh
cd ./build
LD_LIBRARY_PATH=$PWD/../thirdparty/piper1-gpl/libpiper/install/:$PWD/../thirdparty/piper1-gpl/libpiper/install/lib64/ ./run
