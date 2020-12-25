#!/bin/bash
para=$1
if [ -e main.exe  ];then
    if [ -e $path ];then
        ./main.exe $para
    else
        ./main.exe
    fi
fi



