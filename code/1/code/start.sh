#!/bin/bash
test -d build

if [ $? -eq 0 ] ; then
    cd ./build
else 
    mkdir build 
    cd ./build
fi

test -e CMakeCache 
if [ $? -gt 0 ] 
then make clean
fi

cmake .. && make && mv main ../main.exe


